#include "auth_middleware_component.hpp"

#include "jwt_auth_middleware.hpp"

namespace project_service
{

AuthMiddlewareComponent::AuthMiddlewareComponent(const userver::components::ComponentConfig&  config,
                                                 const userver::components::ComponentContext& component_context) :
MiddlewareComponentBase(config, component_context),
auth_client_(component_context.FindComponent<AuthClient>())
{
}

std::shared_ptr<userver::ugrpc::server::MiddlewareBase> AuthMiddlewareComponent::GetMiddleware()
{
    return std::make_shared<JwtAuthMiddleware>(auth_client_);
}

void AppendAuthMiddlewareComponent(userver::components::ComponentList& component_list)
{
    component_list.Append<AuthMiddlewareComponent>();
}

} // namespace project_service
