#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QMap>
#include <string>

using namespace std;

class Style {
    QMap <QString, QString > styles;
    string obj;

    public:
        Style(string, string );
        ~Style();

    public:
        void load_styles(string );
        void set_style(QString , QString );
        QString stylesheet(QString );
        QString get_style(QString );
        QString get_objname();
};

#endif // STYLESHEET_H
