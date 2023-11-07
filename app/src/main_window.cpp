#include "main_window.hpp"

#include "widgets/registration_widget.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    stacked_widget_.addWidget(&registration_widget_);
    stacked_widget_.addWidget(&login_widget_);
    stacked_widget_.addWidget(&profile_widget_);
    setCentralWidget(&stacked_widget_);

    QObject::connect(&registration_widget_, &RegistrationWidget::NextWidget, &stacked_widget_, &QStackedWidget::setCurrentIndex);
    QObject::connect(&login_widget_, &LoginWidget::NextWidget, &stacked_widget_, &QStackedWidget::setCurrentIndex);
    QObject::connect(&profile_widget_, &ProfileWidget::NextWidget, &stacked_widget_, &QStackedWidget::setCurrentIndex);
}
