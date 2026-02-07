#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
QMainWindow (parent),
txtButton (nullptr),
jsonButton (nullptr),
xmlButton (nullptr),
zipButton (nullptr) {
    setUpUI();
    setUpConnections();

    this->setWindowTitle("Application File");

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(WIDTH, HEIGHT);
}

void MainWindow::setUpUI () {
    QWidget* centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QFont font;
    font.setFamily("Arial");
    font.setPointSize(30);
    font.setBold(true);

    diskButton = new QPushButton("DISK INFO", centralWidget);
    txtButton = new QPushButton("TXT files", centralWidget);
    jsonButton = new QPushButton("JSON files", centralWidget);
    xmlButton = new QPushButton("XML files", centralWidget);
    zipButton = new QPushButton("ZIP files", centralWidget);

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
}

void MainWindow::setUpConnections () {
    connect(diskButton, &QPushButton::clicked, this, &MainWindow::onDISKButtonClicked);
    connect(txtButton, &QPushButton::clicked, this, &MainWindow::onTXTButtonClicked);
    connect(jsonButton, &QPushButton::clicked, this, &MainWindow::onJSONButtonClicked);
    connect(xmlButton, &QPushButton::clicked, this, &MainWindow::onXMLButtonClicked);
    connect(zipButton, &QPushButton::clicked, this, &MainWindow::onZIPButtonClicked);
}

void MainWindow::onDISKButtonClicked() {
    qDebug() << "DISK button clicked";
}

void MainWindow::onTXTButtonClicked() {
    qDebug() << "TXT button clicked";
}

void MainWindow::onJSONButtonClicked() {
    qDebug() << "JSON button clicked";
}

void MainWindow::onXMLButtonClicked() {
    qDebug() << "XML button clicked";
}

void MainWindow::onZIPButtonClicked() {
    qDebug() << "ZIP button clicked";
}