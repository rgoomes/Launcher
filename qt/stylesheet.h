#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QMap>

#include <unordered_map>

using namespace std;

class Style {
    QMap <QString, QString > styles;

    public:
        Style();
        ~Style();

    public:
        void update_style(QString key, QString value);
        QString get_stylesheet();
};

#endif // STYLESHEET_H
