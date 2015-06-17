#include <QString>
#include <QDebug>

#include "stylesheet.h"

using namespace std;

Style::Style(){}
Style::~Style(){}

void Style::update_style(QString key, QString value){ styles[key] = value; }

QString Style::get_stylesheet(){
    QString style_sheet;

    QMap<QString, QString>::const_iterator it = styles.begin();
    while (it != styles.constEnd()) {
       style_sheet += it.key() + ":" + it.value() + "\n";
        ++it;
    }

    return style_sheet;
}
