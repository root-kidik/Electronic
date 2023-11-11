#pragma once

#include "userver/ugrpc/server/middlewares/base.hpp"


namespace project_service
{

class AuthClient;

class JwtAuthMiddleware : public userver::ugrpc::server::MiddlewareBase
{
public:
    JwtAuthMiddleware(AuthClient& auth_client);

    void Handle(userver::ugrpc::server::MiddlewareCallContext& context) const override;

private:
    AuthClient& auth_client_;
};

} // namespace project_service
