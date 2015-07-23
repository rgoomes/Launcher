
#include <QString>
#include <QMap>

// GENERATE RANDOM COLOR
QString rand_color(double alpha){
    return QString("rgba(")
         + QString::number(rand() % 255) + ","
         + QString::number(rand() % 255) + ","
         + QString::number(rand() % 255) + ","
         + QString::number(alpha)
         + QString(")");
}

QMap <QString, QString > frameDefaultStyle(){
    QMap <QString, QString > default_styles;

    default_styles["border-radius"] = "20px";
    default_styles["background-color"] = "rgba(255,255,255,1)";

    return default_styles;
}

QMap <QString, QString > sboxDefaultStyle(){
    QMap <QString, QString > default_styles;

    default_styles["color"] = "#000000";
    default_styles["border-radius"] = "0px";
    default_styles["background-color"] = "rgba(255,255,255,0)";

    return default_styles;
}

