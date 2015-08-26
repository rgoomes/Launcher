#include "resultwidget.h"
#include "ui_resultwidget.h"

ResultWidget::ResultWidget(QWidget *parent, QString name) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);
    ui->name->setText(name);
}

ResultWidget::~ResultWidget()
{
    delete ui;
}
