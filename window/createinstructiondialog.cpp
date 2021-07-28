#include "createinstructiondialog.h"
#include "ui_createinstructiondialog.h"
#include "file/pathutils.h"
#include "file/crcchecksum.h"

#include <QFileDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

const quint16 CreateInstructionDialog::magic = 0xB39A;
const quint16 CreateInstructionDialog::searchMagic = 0xb377;
const quint32 CreateInstructionDialog::restartMagic = 0xb35555b3;
const quint32 CreateInstructionDialog::shutdownMagic = 0xb34444b3;

const quint16 CreateInstructionDialog::activeMagic = 0xc66c;
const quint16 CreateInstructionDialog::notActiveMagic = 0;

QByteArray &operator<<(QByteArray &l, quint8 r) {
    l.append(r);
    return l;
}

QByteArray &operator<<(QByteArray &l, quint16 r) {
    return l<<quint8(r>>8)<<quint8(r);
}

QByteArray &operator<<(QByteArray &l, quint32 r) {
    return l<<quint16(r>>16)<<quint16(r);
}

CreateInstructionDialog::CreateInstructionDialog(const XmlReader& config,const QSqlDatabase& db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateInstructionDialog),config(config),db(db) {
    ui->setupUi(this);
    this->setWindowTitle("生成指令文件");
    for(int i=0;i<restartSize;i++){
        restartCheckBoxs.append(new QCheckBox(restartAndShutdownPrefix.arg(i+1)));
        restartCheckBoxs.back()->setCheckState(Qt::Checked);
    }
    for(int i=0;i<shutdownSize;i++){
        shutdownCheckBoxs.append(new QCheckBox(restartAndShutdownPrefix.arg(i+1)));
        shutdownCheckBoxs.back()->setCheckState(Qt::Checked);
    }
    restartCenterCheckBox = new QCheckBox("中控");
    restartCenterCheckBox->setCheckState(Qt::Checked);
    shutdownCenterCheckBox = new QCheckBox("中控");
    shutdownCenterCheckBox->setCheckState(Qt::Checked);
    searchCheckBox = new QCheckBox("运行状态查询指令");
    searchCheckBox->setCheckState(Qt::Checked);
    settingCheckBox = new QCheckBox("设备参数设置指令");
    settingCheckBox->setCheckState(Qt::Unchecked);
    createInstructionButton = new QPushButton("生成指令");
    QHBoxLayout* createInstaructionLayout = new QHBoxLayout();
    createInstaructionLayout->addStretch();
    createInstaructionLayout->addWidget(createInstructionButton);
    createInstaructionLayout->addStretch();

    restartAndShutdownLayout = new QHBoxLayout();
    restartLayout = new QVBoxLayout();
    shutdownLayout = new QVBoxLayout();
    mainLayout = new QVBoxLayout();

    restartLayout->addWidget(new QLabel("重启指令"));
    restartLayout->addWidget(restartCenterCheckBox);
    for(int i=0;i<restartSize;i+=2){
        QHBoxLayout* box = new QHBoxLayout();
        box->addWidget(restartCheckBoxs[i]);
        box->addWidget(restartCheckBoxs[i+1]);
        restartLayout->addLayout(box);
    }
    restartLayout->addStretch();

    shutdownLayout->addWidget(new QLabel("关闭指令"));
    shutdownLayout->addWidget(shutdownCenterCheckBox);
    for(int i=0;i<shutdownSize;i+=2){
        QHBoxLayout* box = new QHBoxLayout();
        box->addWidget(shutdownCheckBoxs[i]);
        box->addWidget(shutdownCheckBoxs[i+1]);
        shutdownLayout->addLayout(box);
    }
    shutdownLayout->addStretch();
    restartAndShutdownLayout->addLayout(restartLayout);
    restartAndShutdownLayout->addLayout(shutdownLayout);

    mainLayout->addLayout(restartAndShutdownLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(searchCheckBox);
    mainLayout->addStretch();
    mainLayout->addWidget(settingCheckBox);
    mainLayout->addStretch();
    mainLayout->addLayout(createInstaructionLayout);
//    mainLayout->addStretch();
    this->setLayout(mainLayout);

    totalRestartCheckBoxs.append(restartCenterCheckBox);
    totalRestartCheckBoxs.append(restartCheckBoxs);

    totalShutdownCheckBoxs.append(shutdownCenterCheckBox);
    totalShutdownCheckBoxs.append(shutdownCheckBoxs);


    connect(createInstructionButton, SIGNAL(clicked()), this, SLOT(handleSaveInstruction()));
}

CreateInstructionDialog::~CreateInstructionDialog()
{
    delete ui;
}

void CreateInstructionDialog::handleSaveInstruction() {
    this->createInstructionButton->setDisabled(true);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString baseName = QString("%1-%2.akg").arg(config.baseconfig.sytemId).arg(currentDateTime.toString("yyyyMMddhhmmss"));
    QString fileName = PathUtils::concatDir(config.baseconfig.outputDir, baseName);
    static const QString insertRunningLog =
            "INSERT INTO running_log(log_type,log_datetime,log_content) VALUES(3, :log_datetime, :log_content)";

    fileName = QFileDialog::getSaveFileName(this, tr("保存指令文件"),
                               fileName,
                               tr("akg (*.akg"));
    QFile file(fileName);
    bool isOK = file.open(QIODevice::WriteOnly);
    if(!isOK){
        QMessageBox::critical(this, "文件件错误", "") ;
        qWarning()<<file.error()<<endl;
        this->createInstructionButton->setEnabled(true);
        return;
    }
    QSqlQuery inseartRunningLogQuery(db);
    inseartRunningLogQuery.prepare(insertRunningLog);
    inseartRunningLogQuery.bindValue(":log_datetime", currentDateTime);
    inseartRunningLogQuery.bindValue(":log_content", QString("生成指令文件%1").arg(fileName));

    QByteArray frontArray;
    frontArray<<magic
             <<quint16(config.baseconfig.sytemId.toUInt())
            <<quint8(currentDateTime.date().year()-2000)
           <<quint8(currentDateTime.date().month())
          <<quint8(currentDateTime.date().day())
         <<quint8(currentDateTime.time().hour())
        <<quint8(currentDateTime.time().minute())
       <<quint8(currentDateTime.time().second())
      <<quint16(currentDateTime.time().msec());
    QByteArray dataArray;
    if(searchCheckBox->isChecked()){
        dataArray<<searchMagic<<quint16(0)<<quint16(0)<<quint16(0);
    }
    dataArray<<restartMagic;
    for(int i=0;i<totalRestartCheckBoxs.size();i++){
        if(totalRestartCheckBoxs[i]->isChecked()){
            dataArray<<activeMagic;
        }else{
            dataArray<<notActiveMagic;
        }
    }
    dataArray<<shutdownMagic;
    for(int i=0;i<totalShutdownCheckBoxs.size();i++){
        if(totalShutdownCheckBoxs[i]->isChecked()){
            dataArray<<activeMagic;
        }else{
            dataArray<<notActiveMagic;
        }
    }
    if(settingCheckBox->isChecked()){
        qWarning()<<"undefined setting param"<<endl;
        this->createInstructionButton->setEnabled(true);
        return;
    }else{
        dataArray<<quint8(0);
    }
    frontArray<<quint32(dataArray.size());

    frontArray.append(dataArray);

    quint16 checkSumValue = CrcChecksum::crc16ForX25(frontArray);
    frontArray<<checkSumValue;
    file.write(frontArray);
    file.close();
    inseartRunningLogQuery.exec();
    if(inseartRunningLogQuery.numRowsAffected()<=0){
        qWarning()<<"file log sql fail!"<<inseartRunningLogQuery.lastError()<<endl;
    }
    QMessageBox::about(this, "成功", "文件保存成功");
    this->createInstructionButton->setEnabled(true);

}
