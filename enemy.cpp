#include "enemy.h"
#include "scene.h"

Enemy::Enemy(QWidget* parent)
    : Unit(parent)
{
}

void Enemy::wounded(int damage)
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

void Enemy::dead()
{
    if (blocker != nullptr) {
        blocker->blocking--;
        for (auto it = blocker->blocked_list.begin(); it != blocker->blocked_list.end(); it++)
            if (*it == this) {
                blocker->blocked_list.erase(it);
                break;
            }
    }
    if (target != nullptr) {
        target->vector_aimed_erase(this);
    }
    for (Unit* x : aimed)
        if (x->target == this)
            x->lost_target();
}

bool Enemy::update_blocked()
{
    if (blocker != nullptr)
        return false;
    for (Agent* p : scene->agents) {
        if (p->m_x == this->m_x && p->m_y == this->m_y && p->blocking < p->block_max) {
            blocker = p;
            blocker->blocking++;
            blocker->blocked_list.push_back(this);
            return true;
        }
    }
    return false;
}

void Enemy::moving()
{
    if (!path.empty() && path.back().x() == m_x && path.back().y() == m_y) {
        int std_x = scene->rect.x() + 10 + scene->square_size.x() * m_x;
        int std_y = scene->rect.y() + 10 + scene->square_size.y() * m_y;
        if (x - speed <= std_x && std_x <= x + speed && y - speed <= std_y && std_y <= y + speed) { //走到path的一个转折点后进行对齐及转向
            path.pop_back();
            if (path.empty()) {
                scene->reach_dest(this);
                this->exist = false;
                //qDebug() << "path empty" << Qt::endl;
                return;
            }
            int tx = path.back().x(), ty = path.back().y();
            if (m_x == tx && m_y < ty)
                orientation = Enemy::SOUTH;
            if (m_x == tx && m_y > ty)
                orientation = Enemy::NORTH;
            if (m_x < tx && m_y == ty)
                orientation = Enemy::EAST;
            if (m_x > tx && m_y == ty)
                orientation = Enemy::WEST;
            x = std_x;
            y = std_y;
            move(x, y);
        }
    }

    switch (orientation) {
    case (Enemy::NORTH):
        y -= speed;
        break;
    case (Enemy::SOUTH):
        y += speed;
        break;
    case (Enemy::WEST):
        x -= speed;
        break;
    case (Enemy::EAST):
        x += speed;
        break;
    }
    move(x, y);

    QPoint t = scene->get_square(QPoint(x, y) + QPoint(45, 45));
    m_x = t.x();
    m_y = t.y();
}

Worm::Worm(QWidget* parent)
    : Enemy(parent)
{
    id = 1;
    class_name = "Worm";

    anim_walk = new QMovie(":/sources/enemies/worm.gif");
    anim_attack = new QMovie(":/sources/enemies/worm.gif");
    setMovie(anim_walk);
    anim_walk->start();
    anim_attack->start();
    show();

    exist = true;
    target = nullptr;
    blocker = nullptr;

    hp_max = hp = 150;
    //hpText->setText(QString::number(hp));
    damage = 20;
    speed = 0.6;
    attack_rate_cur = attack_rate_max = 80;
    frozen = false;
    bleeding = false;
    flash = false;
    aimed.clear();
}

Worm::~Worm()
{
    delete anim_walk;
    delete anim_attack;
}

void Worm::act()
{
    //qDebug() << "Worm::act()"<< endl;
    if (frozen) {
        frozen = false;
        attack_rate_cur = attack_rate_max;
        qDebug() << "frozen in act" << Qt::endl;
        return;
    }
    if (bleeding) {
        this->wounded(1);
        qDebug() << "bleeding in act" << Qt::endl;
    }
    if (blocker == nullptr) {
        setMovie(anim_walk);
        moving();
        if (update_blocked()) {
            target = blocker;
            target->aimed.push_back(this);
            attack_rate_cur = attack_rate_max;
        }
    } else {
        setMovie(anim_attack);
        attack_rate_cur--;
        if (attack_rate_cur <= 0) {
            if (flash) { //越过我方近战塔的阻挡前进
                this->dead();
                blocker = nullptr;
                for (int i = 0; i < 80 / speed; i++)
                    moving();
                return;
            }
            target->wounded(damage);
            attack_rate_cur = attack_rate_max;
        }
    }
    //qDebug() << "END Worm::act()"<< endl;
}

void Worm::lost_target()
{
    target = nullptr;
}

Unit* Worm::find_target()
{
    vector<Agent*> lst;
    lst.clear();
    for (Agent* p : scene->agents) {
        if (p->m_x == this->m_x && p->m_y == this->m_y)
            lst.push_back(p);
    }
    if (lst.empty())
        return nullptr;
    return lst.back();
}
