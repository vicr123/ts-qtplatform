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
    QColor buttonDisabledCol;
    QColor buttonDisabledText;
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
            buttonDisabledCol = QColor(0, 30, 100);
            buttonDisabledText = QColor(greyscale(150));
            break;
        case 1: //Green
            buttonCol = QColor(0, 85, 0);
            buttonText = greyscale(255);
            highlightCol = QColor(0, 100, 0);
            highlightText = greyscale(255);
            buttonDisabledCol = QColor(0, 50, 0);
            buttonDisabledText = QColor(greyscale(150));
            break;
        case 2: //Orange
            buttonCol = QColor(200, 50, 0);
            buttonText = greyscale(255);
            highlightCol = QColor(200, 100, 0);
            highlightText = greyscale(255);
            buttonDisabledCol = QColor(100, 25, 0);
            buttonDisabledText = QColor(greyscale(150));
            break;
        case 3: //Pink
            buttonCol = QColor(150, 0, 150);
            buttonText = greyscale(255);
            highlightCol = QColor(200, 0, 200);
            highlightText = greyscale(255);
            buttonDisabledCol = QColor(100, 0, 100);
            buttonDisabledText = QColor(greyscale(150));
            break;
        case 4: //Turquoise
            buttonCol = QColor(0, 150, 100);
            buttonText = greyscale(255);
            highlightCol = QColor(0, 200, 150);
            highlightText = greyscale(255);
            buttonDisabledCol = QColor(0, 100, 50);
            buttonDisabledText = QColor(greyscale(150));
            break;
        }

        //Set Normal colors
        pal->setColor(QPalette::Window, greyscale(40));
        pal->setColor(QPalette::WindowText, greyscale(255));
        pal->setColor(QPalette::Base, greyscale(40));
        pal->setColor(QPalette::Text, greyscale(255));
        pal->setColor(QPalette::ToolTipText, greyscale(255));

        pal->setColor(QPalette::Disabled, QPalette::WindowText, greyscale(150));
    } else { //Use light colors

        //Get Button Color
        switch (settings->value("color/accent", 0).toInt()) {
        case 0: //Blue
            buttonCol = QColor(0, 200, 255);
            buttonText = greyscale(0);
            highlightCol = QColor(0, 175, 200);
            highlightText = greyscale(0);
            buttonDisabledCol = QColor(0, 150, 200);
            buttonDisabledText = greyscale(100);
            break;
        case 1: //Green
            buttonCol = QColor(0, 255, 128);
            buttonText = greyscale(0);
            highlightCol = QColor(0, 255, 0);
            highlightText = greyscale(0);
            buttonDisabledCol = QColor(0, 150, 50);
            buttonDisabledText = greyscale(150);
            break;
        case 2: //Orange
            buttonCol = QColor(255, 100, 0);
            buttonText = greyscale(0);
            highlightCol = QColor(255, 150, 50);
            highlightText = greyscale(0);
            buttonDisabledCol = QColor(150, 50, 0);
            buttonDisabledText = greyscale(150);
            break;
        case 3: //Pink
            buttonCol = QColor(255, 0, 255);
            buttonText = greyscale(0);
            highlightCol = QColor(255, 0, 255);
            highlightText = greyscale(0);
            buttonDisabledCol = QColor(100, 0, 100);
            buttonDisabledText = greyscale(150);
            break;
        case 4: //Turquoise
            buttonCol = QColor(0, 200, 150);
            buttonText = greyscale(0);
            highlightCol = QColor(0, 200, 150);
            highlightText = greyscale(0);
            buttonDisabledCol = QColor(0, 150, 150);
            buttonDisabledText = greyscale(150);
            break;
        }

        //Set Normal colors
        pal->setColor(QPalette::Window, greyscale(235));
        pal->setColor(QPalette::WindowText, greyscale(0));
        pal->setColor(QPalette::Base, greyscale(235));
        pal->setColor(QPalette::Text, greyscale(0));
        pal->setColor(QPalette::ToolTipText, greyscale(0));

        pal->setColor(QPalette::Disabled, QPalette::WindowText, greyscale(100));
    }

    //Set accent colours
    pal->setColor(QPalette::Button, buttonCol);
    pal->setColor(QPalette::ButtonText, buttonText);
    pal->setColor(QPalette::Highlight, highlightCol);
    pal->setColor(QPalette::HighlightedText, highlightText);
    pal->setColor(QPalette::Disabled, QPalette::Button, buttonDisabledCol);
    pal->setColor(QPalette::Disabled, QPalette::ButtonText, buttonDisabledText);

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
