#pragma once

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class RegistrationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationWidget(QWidget* parent = nullptr);

signals:
    void NextWidget(int index);

private:
    QVBoxLayout vlayout_;
    QPushButton go_to_login_btn_;
    QPushButton registration_btn_;
    QLineEdit   email_input_;
    QLineEdit   password_input_;
};
