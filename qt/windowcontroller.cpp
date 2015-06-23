#include <fstream>

#include <QString>
#include <QDebug>
#include <QMap>

#include "windowcontroller.h"

WindowController::WindowController(){ this->load_user_window_options(); }
WindowController::~WindowController(){}

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

    if(file.good()){
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

