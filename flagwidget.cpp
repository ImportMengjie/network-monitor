#include "flagwidget.h"
#include <QPainter>
#include <QColor>

FlagWidget::FlagWidget(QWidget *parent) : QWidget(parent), height_(0), flag(true), useFlag(false)
{

}


FlagWidget::FlagWidget(int height,QWidget* parent, bool useFlag)
    :QWidget(parent), height_(height), flag(true), useFlag(useFlag) {
    this->setFixedSize(height*2, height);
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
    if(flag)
        painter.setBrush(greenBrush);
    else
        painter.setBrush(grayBrush);
    painter.drawEllipse(0, 0, height_/2-2, height_/2-2);

    if(flag)
        painter.setBrush(grayBrush);
    else
        painter.setBrush(redBrush);
    painter.drawEllipse(height_/2-2, 0, height_/2-2, height_/2-2);
}
