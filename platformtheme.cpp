#include "platformtheme.h"

PlatformTheme::PlatformTheme()
{
    settings = new QSettings("theSuite", "ts-qtplatform");
}

PlatformTheme::~PlatformTheme() {
    settings->deleteLater();
}

QVariant PlatformTheme::themeHint(ThemeHint hint) const {
    switch (hint) {
    case QPlatformTheme::StyleNames:
        return "contemporary";
    case QPlatformTheme::SystemIconThemeName:
        return "breeze-dark";
    default:
        return QPlatformTheme::themeHint(hint);
    }
}

const QPalette* PlatformTheme::palette(Palette type) const {
    QPalette* pal = new QPalette;

    QColor buttonCol;
    QColor buttonText;
    QColor highlightCol;
    QColor highlightText;
    if (settings->value("color/type", "dark") == "dark") { //Use dark colors
        //Get Button Color
        switch (settings->value("color/accent", 0).toInt()) {
        case 0: //Blue
            buttonCol = QColor(0, 50, 150);
            buttonText = greyscale(255);
            highlightCol = QColor(0, 80, 170);
            highlightText = greyscale(255);
            break;
        case 1: //Green
            buttonCol = QColor(0, 85, 0);
            buttonText = greyscale(255);
            highlightCol = QColor(0, 100, 0);
            highlightText = greyscale(255);
            break;
        case 2: //Orange
            buttonCol = QColor(200, 50, 0);
            buttonText = greyscale(255);
            highlightCol = QColor(200, 100, 0);
            highlightText = greyscale(255);
            break;
        case 3: //Pink
            buttonCol = QColor(150, 0, 150);
            buttonText = greyscale(255);
            highlightCol = QColor(200, 0, 200);
            highlightText = greyscale(255);
            break;
        case 4: //Turquoise
            buttonCol = QColor(0, 150, 100);
            buttonText = greyscale(255);
            highlightCol = QColor(0, 200, 150);
            highlightText = greyscale(255);
            break;
        }

        //Set Normal colors
        pal->setColor(QPalette::Window, greyscale(40));
        pal->setColor(QPalette::WindowText, greyscale(255));
        pal->setColor(QPalette::Base, greyscale(40));
        pal->setColor(QPalette::Text, greyscale(255));

        //Set Disabled colors
        pal->setColor(QPalette::Disabled, QPalette::WindowText, greyscale(150));
        pal->setColor(QPalette::Disabled, QPalette::Button, QColor(0, 30, 100));
        pal->setColor(QPalette::Disabled, QPalette::ButtonText, greyscale(150));
    } else { //Use light colors
        buttonCol = QColor(0, 200, 255);
        buttonText = greyscale(0);
        //Set Normal colors
        pal->setColor(QPalette::Window, greyscale(235));
        pal->setColor(QPalette::WindowText, greyscale(0));
        pal->setColor(QPalette::Base, greyscale(235));
        pal->setColor(QPalette::Text, greyscale(0));
        pal->setColor(QPalette::Highlight, QColor(0, 175, 200));
        pal->setColor(QPalette::HighlightedText, greyscale(0));

        //Set Disabled colors
        pal->setColor(QPalette::Disabled, QPalette::WindowText, greyscale(100));
        pal->setColor(QPalette::Disabled, QPalette::Button, QColor(0, 150, 200));
        pal->setColor(QPalette::Disabled, QPalette::ButtonText, greyscale(100));
    }

    //Set accent colours
    pal->setColor(QPalette::Button, buttonCol);
    pal->setColor(QPalette::ButtonText, buttonText);
    pal->setColor(QPalette::Highlight, highlightCol);
    pal->setColor(QPalette::HighlightedText, highlightText);
    return pal;
}

const QFont* PlatformTheme::font(Font type) const {
    QFont* font;
    switch (type) {
    case FixedFont:
        font = new QFont(settings->value("fonts/monospaceFamily", "Hack").toString(), settings->value("fonts/monospaceSize", 9).toInt());
        break;
    case SmallFont:
    case MiniFont:
        font = new QFont(settings->value("fonts/smallFamily", "Rubik").toString(), settings->value("fonts/smallSize", 8).toInt());
        break;
    default:
        font = new QFont(settings->value("fonts/defaultFamily", "Rubik").toString(), settings->value("fonts/defaultSize", 10).toInt());
        break;
    }

    return font;
}

QIconEngine* PlatformTheme::createIconEngine(const QString &iconName) const {
    return new KIconEngine(iconName, KIconLoader::global());
}

QColor PlatformTheme::greyscale(int intensity) const {
    return QColor(intensity, intensity, intensity);
}

bool PlatformTheme::usePlatformNativeDialog(DialogType type) const {
    switch (type) {
    case QPlatformTheme::MessageDialog:
        return true;
        break;
    case QPlatformTheme::FontDialog:
    case QPlatformTheme::ColorDialog:
    case QPlatformTheme::FileDialog:
    default:
        return false;
    }
}

QPlatformDialogHelper* PlatformTheme::createPlatformDialogHelper(DialogType type) const {
    switch (type) {
    case QPlatformTheme::MessageDialog:
        return new MessageDialogHelper;
    case QPlatformTheme::FontDialog:
    case QPlatformTheme::ColorDialog:
    case QPlatformTheme::FileDialog:
    default:
        return NULL;
    }
}
