#include "auth_service.hpp"

#include <userver/clients/dns/component.hpp>
#include <userver/components/component_context.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utils/assert.hpp>

#include <fmt/format.h>

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
                                       password);

    std::string token;

    token = email + password;
    api::auth_service::v1::RegisterResponse response;
    response.set_token(token);
    call.Finish(response);
}

void AuthService::Login(api::auth_service::v1::AuthServiceBase::LoginCall& call, api::auth_service::v1::LoginRequest&& request)
{
    const auto& email    = request.email();
    const auto& password = request.password();
    std::string token;

    token = email + password;
    api::auth_service::v1::LoginResponse response;
    response.set_token(token);
    call.Finish(response);
}

void AppendAuthService(userver::components::ComponentList& component_list)
{
    component_list.Append<AuthService>();
    component_list.Append<userver::components::Postgres>("postgres-db-1");
    component_list.Append<userver::clients::dns::Component>();
}

} // namespace auth_service
