#include "deviceconfig.h"

int DeviceConfig::height=100;
int DeviceConfig::wight=100;

DeviceConfig::DeviceConfig(const QString& imgPath, bool useFlag,const QString& text)
    :imgPath(imgPath),  text(text), useFlag(useFlag){

    this->img = new QLabel();
    QPixmap pix = QPixmap(imgPath);
    img->setPixmap(pix);
    img->setScaledContents(true);
    img->setFixedSize(DeviceConfig::wight, DeviceConfig::height);

    this->flag = new FlagWidget(DeviceConfig::height/2, nullptr, useFlag);

    this->lableText = new QLabel(this->text);
    this->lableText->setFixedSize(DeviceConfig::wight, DeviceConfig::height/2);

}

