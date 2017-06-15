#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>
#include <QUrl>
#include <qpa/qplatformdialoghelper.h>

namespace Ui {
class FileDialog;
}

class FileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileDialog(QWidget *parent = 0);
    ~FileDialog();

    QUrl currentDirectory();
    QList<QUrl> selectedFiles();

signals:
    void clicked(QPlatformDialogHelper::StandardButton button, QPlatformDialogHelper::ButtonRole role);

public slots:
    void setParent(QWindow* parent);
    void setWindowFlags(Qt::WindowFlags type);
    void setTitle(QString title);
    void setCurrentDirectory(QUrl directory);
    void show();
    int exec();

    void prepareShow();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::FileDialog *ui;
    QWindow* parent = NULL;
    Qt::WindowFlags flags;

    QUrl directory;
};

#endif // FILEDIALOG_H
