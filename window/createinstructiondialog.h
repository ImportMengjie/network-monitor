#ifndef CREATEINSTRUCTIONDIALOG_H
#define CREATEINSTRUCTIONDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QVector>

#include "bean/xmlreader.h"

namespace Ui {
class CreateInstructionDialog;
}



class CreateInstructionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateInstructionDialog(const XmlReader& config,const QSqlDatabase& db, QWidget *parent = nullptr);
    ~CreateInstructionDialog();

    const static quint16 magic;
    const static quint16 searchMagic;
    const static quint32 restartMagic;
    const static quint32 shutdownMagic;

    const static quint16 activeMagic;
    const static quint16 notActiveMagic;

public slots:
    void handleSaveInstruction();

private:

    Ui::CreateInstructionDialog *ui;
    XmlReader config;
    QSqlDatabase db;

    int restartSize = 6;
    int shutdownSize = 6;
    QString restartAndShutdownPrefix="转发%1";

    QCheckBox* restartCenterCheckBox;
    QCheckBox* shutdownCenterCheckBox;
    QVector<QCheckBox*> restartCheckBoxs;
    QVector<QCheckBox*> shutdownCheckBoxs;

    QVector<QCheckBox*> totalRestartCheckBoxs;
    QVector<QCheckBox*> totalShutdownCheckBoxs;

    QCheckBox* searchCheckBox;
    QCheckBox* settingCheckBox;

    QPushButton* createInstructionButton;

    QHBoxLayout* restartAndShutdownLayout;
    QVBoxLayout* restartLayout;
    QVBoxLayout* shutdownLayout;
    QVBoxLayout* mainLayout;


};

#endif // CREATEINSTRUCTIONDIALOG_H
