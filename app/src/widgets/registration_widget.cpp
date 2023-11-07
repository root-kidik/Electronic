#include "registration_widget.hpp"

#include "services/auth_service.hpp"
#include "widgets.hpp"

RegistrationWidget::RegistrationWidget(QWidget* parent) : QWidget{parent}
{
    setLayout(&vlayout_);
    vlayout_.setSpacing(20);
    vlayout_.addWidget(&email_input_);
    vlayout_.addWidget(&password_input_);
    vlayout_.addWidget(&go_to_login_btn_);
    vlayout_.addWidget(&registration_btn_);

    go_to_login_btn_.setText("already have account?");
    registration_btn_.setText("register");

    QObject::connect(&go_to_login_btn_, &QPushButton::clicked, this, [this]() { emit NextWidget(Widgets::Login); });

    QObject::connect(&registration_btn_,
                     &QPushButton::clicked,
                     this,
                     [this]()
                     {
                         if (AuthService::GetInstance().TryRegister(email_input_.text(), password_input_.text()))
                             emit NextWidget(Widgets::Profile);
                         // TODO: Add else section
                     });
}
