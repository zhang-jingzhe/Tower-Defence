#ifndef ANIMATION_H
#define ANIMATION_H

#include "enemy.h"
#include "unit.h"

class Animation : public BaseClass {
    Q_OBJECT
public:
    explicit Animation(QWidget* parent = 0);
};

class Bullet : public Animation {
public:
    explicit Bullet(QWidget* parent = 0);
    ~Bullet();
    QMovie* anmi = new QMovie(":/sources/anmi/bullet.png");
    Unit* target;
    int damage;
    bool bleeding;
    QPoint s, t;
    int anmi_time_total;
    int anmi_time_cur;
    void act();
};

class HurtAnim : public Animation {
public:
    explicit HurtAnim(QWidget* parent = 0);
    ~HurtAnim();
    Unit* target;
    int damage;
    int anmi_time_total;
    int anmi_time_cur;
    void act();
};

#endif // ANIMATION_H
