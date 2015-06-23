#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include <QMap>

class WindowController {
    QMap <QString, QString > options;

    public:
        WindowController();
        ~WindowController();

    public:
        void update_file();
        void load_user_window_options();
        void set_option(QString , QString , bool );
        QString get_option(QString );
        QString get_all_options();
};

#endif // WINDOWCONTROLLER_H
