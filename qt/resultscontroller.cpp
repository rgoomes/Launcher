#include "resultscontroller.h"
#include "types.h"

QWidget* stretcher;

ResultsController::~ResultsController(){}
ResultsController::ResultsController(QWidget *resultsWidget){
    setStretcher(true);

    this->resultsWidget = resultsWidget;
    this->resultsLayout = resultsWidget->layout();
}

void ResultsController::addResult(QString name, QString path){
    // TODO: PLACE RESULT ORDERED BY ACCESS DATE
    if(int(results.size()) == mc->getMaxResults())
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

void ResultsController::openFirstResult(){
    if(results.size()){
        ResultWidget *tmp = results[0];
        tmp->open();
    }
}
