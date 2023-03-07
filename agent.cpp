#include "agent.h"
#include "scene.h"

Agent::Agent(QWidget* parent)
    : Unit(parent)
{
    buff_max = 2;
    buff_num = 0;
}

vector<QPoint> Agent::get_cur_attacking_squares()
{
    vector<QPoint> lst;
    lst.clear();
    for (pair<int, int> d : attack_range) { //枚举所有可能的攻击范围
        int x = m_x, y = m_y, dx = d.first, dy = d.second;
        switch (orientation) { //根据agent的方向计算可以攻击到的格子
        case Agent::NORTH:
            x -= dy;
            y += dx;
            break;
        case Agent::SOUTH:
            x += dy;
            y -= dx;
            break;
        case Agent::WEST:
            x -= dx;
            y -= dy;
            break;
        case Agent::EAST:
            x += dx;
            y += dy;
            break;
        }
        if (!(0 <= x && x < scene->map_x && 0 <= y && y < scene->map_y)) //如果超出了地图的范围则忽略
            continue;
        lst.push_back(QPoint(x, y));
    }
    return lst;
}

void Agent::dead()
{
    for (auto x : blocked_list)
        x->blocker = nullptr;
    for (Unit* x : aimed)
        if (x->target == this)
            x->lost_target();
}

void Agent::wounded(int damage)
{
    if (!exist)
        return;
    hp -= damage;
    if (hp <= 0) {
        hp = 0;
        exist = false;
    }
    hpBarfront->setGeometry(10, 5, 60 * ((double)hp / hp_max), 7);

    //    HurtAnim* hurtanim = new HurtAnim(scene);
    //    scene->animations.push_back(hurtanim);
    //    hurtanim->damage = damage;
    //    hurtanim->target = this;
}

void Agent::add_buff(int type)
{
}

Defender76::Defender76(QWidget* parent)
    : Agent(parent)
{
    type = 1;
    class_name = "Defender76";

    anim = new QMovie(":/sources/agents/defender76.gif");
    anim_attack = new QMovie(":/sources/agents/defender76_attack.gif");
    setMovie(anim);
    anim->start();
    anim_attack->start();
    show();

    block_max = 3;
    blocking = 0;
    blocked_list.clear();
    attack_range.clear();
    attack_range.push_back(make_pair(0, 0));
    attack_range.push_back(make_pair(1, 0));
    damage = 20;
    attack_rate_max = attack_rate_cur = 50;
    hp = hp_max = 500;
    ice_injury = false;
    group_injury = false;
}

Defender76::~Defender76()
{
    delete anim;
    delete anim_attack;
}

void Defender76::act()
{
    if (blocking > 0)
        setMovie(anim_attack);
    else
        setMovie(anim);
    if (blocking > 0) {
        attack_rate_cur--;
        if (attack_rate_cur <= 0) {
            if (!group_injury) { //非群伤
                Unit* tar = find_target();
                if (tar != nullptr) {
                    tar->wounded(damage);
                    if (ice_injury) //冰冻伤害
                        ((Enemy*)tar)->frozen = true;
                }
            } else { //群伤
                vector<Unit*> lst = find_targets();
                for (Unit* tar : lst) {
                    if (tar != nullptr) {
                        tar->wounded(damage);
                        if (ice_injury)
                            ((Enemy*)tar)->frozen = true;
                    }
                }
            }
            attack_rate_cur = attack_rate_max;
        }
    }
}

void Defender76::lost_target()
{
    target = NULL;
}

Unit* Defender76::find_target()
{
    vector<QPoint> V = get_cur_attacking_squares();
    vector<Unit*> lst;
    for (Enemy* e : scene->enemies) {
        for (QPoint p : V)
            if (p.x() == e->m_x && p.y() == e->m_y) {
                lst.push_back(e);
                break;
            }
    }
    if (lst.empty())
        return nullptr;
    return lst.front();
}

vector<Unit*> Defender76::find_targets()
{
    vector<QPoint> V = get_cur_attacking_squares();
    vector<Unit*> lst;
    for (Enemy* e : scene->enemies) {
        for (QPoint p : V)
            if (p.x() == e->m_x && p.y() == e->m_y) {
                lst.push_back(e);
                //break;
            }
    }
    if (lst.empty())
        return lst;
    return lst;
}

