#ifndef RESULTSCONTROLLER_H
#define RESULTSCONTROLLER_H

#include <QLayout>
#include <vector>

#include "resultwidget.h"

class ResultsController{
    std::vector<ResultWidget*> results;
    QWidget *resultsWidget;
    QLayout *resultsLayout;
    int maxResults;

    public:
        ResultsController(QWidget *, int);

        void clearResults();
        void setStretcher(bool );
        void addResult(QString , QString );
};

#endif // RESULTSCONTROLLER_H
