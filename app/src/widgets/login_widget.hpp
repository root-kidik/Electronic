#pragma once

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget* parent = nullptr);

signals:
    void NextWidget(int index);

private:
    QVBoxLayout vlayout_;
    QPushButton go_to_registration_btn_;
    QPushButton login_btn_;
    QLineEdit   email_input_;
    QLineEdit   password_input_;
};
