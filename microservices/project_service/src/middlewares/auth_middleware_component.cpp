#include "auth_middleware_component.hpp"

#include "jwt_auth_middleware.hpp"

namespace project_service
{

std::shared_ptr<userver::ugrpc::server::MiddlewareBase> AuthMiddlewareComponent::GetMiddleware()
{
    return std::make_shared<JwtAuthMiddleware>();
}

void AppendAuthMiddlewareComponent(userver::components::ComponentList& component_list)
{
    component_list.Append<AuthMiddlewareComponent>();
}

} // namespace project_service
