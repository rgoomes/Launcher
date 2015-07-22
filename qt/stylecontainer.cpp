#include <fstream>

#include <QString>
#include <QDebug>

#include "stylecontainer.hpp"
#include "stylesheet.h"

Container::~Container(){}
Container::Container(std::string path){
    objects.push_back(Style("Frame", path));
    objects.push_back(Style("Sbox",  path));

    this->path = path;
    this->update_file();
}

void Container::update_file(){
    QString styles;

    for(auto s : objects)
        styles += s.get_objname() + "\n" + s.stylesheet("") + "\n";

    std::ofstream file(this->path);
    file << styles.toUtf8().constData();
    file.close();
}

void Container::reload(int obj){ objects[obj].load_user_preferences(this->path); }
void Container::setStyle(QString key, QString value, int obj){ objects[obj].set_style(key, value); }
QString Container::getStyle(QString key, int obj){ return objects[obj].get_style(key); }
QString Container::getStylesheet(QString obj_name, int obj){ return objects[obj].stylesheet(obj_name); }

