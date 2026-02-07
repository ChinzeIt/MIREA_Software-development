#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QDebug>

#define WIDTH 500
#define HEIGHT 300

class MainWindow: public QMainWindow {
    Q_OBJECT

    public:
    MainWindow(QWidget* parent=nullptr);

    private:
    void setUpUI();
    void setUpConnections();
    
    void onDISKButtonClicked();
    void onTXTButtonClicked();
    void onJSONButtonClicked();
    void onXMLButtonClicked();
    void onZIPButtonClicked();

    QPushButton* diskButton;
    QPushButton* txtButton;
    QPushButton* jsonButton;
    QPushButton* xmlButton;
    QPushButton* zipButton;
};