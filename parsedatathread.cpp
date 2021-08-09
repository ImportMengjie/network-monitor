#include "parsedatathread.h"

#include "file/runningstate.h"
#include "file/pathutils.h"

#include <QSqlDatabase>
#include <QMutex>
#include <QMutexLocker>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

const QString ParseDataThread::insertRunningState = "INSERT INTO running_state(deviceid,create_datetime,parse_datetime,state,content) VALUES(:deviceid, :create_datetime, :parse_datetime, :state, :content)";
const QString ParseDataThread::insertRunningLog = "INSERT INTO running_log(log_type,log_datetime,log_content) VALUES(:log_type, :log_datetime, :log_content)";


ParseDataThread::ParseDataThread(const XmlReader& config):config(config) {
}

void ParseDataThread::addDataPaths(const QVector<QString> &data) {
    {
        QMutexLocker locker(&mutex);
        for(auto& path:data)
            othersThreadDataPaths.append(path);
    }
}

bool ParseDataThread::consumer_data_paths(QQueue<QString> &dataPaths, const XmlReader &config, QSqlQuery& queryRunningState, QSqlQuery& queryRunningLog, bool move) {
    while(dataPaths.size()){
        QString dataPath = dataPaths.head();
        RunningState r;
        if(RunningState::loadeFromFile(dataPath, r)){
            QString errorDeviceId = "";
            for(auto& device:r.getAllDevice()){
                queryRunningState.bindValue(":deviceid", device.getDeviceID());
                queryRunningState.bindValue(":create_datetime", r.reportDateTime);
                queryRunningState.bindValue(":parse_datetime", QDateTime::currentDateTime());
                queryRunningState.bindValue(":state", device.getRunningStatus());
                queryRunningState.bindValue(":content", device.getSatus());
                queryRunningState.exec();
                if(queryRunningState.numRowsAffected()<=0){
                    qDebug()<<"error exec insert sql "<<queryRunningState.lastQuery()<<queryRunningState.lastError()<<endl;
                    return false;
                }
                if(device.runningStatus!=RunningState::goodStatus||!config.isDeviceGood(device.getDeviceID(), device.getSatus(), device.getRunningStatus()))
                    errorDeviceId.append(device.getDeviceID()+",");
            }
            queryRunningLog.bindValue(":log_datetime", r.reportDateTime);
            if(errorDeviceId.isEmpty()){
                queryRunningLog.bindValue(":log_type", 1);
                queryRunningLog.bindValue(":log_content", "parse "+dataPath);
            }
            else{
                queryRunningLog.bindValue(":log_type", 2);
                queryRunningLog.bindValue(":log_content", "parse "+dataPath+",error deviceID:"+errorDeviceId);
            }
            queryRunningLog.exec();
            if(queryRunningLog.numRowsAffected()<=0){
                qDebug()<<"error exec insert sql "<<queryRunningLog.lastQuery()<<queryRunningLog.lastError()<<endl;
            }
            // move to date dir
            QString targetPath = PathUtils::concatDir(PathUtils::getDateDirAndCreate(config.baseconfig.historyDir, r.reportDateTime), PathUtils::getFileName(dataPath));
            if(move){
                PathUtils::mv(dataPath, targetPath);
            }else{
                PathUtils::cp(dataPath, targetPath);
            }

        }else{
            // move to error dir
            QString targetPath = PathUtils::concatDir(config.baseconfig.errorDir,PathUtils::getFileName(dataPath));
            if(move){
                PathUtils::mv(dataPath, targetPath);
            }else{
                PathUtils::cp(dataPath, targetPath);
            }
        }
        dataPaths.dequeue();
    }

    return true;
}

void ParseDataThread::run() {
    QSqlDatabase db = QSqlDatabase::addDatabase(config.baseconfig.dbDriver, "Parse_thread_connect");

    db.setHostName(config.baseconfig.dbHost);
    db.setDatabaseName(config.baseconfig.dbName);
    db.setUserName(config.baseconfig.dbUserName);
    db.setPassword(config.baseconfig.dbPassword);
    db.setPort(config.baseconfig.dbPort);
    db.setConnectOptions("connect_timeout=2");

    db.open();
    while(true){
        while(!db.isOpen()) {
            qWarning() << db.lastError();
            this->sleep(5);
            db.open();
        }

        // monitor dir
        QDir monitorDir(config.baseconfig.monitorDir, "*.amg", QDir::Time, QDir::Files);
        QFileInfoList dataList = monitorDir.entryInfoList();
        for(int i=0;i<dataList.size();i++){
            if(dataList[i].exists())
                myThreadDataPaths.enqueue(dataList[i].absoluteFilePath());
        }

        // user add files
        QQueue<QString> copyPaths;
        {
            QMutexLocker locker(&mutex);
            while(othersThreadDataPaths.size())
                copyPaths.enqueue(othersThreadDataPaths.dequeue());
        }
        QSqlQuery queryRunningState(db);
        QSqlQuery queryRunningLog(db);
        queryRunningState.prepare(insertRunningState);
        queryRunningLog.prepare(insertRunningLog);
        consumer_data_paths(myThreadDataPaths, config, queryRunningState, queryRunningLog, true);
        consumer_data_paths(copyPaths, config, queryRunningState, queryRunningLog, false);

        emit refreshUi();
        this->sleep(1);
    }
}
