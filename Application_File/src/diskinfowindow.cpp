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

    QFont font;
    font.setFamily("Arial");
    font.setPointSize(30);
    font.setBold(true);

    backButton = new QPushButton("BACK", this);

    backButton->setFont(font);

    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    mainLayout->addStretch();
    
    mainLayout->addWidget(backButton);

    mainLayout->setContentsMargins(20, 20, 20, 20);
}

void DiskInfoWindow::setUpConnections () {
    connect(backButton, &QPushButton::clicked, this, &DiskInfoWindow::backToMain);
}

void DiskInfoWindow::setHotKey () {
    new QShortcut(QKeySequence(Qt::Key_Escape), this, [this]() {
        emit backToMain();
    });
}