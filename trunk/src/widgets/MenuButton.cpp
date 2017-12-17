#include "MenuButton.h"

#include <QPainter>

class MenuButtonPrivate
{
public:
    MenuButtonPrivate()
        : normal(true)
        , hover(false)
        , pressed(false)
    {

    }

    void paint(QPainter *painter, const QPixmap &pix, const QString &text, const QColor &textColor, const QRect &r)
    {
        painter->save();
        if (true) {
            painter->translate(QPoint(18, 0));
            QRect r(0, 0, pix.width(), r.height());
            QRect pr(0, 0, pix.width(), pix.height());
            pr.moveCenter(r.center());

            painter->drawPixmap(pr, pix);
        }
        painter->restore();

        painter->save();
        if (true) {
            painter->translate(QPoint(51, 0));

            QFontMetrics fmt(painter->font());
            int textWidth = fmt.width(text);
            QRectF textRect(0, 0, textWidth, r.height());

            painter->setPen(textColor);
            painter->drawText(textRect, text, QTextOption(Qt::AlignVCenter));
        }
        painter->restore();
    }

    MenuButton::MenuButtonData data;

    bool    normal;
    bool    hover;
    bool    pressed;
};

MenuButton::MenuButton(QWidget *parent)
    : QPushButton(parent)
    , impl(new MenuButtonPrivate)
{

}

MenuButton::~MenuButton()
{
    delete impl;
}

void MenuButton::setInfo(const MenuButtonData &data)
{
    impl->data = data;

    update();
}

void MenuButton::setCheck(bool checked)
{
    impl->pressed = checked;

    if (!checked) {
        impl->normal = true;
        impl->hover = false;
    }

    update();
}

void MenuButton::enterEvent(QEvent *e)
{
    if (!impl->pressed) {
        impl->hover = true;
        impl->normal = false;
    }

    update();

    QPushButton::enterEvent(e);
}

void MenuButton::leaveEvent(QEvent *e)
{
    if (!impl->pressed) {
        impl->hover = false;
        impl->normal = true;
    }

    update();

    QPushButton::leaveEvent(e);
}

void MenuButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    if (impl->pressed) {
        p.fillRect(rect(), impl->data.pressedBackgroundColor);
        impl->paint(&p, impl->data.pressPix, text(), impl->data.pressTextColor, rect());
    } else if (impl->normal) {
        p.fillRect(rect(), impl->data.normalBackgroundColor);
        impl->paint(&p, impl->data.normalPix, text(), impl->data.normalTextColor, rect());
    } else if (impl->hover) {
        p.fillRect(rect(), impl->data.normalBackgroundColor);
        impl->paint(&p, impl->data.hoverPix, text(), impl->data.hoverTextColor, rect());
    }
}

class MenuButtonGroupPrivate
{
public:
    QList<MenuButton*> btns;
};

MenuButtonGroup::MenuButtonGroup(QObject *parent)
    : QObject(parent)
    , impl(new MenuButtonGroupPrivate)
{

}

MenuButtonGroup::~MenuButtonGroup()
{
    delete impl;
}

void MenuButtonGroup::addButton(MenuButton *btn)
{
    impl->btns << btn;

    connect(btn, SIGNAL(clicked(bool)), this, SLOT(onButtonClicked()));
}

void MenuButtonGroup::onButtonClicked()
{
    MenuButton *btn = dynamic_cast<MenuButton *> (sender());
    if (!btn) {
        return;
    }

    for (int i = 0; i < impl->btns.size(); ++i) {
        MenuButton *m = impl->btns.at(i);
        if (m != btn) {
            m->setCheck(false);
        }
    }

    btn->setCheck(true);
}
