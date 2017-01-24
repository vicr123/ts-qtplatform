#ifndef MESSAGEDIALOGHELPER_H
#define MESSAGEDIALOGHELPER_H

#include <qpa/qplatformdialoghelper.h>
#include "messagedialog.h"
#include <QWindow>
#include <QSoundEffect>

class MessageDialogHelper : public QPlatformMessageDialogHelper
{
    Q_OBJECT
public:
    explicit MessageDialogHelper();

    void exec() override;
    bool show(Qt::WindowFlags windowFlags, Qt::WindowModality windowModality, QWindow *parent) override;
    void hide() override;
signals:

public slots:

private:
    void updateWindowOptions();
    MessageDialog* dialogWindow;
};

#endif // MESSAGEDIALOGHELPER_H
