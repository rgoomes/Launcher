#ifndef STYLECONTAINER_HPP
#define STYLECONTAINER_HPP

#include <vector>
#include <QString>

#include "stylesheet.h"

class Container {
    std::vector<Style> objects;
    std::string path;

    public:
        Container(std::string );
        ~Container();

    public:
        void reload(int );
        void update_file();
        void setStyle(QString , QString , int );
        QString getStyle(QString , int );
        QString getStylesheet(QString , int );
};

#endif // STYLECONTAINER_HPP
