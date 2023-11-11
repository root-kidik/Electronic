#include "auth_client.hpp"

#include <userver/yaml_config/merge_schemas.hpp>

#include <api/auth_service/v1/auth_service_client.usrv.pb.hpp>

namespace project_service
{

AuthClient::AuthClient(const userver::components::ComponentConfig&  config,
                       const userver::components::ComponentContext& component_context) :
userver::components::LoggableComponentBase(config, component_context),
client_factory_(component_context.FindComponent<userver::ugrpc::client::ClientFactoryComponent>().GetFactory()),
client_(client_factory_.MakeClient<api::auth_service::v1::AuthServiceClient>("auth-client",
                                                                             config["endpoint"].As<std::string>()))
{
}

std::int32_t AuthClient::Auth(std::string token)
{
    api::auth_service::v1::AuthRequest request;
    request.set_token(std::move(token));

    auto context = std::make_unique<grpc::ClientContext>();
    context->set_deadline(userver::engine::Deadline::FromDuration(std::chrono::seconds{20}));

    auto stream = client_.Auth(request, std::move(context));

    api::auth_service::v1::AuthResponse response = stream.Finish();

    return response.id();
}

userver::yaml_config::Schema AuthClient::GetStaticConfigSchema()
{
    return userver::yaml_config::MergeSchemas<userver::components::LoggableComponentBase>(R"(
type: object
description: >
    client for auth service
additionalProperties: false
properties:
    endpoint:
        type: string
        description: >
            auth service port 
)");
}

void AppendAuthClient(userver::components::ComponentList& component_list)
{
    component_list.Append<AuthClient>();
}


} // namespace project_service
