#pragma once

#include "userver/ugrpc/server/middlewares/base.hpp"

namespace project_service
{

class JwtAuthMiddleware : public userver::ugrpc::server::MiddlewareBase
{
public:
    void Handle(userver::ugrpc::server::MiddlewareCallContext& context) const override;
};

} // namespace project_service
