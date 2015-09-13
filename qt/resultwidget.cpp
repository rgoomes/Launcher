#include "resultwidget.h"
#include "ui_resultwidget.h"
#include "utils.h"
#include "types.h"

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

void ResultWidget::mousePressEvent(QMouseEvent *){
    open();
}

void ResultWidget::open(){
    QDesktopServices::openUrl(QUrl("file://" + ui->path->text()));

    if(mc->hideOnApp())
        mc->hideLauncher();
}
