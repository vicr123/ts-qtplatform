#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <qpa/qplatformdialoghelper.h>
#include <QPainter>
#include <QPaintEvent>

namespace Ui {
class MessageDialog;
}

class MessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageDialog(QWidget *parent = 0);
    ~MessageDialog();

public slots:
    void setTitle(QString title);
    void setText(QString text);
    void setButtons(QPlatformDialogHelper::StandardButtons buttons);
    void setIcon(QIcon icon);
    void setParent(QWindow* parent);
    void setWindowFlags(Qt::WindowFlags type);
    void show();
    int exec();

    void prepareShow();

signals:
    void clicked(QPlatformDialogHelper::StandardButton button, QPlatformDialogHelper::ButtonRole role);

private slots:
    void on_okButton_clicked();

    void on_yesButton_clicked();

    void on_noButton_clicked();

    void on_cancelButton_clicked();

    void on_saveButton_clicked();

    void on_saveAllButton_clicked();

    void on_discardButton_clicked();

    void on_closeButton_clicked();


private:
    Ui::MessageDialog *ui;

    void paintEvent(QPaintEvent* event);
    QWindow* parent = NULL;
    Qt::WindowFlags flags;
};

#endif // MESSAGEDIALOG_H
