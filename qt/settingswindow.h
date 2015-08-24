#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
    class SettingsWindow;
}

class SettingsWindow : public QMainWindow{
    Q_OBJECT

    class MainWindow *w;

    public:
        explicit SettingsWindow(QWidget *parent = 0);
        ~SettingsWindow();

    public:
        void inits();
        void setMainWindow(MainWindow *);
        void updateBtnState();

    public slots:
        void setFontSize(const QString& );
        void setFontFamily(const QString& );
        void changeResizeMargin(const QString& );
        void changeBackgroundColor();
        void changeBorderRadius(int );
        void changeBorderColor();
        void changeBorderWidth(int );
        void changeIconTheme(bool );
        void changeShadowAlpha(int);
        void changeBlurRadius(int );
        void changeShadowScale(int );
        void changeWindowMode();
        void changeTextColor();
        void requestDpiChange(int );
        void centerWindow();
        void setRandomColor();
        void onRadioBtnToggled(bool );

    private:
        Ui::SettingsWindow *ui;

    protected:
        void closeEvent(QCloseEvent *event);
        void keyPressEvent(QKeyEvent* event);
};

#endif // SETTINGSWINDOW_H

