#include "plugin.h"

#include <QDesktopWidget>

Plugin::Plugin(QObject *parent) : QPlatformThemePlugin(parent)
{

}

QPlatformTheme* Plugin::create(const QString &key, const QStringList &paramList) {
    if (key.toLower() == "ts") {
        return new PlatformTheme();
    }
    return NULL;
}

float getDPIScaling() {
    float currentDPI = QApplication::desktop()->logicalDpiX();
    return currentDPI / (float) 96;
}
