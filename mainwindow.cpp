#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deviceconfig.h"
#include "widget/devicewidget.h"
#include "window/lookuplogdialog.h"
#include <QSqlError>
#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QFileDialog>


MainWindow::MainWindow(XmlReader* config,QWidget *parent)
    : QMainWindow(parent), config(config)
    , ui(new Ui::MainWindow),monitorLayout(parent),mainLayout(parent){
    ui->setupUi(this);
    deviceWidget = new DeviceWidget(*config);
    deviceWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    mainLayout.addWidget(deviceWidget);
    ui->centralwidget->setLayout(&mainLayout);
    lookUpLogDialog = new LookUpLogDialog(*config);

    createActions();
    createMenu();
}


void MainWindow::createActions() {
    openFilesAction = new QAction("打开文件", this);
    connect(openFilesAction, SIGNAL(triggered()), this, SLOT(openFiles()));

    exitAction = new QAction("退出", this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    generateInstructionsAction = new QAction("生成指令", this);
    connect(generateInstructionsAction, SIGNAL(triggered()), this, SLOT(generateInstructions()));

    lookupLogAction = new QAction("记录查看", this);
    connect(lookupLogAction, SIGNAL(triggered()), this, SLOT(lookupLog()));

}

void MainWindow::createMenu() {
    fileMenu = menuBar()->addMenu("文件");
    fileMenu->addAction(openFilesAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    toolsMenu = menuBar()->addMenu("工具");
    toolsMenu->addAction(generateInstructionsAction);
    toolsMenu->addAction(lookupLogAction);
}

void MainWindow::openFiles() {
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            "选择状态文件amg",
                            "~",
                            "*.amg");
    this->deviceWidget->addStatusFiles(files);
}

void MainWindow::generateInstructions() {

}

void MainWindow::lookupLog() {
    lookUpLogDialog->show();

}

MainWindow::~MainWindow() {
    delete ui;
}

