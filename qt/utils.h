#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QMap>
#include <QColor>
#include <QPixmap>
#include <QFileInfo>
#include <QIcon>
#include <QProxyStyle>
#include <QImageReader>

typedef struct {
    QPixmap img;
    QString path;
    int posx;
    int posy;
} ImageCache;

QString rand_color(int );
QString genColor(QColor, bool , int );
QString extension(QString );

QMap<QString, QString > frameDefaultStyle();
QMap<QString, QString > sboxDefaultStyle();
QMap<QString, QString > iconDefaultStyle();

void initFormatThemes();
bool isImage(QString );

QIcon fileIcon(QString );

#endif // UTILS_H
