
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
    default_styles["background-color"] = "rgba(0,125,255,0.4)";

    return default_styles;
}

QMap <QString, QString > sboxDefaultStyle(){
    QMap <QString, QString > default_styles;

    default_styles["color"] = "#ffffff";
    default_styles["border"] = "1px solid";
    default_styles["border-radius"] = "7px";
    default_styles["border-color"] = "#ffffff";
    default_styles["background"] = "transparent";
    default_styles["padding-left"] = "30px";
    default_styles["padding-right"] = "0px";

    return default_styles;
}

QMap <QString, QString > iconDefaultStyle(){
    QMap <QString, QString > default_styles;

    default_styles["border"] = "none";
    default_styles["padding"] = "0px";

    return default_styles;
}
