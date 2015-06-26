#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include <QMap>

class WindowController {
    QMap <QString, QString > options;
    std::string path;

    public:
        WindowController(std::string );
        ~WindowController();

    public:
        void update_file();
        void load_user_window_options();
        void set_option(QString , QString );
        QString get_option(QString );
        QString windowoptions();
};

#endif // WINDOWCONTROLLER_H
