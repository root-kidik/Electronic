#include "auth_service.hpp"

#include <userver/clients/dns/component.hpp>
#include <userver/components/component_context.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utils/assert.hpp>

#include <bcrypt.h>
#include <fmt/format.h>
#include <jwt-cpp/jwt.h>

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

    auto result = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                       "INSERT INTO auth_schema.users(email, password) VALUES($1, $2) ",
                                       email,
                                       bcrypt::generateHash(password));

    api::auth_service::v1::RegisterResponse response;
    response.set_token(GenerateJwtToken(email));
    call.Finish(response);
}

void AuthService::Login(api::auth_service::v1::AuthServiceBase::LoginCall& call, api::auth_service::v1::LoginRequest&& request)
{
    const auto& email    = request.email();
    const auto& password = request.password();

    auto result = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                       "SELECT password FROM auth_schema.users WHERE email = ($1) ",
                                       email);

    const auto& hash_password = result[0].As<std::string>();

    api::auth_service::v1::LoginResponse response;

    std::string token;
    if (bcrypt::validatePassword(password, hash_password))
        token = GenerateJwtToken(email);

    response.set_token(token);
    call.Finish(response);
}

void AuthService::Auth(api::auth_service::v1::AuthServiceBase::AuthCall& call, api::auth_service::v1::AuthRequest&&)
{
    auto token         = call.GetContext().client_metadata().find("token")->second;
    auto decoded_token = jwt::decode(token.data());

    std::string email;
    try
    {
        jwt::verify()
            .allow_algorithm(jwt::algorithm::hs512{"some-strog-secret-key"})
            .with_issuer(decoded_token.get_issuer())
            .verify(decoded_token);

        email = decoded_token.get_payload_claim("email").as_string();
    } catch (const std::system_error& error)
    {
        std::cout << error.what();
    }

    api::auth_service::v1::AuthResponse response;
    response.set_email(email);
    call.Finish(response);
}

std::string AuthService::GenerateJwtToken(const std::string& email)
{
    return jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_payload_claim("email", jwt::claim(email))
        .sign(jwt::algorithm::hs512{"some-strog-secret-key"});
}

void AppendAuthService(userver::components::ComponentList& component_list)
{
    component_list.Append<AuthService>();
    component_list.Append<userver::components::Postgres>("postgres-db-1");
    component_list.Append<userver::clients::dns::Component>();
}

} // namespace auth_service
