#include "platformtheme.h"

void catch_signal(int signal);
__sighandler_t oldSignalHandler;

#include <QScrollerProperties>
#include <QTranslator>

PlatformTheme::PlatformTheme()
{
    settings = new QSettings("theSuite", "ts-qtplatform");

    //Install translations
    QTranslator localTranslator;
    localTranslator.load(QLocale::system().name(), "/usr/share/theshell/ts-qtplatform/translations");
    QApplication::installTranslator(&localTranslator);

    //oldSignalHandler = signal(SIGTSTP, catch_signal);
    if (QX11Info::isPlatformX11()) {
        //Load cursor library
        QLibrary xc("/usr/lib/libXcursor");

        if (xc.load()) {
            typedef int (*setThemeFunc) (Display*, const char*);
            typedef int (*setSizeFunc) (Display*, int);
            typedef int (*loadCurFunc) (Display*, const char*);
            typedef int (*defCurFunc) (Display*, unsigned long, unsigned long);
            typedef int (*freeCurFunc) (Display*, unsigned long);

            auto XcursorSetTheme = (setThemeFunc) xc.resolve("XcursorSetTheme");
            auto XcursorSetDefaultSize = (setSizeFunc) xc.resolve("XcursorSetDefaultSize");
            auto XcursorLibraryLoadCursor = (loadCurFunc) xc.resolve("XcursorLibraryLoadCursor");
            auto XDefineCursor = (defCurFunc) xc.resolve("XDefineCursor");
            auto XFreeCursor = (freeCurFunc) xc.resolve("XFreeCursor");
            //Set cursors
            XcursorSetTheme(QX11Info::display(), "contemporary_cursors");
            XcursorSetDefaultSize(QX11Info::display(), 24);

            Cursor cursor = XcursorLibraryLoadCursor(QX11Info::display(), "left_ptr");
            XDefineCursor(QX11Info::display(), QX11Info::appRootWindow(), cursor);
            XFreeCursor(QX11Info::display(), cursor);
        }
    }

    themeThread = new ThemeCheckThread();
    QObject::connect(themeThread, SIGNAL(UpdateTheme()), &themeUpdate, SLOT(UpdateTheme()));
    themeThread->start();

    touchFilter = new TouchFilter();
    touchFilter->showCursor();

    QScrollerProperties scrollerProperties;
    scrollerProperties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.1);
    scrollerProperties.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.1);
    QScrollerProperties::setDefaultScrollerProperties(scrollerProperties);
}

ThemeCheckThread::ThemeCheckThread(QObject *parent) : QThread(parent) {

}

ThemeCheckThread::~ThemeCheckThread() {

}

ThemeUpdate::ThemeUpdate(QObject *parent) : QObject(parent) {
    settings = new QSettings("theSuite", "ts-qtplatform");
}

ThemeUpdate::~ThemeUpdate() {
    settings->deleteLater();
}

TouchFilter::TouchFilter(QObject* parent) : QObject(parent) {
    QApplication::instance()->installEventFilter(this);
}

TouchFilter::~TouchFilter() {

}

void TouchFilter::showCursor() {
    if (!cursorHidden) return;

    QApplication::restoreOverrideCursor();

    cursorHidden = false;
}

void TouchFilter::hideCursor() {
    if (cursorHidden) return;

    QCursor c;
    c.setShape(Qt::BlankCursor);
    QApplication::setOverrideCursor(c);

    cursorHidden = true;
}

bool TouchFilter::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::TouchBegin || event->type() == QEvent::TouchUpdate || event->type() == QEvent::TouchEnd) {
        //Hide the cursor
        hideCursor();
    } else if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* e = (QMouseEvent*) event;
        if (e->source() != Qt::MouseEventSynthesizedByQt) {
            //Show the cursor
            showCursor();
        }
    }
    return false;
}

void ThemeUpdate::UpdateTheme() {
    if (QApplication::applicationName() != "QtCreator") {
        QApplication::setStyle(QStyleFactory::create(settings->value("style/name", "contemporary").toString()));
    }
}

