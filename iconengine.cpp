#include "iconengine.h"

IconEngine::IconEngine(QString iconName) : QIconEngine()
{
    icName = iconName;

    if (iconName != "") {
        /*pendingIcons = QtConcurrent::run([=](QString icName) -> QList<IconEngine::iconInfo> {
            return load(icName);
        }, iconName);*/

        bool loadIcons = true;

        for (int i = 0; i < memorySizes.count(); i++) {
            cacheIcon icon = memorySizes.at(i);
            if (icon.name == icName) {
                loadIcons = false;
                i = memorySizes.count();
            }
        }

        if (loadIcons) {
            listOfIcons = load(icName);

            for (iconInfo icons : listOfIcons) {
                QImage px(icons.fileName);
                tintImage(px, QApplication::palette("QLabel").color(QPalette::WindowText));

                cacheIcon cache;
                cache.name = icName;
                cache.size = icons.size;
                cache.pm = QPixmap::fromImage(px);
                memorySizes.append(cache);
            }
        }
    }
}

QList<IconEngine::iconInfo> IconEngine::load(QString icName) {
    QList<IconEngine::iconInfo> retval;
    QString theme = QIcon::themeName();
    //Theme search paths: ~/.local/usr/share/icons /usr/share/icons

    //listOfIcons = getMatchingIcon(QDir::homePath() + "/.local/usr/share/icons/" + theme);
    //if (listOfIcons.count() == 0) {
        retval = getMatchingIcon("/usr/share/icons/" + theme, icName);
        if (retval.count() == 0) {
            retval = getMatchingIcon("/usr/share/icons/", icName, false);
            if (retval.count() == 0) {
                //retval = getMatchingIcon("/usr/share/icons/breeze", icName);
                if (retval.count() == 0) {
                    retval = getMatchingIcon("/usr/share/icons/hicolor/", icName);
                }
            }
        }
    //}

    return retval;
}

QString IconEngine::iconName() const {
    if (listOfIcons.count() == 0) {
        return "";
    } else {
        return icName;
    }
}

void IconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) {
    for (cacheIcon icon : memorySizes) {
        if ((icon.name == icName) && (icon.size == rect.width())) {
            painter->drawPixmap(rect, icon.pm);
            return;
        }
    }

    /*if (pendingIcons.isRunning()) {
        pendingIcons.waitForFinished();
    }
    listOfIcons = pendingIcons.result();*/

    if (listOfIcons.count() > 0) {
        QString fileName;
        for (iconInfo info : listOfIcons) {
            if (info.size == rect.width()) {
                fileName = info.fileName;
            }
        }

        if (fileName == "") {
            int currentSize;
            for (iconInfo info : listOfIcons) {
                if ((info.size > rect.width()) && (info.size < currentSize)) {
                    fileName = info.fileName;
                    currentSize = info.size;
                }
            }

            if (fileName == "") {
                fileName = listOfIcons.first().fileName;
            }
        }

        QString suffix = fileName;
        suffix = suffix.mid(suffix.lastIndexOf(".") + 1);
        QImage px;
        if (suffix == "svg") {
            px = QImage(rect.size(), QImage::Format_ARGB32);
            px.fill(Qt::transparent);
            QSvgRenderer renderer(fileName);
            QPainter painter(&px);
            renderer.render(&painter);
        } else {
            px = QImage(fileName);
        }
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        tintImage(px, QApplication::palette("QLabel").color(QPalette::WindowText));
        painter->drawImage(rect, px);

        cacheIcon cache;
        cache.name = icName;
        cache.size = rect.width();
        cache.pm = QPixmap::fromImage(px);
        memorySizes.append(cache);
    }
}

QList<IconEngine::iconInfo> IconEngine::getMatchingIcon(QString searchPath, QString icName, bool subdirectories) {
    QList<iconInfo> retval;
    QDir path = searchPath;
    if (path.exists()) {
        QDirIterator* iterator;
        if (subdirectories) {
            iterator = new QDirIterator(path, QDirIterator::Subdirectories);
        } else {
            iterator = new QDirIterator(path);
        }

        while (iterator->hasNext()) {
            iterator->next();

            //QFileInfo info = iterator->fileInfo();
            QString baseName = iterator->filePath();
            baseName = baseName.mid(baseName.lastIndexOf("/") + 1);
            baseName = baseName.left(baseName.indexOf("."));
            if (baseName.toLower() == icName.toLower()) {
                QString suffix = iterator->filePath();
                suffix = suffix.mid(suffix.lastIndexOf(".") + 1);
                /*if (suffix == "svg") {
                    for (int i = 0; i < 4; i++) {
                        iconInfo icInfo;
                        icInfo.fileName = iterator->filePath();
                        icInfo.size = 16 + i * 8;
                        retval.append(icInfo);
                    }
                } else {*/
                    QImage ic(iterator->filePath());

                    iconInfo icInfo;
                    icInfo.fileName = iterator->filePath();
                    icInfo.size = ic.size().width();

                    retval.append(icInfo);
                //}
            }
        }

        delete iterator;
    }

    return retval;
}

QIconEngine* IconEngine::clone() const {
    return new IconEngine(icName);
}

QPixmap IconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) {
    QPixmap px(size);
    px.fill(Qt::transparent);

    QPainter painter(&px);
    this->paint(&painter, QRect(0, 0, size.width(), size.height()), mode, state);
    painter.end();
    return px;
    //return QIconEngine::pixmap(size, mode, state);
}

void IconEngine::tintImage(QImage &image, QColor tint) const {
    bool doPaint = true;
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QColor pixelCol = image.pixelColor(x, y);
            if ((pixelCol.blue() > pixelCol.green() - 10 && pixelCol.blue() < pixelCol.green() + 10) &&
                    (pixelCol.green() > pixelCol.red() - 10 && pixelCol.green() < pixelCol.red() + 10)) {
            } else {
                doPaint = false;

                //Break out of loop
                x = image.width();
                y = image.height();
            }
        }
    }

    if (doPaint) {
        QPainter painter(&image);
        painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        painter.fillRect(0, 0, image.width(), image.height(), tint);
        painter.end();
    }
}

QList<QSize> IconEngine::availableSizes(QIcon::Mode mode, QIcon::State state) const {
    QList<QSize> retval;
    for (cacheIcon icon : memorySizes) {
        if (icon.name == icName) {
            retval.append(QSize(icon.size, icon.size));
        }
    }
    return retval;
}

bool IconEngine::isNull() {
    if (listOfIcons.count() == 0) {
        return true;
    } else {
        return false;
    }
}
