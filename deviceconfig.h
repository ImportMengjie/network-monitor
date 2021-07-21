#ifndef DEVICECONFIG_H
#define DEVICECONFIG_H

#include <QLabel>
#include "widget/flagwidget.h"

class DeviceConfig
{

private:
    QString imgPath;
    QString text;
    bool useFlag;
    QLabel* img;
    QLabel* lableText;
    FlagWidget* flag;

public:

    static int height;
    static int wight;

    DeviceConfig(const QString& imgPath, bool useFlag, const QString& text);

    QLabel* getImageLabel(){
        return img;
    }

    QLabel* getTextLabel(){
        return lableText;
    }

    FlagWidget* getFlagwiget(){
        return flag;
    }

};

#endif // DEVICECONFIG_H
