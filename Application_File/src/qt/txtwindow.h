#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class TXTWindow: public QWidget {
    Q_OBJECT

    public:
    TXTWindow(QWidget* parent = nullptr);

    signals:
    void backToMain();

    private:
    void setUpUI();
    void setUpConnections();

    QPushButton* backButton;
};