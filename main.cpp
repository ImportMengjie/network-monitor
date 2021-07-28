#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

#include "bean/xmlreader.h"

#include <QDataStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    XmlReader reader("/Users/mengjie/Documents/project/55/Monitor/setting.xml");
    bool isOk = reader.readFromXml();
    if(!isOk){
        QMessageBox::critical(nullptr, "配置文件错误", "配置文件有误");
        return 1;
    }
    QString errorMessage = reader.verfiy();
    if(errorMessage.size()){
        QMessageBox::critical(nullptr, "配置文件错误", errorMessage);
        return 1;
    }


    MainWindow w(&reader);
    w.setWindowTitle(reader.baseconfig.sytemName+":"+reader.baseconfig.sytemId);
    w.showMaximized();
    return a.exec();
}
