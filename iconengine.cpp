#include "iconengine.h"

QList<IconEngine::cacheIcon> IconEngine::memorySizes = QList<IconEngine::cacheIcon>();

IconEngine::IconEngine(QString iconName) : QIconEngine()
{
    icName = iconName;

    if (iconName != "") {
        /*pendingIcons = QtConcurrent::run([=](QString icName) -> QList<IconEngine::iconInfo> {
            return load(icName);
        }, iconName);*/

        /*bool loadIcons = true;

        for (int i = 0; i < memorySizes.count(); i++) {
            cacheIcon icon = memorySizes.at(i);
            if (icon.name == icName) {
                loadIcons = false;
                i = memorySizes.count();
            }
        }

        if (loadIcons) {*/
            listOfIcons = load(icName);

            /*for (iconInfo icons : listOfIcons) {
                QImage px(icons.fileName);
                tintImage(px, QApplication::palette("QLabel").color(QPalette::WindowText));

                cacheIcon cache;
                cache.name = icName;
                cache.size = icons.size;
                cache.pm = QPixmap::fromImage(px);
                memorySizes.append(cache);
            }*/
        //}
    }
}

QList<IconEngine::iconInfo> IconEngine::load(QString icName) {
    QList<IconEngine::iconInfo> retval;
    QString theme = QIcon::themeName();
    //Theme search paths: ~/.local/usr/share/icons /usr/share/icons

    auto loader = [=](QString icName) {
        QList<IconEngine::iconInfo> retval;
        if (theme.startsWith("local:")) {
            retval = getMatchingIcon(QDir::homePath() + "/.local/share/icons/" + theme.mid(6), icName);
        } else {
            retval = getMatchingIcon("/usr/share/icons/" + theme, icName);
        }

        if (retval.count() != 0) return retval;

        retval = getMatchingIcon("/usr/share/icons/", icName, false);
        if (retval.count() != 0) return retval;

        retval = getMatchingIcon("/usr/share/icons/hicolor/", icName);
        if (retval.count() != 0) return retval;

        retval = getMatchingIcon("/usr/share/pixmaps/", icName);
        if (retval.count() != 0) return retval;

        retval = getMatchingIcon(QDir::homePath() + "/.local/share/icons/hicolor/", icName);
        return retval;
    };

    //Search for right-to-left icons first
    if (QApplication::layoutDirection() == Qt::RightToLeft) {
        retval = loader(icName + "-rtl");
        if (retval.count() != 0) return retval;
    }

    return loader(icName);
}

QString IconEngine::iconName() const {
    if (listOfIcons.count() == 0) {
        return "";
    } else {
        return icName;
    }
}

QImage IconEngine::extractImage(QSharedMemory *sharedMemory) {
    QBuffer imageData;
    QDataStream input(&imageData);
    QImage image;

    if (!sharedMemory->isAttached()) {
        sharedMemory->attach(QSharedMemory::ReadOnly);
    }

    sharedMemory->lock();
    imageData.setData((char*) sharedMemory->data(), sharedMemory->size());
    imageData.open(QBuffer::ReadOnly);
    input >> image;
    sharedMemory->unlock();

    return image;
}

void IconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) {
    /*for (cacheIcon icon : memorySizes) {
        if ((icon.name == icName) && (icon.size == rect.width())) {
            painter->drawPixmap(rect, icon.pm);
            return;
        }
    }*/

    /*if (pendingIcons.isRunning()) {
        pendingIcons.waitForFinished();
    }
    listOfIcons = pendingIcons.result();*/

    if (listOfIcons.count() > 0) {
        QString fileName;
        int currentIcon = -1;
        QImage px;
        bool imageLoaded = false;

        for (int i = 0; i < listOfIcons.count(); i++) {
            iconInfo info = listOfIcons.at(i);
            if (info.size == rect.width()) {
                if (!info.iconData->isAttached()) {
                    if (!info.iconData->attach()) {
                        fileName = info.fileName;
                    }
                } else if (info.iconData->isAttached() && info.iconData->size() != 0) {
                    px = extractImage(info.iconData);
                    if (!px.isNull()) {
                        imageLoaded = true;
                    } else {
                        fileName = info.fileName;
                    }
                } else {
                    fileName = info.fileName;
                }
                currentIcon = i;
            }
        }

        if (!imageLoaded) {
            if (fileName == "") {
                if (listOfIcons.count() > 0) {
                    int currentSize = listOfIcons.at(0).size;
                    for (int i = 0; i < listOfIcons.count(); i++) {
                        iconInfo info = listOfIcons.at(i);
                        if ((info.size > rect.width()) && (info.size < currentSize)) {
                            fileName = info.fileName;
                            currentSize = info.size;
                            currentIcon = i;
                        }
                    }

                    if (fileName == "") {
                        fileName = listOfIcons.first().fileName;
                        currentIcon = 0;
                    }
                }
            }

            QString suffix = fileName;
            suffix = suffix.mid(suffix.lastIndexOf(".") + 1);
            if (suffix == "svg") {
                px = QImage(rect.size(), QImage::Format_ARGB32);
                px.fill(Qt::transparent);
                QSvgRenderer renderer(fileName);
                QPainter painter(&px);
                renderer.render(&painter);
            } else {
                px = QImage(fileName);
            }
            tintImage(px, QApplication::palette("QLabel").color(QPalette::WindowText));

            QBuffer imageData;
            imageData.open(QBuffer::ReadWrite);
            QDataStream output(&imageData);
            output << px;

            iconInfo icInfo = listOfIcons.at(currentIcon);
            //icInfo.iconData = new QSharedMemory("ts-qtpl.icon." + icName + "." + QString::number(rect.size().width()));
            icInfo.iconData->create(imageData.size());

            icInfo.iconData->lock();
            memcpy((char*) icInfo.iconData->data(), imageData.data().data(), qMin(icInfo.iconData->size(), (int) imageData.size()));
            icInfo.iconData->unlock();
            listOfIcons.replace(currentIcon, icInfo);
        }
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        painter->drawImage(rect, px);
    }
}

QList<IconEngine::iconInfo> IconEngine::getMatchingIcon(QString searchPath, QString icName, bool subdirectories) {
    QList<iconInfo> retval;
    QDir path = searchPath;
    if (path.exists()) {
        QDirIterator* iterator;
        if (subdirectories) {
            iterator = new QDirIterator(path, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
        } else {
            iterator = new QDirIterator(path);
        }

        while (iterator->hasNext()) {
            iterator->next();

            QFileInfo info = iterator->fileInfo();
            QString baseName = info.completeBaseName();
            //QString baseName = iterator->filePath();
            //baseName = baseName.mid(baseName.lastIndexOf("/") + 1);
            //baseName = baseName.left(baseName.indexOf("."));
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
                    icInfo.iconData = new QSharedMemory("ts-qtpl.icon." + icName + "." + QString::number(ic.size().width()));

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
    //bool doPaint = true;
    int failNum = 0;
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QColor pixelCol = image.pixelColor(x, y);
            //int blue = pixelCol.blue(), green = pixelCol.green(), red = pixelCol.red();
            if ((pixelCol.blue() > pixelCol.green() - 10 && pixelCol.blue() < pixelCol.green() + 10) &&
                    (pixelCol.green() > pixelCol.red() - 10 && pixelCol.green() < pixelCol.red() + 10)) {
            } else {
                failNum++;
                //doPaint = false;
            }
        }
    }

    if (failNum < (image.size().width() * image.size().height()) / 8) {
        QPainter painter(&image);
        painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        painter.fillRect(0, 0, image.width(), image.height(), tint);
        painter.end();
    }
}

QList<QSize> IconEngine::availableSizes(QIcon::Mode mode, QIcon::State state) const {
    QList<QSize> retval;
    for (iconInfo icon : listOfIcons) {
        retval.append(QSize(icon.size, icon.size));
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
