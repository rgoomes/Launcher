#ifndef RESULTSCONTROLLER_H
#define RESULTSCONTROLLER_H

#include <QScrollArea>
#include <QScrollBar>
#include <QDesktopServices>
#include <QLayout>
#include <QObject>
#include <vector>

#include "resultwidget.h"

class ResultsController : public QObject{
    Q_OBJECT

    std::vector<ResultWidget*> results;
    QScrollArea *scrollArea;
    QWidget *resultsWidget;
    QLayout *resultsLayout;
    int curResultSelected;

    public:
        ResultsController(QWidget *, QScrollArea *);
        ~ResultsController();

        void setStretcher(bool );
        void openSelectedResult();
        void setSelectedResult(ResultWidget *);
        void changeSelection(int );
        void clearSelection(ResultWidget *);
        void updateSelectionColor();
        void updatePreviewImage(QString );

    public slots:
        void clearResults();
        void addResult(QString , QString );
};

#endif // RESULTSCONTROLLER_H
