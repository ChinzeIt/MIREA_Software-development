#include "txtwindow.h"
// Main
TXTWindow::TXTWindow (QWidget* parent) : 
QWidget(parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void TXTWindow::setUpUI () {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFont font;
    font.setFamily("Arial");
    font.setPointSize(30);
    font.setBold(true);

    createButton = new QPushButton("Create TXT", this);
    readButton = new QPushButton("Read TXT", this);
    editButton = new QPushButton("Edit TXT", this);
    removeButton = new QPushButton("Remove TXT", this);
    backButton = new QPushButton("BACK", this);

    createButton->setFont(font);
    readButton->setFont(font);
    editButton->setFont(font);
    removeButton->setFont(font);
    backButton->setFont(font);

    createButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    readButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    editButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    removeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    mainLayout->addWidget(createButton);
    mainLayout->addWidget(readButton);
    mainLayout->addWidget(editButton);
    mainLayout->addWidget(removeButton);
    mainLayout->addWidget(backButton);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(20, 20, 20, 20);
}

void TXTWindow::setUpConnections () {
    connect(createButton, &QPushButton::clicked, this, &TXTWindow::onCreateButton);
    connect(readButton, &QPushButton::clicked, this, &TXTWindow::onReadButton);
    connect(editButton, &QPushButton::clicked, this, &TXTWindow::onEditButton);
    connect(removeButton, &QPushButton::clicked, this, &TXTWindow::onRemoveButton);
    connect(backButton, &QPushButton::clicked, this, &TXTWindow::onBackButton);
}

void TXTWindow::setHotKey() {
    new QShortcut(QKeySequence(Qt::Key_1), this, [this]() {
        onCreateButton();
    });
    new QShortcut(QKeySequence(Qt::Key_2), this, [this]() {
        onReadButton();
    });

    new QShortcut(QKeySequence(Qt::Key_3), this, [this]() {
        onEditButton();
    });

    new QShortcut(QKeySequence(Qt::Key_4), this, [this]() {
        onRemoveButton();
    });

    new QShortcut(QKeySequence(Qt::Key_Escape), this, [this]() {
        onBackButton();
    });
}

void TXTWindow::onCreateButton() {
    emit createTXT();
    qDebug() << "create txt";
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

// Create TXT
TXTWindowCreate::TXTWindowCreate (QWidget* parent) :
QWidget (parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void TXTWindowCreate::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    putPath->setEnabled(true);
    pathCreate->setEnabled(true);

    putPath->clear();
    putPath->show();

    pathCreate->show();

    pathError->hide();
    pathOk->hide();

    putPath->setFocus();
}

bool TXTWindowCreate::eventFilter(QObject* obj, QEvent* event) {
    if (obj == putPath && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            putPath->clearFocus();
            onCreateButton();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void TXTWindowCreate::setUpUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFont fontPath;
    fontPath.setFamily("Arial");
    fontPath.setPointSize(36);
    fontPath.setBold(true);

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

    pathCreate = new QPushButton("↵", this);
    pathCreate->setFont(fontPath);
    pathCreate->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pathCreate->setFixedHeight(60);

    pathError = new QLabel(this);
    pathError->setStyleSheet("color: red;");
    pathError->setWordWrap(true);

    pathOk = new QLabel(this);
    pathOk->setStyleSheet("color: green;");
    pathOk->setWordWrap(true);
    pathOk->setText("File succes create");

    QHBoxLayout* pathLayout = new QHBoxLayout();
    pathLayout->addWidget(putPath);
    pathLayout->addWidget(pathCreate);
    pathLayout->setStretch(0, 3);
    pathLayout->setStretch(1, 1);

    QVBoxLayout* pathLayoutWithError = new QVBoxLayout();
    pathLayoutWithError->addLayout(pathLayout);
    pathLayoutWithError->addWidget(pathError);
    pathLayoutWithError->addWidget(pathOk);

    uButton = new QPushButton("↻", this);
    backUpButton = new QPushButton("⬆", this);
    backButton = new QPushButton("BACK", this);
    uButton->setFont(font);
    backUpButton->setFont(font);
    backButton->setFont(font);
    uButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    backUpButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
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

    mainLayout->addLayout(pathLayoutWithError);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    mainLayout->setContentsMargins(20, 20, 20, 20);
}

void TXTWindowCreate::setUpConnections() {
    connect(pathCreate, &QPushButton::clicked, this, &TXTWindowCreate::onCreateButton);
    connect(uButton, &QPushButton::clicked, this, &TXTWindowCreate::onUButton);
    connect(backUpButton, &QPushButton::clicked, this, &TXTWindowCreate::onBackUpButton);
    connect(backButton, &QPushButton::clicked, this, &TXTWindowCreate::onBackButton);
}

void TXTWindowCreate::setHotKey() {
    new QShortcut(QKeySequence(Qt::Key_Return), this, [this]() {
        onCreateButton();
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

void TXTWindowCreate::onCreateButton() {
    if (isValidPath()) {
        pathError->hide();
        pathOk->show();

        touchFile();

        putPath->setFocus();
        QTextCursor cursor = putPath->textCursor();
        cursor.movePosition(QTextCursor::End);
        putPath->setTextCursor(cursor);
    } else {
        pathOk->hide();
        pathError->show();

        putPath->setFocus();
        QTextCursor cursor = putPath->textCursor();
        cursor.movePosition(QTextCursor::End);
        putPath->setTextCursor(cursor);
    }

    qDebug() << "path button clicked";
}

void TXTWindowCreate::onUButton() {
    putPath->clear();
    putPath->setFocus();
    pathError->clear();
    pathError->hide();
    pathOk->hide();

    qDebug() << "u button clicked";
}

void TXTWindowCreate::onBackUpButton() {
    emit backUp();
    qDebug() << "back button clicked";
}

void TXTWindowCreate::onBackButton() {
    emit backToMain();
    qDebug() << "back button clicked";
}

bool TXTWindowCreate::isValidPath() {
    pathError->clear();
    QString pathStr = putPath->toPlainText();  

    if (checkerPath.checking(pathStr.toStdString())) {
        qDebug() << "valid path";
        return true;
    } else {
        pathError->setText(QString::fromStdString(checkerPath.error()));
        qDebug() << "no valid path";
        return false;
    }
}

void TXTWindowCreate::touchFile() {
    pathError->clear();
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".txt", Qt::CaseInsensitive))
        pathStr += ".txt";

    if (!crFile.createFile(pathStr.toStdString())) {
        pathError->setText("Dont touch file");
        pathError->show();
    }
}

// Read TXT
TXTWindowRead::TXTWindowRead (QWidget* parent) :
QWidget (parent) {
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

    pathError->hide();

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

    pathError = new QLabel(this);
    pathError->setStyleSheet("color: red;");
    pathError->setWordWrap(true);

    QHBoxLayout* pathLayout = new QHBoxLayout();
    pathLayout->addWidget(putPath);
    pathLayout->addWidget(pathButton);
    pathLayout->setStretch(0, 3);
    pathLayout->setStretch(1, 1);

    QVBoxLayout* pathLayoutWithError = new QVBoxLayout();
    pathLayoutWithError->addLayout(pathLayout);
    pathLayoutWithError->addWidget(pathError);

    textInfo = new QTextEdit();
    textInfo->setReadOnly(true);
    textInfo->setFont(fontText);
    textInfo->hide();

    uButton = new QPushButton("↻", this);
    backUpButton = new QPushButton("⬆", this);
    backButton = new QPushButton("BACK", this);
    uButton->setFont(font);
    backUpButton->setFont(font);
    backButton->setFont(font);
    uButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    backUpButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
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

    mainLayout->addLayout(pathLayoutWithError);
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

void TXTWindowRead::onPathButton() {
    if (isValidPath()) {
        putPath->setEnabled(false);
        pathButton->setEnabled(false);
        putPath->hide();
        pathButton->hide();
        pathError->hide();

        loadInformation();

        textInfo->show();
    } else {
        pathError->show();

        putPath->setFocus();
        QTextCursor cursor = putPath->textCursor();
        cursor.movePosition(QTextCursor::End);
        putPath->setTextCursor(cursor);
    }

    qDebug() << "path button clicked";
}

void TXTWindowRead::onUButton() {
    if (putPath->isVisible() && putPath->isEnabled()) {
        putPath->clear();
        putPath->setFocus();
        pathError->hide();
    } else {
        loadInformation();
    }
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

void TXTWindowRead::loadInformation() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".txt", Qt::CaseInsensitive))
        pathStr += ".txt";
    
    QString text = QString::fromStdString(readTXT.readFile(pathStr.toStdString()));
    textInfo->setText(text);
}

bool TXTWindowRead::isValidPath() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".txt", Qt::CaseInsensitive))
        pathStr += ".txt";

    if (checkerPath.checking(pathStr.toStdString(), PathAccessMode::Read)) {
        qDebug() << "valid path";
        return true;
    } else {
        pathError->setText(QString::fromStdString(checkerPath.error()));
        qDebug() << "no valid path";
        return false;
    }
}

