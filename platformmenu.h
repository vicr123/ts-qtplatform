#ifndef PLATFORMMENU_H
#define PLATFORMMENU_H

#include <qpa/qplatformmenu.h>
#include <QList>
#include <QWidget>

class PlatformMenu : public QPlatformMenu
{
    Q_OBJECT
public:
    void insertMenuItem(QPlatformMenuItem *menuItem, QPlatformMenuItem *before);
    void removeMenuItem(QPlatformMenuItem *menuItem);
    void syncMenuItem(QPlatformMenuItem *menuItem);
    void syncSeparatorsCollapsible(bool enable);

    void setTag(quintptr tag);
    quintptr tag() const;

    void setText(const QString &text);
    void setIcon(const QIcon &icon);
    void setEnabled(bool enabled);
    bool isEnabled() const;
    void setVisible(bool visible);

    void showPopup(const QWindow *parentWindow, const QRect &targetRect, const QPlatformMenuItem *item);

    QPlatformMenuItem *menuItemAt(int position) const;
    QPlatformMenuItem *menuItemForTag(quintptr tag) const;
signals:

public slots:

private:
    QList<QPlatformMenuItem*> menuItems;
    quintptr tg;
    QString tx;
    QIcon ic;
    bool en;
    bool vis;
};

class PlatformMenuBar : public QPlatformMenuBar
{
    Q_OBJECT
public:
    PlatformMenuBar();

    void insertMenu(QPlatformMenu *menu, QPlatformMenu *before);
    void removeMenu(QPlatformMenu *menu);
    void syncMenu(QPlatformMenu *menuItem);
    void handleReparent(QWindow *newParentWindow);

    QPlatformMenu *menuForTag(quintptr tag) const;
    QPlatformMenu *createMenu() const;

private:
    QList<QPlatformMenu*> menuItems;
    QWindow* parentWindow;
    QWidget* widget;
};

class PlatformMenuItem : public QPlatformMenuItem
{
    Q_OBJECT
public:
    void setTag(quintptr tag);
    quintptr tag() const;

    void setText(const QString &text);
    void setIcon(const QIcon &icon);
    void setMenu(QPlatformMenu *menu);
    void setVisible(bool isVisible);
    void setIsSeparator(bool isSeparator);
    void setFont(const QFont &font);
    void setRole(MenuRole role);
    void setCheckable(bool checkable);
    void setChecked(bool isChecked);
    void setShortcut(const QKeySequence& shortcut);
    void setEnabled(bool enabled);
    void setIconSize(int size);

};

#endif // PLATFORMMENU_H
