#include "auth_service.hpp"

#include <userver/clients/dns/component.hpp>
#include <userver/components/component_context.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utils/assert.hpp>

#include <bcrypt.h>
#include <fmt/format.h>
#include <grpcpp/support/status.h>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/boost-json/traits.h>

#include <exception>
#include <regex>

namespace auth_service
{

AuthService::AuthService(const userver::components::ComponentConfig&  config,
                         const userver::components::ComponentContext& component_context) :
api::auth_service::v1::AuthServiceBase::Component(config, component_context),
pg_cluster_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster())
{
}

void AuthService::Register(api::auth_service::v1::AuthServiceBase::RegisterCall& call,
                           api::auth_service::v1::RegisterRequest&&              request)
{
    const auto& email    = request.email();
    const auto& password = request.password();

    if (!std::regex_match(email, std::regex(R"([_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4}))")))
    {
        call.FinishWithError(grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Invalid email!"));
        return;
    }

    if (!std::regex_match(password, std::regex(R"(^.*(?=.{8,})(?=.*[a-zA-Z])(?=.*\d)(?=.*[!#$%&? "]).*$)")))
    {
        call.FinishWithError(
            grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Password: num >= 1, special symbol >= 1, length >= 8!"));
        return;
    }

    if (!pg_cluster_
             ->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                       "SELECT email FROM auth_schema.users WHERE email = ($1)",
                       email)
             .IsEmpty())
    {
        call.FinishWithError(grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "User with this email already exist!"));
        return;
    }

    auto result = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                       "INSERT INTO auth_schema.users(email, password) VALUES($1, $2) RETURNING "
                                       "users.id",
                                       email,
                                       bcrypt::generateHash(password));

    const auto id = result.AsSingleRow<std::int32_t>();

    api::auth_service::v1::RegisterResponse response;
    response.set_token(GenerateJwtToken(id));
    call.Finish(response);
}

void AuthService::Login(api::auth_service::v1::AuthServiceBase::LoginCall& call, api::auth_service::v1::LoginRequest&& request)
{
    const auto& email    = request.email();
    const auto& password = request.password();

    auto result = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                       "SELECT id, password FROM auth_schema.users WHERE email = ($1)",
                                       email);
    if (result.IsEmpty())
    {
        call.FinishWithError(grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "User with this email doesn't exists!"));
        return;
    }

    const auto [id, hash_password] = result[0].As<std::int32_t, std::string>();

    if (!bcrypt::validatePassword(password, hash_password))
    {
        call.FinishWithError(grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Wrong password!"));
        return;
    }

    api::auth_service::v1::LoginResponse response;
    response.set_token(GenerateJwtToken(id));
    call.Finish(response);
}

void AuthService::Auth(api::auth_service::v1::AuthServiceBase::AuthCall& call, api::auth_service::v1::AuthRequest&& request)
{
    const auto&  token = request.token();
    std::int32_t id;

    try
    {
        auto decoded_token = jwt::decode<jwt::traits::boost_json>(token);

        jwt::verify<jwt::traits::boost_json>()
            .allow_algorithm(jwt::algorithm::hs512{"some-strog-secret-key"})
            .with_issuer(decoded_token.get_issuer())
            .verify(decoded_token);

        id = decoded_token.get_payload_claim("id").as_integer();
    } catch (const std::exception& error)
    {
        call.FinishWithError(grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Wrong token!"));
        return;
    }

    api::auth_service::v1::AuthResponse response;
    response.set_id(id);
    call.Finish(response);
}

std::string AuthService::GenerateJwtToken(std::int32_t id)
{
    return jwt::create<jwt::traits::boost_json>().set_issuer("auth0").set_type("JWS").set_payload_claim("id", id).sign(
        jwt::algorithm::hs512{"some-strog-secret-key"});
}

void AppendAuthService(userver::components::ComponentList& component_list)
{
    component_list.Append<AuthService>();
    component_list.Append<userver::components::Postgres>("postgres-db-1");
    component_list.Append<userver::clients::dns::Component>();
}

} // namespace auth_service
