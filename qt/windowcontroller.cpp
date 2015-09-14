#include "windowcontroller.h"

WindowController::WindowController(std::string path){ this->path = path; this->load_options(); }
WindowController::~WindowController(){}

QMap <QString, QString > getDefaultOptions(){
    QMap <QString, QString > default_options;

    default_options["x"] = "100";
    default_options["y"] = "100";
    default_options["shadow-scale"] = "3";
    default_options["shadow-alpha"] = "255";
    default_options["shadow-blur-radius"] = "15";
    default_options["height"] = "155";
    default_options["width"] = "450";
    default_options["fullscreen"] = "0";
    default_options["dpi"] = "1.0";
    default_options["search-height"] = "45";
    default_options["font"] = "Liberation Mono";
    default_options["font-size"] = "20";
    default_options["search-icon-pos"] = "1";
    default_options["icon-theme"] = "light";
    default_options["resize-margin"] = "0";
    default_options["hide-on-app"] = "1";
    default_options["search-type"] = "standard";
    default_options["search-time"] = "0";
    default_options["max-results"] = "10";
    default_options["icon-hidden"] = "0";
    default_options["global-shortcut"] = "Ctrl+Shift+R";

    return default_options;
}

void WindowController::update_file(){
    QString window_options = this->windowoptions();
    std::ofstream file(this->path);

    file << window_options.toUtf8().constData();
    file.close();
}

void WindowController::set_option(QString key, QString option){ options[key] = option; }
QString WindowController::get_option(QString key){ return options[key]; }

QString WindowController::windowoptions(){
    QString window_options;

    for(auto key : options.keys())
        window_options += key + ":" + options[key] + ";\n";

    return window_options;
}

void WindowController::load_options(){
    QMap <QString, QString> defs = getDefaultOptions();
    std::ifstream file(this->path);

    if(!file.good())
        options = defs;
    else {
        std::string opt;
        while(std::getline(file, opt)){
            if(!opt.length() || opt.find_first_not_of(" \t\v\f\r") == std::string::npos)
                continue;

            QStringList sopt = QString::fromStdString(opt).split(QRegExp("[:]"), QString::SkipEmptyParts);
            if(sopt.count() <= 1)
                continue;

            this->set_option((*sopt.begin()).remove(QChar(':')), (*++sopt.begin()).remove(QChar(';')));
        }

        for(auto key : defs.keys())
            if(options.find(key) == options.end())
                this->set_option(key, defs[key]);
    }

    file.close();
    update_file();
}

