#include "project_service.hpp"

#include <userver/clients/dns/component.hpp>
#include <userver/components/component_context.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utils/assert.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

namespace project_service
{

ProjectService::ProjectService(const userver::components::ComponentConfig&  config,
                               const userver::components::ComponentContext& component_context) :
api::project_service::v1::ProjectServiceBase::Component(config, component_context),
pg_cluster_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster())
{
}

void ProjectService::Create(api::project_service::v1::ProjectServiceBase::CreateCall& call,
                            api::project_service::v1::CreateRequest&&                 request)
{
}

void ProjectService::Delete(api::project_service::v1::ProjectServiceBase::DeleteCall& call,
                            api::project_service::v1::DeleteRequest&&                 request)
{
}

void AppendProjectService(userver::components::ComponentList& component_list)
{
    component_list.Append<ProjectService>();
    component_list.Append<userver::components::Postgres>("postgres-db-1");
    component_list.Append<userver::clients::dns::Component>();
}

} // namespace project_service
