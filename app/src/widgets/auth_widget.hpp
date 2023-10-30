#pragma once

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "services/auth_service.hpp"

class AuthWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AuthWidget(QWidget* parent = nullptr);

signals:
    void NextWidget(int index);

private:
    QVBoxLayout vlayout_;
    QLabel      label_;
    QPushButton btn_;

    AuthService auth_service_;
};
