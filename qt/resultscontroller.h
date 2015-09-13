#ifndef RESULTSCONTROLLER_H
#define RESULTSCONTROLLER_H

#include <QDesktopServices>
#include <QLayout>
#include <QObject>
#include <vector>

#include "resultwidget.h"

class ResultsController : public QObject{
    Q_OBJECT

    std::vector<ResultWidget*> results;
    QWidget *resultsWidget;
    QLayout *resultsLayout;

    public:
        ResultsController(QWidget *);
        ~ResultsController();

        void setStretcher(bool );
        void openFirstResult();

    public slots:
        void clearResults();
        void addResult(QString , QString );
};

#endif // RESULTSCONTROLLER_H
