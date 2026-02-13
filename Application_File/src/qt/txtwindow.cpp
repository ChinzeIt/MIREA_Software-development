#include "txtwindow.h"
// Main
TXTWindow::TXTWindow (QWidget* parent) : 
QWidget(parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void TXTWindowRead::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    putPath->setEnabled(true);
    pathButton->setEnabled(true);

    putPath->clear();
    putPath->show();

    pathButton->show();

    textInfo->clear();
    textInfo->hide();

    putPath->setFocus();
}

bool TXTWindowRead::eventFilter(QObject* obj, QEvent* event) {
    if (obj == putPath && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            putPath->clearFocus();
            onPathButton();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void TXTWindow::setUpUI () {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFont font;
    font.setFamily("Arial");
    font.setPointSize(30);
    font.setBold(true);

    readButton = new QPushButton("Read TXT", this);
    editButton = new QPushButton("Edit TXT", this);
    removeButton = new QPushButton("Remove TXT", this);
    backButton = new QPushButton("BACK", this);

    readButton->setFont(font);
    editButton->setFont(font);
    removeButton->setFont(font);
    backButton->setFont(font);

    readButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    editButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    removeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    mainLayout->addWidget(readButton);
    mainLayout->addWidget(editButton);
    mainLayout->addWidget(removeButton);
    mainLayout->addWidget(backButton);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(20, 20, 20, 20);
}

void TXTWindow::setUpConnections () {
    connect(readButton, &QPushButton::clicked, this, &TXTWindow::onReadButton);
    connect(editButton, &QPushButton::clicked, this, &TXTWindow::onEditButton);
    connect(removeButton, &QPushButton::clicked, this, &TXTWindow::onRemoveButton);
    connect(backButton, &QPushButton::clicked, this, &TXTWindow::onBackButton);
}

void TXTWindow::setHotKey() {
    new QShortcut(QKeySequence(Qt::Key_1), this, [this]() {
        onReadButton();
    });

    new QShortcut(QKeySequence(Qt::Key_2), this, [this]() {
        onEditButton();
    });

    new QShortcut(QKeySequence(Qt::Key_3), this, [this]() {
        onRemoveButton();
    });

    new QShortcut(QKeySequence(Qt::Key_Escape), this, [this]() {
        onBackButton();
    });
}

void TXTWindow::onReadButton() {
    emit readTXT();
    qDebug() << "read txt";
}

void TXTWindow::onEditButton() {
    emit editTXT();
    qDebug() << "edit txt";
}

void TXTWindow::onRemoveButton() {
    emit removeTXT();
    qDebug() << "remove txt";
}

void TXTWindow::onBackButton() {
    emit backToMain();
    qDebug() << "back button press";
}

// Read TXT
TXTWindowRead::TXTWindowRead (QWidget* parent) :
QWidget (parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void TXTWindowRead::setUpUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFont fontPath;
    fontPath.setFamily("Arial");
    fontPath.setPointSize(36);
    fontPath.setBold(true);

    QFont fontText;
    fontText.setFamily("Arial");
    fontText.setPointSize(16);
    fontText.setBold(true);

    QFont font;
    font.setFamily("Arial");
    font.setPointSize(30);
    font.setBold(true);

    putPath = new QTextEdit(this);
    putPath->installEventFilter(this);
    putPath->setFont(fontPath);
    putPath->setPlaceholderText("Enter path");
    putPath->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    putPath->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    putPath->setFixedHeight(60);

    pathButton = new QPushButton("↵", this);
    pathButton->setFont(fontPath);
    pathButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pathButton->setFixedHeight(60);

    QHBoxLayout* pathLayout = new QHBoxLayout();
    pathLayout->addWidget(putPath);
    pathLayout->addWidget(pathButton);
    pathLayout->setStretch(0, 3);
    pathLayout->setStretch(1, 1);

    textInfo = new QTextEdit();
    textInfo->setReadOnly(true);
    textInfo->setFont(fontText);
    textInfo->hide();

    loadInformation();

    uButton = new QPushButton("↻", this);
    backUpButton = new QPushButton("⬆", this);
    backButton = new QPushButton("BACK", this);
    uButton->setFont(font);
    backUpButton->setFont(font);
    backButton->setFont(font);
    uButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    backUpButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    uButton->setToolTip("Update text information about DISK");
    backUpButton->setToolTip("Back one step");
    uButton->setFixedHeight(60);
    backUpButton->setFixedHeight(60);
    backButton->setFixedHeight(60);


    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(uButton);
    buttonLayout->addWidget(backUpButton);
    buttonLayout->addWidget(backButton);
    buttonLayout->setStretch(0, 1);
    buttonLayout->setStretch(1, 1);
    buttonLayout->setStretch(2, 2);

    mainLayout->addLayout(pathLayout);
    mainLayout->addStretch(); 
    mainLayout->addWidget(textInfo);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setContentsMargins(20, 20, 20, 20);
}

void TXTWindowRead::setUpConnections() {
    connect(pathButton, &QPushButton::clicked, this, &TXTWindowRead::onPathButton);
    connect(uButton, &QPushButton::clicked, this, &TXTWindowRead::onUButton);
    connect(backUpButton, &QPushButton::clicked, this, &TXTWindowRead::onBackUpButton);
    connect(backButton, &QPushButton::clicked, this, &TXTWindowRead::onBackButton);
}

void TXTWindowRead::setHotKey() {
    new QShortcut(QKeySequence(Qt::Key_Return), this, [this]() {
        onPathButton();
    });
    new QShortcut(QKeySequence(Qt::Key_U), this, [this]() {
        onUButton();
    });
    new QShortcut(QKeySequence(Qt::Key_B), this, [this]() {
        onBackUpButton();
    });
    new QShortcut(QKeySequence(Qt::Key_Escape), this, [this]() {
        onBackButton();
    });
}

void TXTWindowRead::onPathButton() { // Подвезти core
    putPath->setEnabled(false);
    pathButton->setEnabled(false);
    putPath->hide();
    pathButton->hide();

    loadInformation();
    textInfo->show();
    qDebug() << "path button clicked";
}

void TXTWindowRead::onUButton() {
    loadInformation();
    qDebug() << "u button clicked";
}

void TXTWindowRead::onBackUpButton() {
    emit backUp();
    qDebug() << "back button clicked";
}

void TXTWindowRead::onBackButton() {
    emit backToMain();
    qDebug() << "back button clicked";
}

void TXTWindowRead::loadInformation() { // Подвезти core

}