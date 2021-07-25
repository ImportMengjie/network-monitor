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
    void handleNextPage(){
        if(this->currentPage+1>=getPageCount())
            return;
        this->currentPage++;
        setPageLabel();
        setTableData();
    }
    void handlePrePage(){
        if(this->currentPage>0){
            this->currentPage--;
            setPageLabel();
            setTableData();
        }
    }
    void handleGoPage(){
        bool ok;
        uint page = switchPageEdit->text().toUInt(&ok);
        if(ok&&page>0){
            currentPage = page-1;
            setPageLabel();
            setTableData();
        }
    }

    void saveToCsvFile(){
        QString fileName = QFileDialog::getSaveFileName(this, "保存日志文件", "", "csv Files (*.csv)");
        if(fileName.isEmpty()) return;
        QFile csvFile(fileName);
        if(csvFile.exists()) return;
        csvFile.open( QIODevice::ReadWrite | QIODevice::Text );
        QTextStream out(&csvFile);
        static QString selectState = "SELECT * FROM running_log";
        QDateTime start(startDateWidget->date(), QTime(0,0));
        QDateTime end(endDateWidget->date(), QTime(0,0));

        QString whereState = QString(" where (log_datetime>=:start and log_datetime<=:end)%1%2 order by seq")
                    .arg(getSelectLogTypeFilter())
                    .arg(getSelectLogKeyword());

        QString selectWhereState = selectState + whereState;
        if(!db.isOpen())
            db.open();
        if(db.isOpen()){
            QSqlQuery pageDataQuery(db);
            pageDataQuery.prepare(selectWhereState);
            pageDataQuery.bindValue(":start", start);
            pageDataQuery.bindValue(":end", end);
            pageDataQuery.exec();
            if(!pageDataQuery.isActive()){
                qWarning()<<pageDataQuery.lastError()<<endl;
                return;
            }
            while(pageDataQuery.next()){
                out<<pageDataQuery.value(0).toUInt()<<','
                  <<pageDataQuery.value(1).toUInt()<<','
                 <<pageDataQuery.value(2).toDateTime().toString("yyyy-MM-dd:hh:mm:ss:zzz")<<','
                <<pageDataQuery.value(3).toString()<<endl;
            }
            csvFile.close();
        }

    }

private:

    uint getTotalCount(){
        static QString countState = "SELECT count(*) FROM running_log";
        QDateTime start(startDateWidget->date(), QTime(0,0));
        QDateTime end(endDateWidget->date(), QTime(0,0));


        QString whereState = QString(" where (log_datetime>=:start and log_datetime<=:end)%1%2 order by seq")
                    .arg(getSelectLogTypeFilter())
                    .arg(getSelectLogKeyword());

        QString countWhereState = countState + whereState;
        if(!db.isOpen())
            db.open();
        if(db.isOpen()){
            QSqlQuery countTotalSize(db);
            countTotalSize.prepare(countWhereState);
            countTotalSize.bindValue(":start", start);
            countTotalSize.bindValue(":end", end);
            countTotalSize.exec();
            if(!countTotalSize.isActive()||!countTotalSize.first()){
                qWarning()<<countTotalSize.lastError()<<endl;
                return 0;
            }
            totalCount = countTotalSize.value(0).toUInt();
            setPageLabel();
            return totalCount;
        }
        return 0;
    }


    void setTableData(){
        static QString selectState = "SELECT * FROM running_log";
        QDateTime start(startDateWidget->date(), QTime(0,0));
        QDateTime end(endDateWidget->date(), QTime(0,0));

        QString whereState = QString(" where (log_datetime>=:start and log_datetime<=:end)%1%2 order by seq")
                    .arg(getSelectLogTypeFilter())
                    .arg(getSelectLogKeyword());
        QString limitOffsetState = QString(" limit %1 offset %2").arg(pageSize).arg(currentPage*pageSize);

        QString selectWhereState = selectState + whereState + limitOffsetState;
        if(!db.isOpen())
            db.open();
        if(db.isOpen()){
            QSqlQuery pageDataQuery(db);
            pageDataQuery.prepare(selectWhereState);
            pageDataQuery.bindValue(":start", start);
            pageDataQuery.bindValue(":end", end);
            pageDataQuery.exec();
            if(!pageDataQuery.isActive()){
                qWarning()<<pageDataQuery.lastError()<<endl;
                return;
            }
            tableQuery->setQuery(pageDataQuery);
            this->setTableQuery();

        }
        qDebug()<<whereState;
    }

    uint getPageCount(){
        return totalCount/pageSize + (totalCount%pageSize==0?0:1);
    }

    void setPageLabel(){
        totalPageLabel->setText(QString("共%2条记录 %1页").arg(getPageCount()).arg(totalCount));
        currentPageLabel->setText(QString("当前%1页").arg(currentPage+1));
    }

    void setTableQuery(){
        tableQuery->setHeaderData(running_log::seq, Qt::Horizontal, "序号");
        tableQuery->setHeaderData(running_log::log_datetime, Qt::Horizontal, "时间");
        tableQuery->setHeaderData(running_log::log_type, Qt::Horizontal, "类型");
        tableQuery->setHeaderData(running_log::log_content, Qt::Horizontal, "内容");

        tableView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->resizeColumnsToContents();
        tableView->horizontalHeader()->setStretchLastSection(true);

    }

    QString getSelectLogTypeFilter(){
        bool status = statusCheckBox->isChecked();
        bool error = errorCheckBox->isChecked();
        bool instruct = instructionCheckBox->isChecked();
        if(!status&&!error&&!instruct)
            return "";
        else if(status&&error&&instruct)
            return "";
        else{
            QString start(" and (");
            QString mid("");
            mid += status?"log_type=1":"";
            if(error){
                if(mid.size())
                    mid+=" or log_type=2";
                else
                    mid+="log_type=2";
            }
            if(instruct){
                if(mid.size())
                    mid+=" or log_type=3";
                else
                    mid+="log_type=3";
            }
            return start+mid+")";
        }
    }

    QString getSelectLogKeyword(){
        if(searchKeywordWidget->text().size()){
            QString text = searchKeywordWidget->text();

            return " and log_content like '%" + text + "%'";
        }
        return "";
    }

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
