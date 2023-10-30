#pragma once

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class ProfileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileWidget(QWidget* parent = nullptr);

signals:
    void NextWidget(int index);

private:
    QVBoxLayout vlayout_;
    QLabel      label_;
    QPushButton btn_;
};
