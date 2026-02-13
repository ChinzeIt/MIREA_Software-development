#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QShortcut>
#include <QTextEdit>
#include <QDebug>

class DiskInfoWindow: public QWidget {
    Q_OBJECT

    public:
    DiskInfoWindow(QWidget* parent = nullptr);

    signals:
    void backToMain();

    private:
    void setUpUI();
    void setUpConnections();
    void setHotKey();

    void onBackButton();
    void onUButton();

    QTextEdit* textInfo;
    QPushButton* backButton;
    QPushButton* uButton;
};