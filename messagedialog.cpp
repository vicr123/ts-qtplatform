#include "messagedialog.h"
#include "ui_messagedialog.h"

#include <QMessageBox>
#include "messagedialoghelper.h"
#include <tvariantanimation.h>

MessageDialog::MessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->resize(SC_DPI_T(this->size(), QSize));
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

void MessageDialog::setTitle(QString title) {
    ui->titleLabel->setText(title);
    this->setWindowTitle(title);
}

void MessageDialog::setText(QString text) {
    ui->messageLabel->setText(text);
}

void MessageDialog::setIcon(QIcon icon) {
    this->setWindowIcon(icon);

    ui->iconLabel->setPixmap(icon.pixmap(SC_DPI(24), SC_DPI(24)));
}

void MessageDialog::setButtons(QPlatformDialogHelper::StandardButtons buttons, QVector<QMessageDialogOptions::CustomButton> customButtons) {
    QBoxLayout* l = static_cast<QBoxLayout*>(ui->buttonWidget->layout());
    QLayoutItem* item;
    while ((item = l->takeAt(0)) != nullptr) {
        l->removeItem(item);
        item->widget()->deleteLater();
        delete item;
    }

    struct ButtonDef {
        ButtonDef() {}
        ButtonDef(QString text, int id, QString icon = "") {
            this->text = text;
            this->id = id;
            this->icon = icon;
            this->role = MessageDialogHelper::buttonRole(static_cast<QPlatformDialogHelper::StandardButton>(id));
        }

        QString text;
        QPlatformDialogHelper::ButtonRole role;
        int id;
        QString icon = "";
    };
    QList<ButtonDef> shownButtons;

    for (QMessageDialogOptions::CustomButton button : customButtons) {
        if (button.label == QMessageBox::tr("Show Details...")) continue; //Ignore this button
        ButtonDef b;
        b.text = button.label;
        b.role = button.role;
        b.id = button.id;
        shownButtons.append(b);
    }

    if (buttons & QPlatformDialogHelper::Discard) shownButtons.append(ButtonDef(tr("Discard"), QPlatformDialogHelper::Discard, "user-trash"));
    if (buttons & QPlatformDialogHelper::Open) shownButtons.append(ButtonDef(tr("Open"), QPlatformDialogHelper::Open, "document-open"));
    if (buttons & QPlatformDialogHelper::Save) shownButtons.append(ButtonDef(tr("Save"), QPlatformDialogHelper::Save, "document-save"));
    if (buttons & QPlatformDialogHelper::SaveAll) shownButtons.append(ButtonDef(tr("Save All"), QPlatformDialogHelper::SaveAll, "document-save-all"));
    if (buttons & QPlatformDialogHelper::Yes) shownButtons.append(ButtonDef(tr("Yes"), QPlatformDialogHelper::Yes, "dialog-ok"));
    if (buttons & QPlatformDialogHelper::YesToAll) shownButtons.append(ButtonDef(tr("Yes To All"), QPlatformDialogHelper::YesToAll, "dialog-ok"));
    if (buttons & QPlatformDialogHelper::No) shownButtons.append(ButtonDef(tr("No"), QPlatformDialogHelper::No, "dialog-cancel"));
    if (buttons & QPlatformDialogHelper::NoToAll) shownButtons.append(ButtonDef(tr("No To All"), QPlatformDialogHelper::NoToAll, "dialog-cancel"));
    if (buttons & QPlatformDialogHelper::Abort) shownButtons.append(ButtonDef(tr("Abort"), QPlatformDialogHelper::Abort, "dialog-close"));
    if (buttons & QPlatformDialogHelper::Retry) shownButtons.append(ButtonDef(tr("Retry"), QPlatformDialogHelper::Retry, "view-refresh"));
    if (buttons & QPlatformDialogHelper::Ignore) shownButtons.append(ButtonDef(tr("Ignore"), QPlatformDialogHelper::Ignore));
    if (buttons & QPlatformDialogHelper::Close) shownButtons.append(ButtonDef(tr("Close"), QPlatformDialogHelper::Close, "dialog-close"));
    if (buttons & QPlatformDialogHelper::Ok) shownButtons.append(ButtonDef(tr("OK"), QPlatformDialogHelper::Ok, "dialog-ok"));
    if (buttons & QPlatformDialogHelper::Cancel) shownButtons.append(ButtonDef(tr("Cancel"), QPlatformDialogHelper::Cancel, "go-previous"));

    if (shownButtons.count() == 0) {
        //Failsafe in case no buttons are provided
        shownButtons.append(ButtonDef(tr("OK"), QPlatformDialogHelper::Ok, "dialog-ok"));
    }

    if (ui->detailedTextLabel->text() != "") {
        //Add button to show detailed text
        QPushButton* b = new QPushButton();
        b->setIcon(QIcon::fromTheme("go-down"));
        connect(b, &QPushButton::clicked, this, [=] {
            tVariantAnimation* anim = new tVariantAnimation(this);
            anim->setStartValue(ui->detailedTextScrollArea->height());
            if (ui->detailedTextScrollArea->height() == 0) {
                anim->setEndValue(qMin(ui->detailedTextScrollArea->sizeHint().height(), SC_DPI(300)));
                b->setIcon(QIcon::fromTheme("go-up"));
            } else {
                anim->setEndValue(0);
                b->setIcon(QIcon::fromTheme("go-down"));
            }
            anim->setDuration(500);
            anim->setEasingCurve(QEasingCurve::OutCubic);
            connect(anim, &tVariantAnimation::valueChanged, this, [=](QVariant value) {
                ui->detailedTextScrollArea->setFixedHeight(value.toInt());
                this->resize(this->width(), this->sizeHint().height());
            });
            connect(anim, &tVariantAnimation::finished, anim, &tVariantAnimation::deleteLater);
            anim->start();
        });
        l->addWidget(b);
    }

    for (ButtonDef button : shownButtons) {
        QPushButton* b = new QPushButton(this);
        b->setText(button.text);
        b->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        if (button.role == QPlatformDialogHelper::DestructiveRole) {
            b->setProperty("type", "destructive");
        }
        if (!button.icon.isEmpty()) b->setIcon(QIcon::fromTheme(button.icon));
        connect(b, &QPushButton::clicked, this, [=] {
            emit clicked(static_cast<QPlatformDialogHelper::StandardButton>(button.id), button.role);
            this->close();
        });
        l->addWidget(b);
    }
}

