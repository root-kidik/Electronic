#pragma once

#include <userver/components/component_list.hpp>
#include <userver/storages/postgres/component.hpp>

#include <api/project_service/v1/project_service_client.usrv.pb.hpp>
#include <api/project_service/v1/project_service_service.usrv.pb.hpp>

#include <string>
#include <string_view>

namespace project_service
{

class ProjectService final : public api::project_service::v1::ProjectServiceBase::Component
{
public:
    static constexpr std::string_view kName = "project-service";

    ProjectService(const userver::components::ComponentConfig&  config,
                   const userver::components::ComponentContext& component_context);

    void Create(api::project_service::v1::ProjectServiceBase::CreateCall& call,
                api::project_service::v1::CreateRequest&&                 request) override;
    void Delete(api::project_service::v1::ProjectServiceBase::DeleteCall& call,
                api::project_service::v1::DeleteRequest&&                 request) override;

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
};

void AppendProjectService(userver::components::ComponentList& component_list);

} // namespace project_service
