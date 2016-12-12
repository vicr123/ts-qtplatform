#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <qpa/qplatformdialoghelper.h>

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

signals:
    void clicked(QPlatformDialogHelper::StandardButton button, QPlatformDialogHelper::ButtonRole role);

private slots:
    void on_okButton_clicked();

    void on_yesButton_clicked();

    void on_noButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::MessageDialog *ui;
};

#endif // MESSAGEDIALOG_H
