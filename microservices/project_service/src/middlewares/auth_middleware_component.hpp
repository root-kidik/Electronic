#pragma once

#include <userver/components/component_list.hpp>
#include <userver/ugrpc/server/middlewares/base.hpp>

namespace project_service
{

class AuthMiddlewareComponent : public userver::ugrpc::server::MiddlewareComponentBase
{
    using userver::ugrpc::server::MiddlewareComponentBase::MiddlewareComponentBase;

public:
    static constexpr std::string_view kName = "auth-middleware";

    std::shared_ptr<userver::ugrpc::server::MiddlewareBase> GetMiddleware() override;
};

void AppendAuthMiddlewareComponent(userver::components::ComponentList& component_list);

} // namespace project_service
