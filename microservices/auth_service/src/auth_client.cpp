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


std::string AuthClient::Register(std::string email, std::string password)
{
    api::auth_service::v1::RegisterRequest request;
    request.set_email(std::move(email));
    request.set_password(std::move(password));

    // Deadline must be set manually for each RPC
    auto context = std::make_unique<grpc::ClientContext>();
    context->set_deadline(userver::engine::Deadline::FromDuration(std::chrono::seconds{20}));

    // Initiate the RPC. No actual actions have been taken thus far besides
    // preparing to send the request.
    auto stream = client_.Register(request, std::move(context));

    // Complete the unary RPC by sending the request and receiving the response.
    // The client should call `Finish` (in case of single response) or `Read`
    // until `false` (in case of response stream), otherwise the RPC will be
    // cancelled.
    api::auth_service::v1::RegisterResponse response = stream.Finish();

    return std::move(*response.mutable_token());
}

std::string AuthClient::Login(std::string email, std::string password)
{
    api::auth_service::v1::LoginRequest request;
    request.set_email(std::move(email));
    request.set_password(std::move(password));

    // Deadline must be set manually for each RPC
    auto context = std::make_unique<grpc::ClientContext>();
    context->set_deadline(userver::engine::Deadline::FromDuration(std::chrono::seconds{20}));

    // Initiate the RPC. No actual actions have been taken thus far besides
    // preparing to send the request.
    auto stream = client_.Login(request, std::move(context));

    // Complete the unary RPC by sending the request and receiving the response.
    // The client should call `Finish` (in case of single response) or `Read`
    // until `false` (in case of response stream), otherwise the RPC will be
    // cancelled.
    api::auth_service::v1::LoginResponse response = stream.Finish();

    return std::move(*response.mutable_token());
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
