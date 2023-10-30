#include "main_window.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    stacked_widget_.addWidget(&auth_widget_);
    stacked_widget_.addWidget(&profile_widget_);
    setCentralWidget(&stacked_widget_);

    QObject::connect(&auth_widget_, &AuthWidget::NextWidget, &stacked_widget_, &QStackedWidget::setCurrentIndex);
    QObject::connect(&profile_widget_, &ProfileWidget::NextWidget, &stacked_widget_, &QStackedWidget::setCurrentIndex);
}
