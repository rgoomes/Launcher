#include "utils.h"

QMap<QString, QString> formatThemes;

// GENERATE RANDOM COLOR
QString rand_color(int alpha){
    return QString("rgba(")
         + QString::number(rand() % 255) + ","
         + QString::number(rand() % 255) + ","
         + QString::number(rand() % 255) + ","
         + QString::number(alpha)
         + QString(")");
}

QString genColor(QColor color, bool random, int alpha){
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

    default_styles["border-radius"] = "20";
    default_styles["background-color"] = "rgba(0,125,255,150)";

    return default_styles;
}

QMap <QString, QString > sboxDefaultStyle(){
    QMap <QString, QString > default_styles;

    default_styles["color"] = "#ffffff";
    default_styles["border-style"] = "solid";
    default_styles["border-width"] = "1";
    default_styles["border-radius"] = "7";
    default_styles["border-color"] = "#ffffff";
    default_styles["background"] = "transparent";
    default_styles["padding-left"] = "30px";
    default_styles["padding-right"] = "0px";
    default_styles["selection-background-color"] = "rgba(255,255,255,100)";

    return default_styles;
}

QMap <QString, QString > iconDefaultStyle(){
    QMap <QString, QString > default_styles;

    default_styles["border"] = "none";
    default_styles["padding"] = "0px";

    return default_styles;
}

void initFormatThemes(){
    formatThemes["pdf"]  = "application-pdf";
    formatThemes["doc"]  = "application-msword";
    formatThemes["docx"] = "application-msword";
    formatThemes["odt"]  = "x-office-document";
    formatThemes["odp"]  = "x-office-presentation";
    formatThemes["zip"]  = "package-x-generic";
    formatThemes["rar"]  = "package-x-generic";
    formatThemes["bz2"]  = "package-x-generic";
    formatThemes["gz"]   = "package-x-generic";
    formatThemes["mp4"]  = "video-x-generic";
    formatThemes["wmv"]  = "video-x-generic";
    formatThemes["html"] = "text-html";
    formatThemes["mp3"]  = "audio-x-generic";
    formatThemes["ttf"]  = "font-x-generic";
    formatThemes["otf"]  = "font-x-generic";
}

QIcon fileIcon(QString filePath){
    QString ext = extension(filePath);

    if(isImage(ext))
        return QIcon(filePath);
    else if(formatThemes.find(ext) != formatThemes.end())
        return QIcon::fromTheme(formatThemes[ext]);
    else{
        QProxyStyle ps;
        return ps.standardIcon(QStyle::SP_FileIcon);
    }
}

bool isImage(QString extension){
    for(QString format : QImageReader::supportedImageFormats())
        if(!format.compare(extension))
            return true;

    return false;
}

QString extension(QString filePath){
    QFileInfo fileInfo(filePath);
    return fileInfo.suffix().toLower();
}
