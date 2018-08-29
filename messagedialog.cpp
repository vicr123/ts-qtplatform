#include "messagedialog.h"
#include "ui_messagedialog.h"

extern float getDPIScaling();

MessageDialog::MessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->resize(this->size() * getDPIScaling());
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

    ui->iconLabel->setPixmap(icon.pixmap(32 * getDPIScaling(), 32 * getDPIScaling()));
}

void MessageDialog::setButtons(QPlatformDialogHelper::StandardButtons buttons) {
    QBoxLayout* l = (QBoxLayout*) ui->buttonWidget->layout();
    QLayoutItem* item;
    while ((item = l->takeAt(0)) != NULL) {
        l->removeItem(item);
        item->widget()->deleteLater();
        delete item;
    }

    l->addStretch();

    bool buttonVisible = false;
    if (buttons & QPlatformDialogHelper::Ok) {
        QPushButton* b = new QPushButton();
        b->setText(tr("OK"));
        b->setIcon(QIcon::fromTheme("dialog-ok"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::Ok, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }
    if (buttons & QPlatformDialogHelper::Yes) {
        QPushButton* b = new QPushButton();
        b->setText(tr("Yes"));
        b->setIcon(QIcon::fromTheme("dialog-ok"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::Yes, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }
    if (buttons & QPlatformDialogHelper::YesToAll) {
        QPushButton* b = new QPushButton();
        b->setText(tr("Yes to All"));
        b->setIcon(QIcon::fromTheme("dialog-ok"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::YesToAll, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }
    if (buttons & QPlatformDialogHelper::No) {
        QPushButton* b = new QPushButton();
        b->setText(tr("No"));
        b->setIcon(QIcon::fromTheme("dialog-cancel"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::No, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }
    if (buttons & QPlatformDialogHelper::NoToAll) {
        QPushButton* b = new QPushButton();
        b->setText(tr("No to All"));
        b->setIcon(QIcon::fromTheme("dialog-cancel"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::NoToAll, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }
    if (buttons & QPlatformDialogHelper::Cancel) {
        QPushButton* b = new QPushButton();
        b->setText(tr("Cancel"));
        b->setIcon(QIcon::fromTheme("go-previous"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::Cancel, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }
    if (buttons & QPlatformDialogHelper::Save) {
        QPushButton* b = new QPushButton();
        b->setText(tr("Save"));
        b->setIcon(QIcon::fromTheme("document-save"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::Save, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }
    if (buttons & QPlatformDialogHelper::SaveAll) {
        QPushButton* b = new QPushButton();
        b->setText(tr("Save All"));
        b->setIcon(QIcon::fromTheme("document-save-all"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::SaveAll, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }
    if (buttons & QPlatformDialogHelper::Open) {
        QPushButton* b = new QPushButton();
        b->setText(tr("Open"));
        b->setIcon(QIcon::fromTheme("document-open"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::Open, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }
    if (buttons & QPlatformDialogHelper::Discard) {
        QPushButton* b = new QPushButton();
        b->setText(tr("Discard"));
        b->setIcon(QIcon::fromTheme("user-trash"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::Discard, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }
    if (buttons & QPlatformDialogHelper::Close) {
        QPushButton* b = new QPushButton();
        b->setText(tr("Close"));
        b->setIcon(QIcon::fromTheme("dialog-close"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::Close, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }
    if (buttons & QPlatformDialogHelper::Abort) {
        QPushButton* b = new QPushButton();
        b->setText(tr("Abort"));
        b->setIcon(QIcon::fromTheme("dialog-close"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::Abort, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }
    if (buttons & QPlatformDialogHelper::Retry) {
        QPushButton* b = new QPushButton();
        b->setText(tr("Retry"));
        b->setIcon(QIcon::fromTheme("view-refresh"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::Retry, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }
    if (buttons & QPlatformDialogHelper::Ignore) {
        QPushButton* b = new QPushButton();
        b->setText(tr("Ignore"));
        //b->setIcon(QIcon::fromTheme("view-refresh"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::Ignore, QPlatformDialogHelper::AcceptRole);
            this->close();
        });
        l->addWidget(b);
        buttonVisible = true;
    }

    if (!buttonVisible) {
        //As a failsafe in case no buttons are provided
        QPushButton* b = new QPushButton();
        b->setText(tr("Cancel"));
        b->setIcon(QIcon::fromTheme("go-previous"));
        connect(b, &QPushButton::clicked, [=] {
            emit clicked(QPlatformDialogHelper::Cancel, QPlatformDialogHelper::AcceptRole);
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
    /*if (this->parent != NULL) {
        QDialog::setWindowFlags(Qt::FramelessWindowHint | flags);
    } else {*/
        QDialog::setWindowFlags(Qt::FramelessWindowHint | flags);
    //}
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
            p.append(QPoint(this->width() - 75 * getDPIScaling(), 0));
            p.append(QPoint(this->width(), 0));
            p.append(QPoint(this->width(), this->height()));
            p.append(QPoint(this->width() - 100 * getDPIScaling(), this->height()));

            QColor c = sliceColor;
            c.setAlpha(127);

            painter.setPen(Qt::transparent);
            painter.setBrush(c);
            painter.drawPolygon(p);
        }

        {
            QPolygon p;
            p.append(QPoint(this->width() - 50 * getDPIScaling(), 0));
            p.append(QPoint(this->width(), 0));
            p.append(QPoint(this->width(), this->height()));
            p.append(QPoint(this->width() - 75 * getDPIScaling(), this->height()));

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
