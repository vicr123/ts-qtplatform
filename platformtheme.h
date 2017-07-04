#ifndef PLATFORMTHEME_H
#define PLATFORMTHEME_H

#include "ts-qtplatform_global.h"
#include "messagedialoghelper.h"
#include "iconengine.h"
#include "platformmenu.h"
#include "filedialoghelper.h"
#include <qpa/qplatformtheme.h>
#include <QVariant>
#include <QSettings>
#include <QPalette>
#include <QFont>
#include <QDebug>
#include <QFontDatabase>
#include <QX11Info>
#include <X11/Xcursor/Xcursor.h>

class TSQTPLATFORMSHARED_EXPORT PlatformTheme : public QPlatformTheme
{

public:
    PlatformTheme();
    ~PlatformTheme();

    QVariant themeHint(ThemeHint hint) const override;
    const QPalette* palette(Palette type) const override;
    const QFont* font(Font type) const override;
    QIconEngine* createIconEngine(const QString &iconName) const override;
    QPlatformMenu* createPlatformMenu() const override;
    QPlatformMenuBar* createPlatformMenuBar() const override;
    QPlatformMenuItem* createPlatformMenuItem() const override;
    QPlatformDialogHelper* createPlatformDialogHelper(DialogType type) const override;
    bool usePlatformNativeDialog(DialogType type) const override;
private:
    QColor greyscale(int intensity) const;
    QSettings* settings;
    QMap<QString, IconEngine*> iconEngines;

    QFont* defaultFont;
};

#endif // PLATFORMTHEME_H
