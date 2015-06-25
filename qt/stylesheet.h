#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QMap>

class Style {
    QMap <QString, QString > styles;

    public:
        Style();
        ~Style();

    public:
        void update_file();
        void load_user_preferences();
        void update_style(QString , QString , bool );
        QString get_stylesheet();
        QString get_style(QString );
};

#endif // STYLESHEET_H
