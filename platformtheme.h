#ifndef PLATFORMTHEME_H
#define PLATFORMTHEME_H

#include "ts-qtplatform_global.h"
#include "messagedialoghelper.h"
#include <qpa/qplatformtheme.h>
#include <qpa/qplatformmenu.h>
#include <QVariant>
#include <QSettings>
#include <QPalette>
#include <QFont>
#include <QDebug>
#include <QFontDatabase>
#include <kiconengine.h>
#include <kiconloader.h>

class TSQTPLATFORMSHARED_EXPORT PlatformTheme : public QPlatformTheme
{

public:
    PlatformTheme();
    ~PlatformTheme();

    QVariant themeHint(ThemeHint hint) const override;
    const QPalette* palette(Palette type) const override;
    const QFont* font(Font type) const override;
    QIconEngine* createIconEngine(const QString &iconName) const override;

    QPlatformDialogHelper* createPlatformDialogHelper(DialogType type) const override;
    bool usePlatformNativeDialog(DialogType type) const override;
private:
    QColor greyscale(int intensity) const;
    QSettings* settings;

    QFont* defaultFont;
};

#endif // PLATFORMTHEME_H
