
#include <QString>
#include <QMap>
#include <QColor>

// GENERATE RANDOM COLOR
QString rand_color(double alpha){
    return QString("rgba(")
         + QString::number(rand() % 255) + ","
         + QString::number(rand() % 255) + ","
         + QString::number(rand() % 255) + ","
         + QString::number(alpha)
         + QString(")");
}

QString genColor(QColor color, bool random, double alpha){
    if(random)
        return rand_color(alpha);
    else {
        return "rgba("
          + QString::number(color.red())   + ","
          + QString::number(color.green()) + ","
          + QString::number(color.blue())  + ","
          + QString::number(color.alpha()) + ")";
    }
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
    default_styles["border-style"] = "solid";
    default_styles["border-width"] = "1px";
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
