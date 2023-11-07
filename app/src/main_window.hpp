#pragma once

#include <QMainWindow>
#include <QStackedWidget>

#include "widgets/login_widget.hpp"
#include "widgets/profile_widget.hpp"
#include "widgets/registration_widget.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    QStackedWidget     stacked_widget_;
    RegistrationWidget registration_widget_;
    LoginWidget        login_widget_;
    ProfileWidget      profile_widget_;
};
