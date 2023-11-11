#include "jwt_auth_middleware.hpp"

namespace project_service
{

void JwtAuthMiddleware::Handle(userver::ugrpc::server::MiddlewareCallContext& context) const
{
    context.Next();
}

} // namespace project_service
