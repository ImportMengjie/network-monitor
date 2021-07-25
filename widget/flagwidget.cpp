#include "flagwidget.h"
#include <QPainter>
#include <QColor>

FlagWidget::FlagWidget(QWidget *parent) : QWidget(parent), height_(0), flag(3), useFlag(false)
{

}


FlagWidget::FlagWidget(int height,QWidget* parent, bool useFlag)
    :QWidget(parent), height_(height), flag(3), useFlag(useFlag) {
    this->setFixedSize(height, height);
}


void FlagWidget::paintEvent(QPaintEvent *event){
    if(!useFlag) return;
    QPen pen;
    pen.setColor(Qt::black);
    QBrush redBrush(Qt::red);
    QBrush grayBrush(Qt::gray);
    QBrush greenBrush(Qt::green);
    QPainter painter(this);
    painter.setPen(pen);
    if(flag==1)
        painter.setBrush(greenBrush);
    else if(flag==2)
        painter.setBrush(redBrush);
    else
        painter.setBrush(grayBrush);
    painter.drawEllipse(0, 0, height_-2, height_-2);
}
