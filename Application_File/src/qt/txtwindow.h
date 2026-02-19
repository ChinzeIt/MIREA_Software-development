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

class TXTWindow: public QWidget {
    Q_OBJECT

    public:
    TXTWindow(QWidget* parent = nullptr);

    signals:
    void createTXT();
    void readTXT();
    void editTXT();
    void removeTXT();
    void backToMain();

    private:
    void setUpUI();
    void setUpConnections();
    void setHotKey();

    void onCreateButton();
    void onReadButton();
    void onEditButton();
    void onRemoveButton();
    void onBackButton();

    QPushButton* createButton;
    QPushButton* readButton;
    QPushButton* editButton;
    QPushButton* removeButton;
    QPushButton* backButton;
};

#include "../adapter/qt/qtCheckCreateService.h"
#include "../adapter/qt/qtCreateFileService.h"
#include "../adapter/qt/qtCheckPathService.h"
#include "../adapter/qt/qtReadFileService.h"

class TXTWindowCreate: public QWidget {
    Q_OBJECT

    public:
    TXTWindowCreate(QWidget* parent = nullptr);

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

    QTCheckCreateService checkerPath;
    QTCreateFileService crFile;
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

class TXTWindowRead: public QWidget {
    Q_OBJECT

    public:
    TXTWindowRead(QWidget* parent = nullptr);

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

    void onPathButton();
    void onUButton();
    void onBackUpButton();
    void onBackButton();

    QTCheckPathService checkerPath;
    QTReadFileService readTXT;

    bool isValidPath();
    QLabel* pathError;
    void loadInformation();

    QTextEdit* putPath;
    QTextEdit* textInfo;
    QPushButton* pathButton;
    QPushButton* uButton;
    QPushButton* backUpButton;
    QPushButton* backButton;
};