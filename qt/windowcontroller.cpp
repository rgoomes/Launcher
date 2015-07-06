#include <fstream>

#include <QString>
#include <QDebug>
#include <QMap>

#include "windowcontroller.h"

WindowController::WindowController(std::string path){ this->path = path; this->load_user_window_options(); }
WindowController::~WindowController(){}

QMap <QString, QString > get_default_options(){
    QMap <QString, QString > default_options;

    default_options["x"] = "100";
    default_options["y"] = "100";
    default_options["shadow_alpha"] = "255";
    default_options["shadow_blur_radius"] = "15";
    default_options["height"] = "120";
    default_options["width"] = "600";
    default_options["fullscreen"] = "0";
    default_options["dpi"] = "1.0";
    default_options["search_height"] = "50";
    default_options["font"] = "Liberation Mono";
    default_options["font_size"] = "20";
    default_options["font_color"] = "#000000";

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

    QMap<QString, QString>::const_iterator it = options.begin();
    while (it != options.end()) {
        window_options += it.key() + ":" + it.value() + ";\n";
        ++it;
    }

    return window_options;
}

void WindowController::load_user_window_options(){
    std::ifstream file(this->path);

    if(!file.good()){
        this->options = get_default_options();
        this->update_file();
    } else {
        std::string opt;
        while(std::getline(file, opt)){
            if(!opt.length())
                continue;
            if(opt.find_first_not_of(" \t\v\f\r") == std::string::npos)
                continue;

            QStringList sopt = QString::fromStdString(opt).split(QRegExp("[:]"), QString::SkipEmptyParts);
            if(sopt.count() <= 1)
                continue;

            this->set_option((*sopt.begin()).remove(QChar(':')), (*++sopt.begin()).remove(QChar(';')));
        }
    }

    file.close();
}

