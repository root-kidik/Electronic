#pragma once

#include <userver/components/component_list.hpp>
#include <userver/storages/postgres/component.hpp>

#include <handlers/auth_service_client.usrv.pb.hpp>
#include <handlers/auth_service_service.usrv.pb.hpp>

#include <string>
#include <string_view>

#include "auth_client.hpp"

namespace auth_service
{

class Auth final : public handlers::api::AuthServiceBase::Component
{
public:
    static constexpr std::string_view kName = "handler-auth";

    Auth(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& component_context);

    void Register(handlers::api::AuthServiceBase::RegisterCall& call, handlers::api::RegisterRequest&& request) override;
    void Login(handlers::api::AuthServiceBase::LoginCall& call, handlers::api::LoginRequest&& request) override;

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
    AuthClient&                             client_;
};

void AppendAuth(userver::components::ComponentList& component_list);

} // namespace auth_service
