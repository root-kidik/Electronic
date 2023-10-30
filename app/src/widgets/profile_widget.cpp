#include "profile_widget.hpp"

#include "widgets.hpp"

ProfileWidget::ProfileWidget(QWidget* parent) : QWidget{parent}
{
    setLayout(&vlayout_);
    vlayout_.setSpacing(20);
    vlayout_.addWidget(&label_);
    vlayout_.addWidget(&btn_);

    label_.setText("Profile widget");
    btn_.setText("exit");

    QObject::connect(&btn_, &QPushButton::clicked, this, [this]() { emit NextWidget(Widgets::Auth); });
}
