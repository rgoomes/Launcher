#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QMap>
#include <QColor>

QString rand_color(double );
QString genColor(QColor, bool , double );

QMap<QString, QString > frameDefaultStyle();
QMap<QString, QString > sboxDefaultStyle();
QMap<QString, QString > iconDefaultStyle();

#endif // UTILS_H
