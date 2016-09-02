#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QUrl>

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
    void selectResult();
    void deselectResult();
    QString path();

private:
    Ui::ResultWidget *ui;

protected:
    void mousePressEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

};

#endif // RESULTWIDGET_H
