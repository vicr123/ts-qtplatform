#include "platformmenu.h"

void PlatformMenu::insertMenuItem(QPlatformMenuItem *menuItem, QPlatformMenuItem *before) {
    if (before == NULL) {
        menuItems.append(menuItem);
    } else {
        bool inserted = false;
        for (int i = 0; i < menuItems.count(); i++) {
            if (menuItems.at(i) == before) {
                menuItems.insert(i, menuItem);
                inserted = true;
                i = menuItems.count();
            }
        }

        if (!inserted) {
            menuItems.append(menuItem);
        }
    }
}

void PlatformMenu::removeMenuItem(QPlatformMenuItem *menuItem) {
    menuItems.removeAll(menuItem);
}

void PlatformMenu::syncMenuItem(QPlatformMenuItem *menuItem) {
    /*menuItem->setText(tx);
    menuItem->setTag(tg);
    menuItem->setIcon(ic);
    menuItem->setEnabled(en);
    menuItem->setVisible(vis);*/
}

void PlatformMenu::syncSeparatorsCollapsible(bool enable) {

}

void PlatformMenu::setTag(quintptr tag) {
    tg = tag;
}

quintptr PlatformMenu::tag() const {
    return tg;
}

void PlatformMenu::setText(const QString &text) {
    tx = text;
}

void PlatformMenu::setIcon(const QIcon &icon) {
    ic = icon;
}

void PlatformMenu::setEnabled(bool enabled) {
    en = enabled;
}

bool PlatformMenu::isEnabled() const {
    return en;
}

void PlatformMenu::setVisible(bool visible) {
    vis = visible;
}

void PlatformMenu::showPopup(const QWindow *parentWindow, const QRect &targetRect, const QPlatformMenuItem *item) {
    Q_UNUSED(parentWindow);
    Q_UNUSED(targetRect);
    Q_UNUSED(item);
    setVisible(true);
}

QPlatformMenuItem* PlatformMenu::menuItemAt(int position) const {
    return menuItems.at(position);
}

QPlatformMenuItem* PlatformMenu::menuItemForTag(quintptr tag) const {
    for (QPlatformMenuItem* item : menuItems) {
        if (item->tag() == tag) {
            return item;
        }
    }

    return NULL;
}

PlatformMenuBar::PlatformMenuBar() {
    widget = new QWidget;
}

void PlatformMenuBar::insertMenu(QPlatformMenu *menu, QPlatformMenu *before) {
    if (before == NULL) {
        menuItems.append(menu);
    } else {
        bool inserted = false;
        for (int i = 0; i < menuItems.count(); i++) {
            if (menuItems.at(i) == before) {
                menuItems.insert(i, menu);
                inserted = true;
                i = menuItems.count();
            }
        }

        if (!inserted) {
            menuItems.append(menu);
        }
    }
}

void PlatformMenuBar::removeMenu(QPlatformMenu *menu) {
    menuItems.removeAll(menu);
}

void PlatformMenuBar::syncMenu(QPlatformMenu *menuItem) {

}

void PlatformMenuBar::handleReparent(QWindow *newParentWindow) {
    if (newParentWindow != NULL) {
        parentWindow = newParentWindow;

        //widget->setParent(newParentWindow);
    }
}

QPlatformMenu* PlatformMenuBar::menuForTag(quintptr tag) const {
    for (QPlatformMenu* item : menuItems) {
        if (item->tag() == tag) {
            return item;
        }
    }

    return NULL;
}

QPlatformMenu* PlatformMenuBar::createMenu() const {
    /*PlatformMenu* menu = new PlatformMenu;
    for (QPlatformMenu* item : menuItems) {
        menu->insertMenuItem(item, NULL);
    }
    return menu;*/
    return new PlatformMenu;
}


void PlatformMenuItem::setTag(quintptr tag) {

}

quintptr PlatformMenuItem::tag() const {
    return 0;
}

void PlatformMenuItem::setText(const QString &text) {

}

void PlatformMenuItem::setIcon(const QIcon &icon) {

}

void PlatformMenuItem::setMenu(QPlatformMenu *menu) {

}

void PlatformMenuItem::setVisible(bool isVisible) {

}

void PlatformMenuItem::setIsSeparator(bool isSeparator) {

}

void PlatformMenuItem::setFont(const QFont &font) {

}

void PlatformMenuItem::setRole(MenuRole role) {

}

void PlatformMenuItem::setCheckable(bool checkable) {

}

void PlatformMenuItem::setChecked(bool isChecked) {

}

void PlatformMenuItem::setShortcut(const QKeySequence& shortcut) {

}

void PlatformMenuItem::setEnabled(bool enabled) {

}

void PlatformMenuItem::setIconSize(int size) {

}
