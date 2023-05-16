#ifndef MYAPPITEMWIDGET_H
#define MYAPPITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QIcon>

class MyAppItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAppItemWidget(QWidget *parent = nullptr);
    void initUI();
    void setIcon(QIcon icon);
    void setIcon(const QString &path);
    void setName(const QString &appName);

signals:
    void clicked(QPoint p);
    void rightClicked(QPoint p);
    void doubleClicked(QPoint p);

protected:
    virtual void mouseReleaseEvent(QMouseEvent * ev)override;
    virtual void mouseDoubleClickEvent(QMouseEvent* e)override;

private:
    QLabel *m_imageLabel;
    QLabel *m_name;
};

#endif // MYAPPITEMWIDGET_H
