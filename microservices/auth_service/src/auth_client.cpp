#include "auth_client.hpp"

#include <userver/yaml_config/merge_schemas.hpp>

#include <fmt/format.h>

namespace auth_service
{

AuthClient::AuthClient(const userver::components::ComponentConfig&  config,
                       const userver::components::ComponentContext& component_context) :
userver::components::LoggableComponentBase(config, component_context),
client_factory_(component_context.FindComponent<userver::ugrpc::client::ClientFactoryComponent>().GetFactory()),
client_(client_factory_.MakeClient<api::auth_service::v1::AuthServiceClient>("auth-client",
                                                                             config["endpoint"].As<std::string>()))
{
}


userver::yaml_config::Schema AuthClient::GetStaticConfigSchema()
{
    return userver::yaml_config::MergeSchemas<userver::components::LoggableComponentBase>(R"(
type: object
description: >
    a user-defined wrapper around api::GreeterServiceClient that provides
    a simplified interface.
additionalProperties: false
properties:
    endpoint:
        type: string
        description: >
            Some other service endpoint (URI). 
)");
}

void AppendAuthClient(userver::components::ComponentList& component_list)
{
    component_list.Append<AuthClient>();
}

} // namespace auth_service
