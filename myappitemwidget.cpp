#include "myappitemwidget.h"
#include <QIcon>
#include <QLayout>
#include <QPixmap>
#include <QDebug>
#include <QMouseEvent>

MyAppItemWidget::MyAppItemWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(100, 100);
    this->setObjectName("mainWidget");
    initUI();
    this->setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet(R"(
        QWidget#mainWidget{
            border: 2px solid #ebebeb;
            border-radius: 15px;
        }

        QWidget#mainWidget:hover{
            background-color: #d0d0d0;
        }
    )");
}

void MyAppItemWidget::initUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    m_imageLabel = new QLabel();
    m_name = new QLabel("appName");

    m_imageLabel->setFixedSize(60, 60);
    m_name->setAlignment(Qt::AlignHCenter);
    m_imageLabel->setAlignment(Qt::AlignHCenter);

    QHBoxLayout *imgLayout = new QHBoxLayout();
    imgLayout->addWidget(m_imageLabel);

    mainLayout->addLayout(imgLayout);
    mainLayout->addWidget(m_name);
}

void MyAppItemWidget::setIcon(QIcon icon)
{
    m_imageLabel->setPixmap(icon.pixmap(60, 60));
    m_imageLabel->setScaledContents(true);
}

void MyAppItemWidget::setIcon(const QString &path)
{
    m_imageLabel->setPixmap(path);
    m_imageLabel->setScaledContents(true);

}

void MyAppItemWidget::setName(const QString &appName)
{
    m_name->setText(appName);
}

void MyAppItemWidget::mouseReleaseEvent(QMouseEvent *e)
{
    emit clicked(e->globalPos());
    if(e->button() == Qt::RightButton) {
        emit rightClicked(e->globalPos());
    }
}

void MyAppItemWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit doubleClicked(e->globalPos());
}
