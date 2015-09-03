#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QMap>
#include <QStringList>

#include <string>
#include <fstream>

class Style {
    QMap <QString, QString > styles;
    std::string obj;

    public:
        Style(std::string, std::string );
        ~Style();

    public:
        void load_styles(std::string );
        void set_style(QString , QString );
        QString stylesheet(QString );
        QString get_style(QString );
        QString get_objname();
};

#endif // STYLESHEET_H
