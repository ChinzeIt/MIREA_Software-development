#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class DiskInfoWindow: public QWidget {
    Q_OBJECT

    public:
    DiskInfoWindow(QWidget* parent = nullptr);

    signals:
    void backToMain();

    private:
    void setUpUI();
    void setUpConnections();

    QPushButton* backButton;
};