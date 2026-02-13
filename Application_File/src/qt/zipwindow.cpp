#include "zipwindow.h"

ZIPWindow::ZIPWindow (QWidget* parent) : 
QWidget(parent) {
    setUpUI();
    setUpConnections();
}

void ZIPWindow::setUpUI () {
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

void ZIPWindow::setUpConnections () {
    connect(backButton, &QPushButton::clicked, this, &ZIPWindow::backToMain);
}