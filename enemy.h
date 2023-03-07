#ifndef ENEMY_H
#define ENEMY_H
#include "unit.h"
#include <QObject>
#include <QWidget>
using namespace std;

class Agent;

class Enemy : public Unit {
    Q_OBJECT
public:
    explicit Enemy(QWidget* parent = nullptr);
    int id, rk;
    double x, y;
    enum { NORTH,
        EAST,
        SOUTH,
        WEST } orientation;
    int damage;
    double speed;
    bool frozen;
    bool bleeding;
    vector<QPoint> path;
    bool flash;
    Agent* blocker = nullptr;
    bool update_blocked();

    void moving();
    void dead();
    void wounded(int damage);
};

class Worm : public Enemy {
    Q_OBJECT
public:
    Worm(QWidget* parent = nullptr);
    ~Worm();
    int attack_rate_cur, attack_rate_max;
    QMovie* anim_walk = nullptr;
    QMovie* anim_attack = nullptr;
    Unit* find_target();
    void lost_target();
    void act();
};

#endif // ENEMY_H
