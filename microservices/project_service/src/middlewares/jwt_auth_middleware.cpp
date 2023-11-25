#include "jwt_auth_middleware.hpp"

#include "../auth_client.hpp"
#include "auth_middleware_component.hpp"

namespace project_service
{

JwtAuthMiddleware::JwtAuthMiddleware(AuthClient& auth_client) : auth_client_(auth_client)
{
}

void JwtAuthMiddleware::Handle(userver::ugrpc::server::MiddlewareCallContext& context) const
{
    const auto id = auth_client_.Auth(context.GetCall().GetContext().client_metadata().find("token")->second.data());
    *g_user_id    = id;

    context.Next();
}

} // namespace project_service
