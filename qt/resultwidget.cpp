#include "resultwidget.h"
#include "ui_resultwidget.h"
#include "utils.h"

ResultWidget::~ResultWidget(){ delete ui; }
ResultWidget::ResultWidget(QWidget *parent, QString name, QString path) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);
    ui->name->setText(name);
    ui->path->setText(path);
    ui->icon->setPixmap(fileIcon(path).pixmap(QSize(48, 48)));
}

