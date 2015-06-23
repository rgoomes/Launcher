#include <fstream>

#include <QString>
#include <QDebug>
#include <QMap>

#include "stylesheet.h"

Style::Style(){ this->load_user_preferences(); }
Style::~Style(){}

QMap <QString, QString > get_default_style(){
    QMap <QString, QString > default_styles;

    default_styles["border-radius"] = "20px";
    default_styles["background-color"] = "rgba(255, 255, 255, 1)";

    return default_styles;
}

void Style::update_file(){
    QString style_sheet = this->get_stylesheet();
    std::ofstream file("../User/stylesheet.user");

    file << style_sheet.toUtf8().constData();
    file.close();
}

void Style::update_style(QString key, QString value, bool to_update){
    styles[key] = value;

    if(to_update)
        this->update_file();
}

QString Style::get_style(QString key){ return styles[key]; }

QString Style::get_stylesheet(){
    QString style_sheet;

    QMap<QString, QString>::const_iterator it = styles.begin();
    while (it != styles.end()) {
        style_sheet += it.key() + ":" + it.value() + ";\n";
        ++it;
    }

    return style_sheet;
}

void Style::load_user_preferences(){
    std::ifstream file("../User/stylesheet.user");

    if(!file.good())
        this->styles = get_default_style();
    else {
        std::string pref;
        while(std::getline(file, pref)){
            if(!pref.length())
                continue;
            if(pref.find_first_not_of(" \t\v\f\r") == std::string::npos)
                continue;

            QStringList spref = QString::fromStdString(pref).split(QRegExp("[:]"), QString::SkipEmptyParts);
            if(spref.count() <= 1)
                continue;

            this->update_style((*spref.begin()).remove(QChar(':')), (*++spref.begin()).remove(QChar(';')), false);
        }
    }

    file.close();
}
