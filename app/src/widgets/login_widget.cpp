#include "login_widget.hpp"

#include "services/auth_service.hpp"
#include "widgets.hpp"

LoginWidget::LoginWidget(QWidget* parent) : QWidget{parent}
{
    setLayout(&vlayout_);
    vlayout_.setSpacing(20);
    vlayout_.addWidget(&email_input_);
    vlayout_.addWidget(&password_input_);
    vlayout_.addWidget(&go_to_registration_btn_);
    vlayout_.addWidget(&login_btn_);

    go_to_registration_btn_.setText("not yet have account?");
    login_btn_.setText("login");

    QObject::connect(&go_to_registration_btn_,
                     &QPushButton::clicked,
                     this,
                     [this]()
                     {
                         emit NextWidget(Widgets::Registration);
                         // TODO: Add else section
                     });


    QObject::connect(&login_btn_,
                     &QPushButton::clicked,
                     this,
                     [this]()
                     {
                         if (AuthService::GetInstance().TryLogin(email_input_.text(), password_input_.text()))
                             emit NextWidget(Widgets::Profile);
                         // TODO: Add else section
                     });
}
