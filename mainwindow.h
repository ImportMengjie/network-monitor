#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include "deviceconfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);

public slots:
    void handleTimeout();

private:
    Ui::MainWindow *ui;
    QGridLayout monitorLayout;
    QVector<DeviceConfig*> leftImgConfig;
    QVector<DeviceConfig*> rightImgConfig;
    QVector<QLabel*> flags;
    DeviceConfig* centralConfig;
    QTimer* timer;
};
#endif // MAINWINDOW_H
