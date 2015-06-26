#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QMap>

class Style {
    QMap <QString, QString > styles;
    std::string path;

    public:
        Style(std::string );
        ~Style();

    public:
        void update_file();
        void load_user_preferences();
        void set_style(QString , QString );
        QString stylesheet(QString );
        QString get_style(QString );
};

#endif // STYLESHEET_H
