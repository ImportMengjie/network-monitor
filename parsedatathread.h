#ifndef PARSEDATATHREAD_H
#define PARSEDATATHREAD_H

#include <QThread>
#include <QQueue>
#include <QSqlQuery>

#include "bean/xmlreader.h"


class ParseDataThread : public QThread{
    Q_OBJECT
public:
    static const QString insertRunningState;
    static const QString insertRunningLog;
    ParseDataThread(const XmlReader& config);

    void addDataPaths(const QVector<QString>& data);

    ~ParseDataThread(){
        this->terminate();
        this->wait();
    }


signals:
    void refreshUi();

private:
    XmlReader config;

    QMutex mutex;

    QQueue<QString> myThreadDataPaths;
    QQueue<QString> othersThreadDataPaths;

    static bool consumer_data_paths(QQueue<QString> &dataPaths, const XmlReader &config, QSqlQuery& queryRunningState, QSqlQuery& queryRunningLog, bool move);


    // QThread interface
protected:
    void run() override;
};

#endif // PARSEDATATHREAD_H
