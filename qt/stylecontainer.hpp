#ifndef STYLECONTAINER_HPP
#define STYLECONTAINER_HPP

#include <vector>
#include <string>
#include "stylesheet.h"

using namespace std;

class Container {
    vector<Style> objects;
    string path;

    public:
        Container(string );
        ~Container();

    public:
        void reload(int );
        void update_file();
        void setStyle(QString , QString , int );
        QString getStyle(QString , int );
        QString getStylesheet(QString , int );
};

#endif // STYLECONTAINER_HPP
