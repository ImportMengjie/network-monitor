#include "lookuplogdialog.h"
#include "ui_lookuplogdialog.h"
#include <QAbstractItemView>
#include <QCheckBox>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

LookUpLogDialog::LookUpLogDialog(const XmlReader &config,const QSqlDatabase& db, QWidget *parent) :
    QDialog(parent),config(config),db(db),
    ui(new Ui::LookUpLogDialog) {
    ui->setupUi(this);
    this->setWindowTitle("记录查看");
    statusCheckBox = new QCheckBox("状态");
    statusCheckBox->setChecked(true);
    errorCheckBox = new QCheckBox("报警");
    errorCheckBox->setChecked(true);
    instructionCheckBox = new QCheckBox("指令");
    instructionCheckBox->setChecked(true);

    searchKeywordWidget = new QLineEdit();

    startDateWidget = new QDateEdit(QDate::currentDate());
    endDateWidget = new QDateEdit(QDate::currentDate().addDays(1));
    startDateWidget->setCalendarPopup(true);
    endDateWidget->setCalendarPopup(true);

    searchButton = new QPushButton("查询");
    outputButton = new QPushButton("导出");

    tableQuery = new QSqlQueryModel(this);

    tableView = new QTableView();
//    tableView->setColumnWidth(4, 10);
    tableView->setModel(tableQuery);


    totalPageLabel = new QLabel();
    currentPageLabel = new QLabel();
    nextPage = new QPushButton("下一页");
    prePage = new QPushButton("上一页");
    switchPageEdit = new QLineEdit();
    switchPageEdit->setFixedWidth(40);
    switchPage = new QPushButton("go");


    mainLayout = new QVBoxLayout();
    searchParamLayout = new QHBoxLayout();
    searchPageLayout = new QHBoxLayout();

    searchParamLayout->addWidget(statusCheckBox);
    searchParamLayout->addWidget(errorCheckBox);
    searchParamLayout->addWidget(instructionCheckBox);

    searchParamLayout->addWidget(searchKeywordWidget);

    searchParamLayout->addWidget(startDateWidget);
    searchParamLayout->addWidget(endDateWidget);

    searchParamLayout->addWidget(searchButton);
    searchParamLayout->addWidget(outputButton);
    searchParamLayout->addStretch();


    searchPageLayout->addWidget(totalPageLabel);


    searchPageLayout->addStretch();

    searchPageLayout->addWidget(prePage);
    searchPageLayout->addWidget(currentPageLabel);
    searchPageLayout->addWidget(nextPage);

    searchPageLayout->addStretch();

    searchPageLayout->addWidget(new QLabel("转到"));
    searchPageLayout->addWidget(switchPageEdit);
    searchPageLayout->addWidget(new QLabel("页"));
    searchPageLayout->addWidget(switchPage);

    mainLayout->addLayout(searchParamLayout);
    mainLayout->addWidget(tableView);
    mainLayout->addLayout(searchPageLayout);

    this->setLayout(mainLayout);
    this->setPageLabel();
    this->setTableQuery();

    connect(searchButton,SIGNAL(clicked()), this, SLOT(handleSearch()));
    connect(nextPage,SIGNAL(clicked()), this, SLOT(handleNextPage()));
    connect(prePage,SIGNAL(clicked()), this, SLOT(handlePrePage()));
    connect(switchPage,SIGNAL(clicked()), this, SLOT(handleGoPage()));
    connect(outputButton,SIGNAL(clicked()), this, SLOT(saveToCsvFile()));
}


LookUpLogDialog::~LookUpLogDialog() {
    delete ui;
}

void LookUpLogDialog::handleSearch() {
    getTotalCount();
    setTableData();

}

void LookUpLogDialog::handleNextPage(){
    if(this->currentPage+1>=getPageCount())
        return;
    this->currentPage++;
    setPageLabel();
    setTableData();
}

void LookUpLogDialog::handlePrePage(){
    if(this->currentPage>0){
        this->currentPage--;
        setPageLabel();
        setTableData();
    }
}

void LookUpLogDialog::handleGoPage(){
    bool ok;
    uint page = switchPageEdit->text().toUInt(&ok);
    if(ok&&page>0){
        currentPage = page-1;
        setPageLabel();
        setTableData();
    }
}

void LookUpLogDialog::saveToCsvFile(){
    QString fileName = QFileDialog::getSaveFileName(this, "保存日志文件", "", "csv Files (*.csv)");
    if(fileName.isEmpty()) return;
    QFile csvFile(fileName);
    if(csvFile.exists()) return;
    csvFile.open( QIODevice::ReadWrite | QIODevice::Text );
    QTextStream out(&csvFile);
    static QString selectState = "SELECT * FROM running_log";
    QDateTime start(startDateWidget->date(), QTime(0,0));
    QDateTime end(endDateWidget->date(), QTime(0,0));

    QString whereState = QString(" where (log_datetime>=:start and log_datetime<=:end)%1%2 order by log_datetime")
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
             <<pageDataQuery.value(1).toDateTime().toString("yyyy-MM-dd:hh:mm:ss:zzz")<<','
            <<pageDataQuery.value(2).toString()<<endl;
        }
        csvFile.close();
        QMessageBox::about(this, "成功", "文件保存成功");
    }

}

uint LookUpLogDialog::getTotalCount(){
    static QString countState = "SELECT count(*) FROM running_log";
    QDateTime start(startDateWidget->date(), QTime(0,0));
    QDateTime end(endDateWidget->date(), QTime(0,0));


    QString whereState = QString(" where (log_datetime>=:start and log_datetime<=:end)%1%2 order by log_datetime")
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

void LookUpLogDialog::setTableData(){
    static QString selectState = "SELECT * FROM running_log";
    QDateTime start(startDateWidget->date(), QTime(0,0));
    QDateTime end(endDateWidget->date(), QTime(0,0));

    QString whereState = QString(" where (log_datetime>=:start and log_datetime<=:end)%1%2 order by log_datetime")
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
}

uint LookUpLogDialog::getPageCount(){
    return totalCount/pageSize + (totalCount%pageSize==0?0:1);
}

void LookUpLogDialog::setPageLabel(){
    totalPageLabel->setText(QString("共%2条记录 %1页").arg(getPageCount()).arg(totalCount));
    currentPageLabel->setText(QString("当前%1页").arg(currentPage+1));
}

void LookUpLogDialog::setTableQuery(){
    // tableQuery->setHeaderData(running_log::seq, Qt::Horizontal, "序号");
    tableQuery->setHeaderData(running_log::log_datetime, Qt::Horizontal, "时间");
    tableQuery->setHeaderData(running_log::log_type, Qt::Horizontal, "类型");
    tableQuery->setHeaderData(running_log::log_content, Qt::Horizontal, "内容");

    tableView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->resizeColumnsToContents();
    tableView->horizontalHeader()->setStretchLastSection(true);

}

QString LookUpLogDialog::getSelectLogTypeFilter(){
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

QString LookUpLogDialog::getSelectLogKeyword(){
    if(searchKeywordWidget->text().size()){
        QString text = searchKeywordWidget->text();

        return " and log_content like '%" + text + "%'";
    }
    return "";
}


