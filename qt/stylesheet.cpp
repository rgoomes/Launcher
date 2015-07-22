#include <fstream>

#include <QString>
#include <QDebug>
#include <QMap>

#include "stylesheet.h"
#include "utils.h"

Style::~Style(){}
Style::Style(std::string obj, std::string path){ this->obj = obj; this->load_user_preferences(path); }

void Style::set_style(QString key, QString value){ styles[key] = value; }
QString Style::get_style(QString key){ return styles[key]; }
QString Style::get_objname(){ return QString::fromStdString(this->obj); }

QString Style::stylesheet(QString obj){
    QString ss;

    QMap<QString, QString>::const_iterator it = styles.begin();
    while (it != styles.end()) {
        ss += it.key() + ":" + it.value() + ";\n";
        ++it;
    }

    return obj.length() ? "#" + obj + "{" + ss + "}" : ss;
}

QMap<QString, QString> getDefaultStyle(std::string obj){
    if(!obj.compare("Frame"))
        return frameDefaultStyle();
    else if(!obj.compare("Sbox"))
        return sboxDefaultStyle();

    return QMap<QString, QString>();
}

void Style::load_user_preferences(std::string path){
    std::ifstream file(path);

    if(!file.good())
        this->styles = getDefaultStyle(this->obj);
    else {
        std::string pref;
        while(std::getline(file, pref))
            if(!pref.compare(this->obj)) break;

        while(std::getline(file, pref)){
            if(!pref.length())
                continue;
            if(pref.find_first_not_of(" \t\v\f\r") == std::string::npos)
                continue;

            QStringList spref = QString::fromStdString(pref).split(QRegExp("[:]"), QString::SkipEmptyParts);
            if(spref.count() <= 1)
                break;

            this->set_style((*spref.begin()).remove(QChar(':')), (*++spref.begin()).remove(QChar(';')));
        }
    }

    file.close();
}
