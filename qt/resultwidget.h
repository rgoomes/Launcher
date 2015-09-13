#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QDesktopServices>

namespace Ui {
class ResultWidget;
}

class ResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultWidget(QWidget *parent = 0, QString = "", QString = "");
    ~ResultWidget();

    void open();

private:
    Ui::ResultWidget *ui;

protected:
    void mousePressEvent(QMouseEvent *);

};

#endif // RESULTWIDGET_H
