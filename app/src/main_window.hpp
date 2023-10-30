#pragma once

#include <QMainWindow>
#include <QStackedWidget>

#include "widgets/auth_widget.hpp"
#include "widgets/profile_widget.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    QStackedWidget stacked_widget_;
    AuthWidget     auth_widget_;
    ProfileWidget  profile_widget_;
};
