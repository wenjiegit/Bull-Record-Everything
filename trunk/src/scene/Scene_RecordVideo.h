#ifndef SCENE_RECORDVIDEO_H
#define SCENE_RECORDVIDEO_H

#include <QWidget>

namespace Ui {
class Scene_RecordVideo;
}

class Scene_RecordVideo : public QWidget
{
    Q_OBJECT

public:
    explicit Scene_RecordVideo(QWidget *parent = 0);
    ~Scene_RecordVideo();

private:
    Ui::Scene_RecordVideo *ui;
};

#endif // SCENE_RECORDVIDEO_H