TXTWindowEdit::TXTWindowEdit (QWidget* parent) :
QWidget (parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void TXTWindowEdit::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    putPath->setEnabled(true);
    pathButton->setEnabled(true);

    putPath->clear();
    putPath->show();

    pathButton->show();

    pathError->hide();

    textEdit->clear();
    textEdit->hide();

    putPath->setFocus();
}

bool TXTWindowEdit::eventFilter(QObject* obj, QEvent* event) {
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

void TXTWindowEdit::setUpUI() {
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

    pathError = new QLabel(this);
    pathError->setStyleSheet("color: red;");
    pathError->setWordWrap(true);

    QHBoxLayout* pathLayout = new QHBoxLayout();
    pathLayout->addWidget(putPath);
    pathLayout->addWidget(pathButton);
    pathLayout->setStretch(0, 3);
    pathLayout->setStretch(1, 1);

    QVBoxLayout* pathLayoutWithError = new QVBoxLayout();
    pathLayoutWithError->addLayout(pathLayout);
    pathLayoutWithError->addWidget(pathError);

    textEdit = new QTextEdit();
    textEdit->setFont(fontText);
    textEdit->hide();

    uButton = new QPushButton("↻", this);
    backUpButton = new QPushButton("⬆", this);
    backButton = new QPushButton("BACK", this);
    uButton->setFont(font);
    backUpButton->setFont(font);
    backButton->setFont(font);
    uButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    backUpButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
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

    mainLayout->addLayout(pathLayoutWithError);
    mainLayout->addStretch(); 
    mainLayout->addWidget(textEdit);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setContentsMargins(20, 20, 20, 20);
}

void TXTWindowEdit::setUpConnections() {
    connect(pathButton, &QPushButton::clicked, this, &TXTWindowEdit::onPathButton);
    connect(uButton, &QPushButton::clicked, this, &TXTWindowEdit::onUButton);
    connect(backUpButton, &QPushButton::clicked, this, &TXTWindowEdit::onBackUpButton);
    connect(backButton, &QPushButton::clicked, this, &TXTWindowEdit::onBackButton);
}

void TXTWindowEdit::setHotKey() {
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

void TXTWindowEdit::onPathButton() {
    if (isValidPath()) {
        QString pathStr = putPath->toPlainText();
        if (!pathStr.endsWith(".txt", Qt::CaseInsensitive))
            pathStr += ".txt";

        try {
            editTXT = std::make_unique<QTEditFileService>(pathStr);
        } catch (const std::runtime_error& e) {
            pathError->clear();
            pathError->setText(e.what());
            pathError->show();
            return;
        }

        putPath->setEnabled(false);
        pathButton->setEnabled(false);
        putPath->hide();
        pathButton->hide();
        pathError->hide();

        editfile();

        textEdit->show();
    } else {
        pathError->show();

        putPath->setFocus();
        QTextCursor cursor = putPath->textCursor();
        cursor.movePosition(QTextCursor::End);
        putPath->setTextCursor(cursor);
    }

    qDebug() << "path button clicked";
}

void TXTWindowEdit::onUButton() {
    if (putPath->isVisible() && putPath->isEnabled()) {
        putPath->clear();
        putPath->setFocus();
        pathError->hide();
    } else {
        savefile();
    }
    qDebug() << "u button clicked";
}

void TXTWindowEdit::onBackUpButton() {
    editTXT.reset();
    emit backUp();
    qDebug() << "back button clicked";
}

void TXTWindowEdit::onBackButton() {
    editTXT.reset();
    emit backToMain();
    qDebug() << "back button clicked";
}

void TXTWindowEdit::editfile() {
    QString text = QString::fromStdString(editTXT->getText());
    textEdit->setText(text);

    textEdit->setFocus();
    QTextCursor cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    textEdit->setTextCursor(cursor);
}

void TXTWindowEdit::savefile() {
    editTXT->setText(textEdit->toPlainText().toStdString());
    qDebug() << "work save";
}

bool TXTWindowEdit::isValidPath() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".txt", Qt::CaseInsensitive))
        pathStr += ".txt";

    if (checkerPath.checking(pathStr.toStdString(), PathAccessMode::Write)) {
        qDebug() << "valid path";
        return true;
    } else {
        pathError->setText(QString::fromStdString(checkerPath.error()));
        qDebug() << "no valid path";
        return false;
    }
}