#include "diskinfowindow.h"

DiskInfoWindow::DiskInfoWindow (QWidget* parent) : 
QWidget(parent),
backButton(nullptr) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void DiskInfoWindow::setUpUI () {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFont fontText;
    fontText.setFamily("Arial");
    fontText.setPointSize(16);
    fontText.setBold(true);

    QFont font;
    font.setFamily("Arial");
    font.setPointSize(30);
    font.setBold(true);

    textInfo = new QTextEdit();
    textInfo->setReadOnly(true);
    textInfo->setFont(fontText);

    loadInformation();

    uButton = new QPushButton("↻", this);
    backButton = new QPushButton("BACK", this);
    uButton->setFont(font);
    backButton->setFont(font);
    uButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    uButton->setToolTip("Update text information about DISK");

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(uButton);
    buttonLayout->addWidget(backButton);
    buttonLayout->setStretch(0, 1);
    buttonLayout->setStretch(1, 4);

    mainLayout->addWidget(textInfo);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 0);
    mainLayout->setContentsMargins(20, 20, 20, 20);
}

void DiskInfoWindow::setUpConnections () {
    connect(uButton, &QPushButton::clicked, this, &DiskInfoWindow::onUButton);
    connect(backButton, &QPushButton::clicked, this, &DiskInfoWindow::onBackButton);
}

void DiskInfoWindow::setHotKey () {
    new QShortcut(QKeySequence(Qt::Key_U), this, [this]() {
        onUButton();
    });
    new QShortcut(QKeySequence(Qt::Key_Escape), this, [this]() {
        onBackButton();
    });
}

void DiskInfoWindow::onUButton () {
    loadInformation();
    qDebug() << "u button clicked";
}

void DiskInfoWindow::onBackButton () {
    emit backToMain();
    qDebug() << "back button clicked";
}

void DiskInfoWindow::loadInformation () {
    information.setDisk();
    information.setInfoLine();
    textInfo->setText(QString::fromStdString(information.getInfoLine()));
}