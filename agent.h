#ifndef AGENT_H
#define AGENT_H
#include "enemy.h"
#include "unit.h"

class Agent : public Unit {
    Q_OBJECT
public:
    explicit Agent(QWidget* parent = nullptr);
    int type;
    int damage;
    int block_max, blocking; //阻拦敌人的最大数量/正在阻拦的数量
    int buff_num, buff_max;
    enum { NORTH,
        EAST,
        SOUTH,
        WEST } orientation;
    vector<pair<int, int>> attack_range; // when facing east
    vector<QPoint> get_cur_attacking_squares();
    vector<Enemy*> blocked_list; //阻拦的敌人列表
    void wounded(int damage);
    void dead();
    virtual void add_buff(int type);
    //virtual vector<QString> info() = 0;
};

class Defender76 : public Agent {
    Q_OBJECT
public:
    QMovie* anim;
    QMovie* anim_attack;
    int attack_rate_max, attack_rate_cur;
    bool ice_injury;
    bool group_injury;
    Defender76(QWidget* parent = nullptr);
    ~Defender76();
    void act();
    Unit* find_target();
    vector<Unit*> find_targets();
    void lost_target();
    void add_buff(int type);
    //vector<QString> info();
};

class Shooter : public Agent {
    Q_OBJECT
public:
    QMovie* anim;
    QMovie* anim_attack;
    int attack_rate_max, attack_rate_cur;
    int attack_anim_max, attack_anim_cur;
    bool bleeding_injury;
    Shooter(QWidget* parent = nullptr);
    ~Shooter();
    void act();
    Unit* find_target();
    void lost_target();
    void add_buff(int type);
    //vector<QString> info();
};

#endif // AGENT_H
