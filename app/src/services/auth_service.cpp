#include "auth_service.hpp"

#include <QGrpcHttp2Channel>

#include "auth_service.qpb.h"

AuthService::AuthService(QObject* parent) : QObject{parent}
{
    QGrpcChannelOptions channel_options(QUrl("http://localhost:8081", QUrl::StrictMode));
    client_.attachChannel(std::make_shared<QGrpcHttp2Channel>(channel_options));
}

QString AuthService::Register(const QString& email, const QString& password)
{
    api::auth_service::v1::RegisterRequest request;
    request.setEmail(email);
    request.setPassword(password);

    client_.Register(request, &register_response_);
    return register_response_.token();
}

QString AuthService::Login(const QString& email, const QString& password)
{
    api::auth_service::v1::LoginRequest request;
    request.setEmail(email);
    request.setPassword(password);

    client_.Login(request, &login_response_);
    return register_response_.token();
}
