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
#include <QToolTip>
#include <QTimer>
#include <QCursor>
#include <QDebug>

class JSONWindow: public QWidget {
    Q_OBJECT

    public:
    JSONWindow(QWidget* parent = nullptr);

    signals:
    void createJSON();
    void readJSON();
    void editJSON();
    void removeJSON();
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

#include "../adapter/qt/qtCreateFileService.h"
#include "../adapter/qt/qtCheckPathService.h"
#include "../adapter/qt/qtReadFileService.h"
#include "../adapter/qt/qtEditFileService.h"
#include "../adapter/qt/qtRemoveFileService.h"
#include "../adapter/qt/qtDeserialJsonXMLService.h"

class JSONWindowCreate: public QWidget {
    Q_OBJECT

    public:
    JSONWindowCreate(QWidget* parent = nullptr);

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

class JSONWindowRead: public QWidget {
    Q_OBJECT

    public:
    JSONWindowRead(QWidget* parent = nullptr);

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
    QTReadFileService readJSON;

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

class JSONWindowEdit: public QWidget {
    Q_OBJECT

    public:
    JSONWindowEdit(QWidget* parent = nullptr);

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
    std::unique_ptr<QTEditFileService> editTXT;
    QTDeserialJsonXMLService checkDeser;

    bool isValidPath();
    QLabel* pathError;
    void editfile();
    void savefile();

    QTextEdit* putPath;
    QTextEdit* textEdit;
    QPushButton* pathButton;
    QPushButton* uButton;
    QPushButton* backUpButton;
    QPushButton* backButton;
};

class JSONWindowRemove: public QWidget {
    Q_OBJECT

    public:
    JSONWindowRemove(QWidget* parent = nullptr);

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

    void onRemoveButton();
    void onUButton();
    void onBackUpButton();
    void onBackButton();

    QTCheckPathService checkerPath;
    QTRemoveFileService removeFile;
    bool isValidPath();
    void delFile();
    QLabel* pathError;
    QLabel* pathOk;

    QTextEdit* putPath;
    QPushButton* pathRemove;
    QPushButton* uButton;
    QPushButton* backUpButton;
    QPushButton* backButton;
};