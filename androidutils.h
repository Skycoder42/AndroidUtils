#ifndef ANDROIDUTILS_H
#define ANDROIDUTILS_H

#include <QString>
#include <QColor>

namespace AndroidUtils
{
void setStatusBarColor(const QColor &color);
void showToast(const QString &message, bool showLong = false);
void hapticFeedback();
}

#endif // ANDROIDUTILS_H
