#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
QMainWindow (parent),
txtButton (nullptr),
jsonButton (nullptr),
xmlButton (nullptr),
zipButton (nullptr) {
    setUpUI();
    setUpConnections();
    setHotKey();

    this->setWindowTitle("Application File");

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(WIDTH, HEIGHT);
}

void MainWindow::setUpUI () {
    stackedWidget = new QStackedWidget(this);
    this->setCentralWidget(stackedWidget);

    mainWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);

    QFont font;
    font.setFamily("Arial");
    font.setPointSize(30);
    font.setBold(true);

    diskButton = new QPushButton("DISK info", mainWidget);
    txtButton = new QPushButton("TXT files", mainWidget);
    jsonButton = new QPushButton("JSON files", mainWidget);
    xmlButton = new QPushButton("XML files", mainWidget);
    zipButton = new QPushButton("ZIP files", mainWidget);

    diskButton->setFont(font);
    txtButton->setFont(font);
    jsonButton->setFont(font);
    xmlButton->setFont(font);
    zipButton->setFont(font);

    mainLayout->addWidget(diskButton);
    mainLayout->addWidget(txtButton);
    mainLayout->addWidget(jsonButton);
    mainLayout->addWidget(xmlButton);
    mainLayout->addWidget(zipButton);

    mainLayout->addStretch();

    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);

    diskInfoWindow = new DiskInfoWindow(this);
    txtWindow = new TXTWindow(this);
    jsonWindow = new JSONWindow(this);
    xmlWindow = new XMLWindow(this);
    zipWindow = new ZIPWindow(this);

    stackedWidget->addWidget(mainWidget); // 0
    stackedWidget->addWidget(diskInfoWindow); // 1
    stackedWidget->addWidget(txtWindow); // 2
    stackedWidget->addWidget(jsonWindow); // 3
    stackedWidget->addWidget(xmlWindow); // 4
    stackedWidget->addWidget(zipWindow); // 5

    stackedWidget->setCurrentIndex(0);
}

void MainWindow::setUpConnections () {
    connect(diskButton, &QPushButton::clicked, this, &MainWindow::onDISKButtonClicked);
    connect(txtButton, &QPushButton::clicked, this, &MainWindow::onTXTButtonClicked);
    connect(jsonButton, &QPushButton::clicked, this, &MainWindow::onJSONButtonClicked);
    connect(xmlButton, &QPushButton::clicked, this, &MainWindow::onXMLButtonClicked);
    connect(zipButton, &QPushButton::clicked, this, &MainWindow::onZIPButtonClicked);

    // Connect for back main window
    connect(diskInfoWindow, &DiskInfoWindow::backToMain, this, [this]() {
        stackedWidget->setCurrentIndex(0);
    });

    connect(txtWindow, &TXTWindow::backToMain, this, [this]() {
        stackedWidget->setCurrentIndex(0);
    });

    connect(jsonWindow, &JSONWindow::backToMain, this, [this]() {
        stackedWidget->setCurrentIndex(0);
    });

    connect(xmlWindow, &XMLWindow::backToMain, this, [this]() {
        stackedWidget->setCurrentIndex(0);
    });

    connect(zipWindow, &ZIPWindow::backToMain, this, [this]() {
        stackedWidget->setCurrentIndex(0);
    });
}

void MainWindow::setHotKey () {
    new QShortcut(QKeySequence(Qt::Key_1), this, [this]() {
        onDISKButtonClicked();
    });

    new QShortcut(QKeySequence(Qt::Key_2), this, [this]() {
        onTXTButtonClicked();
    });

    new QShortcut(QKeySequence(Qt::Key_3), this, [this]() {
        onJSONButtonClicked();
    });

    new QShortcut(QKeySequence(Qt::Key_4), this, [this]() {
        onXMLButtonClicked();
    });

    new QShortcut(QKeySequence(Qt::Key_5), this, [this]() {
        onZIPButtonClicked();
    });
} 

void MainWindow::onDISKButtonClicked() {
    stackedWidget->setCurrentIndex(1);
    qDebug() << "DISK button clicked";
}

void MainWindow::onTXTButtonClicked() {
    stackedWidget->setCurrentIndex(2);
    qDebug() << "TXT button clicked";
}

void MainWindow::onJSONButtonClicked() {
    stackedWidget->setCurrentIndex(3);
    qDebug() << "JSON button clicked";
}

void MainWindow::onXMLButtonClicked() {
    stackedWidget->setCurrentIndex(4);
    qDebug() << "XML button clicked";
}

void MainWindow::onZIPButtonClicked() {
    stackedWidget->setCurrentIndex(5);
    qDebug() << "ZIP button clicked";
}