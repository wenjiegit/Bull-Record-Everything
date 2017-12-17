#ifndef SCENE_GRABSCREEN_H
#define SCENE_GRABSCREEN_H

#include <QWidget>

namespace Ui {
class Scene_GrabScreen;
}

class Scene_GrabScreen : public QWidget
{
    Q_OBJECT

public:
    explicit Scene_GrabScreen(QWidget *parent = 0);
    ~Scene_GrabScreen();

private:
    Ui::Scene_GrabScreen *ui;
};

#endif // SCENE_GRABSCREEN_H
