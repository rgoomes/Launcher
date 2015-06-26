#include <fstream>

#include <QString>
#include <QDebug>
#include <QMap>

#include "stylesheet.h"

Style::~Style(){}
Style::Style(std::string path){ this->path = path; this->load_user_preferences(); }

QMap <QString, QString > get_default_style(){
    QMap <QString, QString > default_styles;

    default_styles["border-radius"] = "20px";
    default_styles["background-color"] = "rgba(255, 255, 255, 1)";

    return default_styles;
}

void Style::update_file(){
    QString style_sheet = this->stylesheet("");
    std::ofstream file(this->path);

    file << style_sheet.toUtf8().constData();
    file.close();
}

void Style::set_style(QString key, QString value){ styles[key] = value; }
QString Style::get_style(QString key){ return styles[key]; }

QString Style::stylesheet(QString obj_name){
    QString style_sheet;

    QMap<QString, QString>::const_iterator it = styles.begin();
    while (it != styles.end()) {
        style_sheet += it.key() + ":" + it.value() + ";\n";
        ++it;
    }

    if(!obj_name.length())
        return style_sheet;

    return "#" + obj_name + "{" + style_sheet + "}";
}

void Style::load_user_preferences(){
    std::ifstream file(this->path);

    if(!file.good()){
        this->styles = get_default_style();
        this->update_file();
    } else {
        std::string pref;
        while(std::getline(file, pref)){
            if(!pref.length())
                continue;
            if(pref.find_first_not_of(" \t\v\f\r") == std::string::npos)
                continue;

            QStringList spref = QString::fromStdString(pref).split(QRegExp("[:]"), QString::SkipEmptyParts);
            if(spref.count() <= 1)
                continue;

            this->set_style((*spref.begin()).remove(QChar(':')), (*++spref.begin()).remove(QChar(';')));
        }
    }

    file.close();
}
