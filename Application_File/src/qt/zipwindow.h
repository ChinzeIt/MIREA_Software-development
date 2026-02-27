#pragma once

#include <QWidget>
#include <QShowEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QShortcut>
#include <QTextEdit>
#include <QString>
#include <QLabel>
#include <QTextCursor>
#include <QDebug>
#include <QtGlobal>

class ZIPWindow: public QWidget {
    Q_OBJECT

    public:
    ZIPWindow(QWidget* parent = nullptr);

    signals:
    void createZIP();
    void contentZIP();
    void editZIP();
    void uploadZIP();
    void removeZIP();
    void backToMain();

    private:
    void setUpUI();
    void setUpConnections();
    void setHotKey();

    void onCreateButton();
    void onContentButton();
    void onEditButton();
    void onUploadButton();
    void onRemoveButton();
    void onBackButton();

    QPushButton* createButton;
    QPushButton* contentButton;
    QPushButton* editButton;
    QPushButton* uploadButton;
    QPushButton* removeButton;
    QPushButton* backButton;
};

#include "../adapter/qt/qtCheckPathService.h"
#include "../core/STDCreateZIPService.h"

class ZIPWindowCreate: public QWidget {
    Q_OBJECT

    public:
    ZIPWindowCreate(QWidget* parent = nullptr);

    protected:
    void showEvent(QShowEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

    signals:
    void backUp();
    void backToMain();

    private:
    void setUpUI();
    void setUpConnections();
    void setHotKey();

    void onCreateButton();
    void onUButton();
    void onBackUpButton();
    void onBackButton();

    QTCheckPathService checkerPath;
    STDCreateZIPService crZIP;
    bool isValidPath();
    void touchFile();
    QLabel* pathError;
    QLabel* pathOk;

    QTextEdit* putPath;
    QPushButton* pathCreate;
    QPushButton* uButton;
    QPushButton* backUpButton;
    QPushButton* backButton;
};