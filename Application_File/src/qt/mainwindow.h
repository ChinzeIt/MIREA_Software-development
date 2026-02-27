#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QShortcut>
#include <QStackedWidget>

// Connect lower window
#include "diskinfowindow.h"
#include "txtwindow.h"
#include "jsonwindow.h"
#include "xmlwindow.h"
#include "zipwindow.h"

#define WIDTH 500
#define HEIGHT 300

class MainWindow: public QMainWindow {
    Q_OBJECT

    public:
    MainWindow(QWidget* parent=nullptr);

    private:
    void setUpUI();
    void setUpConnections();
    void setHotKey();
    
    void onDISKButtonClicked();
    void onTXTButtonClicked();
    void onJSONButtonClicked();
    void onXMLButtonClicked();
    void onZIPButtonClicked();

    QStackedWidget* stackedWidget;
    QWidget* mainWidget;
    DiskInfoWindow* diskInfoWindow;

    TXTWindow* txtWindow;
    TXTWindowCreate* txtWindowCreate;
    TXTWindowRead* txtWindowRead;
    TXTWindowEdit* txtWindowEdit;
    TXTWindowRemove* txtWindowRemove;

    JSONWindow* jsonWindow;
    JSONWindowCreate* jsonWindowCreate;
    JSONWindowRead* jsonWindowRead;
    JSONWindowEdit* jsonWindowEdit;
    JSONWindowRemove* jsonWindowRemove;

    XMLWindow* xmlWindow;

    ZIPWindow* zipWindow;

    QPushButton* diskButton;
    QPushButton* txtButton;
    QPushButton* jsonButton;
    QPushButton* xmlButton;
    QPushButton* zipButton;
};