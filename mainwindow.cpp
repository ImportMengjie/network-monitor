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
    this->leftImgConfig.append(new DeviceConfig("img/zj.png", true, "2Mbpas\n1000md"));
    this->leftImgConfig.append(new DeviceConfig("img/zj.png", true, ""));
    this->leftImgConfig.append(new DeviceConfig("img/zj.png", true, ""));
    this->leftImgConfig.append(new DeviceConfig("img/zj.png", true, ""));
    this->leftImgConfig.append(new DeviceConfig("img/zj.png", true, ""));

    this->rightImgConfig.append(new DeviceConfig("img/zj.png", true, "1/3"));
    this->rightImgConfig.append(new DeviceConfig("img/zj.png", true, ""));
    this->rightImgConfig.append(new DeviceConfig("img/zj.png", true, ""));

    this->centralConfig = new DeviceConfig("img/pc.png", true, "中控");

    int central_idx = (qMax(this->rightImgConfig.size(), this->leftImgConfig.size())*2-1)/2;
    this->monitorLayout.addWidget(this->centralConfig->getImageLabel(), central_idx*2, 8, 2, 2, Qt::AlignCenter);
    this->monitorLayout.addWidget(this->centralConfig->getFlagwiget(), central_idx*2-1, 8, 1, 2, Qt::AlignBottom|Qt::AlignHCenter);
    auto* central_text_label = this->centralConfig->getTextLabel();
    central_text_label->setAlignment(Qt::AlignCenter);
    this->monitorLayout.addWidget(this->centralConfig->getTextLabel(), central_idx*2+2, 8, 1, 2, Qt::AlignTop|Qt::AlignHCenter);
    qDebug()<<central_idx;

    int left_row_add = (central_idx - (this->leftImgConfig.size()*2-1)/2)*2;
    for(int i= 0; i<this->leftImgConfig.size(); i++){
        QLabel* img = this->leftImgConfig[i]->getImageLabel();
        FlagWidget* flagWidget = this->leftImgConfig[i]->getFlagwiget();
        QLabel* textLabel = this->leftImgConfig[i]->getTextLabel();
        int left_row = left_row_add + 4*i;
        this->monitorLayout.addWidget(img, left_row, 0, 2, 2, Qt::AlignRight|Qt::AlignVCenter);
        this->monitorLayout.addWidget(flagWidget, left_row, 2, 1, 2, Qt::AlignLeft|Qt::AlignBottom);
        this->monitorLayout.addWidget(textLabel, left_row + 1, 2, 1, 2, Qt::AlignLeft|Qt::AlignTop);
    }


    int right_row_add = (central_idx - (this->rightImgConfig.size()*2-1)/2)*2;
    for(int i = 0; i<this->rightImgConfig.size();i++){
        QLabel* img = this->rightImgConfig[i]->getImageLabel();
        FlagWidget* flagWidget = this->rightImgConfig[i]->getFlagwiget();
        QLabel* textLabel = this->rightImgConfig[i]->getTextLabel();
        int right_row = right_row_add + 4*i;
        this->monitorLayout.addWidget(img, right_row, 16, 2, 2, Qt::AlignLeft|Qt::AlignVCenter);
        this->monitorLayout.addWidget(flagWidget, right_row, 14, 1, 2, Qt::AlignRight|Qt::AlignBottom);
        this->monitorLayout.addWidget(textLabel, right_row + 1, 14, 1, 2, Qt::AlignRight|Qt::AlignTop);

    }

    deviceWidget = new DeviceWidget(*config);
    deviceWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    mainLayout.addWidget(deviceWidget);
    ui->centralwidget->setLayout(&mainLayout);

//    ui->centralwidget->setLayout(&monitorLayout);

//    timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
//    timer->start(1000);
}

void MainWindow::handleTimeout(){
    // TODO test network
    for (auto& device:this->leftImgConfig) {
        device->getFlagwiget()->setFlag(!device->getFlagwiget()->getFlag());
    }
}


void MainWindow::paintEvent(QPaintEvent *event){
//    QPen pen;
//    pen.setColor(Qt::black);
//    pen.setWidth(2);
//    QPainter painter(this);
//    int central_y = this->centralConfig->getImageLabel()->pos().y()+this->centralConfig->getImageLabel()->height()/2;
//    painter.setPen(pen);
//    if(this->leftImgConfig.size()){
//        int central_x = this->centralConfig->getImageLabel()->pos().x();
//        int left_x = this->leftImgConfig[0]->getImageLabel()->pos().x()+this->leftImgConfig[0]->getImageLabel()->width();
//        int mid_x = left_x + (central_x-left_x)/2;
//        int min_y = this->leftImgConfig.front()->getImageLabel()->pos().y()+this->leftImgConfig.front()->getImageLabel()->height()/2;
//        int max_y = this->leftImgConfig.back()->getImageLabel()->pos().y()+this->leftImgConfig.back()->getImageLabel()->height()/2;
//        for(int i=0;i<this->leftImgConfig.size();i++){
//            int y = this->leftImgConfig[i]->getImageLabel()->pos().y()+this->leftImgConfig[i]->getImageLabel()->height()/2;
//            painter.drawLine(left_x, y, mid_x, y);
//        }
//        painter.drawLine(mid_x, min_y, mid_x, max_y);
//        painter.drawLine(mid_x, central_y, central_x, central_y);
//    }
//    if(this->rightImgConfig.size()){
//        int central_x = this->centralConfig->getImageLabel()->pos().x() + centralConfig->getImageLabel()->width();
//        int right_x = this->rightImgConfig[0]->getImageLabel()->pos().x();
//        int mid_x = central_x + (right_x-central_x)/2;
//        int min_y = this->rightImgConfig.front()->getImageLabel()->pos().y()+this->rightImgConfig.front()->getImageLabel()->height()/2;
//        int max_y = this->rightImgConfig.back()->getImageLabel()->pos().y()+this->rightImgConfig.back()->getImageLabel()->height()/2;
//        for(int i=0;i<this->rightImgConfig.size();i++){
//            int y = this->rightImgConfig[i]->getImageLabel()->pos().y()+this->rightImgConfig[i]->getImageLabel()->height()/2;
//            painter.drawLine(mid_x, y, right_x, y);
//        }
//        painter.drawLine(mid_x, min_y, mid_x, max_y);
//        painter.drawLine(mid_x, central_y, central_x, central_y);
//    }
//    {
//        int flag_y = centralConfig->getImageLabel()->pos().y()-centralConfig->getFlagwiget()->height();
//        centralConfig->getFlagwiget()->setGeometry(centralConfig->getFlagwiget()->pos().x(), flag_y, centralConfig->getFlagwiget()->width(), centralConfig->getFlagwiget()->height());
//        int label_y = centralConfig->getImageLabel()->pos().y() + centralConfig->getImageLabel()->height();
//        this->centralConfig->getTextLabel()->setGeometry(centralConfig->getTextLabel()->x(), label_y, centralConfig->getTextLabel()->width(), centralConfig->getTextLabel()->height());

//    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

