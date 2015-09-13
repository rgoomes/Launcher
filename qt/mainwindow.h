#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QRect>
#include <QToolButton>
#include <QThread>
#include <QIcon>
#include <QShortcut>
#include <QLineEdit>
#include <QFrame>
#include <QWheelEvent>
#include <QKeyEvent>

#include "settingswindow.h"
#include "stylesheet.h"
#include "stylecontainer.hpp"
#include "windowcontroller.h"
#include "worker.h"
#include "resultwidget.h"
#include "resultscontroller.h"

extern const int FRAME;
extern const int SBOX;
extern const int ICON;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public:
        void inits();
        void goFullScreenMode();
        void goWindowMode();
        void goMode();
        void center_window();
        void change_dpi(double , bool);
        void request_resize();
        void setupWorker();
        void setupCleaner();
        void setupGlobalShortcut();
        void signals_handler();
        void updateFiles();
        void updateResults();
        bool in_fullscreen();
        int toDpi(QString );
        int toPx(int );

        QLineEdit* sboxUi();
        QFrame* frameUi();
        QToolButton* iconUi();
        bool settingsOpened = false;

    public slots:
        void clear_trigged();
        void selection_changed();
        void text_changed(QString );
        void onFullscreenShortcut();

    private:
        class SettingsWindow *settingsWindow;
        class GlobalShortcut *globalshortcut;
        class ResultsController *rc;
        class Worker* worker;

        WindowController *wc;
        Ui::MainWindow *ui;
        Container *ct;
        QToolButton *icon;
        QPoint mpos;

        bool resizing = false;
        bool scaling  = false;

    protected:
        bool eventFilter(QObject *, QEvent *);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void resizeEvent(QResizeEvent* event);
        void keyPressEvent(QKeyEvent* event);

};

#endif // MAINWINDOW_H
