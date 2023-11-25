#pragma once

#include <userver/components/component_list.hpp>
#include <userver/engine/task/local_variable.hpp>
#include <userver/ugrpc/server/middlewares/base.hpp>

#include "../auth_client.hpp"

namespace project_service
{

class AuthMiddlewareComponent : public userver::ugrpc::server::MiddlewareComponentBase
{
public:
    static constexpr std::string_view kName = "auth-middleware";

    AuthMiddlewareComponent(const userver::components::ComponentConfig&  config,
                            const userver::components::ComponentContext& component_context);

    std::shared_ptr<userver::ugrpc::server::MiddlewareBase> GetMiddleware() override;

private:
    AuthClient& auth_client_;
};

void AppendAuthMiddlewareComponent(userver::components::ComponentList& component_list);

inline userver::engine::TaskLocalVariable<std::int32_t> g_user_id;

} // namespace project_service
