#include "filedialog.h"
#include "ui_filedialog.h"

#include <QFileSystemModel>
#include <QTimer>
#include <QMessageBox>

struct FileDialogPrivate {
    QWindow* parent = nullptr;
    QFileSystemModel* fsModel;

    Qt::WindowFlags flags;
    QFileDialogOptions::FileDialogOptions options;
    QFileDialogOptions::FileMode fileMode;
    bool discardDelay = false;

    QList<QUrl> selection;
};

FileDialog::FileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDialog)
{
    ui->setupUi(this);
    d = new FileDialogPrivate();

    d->fsModel = new QFileSystemModel();
    d->fsModel->setRootPath("/");
    ui->dirView->setModel(d->fsModel);
}

FileDialog::~FileDialog()
{
    d->fsModel->deleteLater();
    delete d;
    delete ui;
}

void FileDialog::setParent(QWindow *parent) {
    d->parent = parent;
}

void FileDialog::setWindowFlags(Qt::WindowFlags type) {
    d->flags = type;
}

void FileDialog::setTitle(QString title) {
    this->setWindowTitle(title);
}

void FileDialog::setCurrentDirectory(QUrl directory) {
    ui->dirView->setRootIndex(d->fsModel->index(directory.toLocalFile()));
}

QUrl FileDialog::currentDirectory() {
    return QUrl::fromLocalFile(d->fsModel->fileName(ui->dirView->rootIndex()));
}

QList<QUrl> FileDialog::selectedFiles() {
    return d->selection;
}

void FileDialog::on_okButton_clicked()
{
    QStringList filenames;
    for (QModelIndex index : ui->dirView->selectionModel()->selectedRows(0)) {
        filenames.append(d->fsModel->filePath(index));
    }
    finalize(filenames);
    //this->accept();
}

void FileDialog::on_cancelButton_clicked()
{
    this->reject();
}

void FileDialog::show() {
    prepareShow();
    QDialog::show();
}

int FileDialog::exec() {
    d->discardDelay = true;
    return QDialog::exec();
}

void FileDialog::prepareShow() {
    /*if (this->parent != NULL) {
        QDialog::setWindowFlags(Qt::FramelessWindowHint | flags);
    } else {*/
        QDialog::setWindowFlags(d->flags);
    //}
}

void FileDialog::go(QString directory) {
    QModelIndex index = d->fsModel->index(directory);
    ui->dirView->setRootIndex(index);

    QFileInfo info(directory);
    /*if (!info.exists()) {
        setError(tr("That folder doesn't exist."));
    } else if (!info.isDir()) {
        setError(tr("That's not a folder."));
    } else if (!info.isReadable() && !info.isExecutable()) {
        setError(tr("Couldn't get to that folder."));
    } else {
        errorWidget->setVisible(false);
    }*/
}

void FileDialog::on_dirView_activated(const QModelIndex &index)
{
    QFileInfo info = d->fsModel->fileInfo(index);

    if (info.isFile()) {
        //Select the file
        if (ui->savePanel->isVisible()) {
            ui->saveFileName->setText(info.fileName());
        } else {
            switch (d->fileMode) {
                case QFileDialogOptions::AnyFile:
                case QFileDialogOptions::ExistingFile:
                    finalize(QStringList() << info.filePath());
                    break;
            }
        }
    } else {
        go(info.filePath());
    }
}

void FileDialog::setAcceptMode(QFileDialogOptions::AcceptMode mode) {
    if (mode == QFileDialogOptions::AcceptOpen) {
        ui->savePanel->setVisible(false);
    } else {
        ui->savePanel->setVisible(true);
    }
}

void FileDialog::setNameFilters(QStringList nameFilters) {
    QStringList newFilters;
    QRegularExpression regex("(?<=\\().+(?=\\))");
    for (QString filter : nameFilters) {
        QRegularExpressionMatchIterator iterator = regex.globalMatch(filter);
        QRegularExpressionMatch match;
        while (iterator.hasNext()) {
            match = iterator.next();
        }
        if (match.isValid()) {
            newFilters.append(match.captured().split(", "));
        }
    }
    d->fsModel->setNameFilters(newFilters);
}

void FileDialog::setFilters(QDir::Filters filters) {
    d->fsModel->setFilter(filters);
}

void FileDialog::on_upButton_clicked()
{
    QFileInfo info = d->fsModel->fileInfo(ui->dirView->rootIndex());
    if (!info.isRoot()) {
        go(info.path());
    }
}

void FileDialog::showAfterDelay() {
    QTimer::singleShot(100, [=] {
        if (!d->discardDelay) {
            this->show();
        }
        d->discardDelay = false;
    });
}

void FileDialog::setFileMode(QFileDialogOptions::FileMode mode) {
    d->fileMode = mode;
    switch (mode) {
        case QFileDialogOptions::AnyFile:
        case QFileDialogOptions::ExistingFile:
            ui->titleLabel->setText(tr("Select a file"));
            break;
        case QFileDialogOptions::Directory:
        case QFileDialogOptions::DirectoryOnly:
            ui->titleLabel->setText(tr("Select a folder"));
            break;
        case QFileDialogOptions::ExistingFiles:
            ui->titleLabel->setText(tr("Select some files"));
    }
}

void FileDialog::setOptions(QFileDialogOptions::FileDialogOptions options) {
    d->options = options;
}

void FileDialog::finalize(QStringList filenames) {
    if (filenames.count() == 0) return; //Don't do anything

    if (d->options ^ QFileDialogOptions::DontConfirmOverwrite) {
        //Check each file to make sure
        QStringList existingFiles;
        for (QString f : filenames) {
            if (QFile::exists(f)) existingFiles.append(f);
        }

        QString message;
        if (existingFiles.count() == 1) {
            message = tr("Overwrite %1?").arg(existingFiles.first());
        } else if (existingFiles.count() > 1) {
            message = tr("Overwrite %n files?", nullptr, existingFiles.count());
        }

        if (message != "") {
            if (QMessageBox::warning(this, tr("Overwrite Files"), message, QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No) {
                //Don't do anything
                return;
            }
        }
    }

    d->selection.clear();
    for (QString f : filenames) {
        d->selection.append(QUrl::fromLocalFile(f));
    }

    //Tell everyone we're done here
    accept();
}
