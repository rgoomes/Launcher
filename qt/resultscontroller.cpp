#include "resultscontroller.h"
#include "types.h"

#define NO_RESULT_SELECTED -1

QWidget* stretcher;

ResultsController::~ResultsController(){}
ResultsController::ResultsController(QWidget *resultsWidget, QScrollArea *scrollArea){
    setStretcher(true);

    this->scrollArea    = scrollArea;
    this->resultsWidget = resultsWidget;
    this->resultsLayout = resultsWidget->layout();
    this->curResultSelected = NO_RESULT_SELECTED;
}

void ResultsController::addResult(QString name, QString path){
    // TODO: PLACE RESULT ORDERED BY ACCESS DATE
    if(int(results.size()) == mc->getMaxResults())
        return;
    if(results.size())
        resultsLayout->removeWidget(stretcher);

    ResultWidget *res = new ResultWidget(resultsWidget, name, path);
    results.push_back(res);

    if(results.size() == 1 || curResultSelected == NO_RESULT_SELECTED){
        curResultSelected = 0;
        res->selectResult();
        updatePreviewImage(path);
    }

    resultsLayout->addWidget(res);
    resultsLayout->addWidget(stretcher);
}

void ResultsController::clearResults(){
    for(ResultWidget *result : results)
        delete result;

    this->results.clear();
    this->clearSelection(NULL);
}

void ResultsController::setStretcher(bool isVertical){
    if(stretcher)
        delete stretcher;

    stretcher = new QWidget();
    stretcher->setSizePolicy(isVertical ? QSizePolicy::Preferred : QSizePolicy::Expanding,
                             isVertical ? QSizePolicy::Expanding : QSizePolicy::Preferred);
}

void ResultsController::openSelectedResult(){
    if(results.size()){
        ResultWidget *tmp = results[curResultSelected == NO_RESULT_SELECTED ? 0 : curResultSelected];
        tmp->open();
    }
}

void ResultsController::setSelectedResult(ResultWidget *cur){
    if(curResultSelected > NO_RESULT_SELECTED)
        results[curResultSelected]->deselectResult();

    for(int pos = 0; pos < int(results.size()); pos++){
        if(cur == results[pos]){
            curResultSelected = pos;
            updatePreviewImage(cur->path());
            break;
        }
    }
}

void ResultsController::changeSelection(int dir){
    if(!results.size())
        return;
    if(results.size() == 1){
        curResultSelected = 0;
        results[curResultSelected]->selectResult();
        updatePreviewImage(results[curResultSelected]->path());
        return;
    }
    if(curResultSelected == NO_RESULT_SELECTED){
        curResultSelected = dir > 0 ? 0 : results.size() - 1;
        results[curResultSelected]->selectResult();
        updatePreviewImage(results[curResultSelected]->path());
        return;
    }

    results[curResultSelected]->deselectResult();
    curResultSelected = (curResultSelected + dir) % results.size();
    results[curResultSelected]->selectResult();

    updatePreviewImage(results[curResultSelected]->path());

    scrollArea->ensureWidgetVisible(results[curResultSelected], 0,0);
    scrollArea->horizontalScrollBar()->setValue(0);
}

void ResultsController::clearSelection(ResultWidget *cur){
    if(cur == NULL){
        curResultSelected = NO_RESULT_SELECTED;
        return;
    }

    if(cur == results[curResultSelected])
        curResultSelected = NO_RESULT_SELECTED;
}

void ResultsController::updateSelectionColor(){
    if(curResultSelected > NO_RESULT_SELECTED)
        results[curResultSelected]->selectResult();
}

void ResultsController::updatePreviewImage(QString path){
     mc->setPreviewPath(isImage(extension(path)) ? path : "");
     mc->canRender = true;

     // THIS FIXES PAINT EVENTS WHEN IN FULLSCREEN
     mc->reloadPreview();
}