void MessageDialog::show() {
    prepareShow();
    QDialog::show();
}

int MessageDialog::exec() {
    prepareShow();
    return QDialog::exec();
}

void MessageDialog::prepareShow() {
    QDialog::setWindowFlags(Qt::FramelessWindowHint | flags);

    ui->detailedTextScrollArea->setFixedHeight(0);

    if (ui->titleLabel->text().isEmpty()) {
        ui->titleAreaWidget->setVisible(false);
    }

    this->resize(this->width(), this->sizeHint().height());
}

void MessageDialog::setSliceColor(QColor col) {
    this->sliceColor = col;
}

void MessageDialog::paintEvent(QPaintEvent *event) {
    QRect rect = event->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (sliceColor.alpha() != 0) {
        {
            QPolygon p;
            p.append(QPoint(this->width() - SC_DPI(75), 0));
            p.append(QPoint(this->width(), 0));
            p.append(QPoint(this->width(), this->height()));
            p.append(QPoint(this->width() - SC_DPI(100), this->height()));

            QColor c = sliceColor;
            c.setAlpha(127);

            painter.setPen(Qt::transparent);
            painter.setBrush(c);
            painter.drawPolygon(p);
        }

        {
            QPolygon p;
            p.append(QPoint(this->width() - SC_DPI(50), 0));
            p.append(QPoint(this->width(), 0));
            p.append(QPoint(this->width(), this->height()));
            p.append(QPoint(this->width() - SC_DPI(75), this->height()));

            painter.setPen(Qt::transparent);
            painter.setBrush(sliceColor);
            painter.drawPolygon(p);
        }
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    if (this->windowFlags() & Qt::FramelessWindowHint) {
        painter.setPen(this->palette().color(QPalette::Foreground));
        painter.setBrush(Qt::transparent);
        painter.drawRect(rect.adjusted(0, 0, -1, -1));
    }

    QDialog::paintEvent(event);
}

void MessageDialog::setParent(QWindow *parent) {
    this->parent = parent;
}

void MessageDialog::setWindowFlags(Qt::WindowFlags type) {
    this->flags = type;
}

void MessageDialog::setDetailedText(QString detailedText) {
    ui->detailedTextLabel->setText(detailedText);
}
