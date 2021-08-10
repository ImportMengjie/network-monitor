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
#include <QMessageBox>


MainWindow::MainWindow(XmlReader* config,QWidget *parent)
    : QMainWindow(parent), config(config)
    , ui(new Ui::MainWindow),monitorLayout(parent),mainLayout(parent){
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase(config->baseconfig.dbDriver);

    db.setHostName(config->baseconfig.dbHost);
    db.setDatabaseName(config->baseconfig.dbName);
    db.setUserName(config->baseconfig.dbUserName);
    db.setPassword(config->baseconfig.dbPassword);
    db.setPort(config->baseconfig.dbPort);
    db.setConnectOptions("connect_timeout=2");
    db.open();
    if(!db.isValid()||!db.isOpen()){
        QMessageBox::critical(this, "数据库连接错误", "请检查数据库运行状态及配置文件!");
    }
    {
        QSqlQuery q(db);
        q.exec("CREATE TABLE if not exists running_log ( \"log_type\" int4 NOT NULL, \"log_datetime\" timestamptz(6) NOT NULL, \"log_content\" varchar(1000 char) NOT NULL )");
        q.exec("CREATE TABLE if not exists running_state ( \"deviceid\" varchar(10 char) NOT NULL, \"create_datetime\" timestamptz(6) NOT NULL, \"parse_datetime\" timestamptz(6) NOT NULL, \"state\" varchar(10 char) NOT NULL, \"content\" varchar(1000 char) NOT NULL )");
    }

    deviceWidget = new DeviceWidget(*config, db);
    deviceWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    mainLayout.addWidget(deviceWidget);
    ui->centralwidget->setLayout(&mainLayout);
    lookUpLogDialog = new LookUpLogDialog(*config, db);
    createInstructionDialog = new CreateInstructionDialog(*config, db);

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
    createInstructionDialog->show();
}

void MainWindow::lookupLog() {
    lookUpLogDialog->show();
}

MainWindow::~MainWindow() {
    delete ui;
}

