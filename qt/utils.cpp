
#include <QString>

// GENERATE RANDOM COLOR
QString rand_color(double alpha){
    return QString("rgba(")
         + QString::number(rand() % 255) + ","
         + QString::number(rand() % 255) + ","
         + QString::number(rand() % 255) + ","
         + QString::number(alpha)
         + QString(")");
}

