#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>
#include <QUrl>
#include <qpa/qplatformdialoghelper.h>

namespace Ui {
class FileDialog;
}

struct FileDialogPrivate;
class FileDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit FileDialog(QWidget *parent = 0);
        ~FileDialog();

        QUrl currentDirectory();
        QList<QUrl> selectedFiles();

    public slots:
        void setParent(QWindow* parent);
        void setWindowFlags(Qt::WindowFlags type);
        void setAcceptMode(QFileDialogOptions::AcceptMode mode);
        void setTitle(QString title);
        void setCurrentDirectory(QUrl directory);
        void setNameFilters(QStringList filters);
        void setFilters(QDir::Filters filters);
        void setFileMode(QFileDialogOptions::FileMode mode);
        void setOptions(QFileDialogOptions::FileDialogOptions options);
        void show();
        void showAfterDelay();
        int exec();

        void prepareShow();

    private slots:
        void on_okButton_clicked();

        void on_cancelButton_clicked();

        void go(QString directory);

        void on_dirView_activated(const QModelIndex &index);

        void on_upButton_clicked();

        void finalize(QStringList filenames);

    private:
        Ui::FileDialog *ui;
        FileDialogPrivate* d;
};

#endif // FILEDIALOG_H