void ThemeCheckThread::run() {
    settings = new QSettings("theSuite", "ts-qtplatform");
    pollSettingsTimer = new QTimer();

    currentStyle = settings->value("style/name", "contemporary").toString();
    currentColor = settings->value("color/type", "dark").toString();
    currentAccent = settings->value("color/accent", 0).toInt();

    pollSettingsTimer->setInterval(1000);
    pollSettingsTimer->connect(pollSettingsTimer, &QTimer::timeout, [=] {
        QSettings settings("theSuite", "ts-qtplatform");
        bool reloadStyle = false;
        if (currentStyle != settings.value("style/name", "contemporary").toString()) {
            reloadStyle = true;
        }
        currentStyle = settings.value("style/name", "contemporary").toString();

        if (currentColor != settings.value("color/type", "dark").toString()) {
            reloadStyle = true;
        }
        currentColor = settings.value("color/type", "dark").toString();

        if (currentAccent != settings.value("color/accent", 0).toInt()) {
            reloadStyle = true;
        }
        currentAccent = settings.value("color/accent", 0).toInt();

        if (QApplication::applicationName() == "QtCreator") {
            //Specifically ignore Qt Creator; it tends to crash :(
            reloadStyle = false;
        }

        if (reloadStyle) {
            emit UpdateTheme();
        }
    });
    pollSettingsTimer->start();

    this->exec();

    pollSettingsTimer->stop();
    pollSettingsTimer->deleteLater();
    settings->deleteLater();
}

void catch_signal(int sig) {
    if (sig == SIGTSTP) {
        qDebug() << "SIGTSTP";
        signal(SIGTSTP, SIG_DFL);
        raise(SIGTSTP);
        oldSignalHandler = signal(SIGTSTP, catch_signal);
    }
}

PlatformTheme::~PlatformTheme() {
    touchFilter->deleteLater();
    themeThread->quit();
    themeThread->deleteLater();
    signal(SIGTSTP, oldSignalHandler);
    settings->deleteLater();
}

QVariant PlatformTheme::themeHint(ThemeHint hint) const {
    switch (hint) {
        case QPlatformTheme::StyleNames:
            return settings->value("style/name", "contemporary").toString();
        case QPlatformTheme::SystemIconThemeName:
            return settings->value("icons/theme", "contemporary").toString();
        case QPlatformTheme::SystemIconFallbackThemeName:
            return settings->value("icons/themeFallback", "contemporary").toString();
        case QPlatformTheme::PasswordMaskCharacter:
            return QChar(0x2022 /* Bullet character */);
        case QPlatformTheme::ItemViewActivateItemOnSingleClick:
            return true;
        case QPlatformTheme::UiEffects:
            return QPlatformTheme::AnimateComboUiEffect | QPlatformTheme::AnimateMenuUiEffect | QPlatformTheme::AnimateToolBoxUiEffect | QPlatformTheme::AnimateTooltipUiEffect;
        default:
            return QPlatformTheme::themeHint(hint);
    }
}

