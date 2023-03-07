#include "animation.h"
#include "scene.h"

Animation::Animation(QWidget* parent)
    : BaseClass(parent)
{
}

Bullet::Bullet(QWidget* parent)
    : Animation(parent)
{
    exist = true;
    bleeding = false;
    setGeometry(0, 0, 20, 20);
    setMovie(anmi);
    anmi->start();
    show();
    raise();
}

Bullet::~Bullet()
{
    delete anmi;
}
void Bullet::act()
{
    if (anmi_time_cur >= anmi_time_total) {
        target->wounded(damage);
        if (bleeding) {
            ((Enemy*)target)->bleeding = true;
        }
        exist = false;
        return;
    }
    anmi_time_cur++;
    int cur_x = s.x() + 10 + (double)anmi_time_cur / anmi_time_total * (target->x() - s.x());
    int cur_y = s.y() + 20 + (double)anmi_time_cur / anmi_time_total * (target->y() - s.y());
    move(cur_x, cur_y);
}

HurtAnim::HurtAnim(QWidget* parent)
    : Animation(parent)
{
    exist = true;
    setGeometry(0, 0, 40, 40);

    setFont(QFont("Calibri", 15));
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::red);
    setPalette(pa);

    damage = 0;
    anmi_time_total = 15;
    anmi_time_cur = 0;
    show();
    raise();
}

HurtAnim::~HurtAnim()
{
}

void HurtAnim::act()
{
    if (anmi_time_cur >= anmi_time_total) {
        exist = false;
        return;
    }
    bool target_exist = false;
    for (Enemy* ene : scene->enemies) {
        if (ene == this->target) {
            target_exist = true;
            break;
        }
    }
    for (Agent* age : scene->agents) {
        if (age == this->target) {
            target_exist = true;
            break;
        }
    }
    setText(QString("-") + QString::number(damage));
    if (target_exist)
        move(target->x() + 15 + 10.0 * anmi_time_cur / anmi_time_total,
            target->y() + 20 - 10.0 * anmi_time_cur / anmi_time_total);
    anmi_time_cur++;
}
