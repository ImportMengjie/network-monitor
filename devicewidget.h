#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QtGui>
#include "bean/xmlreader.h"
#include "flagwidget.h"

struct DeviceWidgets{
    QLabel* text;
    FlagWidget* flag;
    QLabel* img;
    DeviceInfo* deviceInfo;
};

class DeviceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceWidget(XmlReader config, QWidget *parent = nullptr);

    static DeviceWidgets genWidgetsFromDeviceInfo(DeviceInfo* v);
    static void setDeviceWidgetsInfo(DeviceWidgets& deviceWidgets);
    static const int imgHeight;
    static const int imgWidth;
    QGridLayout monitorLayout;


signals:

private:
    XmlReader config;

    QVector<DeviceWidgets> leftWidgets;
    QVector<DeviceWidgets> rightWidgets;
    QVector<DeviceWidgets> midWidgets;
    DeviceWidgets centerWidget;
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DEVICEWIDGET_H
