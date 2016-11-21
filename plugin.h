#ifndef PLUGIN_H
#define PLUGIN_H

#include <qpa/qplatformthemeplugin.h>
#include "platformtheme.h"

class Plugin : public QPlatformThemePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QPA.QPlatformThemeFactoryInterface.5.1" FILE "plugin.json")
public:
    explicit Plugin(QObject *parent = 0);

    QPlatformTheme* create(const QString &key, const QStringList &paramList);
signals:

public slots:
};

#endif // PLUGIN_H
