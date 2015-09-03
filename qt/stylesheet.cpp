#include "stylesheet.h"
#include "utils.h"

Style::~Style(){}
Style::Style(std::string obj, std::string path){ this->obj = obj; this->load_styles(path); }

void Style::set_style(QString key, QString value){ styles[key] = value; }
QString Style::get_style(QString key){ return styles[key]; }
QString Style::get_objname(){ return QString::fromStdString(this->obj); }

QString Style::stylesheet(QString obj){
    QString ss;

    for(auto key : styles.keys())
        ss += key + ":" + styles[key] + ";\n";

    return obj.length() ? "#" + obj + "{" + ss + "}" : ss;
}

QMap<QString, QString> getDefaultStyle(std::string obj){
    if(!obj.compare("Frame"))
        return frameDefaultStyle();
    else if(!obj.compare("Sbox"))
        return sboxDefaultStyle();
    else if(!obj.compare("Icon"))
        return iconDefaultStyle();

    return QMap<QString, QString>();
}

void Style::load_styles(std::string path){
    QMap<QString, QString> defs = getDefaultStyle(this->obj);
    std::ifstream file(path);

    if(!file.good())
        styles = defs;
    else {
        std::string pref;
        while(std::getline(file, pref))
            if(!pref.compare(this->obj)) break;

        while(std::getline(file, pref)){
            if(!pref.length() || pref.find_first_not_of(" \t\v\f\r") == std::string::npos)
                continue;

            QStringList spref = QString::fromStdString(pref).split(QRegExp("[:]"), QString::SkipEmptyParts);
            if(spref.count() <= 1)
                break;

            this->set_style((*spref.begin()).remove(QChar(':')), (*++spref.begin()).remove(QChar(';')));
        }

        for(auto key : defs.keys())
            if(styles.find(key) == styles.end())
                this->set_style(key, defs[key]);
    }

    file.close();
}
