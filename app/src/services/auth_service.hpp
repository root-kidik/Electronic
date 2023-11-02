#pragma once

#include <QObject>
#include <QtCore/qstring.h>

#include "auth_service.qpb.h"
#include "auth_service_client.grpc.qpb.h"

class AuthService : public QObject
{
public:
    explicit AuthService(QObject* parent = nullptr);

    QString Register(const QString& email, const QString& password);
    QString Login(const QString& email, const QString& password);

private:
    api::auth_service::v1::AuthService::Client client_;
    api::auth_service::v1::RegisterResponse    register_response_;
    api::auth_service::v1::LoginResponse       login_response_;
};
