#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <qpa/qplatformdialoghelper.h>
#include <QPainter>
#include <QPaintEvent>
#include <QLayout>
#include <QBoxLayout>

namespace Ui {
class MessageDialog;
}

class MessageDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit MessageDialog(QWidget *parent = nullptr);
        ~MessageDialog();

    public slots:
        void setTitle(QString title);
        void setText(QString text);
        void setDetailedText(QString detailedText);
        void setButtons(QPlatformDialogHelper::StandardButtons buttons, QVector<QMessageDialogOptions::CustomButton> custom);
        void setIcon(QIcon icon);
        void setParent(QWindow* parent);
        void setWindowFlags(Qt::WindowFlags type);
        void setSliceColor(QColor col);
        void show();
        int exec();

        void prepareShow();

    signals:
        void clicked(QPlatformDialogHelper::StandardButton button, QPlatformDialogHelper::ButtonRole role);
        void clickedCustom(int id);

    private:
        Ui::MessageDialog *ui;

        void paintEvent(QPaintEvent* event);
        QWindow* parent = nullptr;
        QColor sliceColor = QColor(Qt::transparent);
        Qt::WindowFlags flags;
};

#endif // MESSAGEDIALOG_H
