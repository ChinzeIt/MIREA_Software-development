#include "jsonwindow.h"

JSONWindow::JSONWindow (QWidget* parent) : 
QWidget(parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void JSONWindow::setUpUI () {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFont font;
    font.setFamily("Arial");
    font.setPointSize(30);
    font.setBold(true);

    createButton = new QPushButton("Create JSON", this);
    readButton = new QPushButton("Read JSON", this);
    editButton = new QPushButton("Edit JSON", this);
    removeButton = new QPushButton("Remove JSON", this);
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

void JSONWindow::setUpConnections () {
    connect(createButton, &QPushButton::clicked, this, &JSONWindow::onCreateButton);
    connect(readButton, &QPushButton::clicked, this, &JSONWindow::onReadButton);
    connect(editButton, &QPushButton::clicked, this, &JSONWindow::onEditButton);
    connect(removeButton, &QPushButton::clicked, this, &JSONWindow::onRemoveButton);
    connect(backButton, &QPushButton::clicked, this, &JSONWindow::onBackButton);
}

void JSONWindow::setHotKey() {
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

void JSONWindow::onCreateButton() {
    emit createJSON();
    qDebug() << "create JSON";
}

void JSONWindow::onReadButton() {
    emit readJSON();
    qDebug() << "read JSON";
}

void JSONWindow::onEditButton() {
    emit editJSON();
    qDebug() << "edit JSON";
}

void JSONWindow::onRemoveButton() {
    emit removeJSON();
    qDebug() << "remove JSON";
}

void JSONWindow::onBackButton() {
    emit backToMain();
    qDebug() << "back button press";
}

// Create TXT
JSONWindowCreate::JSONWindowCreate (QWidget* parent) :
QWidget (parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void JSONWindowCreate::showEvent(QShowEvent* event) {
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

bool JSONWindowCreate::eventFilter(QObject* obj, QEvent* event) {
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

void JSONWindowCreate::setUpUI() {
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

void JSONWindowCreate::setUpConnections() {
    connect(pathCreate, &QPushButton::clicked, this, &JSONWindowCreate::onCreateButton);
    connect(uButton, &QPushButton::clicked, this, &JSONWindowCreate::onUButton);
    connect(backUpButton, &QPushButton::clicked, this, &JSONWindowCreate::onBackUpButton);
    connect(backButton, &QPushButton::clicked, this, &JSONWindowCreate::onBackButton);
}

void JSONWindowCreate::setHotKey() {
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

void JSONWindowCreate::onCreateButton() {
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

void JSONWindowCreate::onUButton() {
    putPath->clear();
    putPath->setFocus();
    pathError->clear();
    pathError->hide();
    pathOk->hide();

    qDebug() << "u button clicked";
}

void JSONWindowCreate::onBackUpButton() {
    emit backUp();
    qDebug() << "back button clicked";
}

void JSONWindowCreate::onBackButton() {
    emit backToMain();
    qDebug() << "back button clicked";
}

bool JSONWindowCreate::isValidPath() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".json", Qt::CaseInsensitive))
        pathStr += ".json";

    if (checkerPath.checking(pathStr.toStdString(), PathAccessMode::Create)) {
        qDebug() << "valid path";
        return true;
    } else {
        pathError->setText(QString::fromStdString(checkerPath.error()));
        qDebug() << "no valid path";
        return false;
    }
}

void JSONWindowCreate::touchFile() {
    pathError->clear();
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".json", Qt::CaseInsensitive))
        pathStr += ".json";

    if (!crFile.createFile(pathStr.toStdString())) {
        pathError->setText("Dont touch file");
        pathError->show();
    }
}

// Read TXT
JSONWindowRead::JSONWindowRead (QWidget* parent) :
QWidget (parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void JSONWindowRead::showEvent(QShowEvent* event) {
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

bool JSONWindowRead::eventFilter(QObject* obj, QEvent* event) {
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

void JSONWindowRead::setUpUI() {
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

void JSONWindowRead::setUpConnections() {
    connect(pathButton, &QPushButton::clicked, this, &JSONWindowRead::onPathButton);
    connect(uButton, &QPushButton::clicked, this, &JSONWindowRead::onUButton);
    connect(backUpButton, &QPushButton::clicked, this, &JSONWindowRead::onBackUpButton);
    connect(backButton, &QPushButton::clicked, this, &JSONWindowRead::onBackButton);
}

void JSONWindowRead::setHotKey() {
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

void JSONWindowRead::onPathButton() {
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

void JSONWindowRead::onUButton() {
    if (putPath->isVisible() && putPath->isEnabled()) {
        putPath->clear();
        putPath->setFocus();
        pathError->hide();
    } else {
        loadInformation();
    }
    qDebug() << "u button clicked";
}

void JSONWindowRead::onBackUpButton() {
    emit backUp();
    qDebug() << "back button clicked";
}

void JSONWindowRead::onBackButton() {
    emit backToMain();
    qDebug() << "back button clicked";
}

void JSONWindowRead::loadInformation() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".json", Qt::CaseInsensitive))
        pathStr += ".json";
    
    QString text = QString::fromStdString(readJSON.readFile(pathStr.toStdString()));
    textInfo->setText(text);
}

bool JSONWindowRead::isValidPath() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".json", Qt::CaseInsensitive))
        pathStr += ".json";

    if (checkerPath.checking(pathStr.toStdString(), PathAccessMode::Read)) {
        qDebug() << "valid path";
        return true;
    } else {
        pathError->setText(QString::fromStdString(checkerPath.error()));
        qDebug() << "no valid path";
        return false;
    }
}

JSONWindowEdit::JSONWindowEdit (QWidget* parent) :
QWidget (parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void JSONWindowEdit::showEvent(QShowEvent* event) {
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

bool JSONWindowEdit::eventFilter(QObject* obj, QEvent* event) {
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

void JSONWindowEdit::setUpUI() {
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

void JSONWindowEdit::setUpConnections() {
    connect(pathButton, &QPushButton::clicked, this, &JSONWindowEdit::onPathButton);
    connect(uButton, &QPushButton::clicked, this, &JSONWindowEdit::onUButton);
    connect(backUpButton, &QPushButton::clicked, this, &JSONWindowEdit::onBackUpButton);
    connect(backButton, &QPushButton::clicked, this, &JSONWindowEdit::onBackButton);
}

void JSONWindowEdit::setHotKey() {
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

void JSONWindowEdit::onPathButton() {
    if (isValidPath()) {
        QString pathStr = putPath->toPlainText();
        if (!pathStr.endsWith(".json", Qt::CaseInsensitive))
            pathStr += ".json";

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

void JSONWindowEdit::onUButton() {
    if (putPath->isVisible() && putPath->isEnabled()) {
        putPath->clear();
        putPath->setFocus();
        pathError->hide();
    } else {
        savefile();
    }
    qDebug() << "u button clicked";
}

void JSONWindowEdit::onBackUpButton() {
    editTXT.reset();
    QToolTip::hideText();
    emit backUp();
    qDebug() << "back button clicked";
}

void JSONWindowEdit::onBackButton() {
    editTXT.reset();
    QToolTip::hideText();
    emit backToMain();
    qDebug() << "back button clicked";
}

void JSONWindowEdit::editfile() {
    QString text = QString::fromStdString(editTXT->getText());
    textEdit->setText(text);

    textEdit->setFocus();
    QTextCursor cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    textEdit->setTextCursor(cursor);
}

void JSONWindowEdit::savefile() {
    QToolTip::hideText();
    try
    {
        checkDeser.checkDes(textEdit->toPlainText().toStdString(), TypeD::JSON);
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

bool JSONWindowEdit::isValidPath() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".json", Qt::CaseInsensitive))
        pathStr += ".json";

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
JSONWindowRemove::JSONWindowRemove (QWidget* parent) :
QWidget (parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void JSONWindowRemove::showEvent(QShowEvent* event) {
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

bool JSONWindowRemove::eventFilter(QObject* obj, QEvent* event) {
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

void JSONWindowRemove::setUpUI() {
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

void JSONWindowRemove::setUpConnections() {
    connect(pathRemove, &QPushButton::clicked, this, &JSONWindowRemove::onRemoveButton);
    connect(uButton, &QPushButton::clicked, this, &JSONWindowRemove::onUButton);
    connect(backUpButton, &QPushButton::clicked, this, &JSONWindowRemove::onBackUpButton);
    connect(backButton, &QPushButton::clicked, this, &JSONWindowRemove::onBackButton);
}

void JSONWindowRemove::setHotKey() {
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

void JSONWindowRemove::onRemoveButton() {
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

void JSONWindowRemove::onUButton() {
    putPath->clear();
    putPath->setFocus();
    pathError->clear();
    pathError->hide();
    pathOk->hide();

    qDebug() << "u button clicked";
}

void JSONWindowRemove::onBackUpButton() {
    emit backUp();
    qDebug() << "back button clicked";
}

void JSONWindowRemove::onBackButton() {
    emit backToMain();
    qDebug() << "back button clicked";
}

bool JSONWindowRemove::isValidPath() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".json", Qt::CaseInsensitive))
        pathStr += ".json";

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

void JSONWindowRemove::delFile() {
    pathError->clear();
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".json", Qt::CaseInsensitive))
        pathStr += ".json";

    try {
        removeFile.deleteFile(pathStr.toStdString());
    } catch(const std::exception& e) {
        pathOk->hide();
        pathError->setText(e.what());
        pathError->show();
        return;
    }
}