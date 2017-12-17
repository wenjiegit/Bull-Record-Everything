#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QPushButton>

class MenuButtonPrivate;

class MenuButton : public QPushButton
{
    Q_OBJECT
public:

    struct MenuButtonData {
        QPixmap normalPix;
        QPixmap hoverPix;
        QPixmap pressPix;

        QColor  normalTextColor;
        QColor  hoverTextColor;
        QColor  pressTextColor;
        QColor  pressedBackgroundColor;
        QColor  normalBackgroundColor;
    };

    explicit MenuButton(QWidget *parent = 0);
    ~MenuButton();

    void setInfo(const MenuButtonData &data);
    void setCheck(bool checked);

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

protected:
    void paintEvent(QPaintEvent *);

private:
    MenuButtonPrivate* impl;
};

class MenuButtonGroupPrivate;
class MenuButtonGroup : public QObject
{
    Q_OBJECT
public:
    explicit MenuButtonGroup(QObject *parent = 0);
    ~MenuButtonGroup();

    void addButton(MenuButton *btn);

protected slots:
    void onButtonClicked();

private:
    MenuButtonGroupPrivate* impl;
};

#endif // MENUBUTTON_H
