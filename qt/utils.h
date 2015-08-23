#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QMap>
#include <QColor>

QString rand_color(int );
QString genColor(QColor, bool , int );

QMap<QString, QString > frameDefaultStyle();
QMap<QString, QString > sboxDefaultStyle();
QMap<QString, QString > iconDefaultStyle();

#endif // UTILS_H