const QPalette* PlatformTheme::palette(Palette type) const {
    QPalette* pal = new QPalette;

    if (settings->value("accessibility/highcontrast", false).toBool()) {
        //Set Normal colors
        pal->setColor(QPalette::Window, greyscale(255));
        pal->setColor(QPalette::WindowText, greyscale(0));
        pal->setColor(QPalette::Base, greyscale(255));
        pal->setColor(QPalette::AlternateBase, greyscale(235));
        pal->setColor(QPalette::Text, greyscale(0));
        pal->setColor(QPalette::ToolTipText, greyscale(0));

        pal->setColor(QPalette::Disabled, QPalette::WindowText, greyscale(50));

        //Set accent colours
        pal->setColor(QPalette::Button, greyscale(0));
        pal->setColor(QPalette::ButtonText, greyscale(255));
        pal->setColor(QPalette::Highlight, QColor(0, 0, 100));
        pal->setColor(QPalette::HighlightedText, greyscale(255));
        pal->setColor(QPalette::Disabled, QPalette::Button, greyscale(40));
        pal->setColor(QPalette::Disabled, QPalette::ButtonText, greyscale(235));

    } else {
        QColor buttonCol;
        QColor buttonText;
        QColor buttonDisabledCol;
        QColor buttonDisabledText;
        QColor highlightCol;
        QColor highlightText;

        QString colourType = settings->value("color/type", "dark").toString();
        if (colourType == "dark" || colourType == "black") { //Use dark colors
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
            if (colourType == "dark") {
                pal->setColor(QPalette::Window, greyscale(40));
                pal->setColor(QPalette::Base, greyscale(40));
                pal->setColor(QPalette::AlternateBase, greyscale(60));
            } else { //Black
                pal->setColor(QPalette::Window, greyscale(0));
                pal->setColor(QPalette::Base, greyscale(0));
                pal->setColor(QPalette::AlternateBase, greyscale(40));
            }
            pal->setColor(QPalette::WindowText, greyscale(255));
            pal->setColor(QPalette::Text, greyscale(255));
            pal->setColor(QPalette::ToolTipText, greyscale(255));

            pal->setColor(QPalette::Disabled, QPalette::WindowText, greyscale(150));
        } else if (colourType == "light") { //Use light colors
            QColor windowCol, altWindowCol;
            //Get Button Color
            switch (settings->value("color/accent", 0).toInt()) {
            case 0: //Blue
                buttonCol = QColor(0, 200, 255);
                buttonText = greyscale(0);
                highlightCol = QColor(0, 175, 200);
                highlightText = greyscale(0);
                buttonDisabledCol = QColor(0, 150, 200);
                buttonDisabledText = greyscale(100);
                windowCol = QColor(225, 225, 235);
                altWindowCol = QColor(215, 215, 225);
                break;
            case 1: //Green
                buttonCol = QColor(36, 175, 62);
                buttonText = greyscale(0);
                highlightCol = QColor(45, 220, 77);
                highlightText = greyscale(0);
                buttonDisabledCol = QColor(33, 163, 57);
                buttonDisabledText = greyscale(150);
                windowCol = QColor(225, 235, 225);
                altWindowCol = QColor(215, 225, 215);
                break;
            case 2: //Orange
                buttonCol = QColor(255, 100, 0);
                buttonText = greyscale(0);
                highlightCol = QColor(255, 150, 50);
                highlightText = greyscale(0);
                buttonDisabledCol = QColor(150, 50, 0);
                buttonDisabledText = greyscale(150);
                windowCol = QColor(235, 230, 220);
                altWindowCol = QColor(225, 220, 210);
                break;
            case 3: //Pink
                buttonCol = QColor(255, 0, 255);
                buttonText = greyscale(0);
                highlightCol = QColor(255, 0, 255);
                highlightText = greyscale(0);
                buttonDisabledCol = QColor(100, 0, 100);
                buttonDisabledText = greyscale(150);
                windowCol = QColor(235, 225, 235);
                altWindowCol = QColor(225, 215, 225);
                break;
            case 4: //Turquoise
                buttonCol = QColor(0, 200, 150);
                buttonText = greyscale(0);
                highlightCol = QColor(0, 200, 150);
                highlightText = greyscale(0);
                buttonDisabledCol = QColor(0, 150, 150);
                buttonDisabledText = greyscale(150);
                windowCol = QColor(220, 230, 235);
                altWindowCol = QColor(210, 220, 225);
                break;
            }

            //Set Normal colors
            pal->setColor(QPalette::Window, windowCol);
            pal->setColor(QPalette::WindowText, greyscale(0));
            pal->setColor(QPalette::Base, windowCol);
            pal->setColor(QPalette::AlternateBase, altWindowCol);
            pal->setColor(QPalette::Text, greyscale(0));
            pal->setColor(QPalette::ToolTipText, greyscale(0));

            pal->setColor(QPalette::Disabled, QPalette::WindowText, greyscale(100));
        } else if (colourType == "gray") { //Use gray colors

            //Get Button Color
            switch (settings->value("color/accent", 0).toInt()) {
            case 0: //Blue
                buttonCol = QColor(25, 96, 122);
                buttonText = greyscale(255);
                highlightCol = QColor(25, 96, 122);
                highlightText = greyscale(255);
                buttonDisabledCol = QColor(20, 80, 110);
                buttonDisabledText = QColor(greyscale(150));
                break;
            case 1: //Green
                buttonCol = QColor(36, 122, 26);
                buttonText = greyscale(255);
                highlightCol = QColor(36, 122, 26);
                highlightText = greyscale(255);
                buttonDisabledCol = QColor(20, 68, 14);
                buttonDisabledText = QColor(greyscale(150));
                break;
            case 2: //Orange
                buttonCol = QColor(176, 111, 31);
                buttonText = greyscale(255);
                highlightCol = QColor(176, 111, 31);
                highlightText = greyscale(255);
                buttonDisabledCol = QColor(90, 57, 16);
                buttonDisabledText = QColor(greyscale(150));
                break;
            case 3: //Pink
                buttonCol = QColor(220, 58, 212);
                buttonText = greyscale(255);
                highlightCol = QColor(220, 58, 212);
                highlightText = greyscale(255);
                buttonDisabledCol = QColor(93, 27, 95);
                buttonDisabledText = QColor(greyscale(150));
                break;
            case 4: //Turquoise
                buttonCol = QColor(39, 130, 115);
                buttonText = greyscale(255);
                highlightCol = QColor(39, 130, 115);
                highlightText = greyscale(255);
                buttonDisabledCol = QColor(29, 95, 84);
                buttonDisabledText = QColor(greyscale(150));
                break;
            }

            //Set Normal colors
            pal->setColor(QPalette::Window, greyscale(80));
            pal->setColor(QPalette::WindowText, greyscale(255));
            pal->setColor(QPalette::Base, greyscale(80));
            pal->setColor(QPalette::AlternateBase, greyscale(100));
            pal->setColor(QPalette::Text, greyscale(255));
            pal->setColor(QPalette::ToolTipText, greyscale(255));

            pal->setColor(QPalette::Disabled, QPalette::WindowText, greyscale(150));
        } else if (colourType == "decorative") { //Set decorative colours
            //Get Colours
            switch (settings->value("color/accent", 0).toInt()) {
                case 0: //Oxygen
                    buttonCol = QColor(223, 220, 217);
                    buttonText = QColor(34, 31, 30);
                    highlightCol = QColor(67, 172, 232);
                    highlightText = greyscale(255);
                    buttonDisabledCol = QColor(240, 243, 237);
                    buttonDisabledText = greyscale(50);

                    pal->setColor(QPalette::Window, QColor(214, 210, 208));
                    pal->setColor(QPalette::WindowText, QColor(34, 31, 30));
                    pal->setColor(QPalette::Base, greyscale(255));
                    pal->setColor(QPalette::AlternateBase, QColor(248, 247, 246));
                    pal->setColor(QPalette::Text, QColor(31, 28, 27));
                    pal->setColor(QPalette::ToolTipText, QColor(31, 28, 27));

                    pal->setColor(QPalette::Disabled, QPalette::WindowText, greyscale(50));
            }
        }

        //Set accent colours
        pal->setColor(QPalette::Button, buttonCol);
        pal->setColor(QPalette::ButtonText, buttonText);
        pal->setColor(QPalette::Highlight, highlightCol);
        pal->setColor(QPalette::HighlightedText, highlightText);
        pal->setColor(QPalette::Disabled, QPalette::Button, buttonDisabledCol);
        pal->setColor(QPalette::Disabled, QPalette::ButtonText, buttonDisabledText);
    }

    return pal;
}

