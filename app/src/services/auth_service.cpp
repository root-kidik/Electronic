#include "auth_service.hpp"

#include <QGrpcChannelOptions>
#include <QGrpcHttp2Channel>

AuthService& AuthService::GetInstance()
{
    static AuthService auth_service;
    return auth_service;
}

AuthService::AuthService()
{
    auto channel_options = QGrpcChannelOptions(QUrl("http://localhost:8081", QUrl::StrictMode));
    client_.attachChannel(std::make_shared<QGrpcHttp2Channel>(channel_options));
}

bool AuthService::TryRegister(const QString& email, const QString& password)
{
    api::auth_service::v1::RegisterRequest request;
    request.setEmail(email);
    request.setPassword(password);

    client_.Register(request, &register_response_);
    auto token = register_response_.token();

    return token != "";
}

bool AuthService::TryLogin(const QString& email, const QString& password)
{
    api::auth_service::v1::LoginRequest request;
    request.setEmail(email);
    request.setPassword(password);

    client_.Login(request, &login_response_);
    auto token = login_response_.token();

    return token != "";
}
