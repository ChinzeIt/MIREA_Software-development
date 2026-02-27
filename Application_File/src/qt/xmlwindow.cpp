#include "xmlwindow.h"

XMLWindow::XMLWindow (QWidget* parent) : 
QWidget(parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void XMLWindow::setUpUI () {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFont font;
    font.setFamily("Arial");
    font.setPointSize(30);
    font.setBold(true);

    createButton = new QPushButton("Create XML", this);
    readButton = new QPushButton("Read XML", this);
    editButton = new QPushButton("Edit XML", this);
    removeButton = new QPushButton("Remove XML", this);
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

void XMLWindow::setUpConnections () {
    connect(createButton, &QPushButton::clicked, this, &XMLWindow::onCreateButton);
    connect(readButton, &QPushButton::clicked, this, &XMLWindow::onReadButton);
    connect(editButton, &QPushButton::clicked, this, &XMLWindow::onEditButton);
    connect(removeButton, &QPushButton::clicked, this, &XMLWindow::onRemoveButton);
    connect(backButton, &QPushButton::clicked, this, &XMLWindow::onBackButton);
}

void XMLWindow::setHotKey() {
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

void XMLWindow::onCreateButton() {
    emit createXML();
    qDebug() << "create XML";
}

void XMLWindow::onReadButton() {
    emit readXML();
    qDebug() << "read XML";
}

void XMLWindow::onEditButton() {
    emit editXML();
    qDebug() << "edit XML";
}

void XMLWindow::onRemoveButton() {
    emit removeXML();
    qDebug() << "remove XML";
}

void XMLWindow::onBackButton() {
    emit backToMain();
    qDebug() << "back button press";
}

// Create TXT
XMLWindowCreate::XMLWindowCreate (QWidget* parent) :
QWidget (parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void XMLWindowCreate::showEvent(QShowEvent* event) {
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

bool XMLWindowCreate::eventFilter(QObject* obj, QEvent* event) {
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

void XMLWindowCreate::setUpUI() {
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

void XMLWindowCreate::setUpConnections() {
    connect(pathCreate, &QPushButton::clicked, this, &XMLWindowCreate::onCreateButton);
    connect(uButton, &QPushButton::clicked, this, &XMLWindowCreate::onUButton);
    connect(backUpButton, &QPushButton::clicked, this, &XMLWindowCreate::onBackUpButton);
    connect(backButton, &QPushButton::clicked, this, &XMLWindowCreate::onBackButton);
}

void XMLWindowCreate::setHotKey() {
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

void XMLWindowCreate::onCreateButton() {
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

void XMLWindowCreate::onUButton() {
    putPath->clear();
    putPath->setFocus();
    pathError->clear();
    pathError->hide();
    pathOk->hide();

    qDebug() << "u button clicked";
}

void XMLWindowCreate::onBackUpButton() {
    emit backUp();
    qDebug() << "back button clicked";
}

void XMLWindowCreate::onBackButton() {
    emit backToMain();
    qDebug() << "back button clicked";
}

bool XMLWindowCreate::isValidPath() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".xml", Qt::CaseInsensitive))
        pathStr += ".xml";

    if (checkerPath.checking(pathStr.toStdString(), PathAccessMode::Create)) {
        qDebug() << "valid path";
        return true;
    } else {
        pathError->setText(QString::fromStdString(checkerPath.error()));
        qDebug() << "no valid path";
        return false;
    }
}

void XMLWindowCreate::touchFile() {
    pathError->clear();
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".xml", Qt::CaseInsensitive))
        pathStr += ".xml";

    if (!crFile.createFile(pathStr.toStdString())) {
        pathError->setText("Dont touch file");
        pathError->show();
    }
}

// Read TXT
XMLWindowRead::XMLWindowRead (QWidget* parent) :
QWidget (parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void XMLWindowRead::showEvent(QShowEvent* event) {
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

bool XMLWindowRead::eventFilter(QObject* obj, QEvent* event) {
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

void XMLWindowRead::setUpUI() {
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

void XMLWindowRead::setUpConnections() {
    connect(pathButton, &QPushButton::clicked, this, &XMLWindowRead::onPathButton);
    connect(uButton, &QPushButton::clicked, this, &XMLWindowRead::onUButton);
    connect(backUpButton, &QPushButton::clicked, this, &XMLWindowRead::onBackUpButton);
    connect(backButton, &QPushButton::clicked, this, &XMLWindowRead::onBackButton);
}

void XMLWindowRead::setHotKey() {
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

void XMLWindowRead::onPathButton() {
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

void XMLWindowRead::onUButton() {
    if (putPath->isVisible() && putPath->isEnabled()) {
        putPath->clear();
        putPath->setFocus();
        pathError->hide();
    } else {
        loadInformation();
    }
    qDebug() << "u button clicked";
}

void XMLWindowRead::onBackUpButton() {
    emit backUp();
    qDebug() << "back button clicked";
}

void XMLWindowRead::onBackButton() {
    emit backToMain();
    qDebug() << "back button clicked";
}

void XMLWindowRead::loadInformation() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".xml", Qt::CaseInsensitive))
        pathStr += ".xml";
    
    QString text = QString::fromStdString(readXML.readFile(pathStr.toStdString()));
    textInfo->setText(text);
}

bool XMLWindowRead::isValidPath() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".xml", Qt::CaseInsensitive))
        pathStr += ".xml";

    if (checkerPath.checking(pathStr.toStdString(), PathAccessMode::Read)) {
        qDebug() << "valid path";
        return true;
    } else {
        pathError->setText(QString::fromStdString(checkerPath.error()));
        qDebug() << "no valid path";
        return false;
    }
}

XMLWindowEdit::XMLWindowEdit (QWidget* parent) :
QWidget (parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void XMLWindowEdit::showEvent(QShowEvent* event) {
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

bool XMLWindowEdit::eventFilter(QObject* obj, QEvent* event) {
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

void XMLWindowEdit::setUpUI() {
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

void XMLWindowEdit::setUpConnections() {
    connect(pathButton, &QPushButton::clicked, this, &XMLWindowEdit::onPathButton);
    connect(uButton, &QPushButton::clicked, this, &XMLWindowEdit::onUButton);
    connect(backUpButton, &QPushButton::clicked, this, &XMLWindowEdit::onBackUpButton);
    connect(backButton, &QPushButton::clicked, this, &XMLWindowEdit::onBackButton);
}

void XMLWindowEdit::setHotKey() {
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

void XMLWindowEdit::onPathButton() {
    if (isValidPath()) {
        QString pathStr = putPath->toPlainText();
        if (!pathStr.endsWith(".xml", Qt::CaseInsensitive))
            pathStr += ".xml";

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

void XMLWindowEdit::onUButton() {
    if (putPath->isVisible() && putPath->isEnabled()) {
        putPath->clear();
        putPath->setFocus();
        pathError->hide();
    } else {
        savefile();
    }
    qDebug() << "u button clicked";
}

void XMLWindowEdit::onBackUpButton() {
    editTXT.reset();
    QToolTip::hideText();
    emit backUp();
    qDebug() << "back button clicked";
}

void XMLWindowEdit::onBackButton() {
    editTXT.reset();
    QToolTip::hideText();
    emit backToMain();
    qDebug() << "back button clicked";
}

void XMLWindowEdit::editfile() {
    QString text = QString::fromStdString(editTXT->getText());
    textEdit->setText(text);

    textEdit->setFocus();
    QTextCursor cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    textEdit->setTextCursor(cursor);
}

void XMLWindowEdit::savefile() {
    QToolTip::hideText();
    try
    {
        checkDeser.checkDes(textEdit->toPlainText().toStdString(), TypeD::XML);
    }
    catch(const std::exception& e)
    {
        QToolTip::showText(QCursor::pos(), e.what());
        QTimer::singleShot(5000, []() {
            QToolTip::hideText();
        });
        return;
    }
    
    editTXT->setText(textEdit->toPlainText().toStdString());
    qDebug() << "work save";
}

bool XMLWindowEdit::isValidPath() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".xml", Qt::CaseInsensitive))
        pathStr += ".xml";

    if (checkerPath.checking(pathStr.toStdString(), PathAccessMode::Write)) {
        qDebug() << "valid path";
        return true;
    } else {
        pathError->setText(QString::fromStdString(checkerPath.error()));
        qDebug() << "no valid path";
        return false;
    }
}

// Remove TXT
XMLWindowRemove::XMLWindowRemove (QWidget* parent) :
QWidget (parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void XMLWindowRemove::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    putPath->setEnabled(true);
    pathRemove->setEnabled(true);

    putPath->clear();
    putPath->show();

    pathRemove->show();

    pathError->hide();
    pathOk->hide();

    putPath->setFocus();
}

bool XMLWindowRemove::eventFilter(QObject* obj, QEvent* event) {
    if (obj == putPath && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            putPath->clearFocus();
            onRemoveButton();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void XMLWindowRemove::setUpUI() {
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

    pathRemove = new QPushButton("↵", this);
    pathRemove->setFont(fontPath);
    pathRemove->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pathRemove->setFixedHeight(60);

    pathError = new QLabel(this);
    pathError->setStyleSheet("color: red;");
    pathError->setWordWrap(true);

    pathOk = new QLabel(this);
    pathOk->setStyleSheet("color: green;");
    pathOk->setWordWrap(true);
    pathOk->setText("File succes remove");

    QHBoxLayout* pathLayout = new QHBoxLayout();
    pathLayout->addWidget(putPath);
    pathLayout->addWidget(pathRemove);
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

void XMLWindowRemove::setUpConnections() {
    connect(pathRemove, &QPushButton::clicked, this, &XMLWindowRemove::onRemoveButton);
    connect(uButton, &QPushButton::clicked, this, &XMLWindowRemove::onUButton);
    connect(backUpButton, &QPushButton::clicked, this, &XMLWindowRemove::onBackUpButton);
    connect(backButton, &QPushButton::clicked, this, &XMLWindowRemove::onBackButton);
}

void XMLWindowRemove::setHotKey() {
    new QShortcut(QKeySequence(Qt::Key_Return), this, [this]() {
        onRemoveButton();
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

void XMLWindowRemove::onRemoveButton() {
    if (isValidPath()) {
        pathError->hide();
        pathOk->show();

        delFile();

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

void XMLWindowRemove::onUButton() {
    putPath->clear();
    putPath->setFocus();
    pathError->clear();
    pathError->hide();
    pathOk->hide();

    qDebug() << "u button clicked";
}

void XMLWindowRemove::onBackUpButton() {
    emit backUp();
    qDebug() << "back button clicked";
}

void XMLWindowRemove::onBackButton() {
    emit backToMain();
    qDebug() << "back button clicked";
}

bool XMLWindowRemove::isValidPath() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".xml", Qt::CaseInsensitive))
        pathStr += ".xml";

    if (checkerPath.checking(pathStr.toStdString(), PathAccessMode::Remove)) {
        qDebug() << "valid path";
    } else {
        pathError->setText(QString::fromStdString(checkerPath.error()));
        qDebug() << "no valid path";
        return false;
    }

    if (!removeFile.checkBlock(pathStr.toStdString())) {
        pathError->setText("File have block");
        return false;
    }
    return true;
}

void XMLWindowRemove::delFile() {
    pathError->clear();
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".xml", Qt::CaseInsensitive))
        pathStr += ".xml";

    try {
        removeFile.deleteFile(pathStr.toStdString());
    } catch(const std::exception& e) {
        pathOk->hide();
        pathError->setText(e.what());
        pathError->show();
        return;
    }
}