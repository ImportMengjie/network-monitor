#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include "deviceconfig.h"
#include "bean/xmlreader.h"
#include "widget/devicewidget.h"
#include "parsedatathread.h"
#include "window/lookuplogdialog.h"
#include "window/createinstructiondialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(XmlReader* config,QWidget *parent = nullptr);
    ~MainWindow();

    void createActions();
    void createMenu();


public slots:
    void openFiles();
    void generateInstructions();
    void lookupLog();

private:
    XmlReader* config;

    QSqlDatabase db;
    QAction* openFilesAction;
    QAction* exitAction;
    QAction* generateInstructionsAction;
    QAction* lookupLogAction;

    LookUpLogDialog* lookUpLogDialog;
    CreateInstructionDialog* createInstructionDialog;

    QMenu* fileMenu;
    QMenu* toolsMenu;

    Ui::MainWindow *ui;
    QGridLayout monitorLayout;
    QVBoxLayout mainLayout;
    DeviceWidget* deviceWidget;
};
#endif // MAINWINDOW_H
