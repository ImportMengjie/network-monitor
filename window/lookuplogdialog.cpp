#include "lookuplogdialog.h"
#include "ui_lookuplogdialog.h"
#include <QAbstractItemView>
#include <QCheckBox>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

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


