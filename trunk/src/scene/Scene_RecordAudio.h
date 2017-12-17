#ifndef SCENE_RECORDAUDIO_H
#define SCENE_RECORDAUDIO_H

#include <QWidget>

namespace Ui {
class Scene_RecordAudio;
}

class Scene_RecordAudio : public QWidget
{
    Q_OBJECT

public:
    explicit Scene_RecordAudio(QWidget *parent = 0);
    ~Scene_RecordAudio();

private:
    Ui::Scene_RecordAudio *ui;
};

#endif // SCENE_RECORDAUDIO_H
