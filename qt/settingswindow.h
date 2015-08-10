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
        void updateBtnWindowState();

    public slots:
        void radiobtn_toggled(bool );
        void checkbox_toggled(bool );
        void new_bordersize(int );
        void font_sizechange(const QString& );
        void change_windowstate();
        void change_shadowstate();
        void random_color();

    private:
        Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H

