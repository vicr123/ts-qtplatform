#include "plugin.h"

Plugin::Plugin(QObject *parent) : QPlatformThemePlugin(parent)
{

}

QPlatformTheme* Plugin::create(const QString &key, const QStringList &paramList) {
    if (key.toLower() == "ts") {
        return new PlatformTheme();
    }
    return NULL;
}
