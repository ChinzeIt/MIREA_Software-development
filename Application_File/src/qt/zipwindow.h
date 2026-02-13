#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class ZIPWindow: public QWidget {
    Q_OBJECT

    public:
    ZIPWindow(QWidget* parent = nullptr);

    signals:
    void backToMain();

    private:
    void setUpUI();
    void setUpConnections();

    QPushButton* backButton;
};