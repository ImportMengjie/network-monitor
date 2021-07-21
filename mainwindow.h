#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include "deviceconfig.h"
#include "bean/xmlreader.h"
#include "widget/devicewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(XmlReader* config,QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);

public slots:
    void handleTimeout();

private:
    XmlReader* config;
    Ui::MainWindow *ui;
    QGridLayout monitorLayout;
    QVector<DeviceConfig*> leftImgConfig;
    QVector<DeviceConfig*> rightImgConfig;
    QVector<QLabel*> flags;
    DeviceConfig* centralConfig;
    QVBoxLayout mainLayout;
    QTimer* timer;
    DeviceWidget* deviceWidget;
};
#endif // MAINWINDOW_H
