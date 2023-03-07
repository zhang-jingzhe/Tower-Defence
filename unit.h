#ifndef UNIT_H
#define UNIT_H

#include <QLabel>
#include <QMouseEvent>
#include <QMovie>
using namespace std;

class Scene;

class BaseClass : public QLabel {
    Q_OBJECT
public:
    explicit BaseClass(QWidget* parent = 0);
    virtual ~BaseClass() {};
    bool exist = true;
    Scene* scene;
    void virtual act() {};
};

class Unit : public BaseClass {
    Q_OBJECT
public:
    explicit Unit(QWidget* parent = 0);
    ~Unit();
    bool fly = false;
    string class_name;
    int m_x, m_y;
    int hp_max, hp;
    QLabel *hpBar, *hpBarfront, square_tmp;
    //Scene* scene;
    vector<Unit*> aimed; //以自身为目标的对象列表
    Unit* target; //目标
    void vector_aimed_erase(Unit* p)
    {
        for (auto it = aimed.begin(); it != aimed.end(); it++)
            if (*it == p) {
                aimed.erase(it);
                break;
            }
    }
    void virtual act() {};
    virtual Unit* find_target() { return nullptr; }; //寻找目标
    virtual void lost_target() {}; //放弃目标
    virtual void dead() {}; //死亡
    virtual void wounded(int damage) {}; //受伤
};

#endif // UNIT_H
