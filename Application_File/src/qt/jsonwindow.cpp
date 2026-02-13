#include "jsonwindow.h"

JSONWindow::JSONWindow (QWidget* parent) : 
QWidget(parent) {
    setUpUI();
    setUpConnections();
}

void JSONWindow::setUpUI () {
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

void JSONWindow::setUpConnections () {
    connect(backButton, &QPushButton::clicked, this, &JSONWindow::backToMain);
}