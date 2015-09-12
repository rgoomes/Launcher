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

QString rand_color(int );
QString genColor(QColor, bool , int );

QMap<QString, QString > frameDefaultStyle();
QMap<QString, QString > sboxDefaultStyle();
QMap<QString, QString > iconDefaultStyle();

void initFormatThemes();
QIcon fileIcon(QString filePath);

#endif // UTILS_H
