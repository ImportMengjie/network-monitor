#ifndef LOOKUPLOGDIALOG_H
#define LOOKUPLOGDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QBoxLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QTableWidget>
#include "bean/xmlreader.h"

namespace Ui {
class LookUpLogDialog;
}

class LookUpLogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LookUpLogDialog(const XmlReader& config,QWidget *parent = nullptr);
    ~LookUpLogDialog();

private:
    XmlReader config;
    Ui::LookUpLogDialog *ui;

    QButtonGroup* searchTypeButtonGroup;
    QCheckBox* statusCheckBox;
    QCheckBox* errorCheckBox;
    QCheckBox* instructionCheckBox;

    QLineEdit* searchKeywordWidget;
    QDateEdit* startDateWidget;
    QDateEdit* endDateWidget;
    QPushButton* searchButton;
    QPushButton* outputButton;
    QTableWidget* tableWidget;



    QVBoxLayout* mainLayout;
    QHBoxLayout* searchParamLayout;

};

#endif // LOOKUPLOGDIALOG_H
