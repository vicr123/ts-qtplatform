#ifndef ICONENGINE_H
#define ICONENGINE_H

#include <QIconEngine>
#include <QPainter>
#include <QIcon>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QApplication>
#include <QImage>
#include <QPalette>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QSvgRenderer>

class IconEngine : public QIconEngine
{
public:
    IconEngine(QString iconName);

    struct iconInfo {
        QString fileName;
        int size;

        bool operator==(const iconInfo& other) const {
            if ((this->fileName == other.fileName) && (this->size == other.size)) {
                return true;
            } else {
                return false;
            }
        }

        bool operator<(const iconInfo& other) const {
            return this->size < other.size;
        }

        bool operator>(const iconInfo& other) const {
            return this->size > other.size;
        }
    };

    struct cacheIcon {
        QString name;
        int size;
        QPixmap pm;
    };

    QString iconName() const;
    QIconEngine* clone() const;
    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state);
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state);
    QList<iconInfo> getMatchingIcon(QString searchPath, QString icName, bool subdirectories = true);
    QList<QSize> availableSizes(QIcon::Mode mode, QIcon::State state) const;
    bool isNull();
    QList<IconEngine::iconInfo> load(QString icName);

private:
    QString icName;

    QList<iconInfo> listOfIcons;
    void tintImage(QImage &image, QColor tint) const;
    QFuture<QList<IconEngine::iconInfo>> pendingIcons;
};

inline QList<IconEngine::cacheIcon> memorySizes = QList<IconEngine::cacheIcon>();

#endif // ICONENGINE_H
