#pragma once

#include <userver/components/component.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/ugrpc/client/client_factory_component.hpp>

#include <api/auth_service/v1/auth_service_client.usrv.pb.hpp>

namespace auth_service
{

class AuthClient final : public userver::components::LoggableComponentBase
{
public:
    static constexpr std::string_view kName = "auth-client";

    AuthClient(const userver::components::ComponentConfig&  config,
               const userver::components::ComponentContext& component_context);

    static userver::yaml_config::Schema GetStaticConfigSchema();

private:
    userver::ugrpc::client::ClientFactory&   client_factory_;
    api::auth_service::v1::AuthServiceClient client_;
};

void AppendAuthClient(userver::components::ComponentList& component_list);


} // namespace auth_service

template <>
inline constexpr bool userver::components::kHasValidate<auth_service::AuthClient> = true;
