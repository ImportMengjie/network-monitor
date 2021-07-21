#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deviceconfig.h"
#include "devicewidget.h"
#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QTimer>

MainWindow::MainWindow(XmlReader* config,QWidget *parent)
    : QMainWindow(parent), config(config)
    , ui(new Ui::MainWindow),monitorLayout(parent),mainLayout(parent){
    ui->setupUi(this);
    deviceWidget = new DeviceWidget(*config);
    deviceWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    mainLayout.addWidget(deviceWidget);
    ui->centralwidget->setLayout(&mainLayout);

//    timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
//    timer->start(1000);
}

void MainWindow::handleTimeout(){
    // TODO test network
}


void MainWindow::paintEvent(QPaintEvent *event){
}

MainWindow::~MainWindow()
{
    delete ui;
}

