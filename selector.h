#ifndef SELECTOR_H
#define SELECTOR_H
#include "unit.h"

class Agent;

class Selector : public BaseClass {
public:
    Selector(QWidget* parent = nullptr);
    ~Selector();
    int agent_id;
    string available;
    Agent* tmp = nullptr;
    int cost = 50;
    int cd_max = 1, cd_cur = 1;
    int status = 0;
    QPoint selector_pos_origin; //selector原本所在的位置
    virtual void act();
    QWidget* front;
    QWidget* back;
    QLabel* frontText;
    void set_id(int id);
    void change_progress();

protected:
    void mousePressEvent(QMouseEvent* event);
};

class RemoveSelector : public Selector {
public:
    RemoveSelector(QWidget* parent = nullptr);
    ~RemoveSelector();

private:
    QMovie* anim = new QMovie(":/sources/icon/Remove.png");
};

class Defender76Selector : public Selector {
public:
    Defender76Selector(QWidget* parent = nullptr);
    ~Defender76Selector();

private:
    QMovie* anim = new QMovie(":/sources/agents/defender76.gif");
};

class ShooterSelector : public Selector {
public:
    ShooterSelector(QWidget* parent = nullptr);
    ~ShooterSelector();

private:
    QMovie* anim = new QMovie(":/sources/agents/shooter.gif");
};

class Buff1Selector : public Selector {
public:
    Buff1Selector(QWidget* parent = nullptr);
    ~Buff1Selector();

private:
    QMovie* anim = new QMovie(":/sources/buffs/buff1.gif");
};

class Buff2Selector : public Selector {
public:
    Buff2Selector(QWidget* parent = nullptr);
    ~Buff2Selector();

private:
    QMovie* anim = new QMovie(":/sources/buffs/buff2.gif");
};

class Buff3Selector : public Selector {
public:
    Buff3Selector(QWidget* parent = nullptr);
    ~Buff3Selector();

private:
    QMovie* anim = new QMovie(":/sources/buffs/buff3.gif");
};

class Buff4Selector : public Selector {
public:
    Buff4Selector(QWidget* parent = nullptr);
    ~Buff4Selector();

private:
    QMovie* anim = new QMovie(":/sources/buffs/buff4.gif");
};
#endif // SELECTOR_H
