#include "mainwindow.h"

#include <QApplication>

#include "bean/xmlreader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    XmlReader reader("/Users/mengjie/Documents/project/55/Monitor/setting.xml");
    reader.readFromXml();

    MainWindow w(&reader);
    w.setWindowTitle(reader.baseconfig.sytemName+":"+reader.baseconfig.sytemId);
    w.show();
    return a.exec();
}