const QFont* PlatformTheme::font(Font type) const {
    QFont* font;
    QString defaultFont;
    defaultFont = "Contemporary";

    switch (type) {
        case FixedFont:
            font = new QFont(settings->value("fonts/monospaceFamily", "Hack").toString(), settings->value("fonts/monospaceSize", 9).toInt());
            break;
        case SmallFont:
        case MiniFont:
            font = new QFont(settings->value("fonts/smallFamily", defaultFont).toString(), settings->value("fonts/smallSize", 8).toInt());
            break;
        default:
            font = new QFont(settings->value("fonts/defaultFamily", defaultFont).toString(), settings->value("fonts/defaultSize", 10).toInt());
            break;
    }

    if (settings->value("accessibility/largeText", false).toBool()) {
        font->setPointSizeF(font->pointSizeF() * 1.25);
    }

    return font;
}

QIconEngine* PlatformTheme::createIconEngine(const QString &iconName) const {
    return new IconEngine(iconName);
}

QColor PlatformTheme::greyscale(int intensity) const {
    return QColor(intensity, intensity, intensity);
}

bool PlatformTheme::usePlatformNativeDialog(DialogType type) const {
    switch (type) {
        case QPlatformTheme::MessageDialog:
            return true;
        case QPlatformTheme::FileDialog:
        case QPlatformTheme::FontDialog:
        case QPlatformTheme::ColorDialog:
        default:
            return false;
    }
}

QPlatformDialogHelper* PlatformTheme::createPlatformDialogHelper(DialogType type) const {
    switch (type) {
        case QPlatformTheme::MessageDialog:
            return new MessageDialogHelper;
        case QPlatformTheme::FileDialog:
            //return new FileDialogHelper;
        case QPlatformTheme::FontDialog:
        case QPlatformTheme::ColorDialog:
        default:
            return NULL;
    }
}

QPlatformMenu* PlatformTheme::createPlatformMenu() const {
    //return new PlatformMenu();
    return NULL;
}

QPlatformMenuBar* PlatformTheme::createPlatformMenuBar() const {
    //return new PlatformMenuBar;
    return NULL;
}

QPlatformMenuItem* PlatformTheme::createPlatformMenuItem() const {
    //return new PlatformMenuItem;
    return NULL;
}
