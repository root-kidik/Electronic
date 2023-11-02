#include "auth.hpp"

#include <userver/clients/dns/component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utils/assert.hpp>

#include <fmt/format.h>

namespace auth_service
{

Auth::Auth(const userver::components::ComponentConfig&  config,
           const userver::components::ComponentContext& component_context) :
api::auth_service::v1::AuthServiceBase::Component(config, component_context),
pg_cluster_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()),
client_(component_context.FindComponent<AuthClient>())
{
}


void Auth::Register(api::auth_service::v1::AuthServiceBase::RegisterCall& call,
                    api::auth_service::v1::RegisterRequest&&              request)
{
    const auto& email    = request.email();
    const auto& password = request.password();
    std::string token;

    if (email.substr(0, 5) == "mock_" && password.substr(0, 5) == "mock_")
        token = client_.Register(email.substr(5), password.substr(5));

    token = email + password;
    api::auth_service::v1::RegisterResponse response;
    response.set_token(token);
    call.Finish(response);
}

void Auth::Login(api::auth_service::v1::AuthServiceBase::LoginCall& call, api::auth_service::v1::LoginRequest&& request)
{
    const auto& email    = request.email();
    const auto& password = request.password();
    std::string token;

    if (email.substr(0, 5) == "mock_" && password.substr(0, 5) == "mock_")
        token = client_.Login(email.substr(5), password.substr(5));

    token = email + password;
    api::auth_service::v1::LoginResponse response;
    response.set_token(token);
    call.Finish(response);
}

void AppendAuth(userver::components::ComponentList& component_list)
{
    component_list.Append<Auth>();
    component_list.Append<userver::components::Postgres>("postgres-db-1");
    component_list.Append<userver::clients::dns::Component>();
}

} // namespace auth_service
