#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class JSONWindow: public QWidget {
    Q_OBJECT

    public:
    JSONWindow(QWidget* parent = nullptr);

    signals:
    void backToMain();

    private:
    void setUpUI();
    void setUpConnections();

    QPushButton* backButton;
};