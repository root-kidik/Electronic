#include "auth_widget.hpp"

#include "widgets.hpp"

AuthWidget::AuthWidget(QWidget* parent) : QWidget{parent}
{
    setLayout(&vlayout_);
    vlayout_.setSpacing(20);
    vlayout_.addWidget(&label_);
    vlayout_.addWidget(&btn_);

    label_.setText("Login widget");
    btn_.setText("login");

    QObject::connect(&btn_,
                     &QPushButton::clicked,
                     this,
                     [this]()
                     {
                         qDebug() << "Register: " << auth_service_.Register("user@mail.ru", "password");
                         qDebug() << "Login: " << auth_service_.Login("user@mail.ru", "password");

                         emit NextWidget(Widgets::Profile);
                     });
}
