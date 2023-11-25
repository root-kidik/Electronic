#include "project_service.hpp"

#include <userver/clients/dns/component.hpp>
#include <userver/components/component_context.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utils/assert.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include "middlewares/auth_middleware_component.hpp"

namespace
{
constexpr std::string_view kWrongNameError            = "Name can't be empty!";
constexpr std::string_view kWrongIdError              = "Wrong project id!";
constexpr std::string_view kRepeatedProjectNameError  = "Project with this name already exist!";
constexpr std::string_view kDoesntExistProjectIdError = "Project with this id doesn't exist!";
constexpr std::string_view kYouHaveNoPermissonError   = "You have no permission!";

constexpr std::string_view kSelectProjectByNameQuery = "SELECT name FROM project_schema.projects WHERE name = ($1)";
constexpr std::string_view kSelectProjectUserIdByIdQuery =
    "SELECT user_id FROM project_schema.projects WHERE id = "
    "($1)";
constexpr std::string_view
    kInsertProjectQuery = "INSERT INTO project_schema.projects(user_id, name) VALUES($1, $2) RETURNING projects.id";
constexpr std::string_view kDeleteProjectByIdQuery = "DELETE FROM project_schema.projects WHERE id = ($1)";
} // namespace

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
    const auto& name = request.name();

    if (name.empty())
    {
        call.FinishWithError(grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, kWrongNameError.data()));
        return;
    }

    if (!pg_cluster_
             ->Execute(userver::storages::postgres::ClusterHostType::kMaster, kSelectProjectByNameQuery.data(), name)
             .IsEmpty())
    {
        call.FinishWithError(grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, kRepeatedProjectNameError.data()));
        return;
    }

    auto result = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                       kInsertProjectQuery.data(),
                                       *g_user_id,
                                       name);

    api::project_service::v1::CreateResponse response;
    response.set_id(result.AsSingleRow<std::int32_t>());
    call.Finish(response);
}

void ProjectService::Delete(api::project_service::v1::ProjectServiceBase::DeleteCall& call,
                            api::project_service::v1::DeleteRequest&&                 request)
{
    const auto& id = request.id();

    if (!id)
    {
        call.FinishWithError(grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, kWrongIdError.data()));
        return;
    }

    const auto& project = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                               kSelectProjectUserIdByIdQuery.data(),
                                               id);
    if (project.IsEmpty())
    {
        call.FinishWithError(grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, kDoesntExistProjectIdError.data()));
        return;
    }

    if (project.AsSingleRow<std::int32_t>() != *g_user_id)
    {
        call.FinishWithError(grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, kYouHaveNoPermissonError.data()));
        return;
    }

    auto result = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                       kDeleteProjectByIdQuery.data(),
                                       id);

    api::project_service::v1::DeleteResponse response;
    call.Finish(response);
}

void AppendProjectService(userver::components::ComponentList& component_list)
{
    component_list.Append<ProjectService>();
    component_list.Append<userver::components::Postgres>("postgres-db-1");
    component_list.Append<userver::clients::dns::Component>();
}

} // namespace project_service
