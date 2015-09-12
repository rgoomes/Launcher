#include "resultscontroller.h"

QWidget* stretcher;

ResultsController::ResultsController(QWidget *resultsWidget, int maxResults){
    setStretcher(true);
    this->maxResults = maxResults;
    this->resultsWidget = resultsWidget;
    this->resultsLayout = resultsWidget->layout();
}

void ResultsController::addResult(QString name, QString path){
    // TODO: PLACE RESULT ORDERED BY ACCESS DATE
    if(int(results.size()) == maxResults)
        return;
    if(results.size())
        resultsLayout->removeWidget(stretcher);

    ResultWidget *res = new ResultWidget(resultsWidget, name, path);
    results.push_back(res);

    resultsLayout->addWidget(res);
    resultsLayout->addWidget(stretcher);
}

void ResultsController::clearResults(){
    for(ResultWidget *result : results)
        delete result;

    results.clear();
}

void ResultsController::setStretcher(bool isVertical){
    if(stretcher)
        delete stretcher;

    stretcher = new QWidget();
    stretcher->setSizePolicy(isVertical ? QSizePolicy::Preferred : QSizePolicy::Expanding,
                             isVertical ? QSizePolicy::Expanding : QSizePolicy::Preferred);
}
