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
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QLabel>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QHeaderView>
#include <QFileDialog>
#include "bean/xmlreader.h"

namespace Ui {
class LookUpLogDialog;
}

class LookUpLogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LookUpLogDialog(const XmlReader& config,const QSqlDatabase& db,QWidget *parent = nullptr);
    ~LookUpLogDialog();

public slots:
    void handleSearch();
    void handleNextPage();
    void handlePrePage();
    void handleGoPage();

    void saveToCsvFile();

private:

    uint getTotalCount();


    void setTableData();

    uint getPageCount();

    void setPageLabel();

    void setTableQuery();

    QString getSelectLogTypeFilter();

    QString getSelectLogKeyword();

    enum running_log{
        seq=0,
        log_type=1,
        log_datetime=2,
        log_content=3

    };
    XmlReader config;
    QSqlDatabase db;
    Ui::LookUpLogDialog *ui;

    uint totalCount = 0;
    uint currentPage = 0;
    uint pageSize = 20;

    QCheckBox* statusCheckBox;
    QCheckBox* errorCheckBox;
    QCheckBox* instructionCheckBox;
    QLineEdit* searchKeywordWidget;
    QDateEdit* startDateWidget;
    QDateEdit* endDateWidget;
    QPushButton* searchButton;
    QPushButton* outputButton;


    QLabel* totalPageLabel;
    QLabel* currentPageLabel;
    QPushButton* nextPage;
    QPushButton* prePage;
    QLineEdit* switchPageEdit;
    QPushButton* switchPage;

    QSqlQueryModel* tableQuery;
    QTableView* tableView;

    QVBoxLayout* mainLayout;
    QHBoxLayout* searchParamLayout;
    QHBoxLayout* searchPageLayout;


};

#endif // LOOKUPLOGDIALOG_H
