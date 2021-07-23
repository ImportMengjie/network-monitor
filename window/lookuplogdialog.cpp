#include "lookuplogdialog.h"
#include "ui_lookuplogdialog.h"
#include <QCheckBox>

LookUpLogDialog::LookUpLogDialog(const XmlReader &config, QWidget *parent) :
    QDialog(parent),config(config),
    ui(new Ui::LookUpLogDialog) {
    ui->setupUi(this);
    this->setWindowTitle("记录查看");
    searchTypeButtonGroup = new QButtonGroup();
    statusCheckBox = new QCheckBox("状态");
    statusCheckBox->setChecked(true);
    errorCheckBox = new QCheckBox("报警");
    errorCheckBox->setChecked(true);
    instructionCheckBox = new QCheckBox("指令");
    instructionCheckBox->setChecked(true);
//    searchTypeButtonGroup->addButton(statusCheckBox, 1);
//    searchTypeButtonGroup->addButton(errorCheckBox, 2);
//    searchTypeButtonGroup->addButton(instructionCheckBox, 3);
//    searchButton->setAutoExclusive(false);

    searchKeywordWidget = new QLineEdit();

    startDateWidget = new QDateEdit(QDate::currentDate());
    endDateWidget = new QDateEdit(QDate::currentDate().addDays(1));
    startDateWidget->setCalendarPopup(true);
    endDateWidget->setCalendarPopup(true);

    searchButton = new QPushButton("查询");
    outputButton = new QPushButton("导出");

    tableWidget = new QTableWidget(40, 4);
    tableWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);


    mainLayout = new QVBoxLayout();
    searchParamLayout = new QHBoxLayout();

    searchParamLayout->addWidget(statusCheckBox);
    searchParamLayout->addWidget(errorCheckBox);
    searchParamLayout->addWidget(instructionCheckBox);

    searchParamLayout->addWidget(searchKeywordWidget);

    searchParamLayout->addWidget(startDateWidget);
    searchParamLayout->addWidget(endDateWidget);

    searchParamLayout->addWidget(searchButton);
    searchParamLayout->addWidget(outputButton);
    searchParamLayout->addStretch();

    mainLayout->addLayout(searchParamLayout);
    mainLayout->addWidget(tableWidget);
//    mainLayout->addStretch();

    this->setLayout(mainLayout);
}


LookUpLogDialog::~LookUpLogDialog() {
    delete ui;
}
