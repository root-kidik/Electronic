#pragma once

#include <QObject>
#include <QString>

#include "auth_service.qpb.h"
#include "auth_service_client.grpc.qpb.h"

class AuthService
{
public:
    static AuthService& GetInstance();

    bool TryRegister(const QString& email, const QString& password);
    bool TryLogin(const QString& email, const QString& password);

private:
    AuthService();

    api::auth_service::v1::AuthService::Client client_;
    api::auth_service::v1::RegisterResponse    register_response_;
    api::auth_service::v1::LoginResponse       login_response_;

    QString token_;
};
