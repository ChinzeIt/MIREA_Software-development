#include "zipwindow.h"

ZIPWindow::ZIPWindow (QWidget* parent) : 
QWidget(parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void ZIPWindow::setUpUI () {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFont font;
    font.setFamily("Arial");
    font.setPointSize(30);
    font.setBold(true);

    createButton = new QPushButton("Create ZIP", this);
    contentButton = new QPushButton("Content ZIP", this);
    editButton = new QPushButton("Edit ZIP", this);
    uploadButton = new QPushButton("Upload ZIP", this);
    removeButton = new QPushButton("Remove ZIP", this);
    backButton = new QPushButton("BACK", this);

    createButton->setFont(font);
    contentButton->setFont(font);
    editButton->setFont(font);
    uploadButton->setFont(font);
    removeButton->setFont(font);
    backButton->setFont(font);

    createButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    contentButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    editButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    uploadButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    removeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    mainLayout->addWidget(createButton);
    mainLayout->addWidget(contentButton);
    mainLayout->addWidget(editButton);
    mainLayout->addWidget(uploadButton);
    mainLayout->addWidget(removeButton);
    mainLayout->addWidget(backButton);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(20, 20, 20, 20);
}

void ZIPWindow::setUpConnections () {
    connect(createButton, &QPushButton::clicked, this, &ZIPWindow::onCreateButton);
    connect(contentButton, &QPushButton::clicked, this, &ZIPWindow::onContentButton);
    connect(editButton, &QPushButton::clicked, this, &ZIPWindow::onEditButton);
    connect(uploadButton, &QPushButton::clicked, this, &ZIPWindow::onUploadButton);
    connect(removeButton, &QPushButton::clicked, this, &ZIPWindow::onRemoveButton);
    connect(backButton, &QPushButton::clicked, this, &ZIPWindow::onBackButton);
}

void ZIPWindow::setHotKey() {
    new QShortcut(QKeySequence(Qt::Key_1), this, [this]() {
        onCreateButton();
    });
    new QShortcut(QKeySequence(Qt::Key_2), this, [this]() {
        onContentButton();
    });

    new QShortcut(QKeySequence(Qt::Key_3), this, [this]() {
        onEditButton();
    });

    new QShortcut(QKeySequence(Qt::Key_4), this, [this]() {
        onUploadButton();
    });

    new QShortcut(QKeySequence(Qt::Key_5), this, [this]() {
        onRemoveButton();
    });

    new QShortcut(QKeySequence(Qt::Key_Escape), this, [this]() {
        onBackButton();
    });
}

void ZIPWindow::onCreateButton() {
    emit createZIP();
    qDebug() << "create ZIP";
}

void ZIPWindow::onContentButton() {
    emit contentZIP();
    qDebug() << "read ZIP";
}

void ZIPWindow::onEditButton() {
    emit editZIP();
    qDebug() << "edit ZIP";
}

void ZIPWindow::onUploadButton() {
    emit uploadZIP();
    qDebug() << "upload ZIP";
}

void ZIPWindow::onRemoveButton() {
    emit removeZIP();
    qDebug() << "remove ZIP";
}

void ZIPWindow::onBackButton() {
    emit backToMain();
    qDebug() << "back button press";
}

// Create ZIP
ZIPWindowCreate::ZIPWindowCreate (QWidget* parent) :
QWidget (parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void ZIPWindowCreate::showEvent(QShowEvent* event) {
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

bool ZIPWindowCreate::eventFilter(QObject* obj, QEvent* event) {
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

void ZIPWindowCreate::setUpUI() {
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

void ZIPWindowCreate::setUpConnections() {
    connect(pathCreate, &QPushButton::clicked, this, &ZIPWindowCreate::onCreateButton);
    connect(uButton, &QPushButton::clicked, this, &ZIPWindowCreate::onUButton);
    connect(backUpButton, &QPushButton::clicked, this, &ZIPWindowCreate::onBackUpButton);
    connect(backButton, &QPushButton::clicked, this, &ZIPWindowCreate::onBackButton);
}

void ZIPWindowCreate::setHotKey() {
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

void ZIPWindowCreate::onCreateButton() {
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

void ZIPWindowCreate::onUButton() {
    putPath->clear();
    putPath->setFocus();
    pathError->clear();
    pathError->hide();
    pathOk->hide();

    qDebug() << "u button clicked";
}

void ZIPWindowCreate::onBackUpButton() {
    emit backUp();
    qDebug() << "back button clicked";
}

void ZIPWindowCreate::onBackButton() {
    emit backToMain();
    qDebug() << "back button clicked";
}

bool ZIPWindowCreate::isValidPath() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".zip", Qt::CaseInsensitive))
        pathStr += ".zip";

    if (checkerPath.checking(pathStr.toStdString(), PathAccessMode::Create)) {
        qDebug() << "valid path";
        return true;
    } else {
        pathError->setText(QString::fromStdString(checkerPath.error()));
        qDebug() << "no valid path";
        return false;
    }
}

void ZIPWindowCreate::touchFile() {
    pathError->clear();
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".zip", Qt::CaseInsensitive))
        pathStr += ".zip";

    if (!crZIP.createZIP(pathStr.toStdString())) {
        pathError->setText("Dont touch ZIP");
        pathError->show();
    }
}

ZIPWindowContent::ZIPWindowContent (QWidget* parent) :
QWidget (parent) {
    setUpUI();
    setUpConnections();
    setHotKey();
}

void ZIPWindowContent::showEvent(QShowEvent* event) {
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

bool ZIPWindowContent::eventFilter(QObject* obj, QEvent* event) {
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

void ZIPWindowContent::setUpUI() {
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

    textInfo = new QTableWidget(this);
    textInfo->setColumnCount(3);
    textInfo->setHorizontalHeaderLabels({"Path", "Size (MB)", "Date/Time"});
    textInfo->horizontalHeader()->setStretchLastSection(false);
    textInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    textInfo->verticalHeader()->setVisible(false);
    textInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    textInfo->setSelectionMode(QAbstractItemView::NoSelection);
    textInfo->setShowGrid(true);
    textInfo->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    textInfo->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
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

void ZIPWindowContent::setUpConnections() {
    connect(pathButton, &QPushButton::clicked, this, &ZIPWindowContent::onPathButton);
    connect(uButton, &QPushButton::clicked, this, &ZIPWindowContent::onUButton);
    connect(backUpButton, &QPushButton::clicked, this, &ZIPWindowContent::onBackUpButton);
    connect(backButton, &QPushButton::clicked, this, &ZIPWindowContent::onBackButton);
}

void ZIPWindowContent::setHotKey() {
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

void ZIPWindowContent::onPathButton() {
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

void ZIPWindowContent::onUButton() {
    if (putPath->isVisible() && putPath->isEnabled()) {
        putPath->clear();
        putPath->setFocus();
        pathError->hide();
    } else {
        loadInformation();
    }
    qDebug() << "u button clicked";
}

void ZIPWindowContent::onBackUpButton() {
    emit backUp();
    qDebug() << "back button clicked";
}

void ZIPWindowContent::onBackButton() {
    emit backToMain();
    qDebug() << "back button clicked";
}

void ZIPWindowContent::loadInformation() {
    QString pathStr = putPath->toPlainText();
    if (!pathStr.endsWith(".zip", Qt::CaseInsensitive))
        pathStr += ".zip";

    con.loadContent(pathStr.toStdString());
    std::string formattedText = con.formatText();
    QString qText = QString::fromStdString(formattedText);

    textInfo->clear();
    textInfo->setColumnCount(3);
    textInfo->setRowCount(0);
    textInfo->setHorizontalHeaderLabels({"Path", "Size", "Date"});

    textInfo->verticalHeader()->setVisible(false);
    textInfo->setShowGrid(false);
    textInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    textInfo->setSelectionMode(QAbstractItemView::NoSelection);
    textInfo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textInfo->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QStringList lines = qText.split('\n', Qt::SkipEmptyParts);
    for (int i = 1; i < lines.size(); ++i) {
        QStringList parts = lines[i].split('|', Qt::SkipEmptyParts);
        if (parts.size() != 3) continue;

        int row = textInfo->rowCount();
        textInfo->insertRow(row);

        QString pathPart = parts[0].trimmed();
        QTableWidgetItem* pathItem = new QTableWidgetItem(pathPart);
        pathItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        textInfo->setItem(row, 0, pathItem);

        QTableWidgetItem* sizeItem = new QTableWidgetItem(parts[1].trimmed());
        sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        textInfo->setItem(row, 1, sizeItem);

        QTableWidgetItem* dateItem = new QTableWidgetItem(parts[2].trimmed());
        dateItem->setTextAlignment(Qt::AlignCenter);
        textInfo->setItem(row, 2, dateItem);
    }

    QTimer::singleShot(0, this, [this]() {
        int totalWidth = textInfo->viewport()->width();
        int colWidth = totalWidth / 4;

        textInfo->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        textInfo->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
        textInfo->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);

        textInfo->setColumnWidth(1, colWidth);
        textInfo->setColumnWidth(2, colWidth);

        for (int r = 0; r < textInfo->rowCount(); ++r) {
            auto pathItem = textInfo->item(r, 0);
            if (pathItem) {
                QFontMetrics fm(textInfo->font());
                QString elided = fm.elidedText(pathItem->text(), Qt::ElideLeft, textInfo->columnWidth(0) - 10);
                pathItem->setText(elided);
            }

            auto sizeItem = textInfo->item(r, 1);
            if (sizeItem) {
                sizeItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            }
            auto dateItem = textInfo->item(r, 2);
            if (dateItem) {
                dateItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            }
        }
    });
}

bool ZIPWindowContent::isValidPath() {
    QString pathStr = putPath->toPlainText();

    if (!pathStr.endsWith(".zip", Qt::CaseInsensitive))
        pathStr += ".zip";

    if (checkerPath.checking(pathStr.toStdString(), PathAccessModeZIP::CONTENT)) {
        qDebug() << "valid path";
        return true;
    } else {
        pathError->setText(QString::fromStdString(checkerPath.error()));
        qDebug() << "no valid path";
        return false;
    }
}