#include <fstream>

#include <QString>
#include <QDebug>
#include <QMap>

#include "windowcontroller.h"

WindowController::WindowController(){ this->load_user_window_options(); }
WindowController::~WindowController(){}

QMap <QString, QString > get_default_options(){
    QMap <QString, QString > default_options;

    default_options["x"] = "100";
    default_options["y"] = "100";
    default_options["shadow_alpha"] = "255";
    default_options["shadow_blur_radius"] = "15";
    default_options["height"] = "90";
    default_options["width"] = "600";
    default_options["fullscreen"] = "0";

    return default_options;
}

void WindowController::update_file(){
    QString window_options = this->get_all_options();
    std::ofstream file("../User/window.user");

    file << window_options.toUtf8().constData();
    file.close();
}

void WindowController::set_option(QString key, QString option, bool to_update){
    options[key] = option;

    if(to_update)
        this->update_file();
}

QString WindowController::get_option(QString key){ return options[key]; }

QString WindowController::get_all_options(){
    QString window_options;

    QMap<QString, QString>::const_iterator it = options.begin();
    while (it != options.end()) {
        window_options += it.key() + ":" + it.value() + ";\n";
        ++it;
    }

    return window_options;
}

void WindowController::load_user_window_options(){
    std::ifstream file("../User/window.user");

    if(!file.good()){
        this->options = get_default_options();
        this->update_file();
        qDebug() << "erro";
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

            this->set_option((*sopt.begin()).remove(QChar(':')), (*++sopt.begin()).remove(QChar(';')), false);
        }
    }

    file.close();
}

