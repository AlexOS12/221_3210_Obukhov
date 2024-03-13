#include "recordwiget.h"
#include "ui_recordwiget.h"

recordWiget::recordWiget(QWidget *parent, int recordId, QString site)
    : QWidget(parent)
    , ui(new Ui::recordWiget)
{
    ui->setupUi(this);
    ui->siteLabel->setText(site);
    this->recordId = recordId;
}

recordWiget::~recordWiget()
{
    delete ui;
}

void recordWiget::on_copyLogin_clicked()
{
    emit copyRecordLogin(this->recordId);
}


void recordWiget::on_copyPass_clicked()
{
    emit copyRecordPass(this->recordId);
}

