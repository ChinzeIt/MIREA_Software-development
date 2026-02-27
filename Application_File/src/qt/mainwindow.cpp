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

    // Window TXT
    txtWindowCreate = new TXTWindowCreate(this);
    txtWindowRead = new TXTWindowRead(this);
    txtWindowEdit = new TXTWindowEdit(this);
    txtWindowRemove = new TXTWindowRemove(this);
    jsonWindowCreate = new JSONWindowCreate(this);
    jsonWindowRead = new JSONWindowRead(this);
    jsonWindowEdit = new JSONWindowEdit(this);
    jsonWindowRemove = new JSONWindowRemove(this);
    xmlWindowCreate = new XMLWindowCreate(this);
    xmlWindowRead = new XMLWindowRead(this);
    xmlWindowEdit = new XMLWindowEdit(this);
    xmlWindowRemove = new XMLWindowRemove(this);
    stackedWidget->addWidget(txtWindowCreate); // 6
    stackedWidget->addWidget(txtWindowRead); // 7
    stackedWidget->addWidget(txtWindowEdit); // 8
    stackedWidget->addWidget(txtWindowRemove); // 9
    stackedWidget->addWidget(jsonWindowCreate); // 10
    stackedWidget->addWidget(jsonWindowRead); // 11
    stackedWidget->addWidget(jsonWindowEdit); // 12
    stackedWidget->addWidget(jsonWindowRemove); // 13
    stackedWidget->addWidget(xmlWindowCreate); // 14
    stackedWidget->addWidget(xmlWindowRead);// 15
    stackedWidget->addWidget(xmlWindowEdit); // 16
    stackedWidget->addWidget(xmlWindowRemove); // 17

    stackedWidget->setCurrentIndex(0);
}

void MainWindow::setUpConnections () {
    connect(diskButton, &QPushButton::clicked, this, &MainWindow::onDISKButtonClicked);
    connect(txtButton, &QPushButton::clicked, this, &MainWindow::onTXTButtonClicked);
    connect(jsonButton, &QPushButton::clicked, this, &MainWindow::onJSONButtonClicked);
    connect(xmlButton, &QPushButton::clicked, this, &MainWindow::onXMLButtonClicked);
    connect(zipButton, &QPushButton::clicked, this, &MainWindow::onZIPButtonClicked);

    // Connect for back main window
    connect(diskInfoWindow, &DiskInfoWindow::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });

    connect(txtWindow, &TXTWindow::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });

    connect(jsonWindow, &JSONWindow::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });

    connect(xmlWindow, &XMLWindow::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });

    connect(zipWindow, &ZIPWindow::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });

    // Connect txt lower widget
    connect(txtWindow, &TXTWindow::createTXT, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(6);
    });
    connect(txtWindowCreate, &TXTWindowCreate::backUp, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(2);
    });
    connect(txtWindowCreate, &TXTWindowCreate::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });
    connect(txtWindow, &TXTWindow::readTXT, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(7);
    });
    connect(txtWindowRead, &TXTWindowRead::backUp, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(2);
    });
    connect(txtWindowRead, &TXTWindowRead::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });
    connect(txtWindow, &TXTWindow::editTXT, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(8);
    });
    connect(txtWindowEdit, &TXTWindowEdit::backUp, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(2);
    });
    connect(txtWindowEdit, &TXTWindowEdit::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });
    connect(txtWindow, &TXTWindow::removeTXT, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(9);
    });
    connect(txtWindowRemove, &TXTWindowRemove::backUp, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(2);
    });
    connect(txtWindowRemove, &TXTWindowRemove::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });

    // Connect json lower widget
    connect(jsonWindow, &JSONWindow::createJSON, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(10);
    });
    connect(jsonWindowCreate, &JSONWindowCreate::backUp, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(3);
    });
    connect(jsonWindowCreate, &JSONWindowCreate::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });
    connect(jsonWindow, &JSONWindow::readJSON, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(11);
    });
    connect(jsonWindowRead, &JSONWindowRead::backUp, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(3);
    });
    connect(jsonWindowRead, &JSONWindowRead::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });
    connect(jsonWindow, &JSONWindow::editJSON, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(12);
    });
    connect(jsonWindowEdit, &JSONWindowEdit::backUp, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(3);
    });
    connect(jsonWindowEdit, &JSONWindowEdit::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });
    connect(jsonWindow, &JSONWindow::removeJSON, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(13);
    });
    connect(jsonWindowRemove, &JSONWindowRemove::backUp, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(3);
    });
    connect(jsonWindowRemove, &JSONWindowRemove::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });

    // Connect XML lower window
    connect(xmlWindow, &XMLWindow::createXML, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(14);
    });
    connect(xmlWindowCreate, &XMLWindowCreate::backUp, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(4);
    });
    connect(xmlWindowCreate, &XMLWindowCreate::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });
    connect(xmlWindow, &XMLWindow::readXML, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(15);
    });
    connect(xmlWindowRead, &XMLWindowRead::backUp, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(4);
    });
    connect(xmlWindowRead, &XMLWindowRead::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });
    connect(xmlWindow, &XMLWindow::editXML, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(16);
    });
    connect(xmlWindowEdit, &XMLWindowEdit::backUp, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(4);
    });
    connect(xmlWindowEdit, &XMLWindowEdit::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });
    connect(xmlWindow, &XMLWindow::removeXML, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(17);
    });
    connect(xmlWindowRemove, &XMLWindowRemove::backUp, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(4);
    });
    connect(xmlWindowRemove, &XMLWindowRemove::backToMain, mainWidget, [this]() {
        stackedWidget->setCurrentIndex(0);
    });
}

void MainWindow::setHotKey () {
    new QShortcut(QKeySequence(Qt::Key_1), mainWidget, [this]() {
        onDISKButtonClicked();
    });

    new QShortcut(QKeySequence(Qt::Key_2), mainWidget, [this]() {
        onTXTButtonClicked();
    });

    new QShortcut(QKeySequence(Qt::Key_3), mainWidget, [this]() {
        onJSONButtonClicked();
    });

    new QShortcut(QKeySequence(Qt::Key_4), mainWidget, [this]() {
        onXMLButtonClicked();
    });

    new QShortcut(QKeySequence(Qt::Key_5), mainWidget, [this]() {
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