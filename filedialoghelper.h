#ifndef FILEDIALOGHELPER_H
#define FILEDIALOGHELPER_H

#include <qpa/qplatformdialoghelper.h>
#include <QObject>
#include <QWindow>
#include "filedialog.h"

class FileDialogHelper : public QPlatformFileDialogHelper
{
    Q_OBJECT
public:
    explicit FileDialogHelper();

    void exec() override;
    bool show(Qt::WindowFlags windowFlags, Qt::WindowModality windowModality, QWindow *parent) override;
    void hide() override;

    bool defaultNameFilterDisables() const override;
    void setDirectory(const QUrl &directory) override;
    QUrl directory() const override;
    void selectFile(const QUrl &filename) override;
    QList<QUrl> selectedFiles() const override;
    void setFilter() override;
    void selectNameFilter(const QString &filter) override;
    QString selectedNameFilter() const override;

signals:

public slots:

private:
    void updateWindowOptions();
    FileDialog* dialogWindow;
};

#endif // FILEDIALOGHELPER_H
