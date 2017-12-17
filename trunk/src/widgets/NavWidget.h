#ifndef NAVWIDGET_H
#define NAVWIDGET_H

#include <QWidget>

namespace Ui {
class NavWidget;
}

class QSignalMapper;
class NavWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NavWidget(QWidget *parent = 0);
    ~NavWidget();

    void addTab(const QString &text, QWidget *w);

signals:
    void tabChanged(QWidget *w);

private:
    Ui::NavWidget *ui;
    QSignalMapper *m_signalMapper;
};

#endif // NAVWIDGET_H
