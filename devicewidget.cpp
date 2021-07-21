#include "devicewidget.h"

const int DeviceWidget::imgHeight = 100;
const int DeviceWidget::imgWidth = 100;

DeviceWidget::DeviceWidget(XmlReader config, QWidget *parent)
    :QWidget(parent), monitorLayout(parent), config(config){
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    for(auto& device:config.sendDeviceList){
        this->leftWidgets.append(genWidgetsFromDeviceInfo(&device));
    }
    for(auto& device:config.receiveDeviceList){
        this->rightWidgets.append(genWidgetsFromDeviceInfo(&device));
    }
    this->centerWidget = genWidgetsFromDeviceInfo(&config.centerDevice);
    if(config.useBridge){
        this->midWidgets.append(genWidgetsFromDeviceInfo(&config.sendBridgeDevice));
        this->midWidgets.append(genWidgetsFromDeviceInfo(&config.receiveBridgeDevice));
    }
    int center_idx = (qMax(leftWidgets.size(),rightWidgets.size())*2-1)/2;

    int mid_col = 6;
    if(config.useBridge){
        for(auto& device:midWidgets){
            monitorLayout.addWidget(device.img, center_idx*2, mid_col, 2, 2, Qt::AlignCenter);
            monitorLayout.addWidget(device.text, center_idx*2+2, mid_col, 2, 2, Qt::AlignTop|Qt::AlignHCenter);
            monitorLayout.addWidget(device.flag, center_idx*2, mid_col+2, Qt::AlignBottom|Qt::AlignLeft);
            mid_col += 3;
        }
    }
    // add center
    monitorLayout.addWidget(centerWidget.img, center_idx*2, mid_col, 2, 2, Qt::AlignCenter);
    monitorLayout.addWidget(centerWidget.text, center_idx*2+2, mid_col, 2, 2, Qt::AlignTop|Qt::AlignHCenter);
    monitorLayout.addWidget(centerWidget.flag, center_idx*2, mid_col+2, Qt::AlignBottom|Qt::AlignLeft);
    int right_left_col = mid_col+4;

    //add left
    int left_row_add = (center_idx - (this->leftWidgets.size()*2-1)/2)*2;
    for(int i=0; i<leftWidgets.size();i++){
        int left_row = left_row_add + i*4;
        monitorLayout.addWidget(leftWidgets[i].img, left_row, 2, 2, 2, Qt::AlignLeft);
        monitorLayout.addWidget(leftWidgets[i].text, left_row, 0, 2, 2, Qt::AlignRight);
        monitorLayout.addWidget(leftWidgets[i].flag, left_row, 3, Qt::AlignBottom|Qt::AlignLeft);

    }

    //add right
    int right_row_add = (center_idx - (this->rightWidgets.size()*2-1)/2)*2;
    for(int i=0; i<rightWidgets.size();i++){
        int right_row = right_row_add + i*4;
        monitorLayout.addWidget(rightWidgets[i].img, right_row, right_left_col, 2, 2, Qt::AlignRight);
        monitorLayout.addWidget(rightWidgets[i].text, right_row, right_left_col+2, 2, 2, Qt::AlignLeft);
        monitorLayout.addWidget(rightWidgets[i].flag, right_row, right_left_col-1, Qt::AlignBottom|Qt::AlignRight);
    }
    this->setLayout(&monitorLayout);
}

DeviceWidgets DeviceWidget::genWidgetsFromDeviceInfo(DeviceInfo* v) {
    DeviceWidgets ret;

    ret.deviceInfo = v;
    ret.text = new QLabel();
    ret.flag = new FlagWidget(DeviceWidget::imgHeight/4);
    ret.img = new QLabel();

    QPixmap pix = QPixmap(v->getDeviceIco());
    ret.img->setPixmap(pix);
    ret.img->setScaledContents(true);
    ret.img->setFixedSize(DeviceWidget::imgWidth, DeviceWidget::imgHeight);
    setDeviceWidgetsInfo(ret);
    return ret;
}

void DeviceWidget::setDeviceWidgetsInfo(DeviceWidgets &deviceWidgets) {
    DeviceInfo* deviceInfo = deviceWidgets.deviceInfo;
    QString text = "设备名: "+deviceInfo->getDeviceName()+"\n设备ID: "+deviceInfo->getDeviceType()+deviceInfo->getDeviceId();
    for(Value& v:deviceWidgets.deviceInfo->otherList){
        text += "\n"+v.key+": "+v.value;
    }
    deviceWidgets.text->setText(text);
    deviceWidgets.flag->setFlag(deviceInfo->deviceState);
}




void DeviceWidget::paintEvent(QPaintEvent *event) {
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    QPainter painter(this);
    int central_y = this->centerWidget.img->pos().y()+this->centerWidget.img->height()/2;
    painter.setPen(pen);
    if(this->leftWidgets.size()){
        int central_x = 0;
        if(this->config.useBridge&&this->midWidgets.size()){
            central_x = this->midWidgets.front().img->pos().x();
        }else{
            central_x = this->centerWidget.img->pos().x();
        }
        int left_x = this->leftWidgets[0].img->pos().x()+this->leftWidgets[0].img->width();
        int mid_x = left_x + (central_x-left_x)/2;
        int min_y = this->leftWidgets.front().img->pos().y()+this->leftWidgets.front().img->height()/2;
        int max_y = this->leftWidgets.back().img->pos().y()+this->leftWidgets.back().img->height()/2;
        for(int i=0;i<this->leftWidgets.size();i++){
            int y = this->leftWidgets[i].img->pos().y()+this->leftWidgets[i].img->height()/2;
            painter.drawLine(left_x, y, mid_x, y);
        }
        painter.drawLine(mid_x, min_y, mid_x, max_y);
        painter.drawLine(mid_x, central_y, central_x, central_y);
    }
    if(this->rightWidgets.size()){
        int central_x = this->centerWidget.img->pos().x() + centerWidget.img->width();
        int right_x = this->rightWidgets[0].img->pos().x();
        int mid_x = central_x + (right_x-central_x)/2;
        int min_y = this->rightWidgets.front().img->pos().y()+this->rightWidgets.front().img->height()/2;
        int max_y = this->rightWidgets.back().img->pos().y()+this->rightWidgets.back().img->height()/2;
        for(int i=0;i<this->rightWidgets.size();i++){
            int y = this->rightWidgets[i].img->pos().y()+this->rightWidgets[i].img->height()/2;
            painter.drawLine(mid_x, y, right_x, y);
        }
        painter.drawLine(mid_x, min_y, mid_x, max_y);
        painter.drawLine(mid_x, central_y, central_x, central_y);
    }
    if(config.useBridge&&this->midWidgets.size()){
        int left_x = this->midWidgets.front().img->pos().x() + midWidgets.front().img->width();
        int right_x = this->midWidgets.back().img->pos().x();
        painter.drawLine(left_x, central_y, right_x, central_y);
        int central_x = this->centerWidget.img->pos().x();
        right_x += this->midWidgets.back().img->width();
        painter.drawLine(right_x, central_y, central_x, central_y);
    }
}
