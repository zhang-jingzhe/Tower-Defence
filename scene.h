#ifndef SCENE_H
#define SCENE_H

#include "agent.h"
#include "animation.h"
#include "enemy.h"
#include "mapconfig.h"
#include "selector.h"
#include "unit.h"
#include <QPushButton>
#include <QTimer>

class Scene : public BaseClass {
    Q_OBJECT
public:
    explicit Scene(QWidget* parent = 0);
    ~Scene();
    MapConfig mapconfig;
    vector<Enemy*> enemies;
    vector<Agent*> agents;
    vector<Selector*> selectors;
    vector<Animation*> animations;
    vector<BaseClass*> trash;
    int map_x = 9, map_y = 5;
    bool failed = false;
    bool won = false;
    QPoint m; //鼠标所在的位置
    QPoint square_size = QPoint(1, 1); //地图中一格的大小
    QRect rect = QRect(0, 0, 1, 1); //地图
    QRect screen = QRect(170, 0, 900, 600);
    QTimer* timer = nullptr;
    uint64_t cnt_timer;
    int points;
    QLabel* PointsFront = new QLabel(this);
    QLabel* PointsBack = new QLabel(this);
    QMovie* pointsbackground = new QMovie(":/sources/background/PointsBack.png");
    Selector* cur_selector = nullptr; //当前已选中的selector
    vector<QPoint> highlight;

    void remove_dead();
    void act();
    virtual void init();
    QPoint get_square(QPoint t);
    QPoint get_square_m();
    //Enemy* agent_find_target(Agent* cur);
    void put_selector_agent();
    Agent* get_agent(int id);
    void reach_dest(Enemy* e);
    void all_cleaned();

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
signals:
    void toTitle();
    void to_1_1();
    void to_1_2();
};

class StartScene : public Scene {
    Q_OBJECT
public:
    explicit StartScene(QWidget* parent = 0);
    ~StartScene();

private:
    QMovie* background = new QMovie(":/sources/background/Select.jpg");
    QMovie* level_1_1 = new QMovie(":/sources/icon/1-1.png");
    QLabel* btn_1_1 = new QLabel(this);
    QLabel* title = new QLabel(this);
private slots:
    void mousePressEvent(QMouseEvent* event);
};

#endif // SCENE_H
