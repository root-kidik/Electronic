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

    QObject::connect(&btn_, &QPushButton::clicked, this, [this]() { emit NextWidget(Widgets::Profile); });
}
