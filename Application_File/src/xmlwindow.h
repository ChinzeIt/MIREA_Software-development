#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class XMLWindow: public QWidget {
    Q_OBJECT

    public:
    XMLWindow(QWidget* parent = nullptr);

    signals:
    void backToMain();

    private:
    void setUpUI();
    void setUpConnections();

    QPushButton* backButton;
};