void Defender76::add_buff(int type)
{
    if (buff_num >= buff_max) {
        qDebug() << "buff is full" << Qt::endl;
        return;
    } else {
        if (type == 3) { //狂暴
            buff_num++;
            attack_rate_max = attack_rate_max / 2;
            attack_rate_cur = attack_rate_max;
        } else if (type == 4) { //冰冻
            buff_num++;
            ice_injury = true;
            qDebug() << "ice_injury is true" << Qt::endl;

        } else if (type == 5) { //群伤
            buff_num++;
            group_injury = true;
            qDebug() << "group_injury is true" << Qt::endl;
        }
    }
}

Shooter::Shooter(QWidget* parent)
    : Agent(parent)
{
    type = 2;
    class_name = "Shooter";

    anim = new QMovie(":/sources/agents/shooter.gif");
    anim_attack = new QMovie(":/sources/agents/shooter_attack.gif");
    anim->start();
    anim_attack->start();
    setMovie(anim);
    show();

    block_max = 1;
    blocking = 0;
    blocked_list.clear();
    attack_range.clear();
    attack_range.push_back(make_pair(0, 0));
    attack_range.push_back(make_pair(0, -1));
    attack_range.push_back(make_pair(0, 1));
    attack_range.push_back(make_pair(1, 0));
    attack_range.push_back(make_pair(1, -1));
    attack_range.push_back(make_pair(1, 1));
    attack_range.push_back(make_pair(2, 0));
    attack_range.push_back(make_pair(2, -1));
    attack_range.push_back(make_pair(2, 1));
    attack_range.push_back(make_pair(3, 0));

    damage = 40;
    attack_rate_max = attack_rate_cur = 80;
    attack_anim_max = attack_anim_cur = 30;

    hp = hp_max = 200;
    bleeding_injury = false;
    //hpText->setText(QString::number(hp));
}

Shooter::~Shooter()
{
    delete anim;
    delete anim_attack;
}

void Shooter::act()
{
    //qDebug() << "Shooter::act()" << Qt::endl;
    if (target != nullptr) {
        setMovie(anim_attack);
        attack_anim_cur--;
        if (attack_anim_cur <= 0) {

            Bullet* bullet = new Bullet(scene);
            scene->animations.push_back(bullet);
            bullet->s = pos();
            bullet->t = target->pos();
            bullet->anmi_time_cur = 0;
            bullet->anmi_time_total = 10;
            bullet->target = target;
            bullet->damage = damage;
            if (bleeding_injury)
                bullet->bleeding = true;

            target->vector_aimed_erase(this);
            target = nullptr;
            attack_rate_cur = attack_rate_max;
        }
    } else {
        setMovie(anim);
        attack_rate_cur--;
        if (attack_rate_cur <= 0) {
            Unit* tar = find_target();
            if (tar != nullptr) {
                target = tar;
                target->aimed.push_back(this);
                attack_anim_cur = attack_anim_max;
            } else
                attack_rate_cur = attack_rate_max;
        }
    }
    //qDebug() << "END Shooter::act()" << Qt::endl;
}

void Shooter::lost_target()
{
    target = nullptr;
    attack_rate_cur = attack_rate_max;
}

Unit* Shooter::find_target()
{
    vector<QPoint> V = get_cur_attacking_squares();
    vector<Unit*> lst;
    for (Enemy* e : scene->enemies) {
        for (QPoint p : V)
            if (p.x() == e->m_x && p.y() == e->m_y) {
                lst.push_back(e);
                break;
            }
    }
    if (lst.empty())
        return nullptr;
    return lst.front();
}

void Shooter::add_buff(int type)
{
    if (buff_num >= buff_max) {
        qDebug() << "buff is full" << Qt::endl;
        return;
    } else {
        if (type == 6) { //放血
            buff_num++;
            bleeding_injury = true;
            qDebug() << "bleeding_injury is true" << Qt::endl;
        }
    }
}
