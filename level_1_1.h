#ifndef LEVEL_1_1_H
#define LEVEL_1_1_H

#include "scene.h"

class Level_1_1 : public Scene {
    Q_OBJECT
public:
    explicit Level_1_1(QWidget* parent = nullptr);
    ~Level_1_1();
    QMovie* background = new QMovie(":/sources/background/background1.jpg");
    QPushButton* exit = new QPushButton(this);
    void init();
    void paintEvent(QPaintEvent* event);
private slots:
    void onTimer();
    void leave();
    // void mousePressEvent(QMouseEvent *event);
};
#endif // LEVEL_1_1_H
