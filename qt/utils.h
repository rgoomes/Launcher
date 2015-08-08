#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QMap>

QString rand_color(double alpha);

QMap<QString, QString > frameDefaultStyle();
QMap<QString, QString > sboxDefaultStyle();
QMap<QString, QString > iconDefaultStyle();

#endif // UTILS_H
