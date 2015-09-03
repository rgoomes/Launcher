#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include <QMap>
#include <QString>
#include <QStringList>

#include <string>
#include <fstream>

class WindowController {
    QMap <QString, QString > options;
    std::string path;

    public:
        WindowController(std::string );
        ~WindowController();

    public:
        void update_file();
        void load_options();
        void set_option(QString , QString );
        QString get_option(QString );
        QString windowoptions();
};

#endif // WINDOWCONTROLLER_H
