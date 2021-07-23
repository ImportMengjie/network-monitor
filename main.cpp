#include "mainwindow.h"

#include <QApplication>

#include "bean/xmlreader.h"

#include <QDataStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    XmlReader reader("/Users/mengjie/Documents/project/55/Monitor/setting.xml");
    reader.readFromXml();

    quint8 c = 0x0A;
    QString s = QString("%1").arg(c, 2, 16, QLatin1Char('0'));
    qDebug()<<QString("%1").arg(c, 2, 16, QLatin1Char('0'));


    MainWindow w(&reader);
    w.setWindowTitle(reader.baseconfig.sytemName+":"+reader.baseconfig.sytemId);
//    w.show();
    w.showMaximized();
    return a.exec();
}
