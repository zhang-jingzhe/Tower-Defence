#include "scene.h"

Scene::Scene(QWidget* parent)
    : BaseClass(parent)
{
    setMouseTracking(true);
    grabKeyboard();
}

Scene::~Scene()
{
    for (auto p : trash)
        delete p;
    for (auto p : agents)
        delete p;
    for (auto p : enemies)
        delete p;
    //    if (timer != nullptr)
    //        delete timer;
}

QPoint Scene::get_square_m() //获取鼠标指向的地图格
{
    if (this->rect.contains(this->m))
        return QPoint((this->m.x() - this->rect.left()) / this->square_size.x(),
            (this->m.y() - this->rect.top()) / this->square_size.y());
    return QPoint(-1, -1);
}

QPoint Scene::get_square(QPoint t) //获取点t所在的地图格
{
    if (this->rect.contains(t))
        return QPoint((t.x() - this->rect.left()) / this->square_size.x(),
            (t.y() - this->rect.top()) / this->square_size.y());
    return QPoint(-1, -1);
}

void Scene::remove_dead()
{
    //qDebug() << "Scene::remove_dead()" << Qt::endl;
    for (Agent* p : agents)
        if (p->exist == false)
            p->dead();
    for (Enemy* p : enemies)
        if (p->exist == false)
            p->dead();
    //qDebug() << "1" << Qt::endl;
    for (vector<Agent*>::iterator it = agents.begin(); it != agents.end();) {
        if ((*it)->exist == false) {
            (*it)->hide();
            it = agents.erase(it);
        } else {
            ++it;
        }
    }
    for (vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end();) {
        if ((*it)->exist == false) {
            (*it)->hide();
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
    for (vector<Animation*>::iterator it = animations.begin(); it != animations.end();) {
        if ((*it)->exist == false) {
            (*it)->hide();
            it = animations.erase(it);
        } else {
            ++it;
        }
    }
    //qDebug() << "END Scene::remove_dead()" << Qt::endl;
}

void Scene::act()
{
    //qDebug() << "Scene::act()" << Qt::endl;
    for (Selector* p : selectors)
        p->act();
    for (Agent* p : agents)
        p->act();
    //qDebug() << "hh" << Qt::endl;
    for (Enemy* p : enemies)
        p->act();
    for (Animation* p : animations)
        p->act();
    //    qDebug() << "END Scene::act()" << Qt::endl;
}

Agent* Scene::get_agent(int id)
{
    Agent* res;
    switch (id) {
    case 1:
        res = new Defender76(this);
        break;
    case 2:
        res = new Shooter(this);
        break;
    default:
        res = nullptr;
        break;
    }
    return res;
}

void Scene::reach_dest(Enemy* e)
{
    e->exist = false;
    failed = true;
    update();
    timer->stop();
    qDebug() << "FAILED!" << Qt::endl;
}

void Scene::all_cleaned()
{
    won = true;
    update();
    timer->stop();
}

void Scene::put_selector_agent()
{
    Agent* agent = nullptr;
    QPoint pos = get_square(cur_selector->pos() - QPoint(-20, 1)); //获取当前选中的块所在的位置（随鼠标移动）

    if (cur_selector->agent_id == 0) { //选中的块为remove
        cur_selector->move(cur_selector->selector_pos_origin); //selector回到原来的位置
        cur_selector->status = 0; //selector状态归零
        cur_selector = nullptr; //当前无选中的selector
        for (Agent* p : agents) {
            if ((p->m_y == pos.y()) && (p->m_x == pos.x())) { //如果pos的位置处有agent：移除
                p->exist = false;
                points += 5;
                break;
            }
        }
        return;
    } else if (cur_selector->agent_id >= 3) { //buff
        for (Agent* p : agents) {
            if ((p->m_y == pos.y()) && (p->m_x == pos.x())) { //如果pos的位置处有agent：加buff
                //TODO:为agent p加buff
                p->add_buff(cur_selector->agent_id);
                break;
            }
        }
        cur_selector->move(cur_selector->selector_pos_origin); //selector回到原来的位置
        cur_selector = nullptr; //当前无选中的selector
        return;
    }
    agent = get_agent(cur_selector->agent_id);
    agent->scene = this;

    agent->setGeometry(rect.x() + square_size.x() * pos.x(),
        rect.y() + square_size.y() * pos.y(), 90, 90);

    agent->m_y = pos.y();
    agent->m_x = pos.x();
    agent->orientation = cur_selector->tmp->orientation;
    agent->scene = this;
    agents.push_back(agent);
    points -= cur_selector->cost;
    cur_selector->cd_cur = cur_selector->cd_max;
    cur_selector->move(cur_selector->selector_pos_origin);
    cur_selector = nullptr;
    highlight.clear(); //清除选中效果
}

void Scene::mouseMoveEvent(QMouseEvent* event)
{
    m = event->pos();
    if (this->cur_selector != nullptr) {
        if (cur_selector->status == 0) { //选择了selector后尚未选择放置的位置
            this->cur_selector->move(m + QPoint(-20, 1));
        } else { //选择了selector放置的位置后选择selector的方向
            QPoint selector_pos = cur_selector->pos(), mouse_pos = event->pos();
            cur_selector->tmp->m_x = get_square(selector_pos).x();
            cur_selector->tmp->m_y = get_square(selector_pos).y();

            int dx = mouse_pos.x() - selector_pos.x(), dy = mouse_pos.y() - selector_pos.y();
            if (dx - dy > 0 && dx + dy > 0)
                cur_selector->tmp->orientation = Agent::EAST;
            if (dx - dy > 0 && dx + dy < 0)
                cur_selector->tmp->orientation = Agent::SOUTH;
            if (dx - dy < 0 && dx + dy > 0)
                cur_selector->tmp->orientation = Agent::NORTH;
            if (dx - dy < 0 && dx + dy < 0)
                cur_selector->tmp->orientation = Agent::WEST;

            highlight = cur_selector->tmp->get_cur_attacking_squares();
            update();
        }
    }
}

void Scene::mousePressEvent(QMouseEvent* event)
{
    QPoint pos = get_square_m();

    qDebug() << "mouse: (" << event->pos().x() << "," << event->pos().y() << ")";
    qDebug() << " (" << pos.x() << "," << pos.y() << ")";

    if (event->button() == Qt::LeftButton) {
        if (pos.x() > -1 && cur_selector != nullptr) { //鼠标点下的位置在地图中且已选中了selector
            if (cur_selector->agent_id == 0) { //remove
                put_selector_agent();
                return;
            }
            if (cur_selector->agent_id >= 3) { //buff
                put_selector_agent();
                return;
            }
            if (cur_selector->status == 0) { //选择放置位置

                bool flg = false;
                for (char c : cur_selector->available) {
                    if (c == mapconfig.map[pos.x()][pos.y()]) //检查当前位置是否可以放置该selector
                        flg = true;
                }
                if (flg == false)
                    return;

                for (Agent* t : agents)
                    if (t->m_y == pos.y() && t->m_x == pos.x() && cur_selector->agent_id > 0) //如果此处已有其他selector：返回
                        return;
                cur_selector->status = 1;
                if (cur_selector->tmp != nullptr)
                    delete cur_selector->tmp;
                cur_selector->tmp = get_agent(cur_selector->agent_id);
                cur_selector->tmp->scene = this;
                cur_selector->tmp->hide();
                //qDebug() << "status change to 1" << endl;
            } else { //选择放置方向
                //qDebug() << "entering put_seletor_agent()" << endl;
                put_selector_agent();
            }
        }
    } else {
        if (cur_selector != nullptr) {
            cur_selector->move(cur_selector->selector_pos_origin);
            cur_selector->status = 0;
            cur_selector = nullptr;
            highlight.clear();
        }
    }
}

void Scene::init()
{
}

StartScene::StartScene(QWidget* parent)
    : Scene(parent)
{
    setGeometry(0, 0, 800, 600);
    setMovie(background);
    background->start();
    show();
    btn_1_1->setStyleSheet("QLabel{border: 5px solid #000000;} QLabel:hover{border:10px solid #EE0000;}");
    btn_1_1->setMovie(level_1_1);
    level_1_1->start();
    btn_1_1->setGeometry(120, 205, 180, 180);
    btn_1_1->show();
    title->setText("Please Select One");
    title->setGeometry(290, 0, 240, 100);
    title->setFont(QFont("Consolas", 18));
    title->show();
}

StartScene::~StartScene()
{
    delete background;
    delete this->level_1_1;
    delete this->btn_1_1;
    delete this->title;
}

void StartScene::mousePressEvent(QMouseEvent* event)
{
    if (QRect(120, 205, 180, 180).contains(event->pos()))
        emit to_1_1();
}
