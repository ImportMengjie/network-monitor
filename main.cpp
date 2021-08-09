#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

#include "bean/xmlreader.h"
#include "file/pathutils.h"

#include <QDataStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "The driver should be put in this path:" << QCoreApplication::libraryPaths();
        qDebug() << "Available drivers:" << QSqlDatabase::drivers();

    XmlReader reader(PathUtils::getAbsPath("setting.xml"));
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
