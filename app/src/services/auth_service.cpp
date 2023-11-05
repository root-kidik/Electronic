#include "auth_service.hpp"

#include <QGrpcChannelOptions>
#include <QGrpcHttp2Channel>

AuthService::AuthService(QObject* parent) : QObject{parent}
{
    auto          channel_options = QGrpcChannelOptions(QUrl("http://localhost:8081", QUrl::StrictMode));
    QGrpcMetadata metadata        = {{"token",
                                      "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXUyJ9."
                                             "eyJlbWFpbCI6InVzZXJAbWFpbC5ydSIsImlzcyI6ImF1dGgwIn0."
                                             "4cleWbueLRENf4gBhPhS3GEHvDYT4aEgieEo5cUGOKfIXbLHkqiR3S5UffJnfQeToSfKQxV5OIg9uzdmgJhBT"
                                             "A"}};
    channel_options.withMetadata(metadata);
    client_.attachChannel(std::make_shared<QGrpcHttp2Channel>(channel_options));
}

void AuthService::Register(const QString& email, const QString& password)
{
    api::auth_service::v1::RegisterRequest request;
    request.setEmail(email);
    request.setPassword(password);

    client_.Register(request, &register_response_);
    auto token = register_response_.token();
}

void AuthService::Login(const QString& email, const QString& password)
{
    api::auth_service::v1::LoginRequest request;
    request.setEmail(email);
    request.setPassword(password);

    client_.Login(request, &login_response_);
    auto token = login_response_.token();
    qDebug() << token << "\n";
}

void AuthService::Auth()
{
    client_.Auth({}, &auth_response_);
    qDebug() << auth_response_.email() << "\n";
}
