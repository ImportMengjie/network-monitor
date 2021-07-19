#include "mainwindow.h"

#include <QApplication>

#include "bean/xmlreader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    XmlReader reader("setting.xml");
    reader.readFromXml();

    MainWindow w;
    w.show();
    return a.exec();
}
