#include "jwt_auth_middleware.hpp"

#include "../auth_client.hpp"

namespace project_service
{

JwtAuthMiddleware::JwtAuthMiddleware(AuthClient& auth_client) : auth_client_(auth_client)
{
}

void JwtAuthMiddleware::Handle(userver::ugrpc::server::MiddlewareCallContext& context) const
{
    context.Next();
}

} // namespace project_service
