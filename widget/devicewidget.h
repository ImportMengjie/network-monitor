#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include "parsedatathread.h"
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QtGui>
#include "bean/xmlreader.h"
#include "widget/flagwidget.h"

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

    void addStatusFiles(const QStringList& files);

    static DeviceWidgets genWidgetsFromDeviceInfo(DeviceInfo* v);
    static void setDeviceWidgetsInfo(DeviceWidgets& deviceWidgets);
    static int getStatuValue(const QString& content, const QString& mask);
    static const int imgHeight;
    static const int imgWidth;
    static const QString goodState;
    static const QString badState;
    static const QString selectRunningStateSegment;
    QGridLayout monitorLayout;

public slots:
    void updateUi();


signals:

private:
    XmlReader config;
    ParseDataThread thread;
    QSqlDatabase db;
    QTimer* timer;

    QVector<DeviceWidgets> leftWidgets;
    QVector<DeviceWidgets> rightWidgets;
    QVector<DeviceWidgets> midWidgets;
    DeviceWidgets centerWidget;

    QVector<DeviceWidgets> totalWidgets;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DEVICEWIDGET_H
