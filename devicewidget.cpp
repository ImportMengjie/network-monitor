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
            monitorLayout.addWidget(device.text, center_idx*2+2, mid_col, 2, 2, Qt::AlignTop|Qt::AlignLeft);
            monitorLayout.addWidget(device.flag, center_idx*2-1, mid_col+2, Qt::AlignBottom|Qt::AlignLeft);
            mid_col += 3;
        }
    }
    // add center
    monitorLayout.addWidget(centerWidget.img, center_idx*2, mid_col, 2, 2, Qt::AlignCenter);
    monitorLayout.addWidget(centerWidget.text, center_idx*2+2, mid_col, 2, 2, Qt::AlignTop|Qt::AlignLeft);
    monitorLayout.addWidget(centerWidget.flag, center_idx*2-1, mid_col+2, Qt::AlignBottom|Qt::AlignLeft);
    int right_left_col = mid_col+4;

    //add left
    int left_row_add = (center_idx - (this->leftWidgets.size()*2-1)/2)*2;
    for(int i=0; i<leftWidgets.size();i++){
        int left_row = left_row_add + i*4;
        monitorLayout.addWidget(leftWidgets[i].img, left_row, 2, 2, 2, Qt::AlignCenter);
        monitorLayout.addWidget(leftWidgets[i].text, left_row, 0, 2, 2, Qt::AlignCenter);
        monitorLayout.addWidget(leftWidgets[i].flag, left_row, 4, Qt::AlignBottom|Qt::AlignLeft);

    }

    //add right
    int right_row_add = (center_idx - (this->leftWidgets.size()*2-1)/2)*2;
    for(int i=0; i<rightWidgets.size();i++){
        int right_row = right_row_add + i*4;
        monitorLayout.addWidget(rightWidgets[i].img, right_row, right_left_col, 2, 2, Qt::AlignCenter);
        monitorLayout.addWidget(rightWidgets[i].text, right_row, right_left_col+2, 2, 2, Qt::AlignCenter);
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
    QString text = "设备名: "+deviceInfo->getDeviceName()+"\n设备ID"+deviceInfo->getDeviceType()+deviceInfo->getDeviceId();
    for(Value& v:deviceWidgets.deviceInfo->otherList){
        text += "\n"+v.key+": "+v.value;
    }
    deviceWidgets.text->setText(text);
    deviceWidgets.flag->setFlag(deviceInfo->deviceState);
}




void DeviceWidget::paintEvent(QPaintEvent *event) {
}
