#include "level_1_1.h"
#include <QPainter>
#include <cstdlib>
#include <ctime>

Level_1_1::Level_1_1(QWidget* parent)
    : Scene(parent)
{
    QString target_path = QCoreApplication::applicationDirPath();
    target_path += "/map_configs/1_1.txt";
    qDebug() << target_path << Qt::endl;
    QByteArray ba = target_path.toLatin1();
    mapconfig.load_config_file(ba.data()); //解析地图

    setGeometry(-120, 0, 3000, 3000);
    square_size = QPoint(81, 100);
    map_x = mapconfig.map_x;
    map_y = mapconfig.map_y;
    rect = QRect(250, 85, 250 + map_x * square_size.x(), 85 + map_y * square_size.y());

    //qDebug() << "MAP Loaded" << map_x << map_y << endl;

    show();
    failed = false;
    won = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->start(20);
    init();
}

void Level_1_1::paintEvent(QPaintEvent* event)
{

    QLabel::paintEvent(event);
    QPainter* painter = new QPainter(this);

    painter->setPen(QPen(Qt::black, 2));
    for (int i = 0; i <= map_y; i++) { //画横线
        painter->drawLine(rect.left(),
            rect.top() + i * square_size.y(),
            rect.left() + map_x * square_size.x(),
            rect.top() + i * square_size.y());
    }
    for (int i = 0; i <= map_x; i++) { //画竖线
        painter->drawLine(rect.left() + i * square_size.x(),
            rect.top(),
            rect.left() + i * square_size.x(),
            rect.top() + map_y * square_size.y());
    }

    for (int i = 0; i < map_x; i++)
        for (int j = 0; j < map_y; j++) {
            if (painter != nullptr) {
                delete painter;
                painter = new QPainter(this);
            }
            switch (mapconfig.map[i][j]) {
            case 'S': //起点
                painter->setPen(QPen(Qt::red, 5));
                painter->drawRect(rect.left() + i * square_size.x(), rect.top() + j * square_size.y(), square_size.x(), square_size.y());
                break;
            case 'T': //终点
                painter->setPen(QPen(Qt::blue, 5));
                painter->drawRect(rect.left() + i * square_size.x(), rect.top() + j * square_size.y(), square_size.x(), square_size.y());
                break;
            case '_': //路径
                break;
            case '#': //高台
                painter->setPen(QPen(Qt::black, 5));
                painter->setBrush(QBrush(Qt::darkGray, Qt::SolidPattern));
                painter->drawRect(rect.left() + i * square_size.x(), rect.top() + j * square_size.y(), square_size.x(), square_size.y());
                break;
            default:
                break;
            }
        }

    painter->setPen(QPen(Qt::blue, 8));
    for (QPoint p : highlight) { //绘制高光效果
        painter->drawRect(rect.left() + p.x() * square_size.x(), rect.top() + p.y() * square_size.y(), square_size.x(), square_size.y());
    }

    if (failed) {
        QFont font = painter->font();
        font.setPixelSize(150); //改变字体大小
        painter->setFont(font);
        painter->setPen(QPen(Qt::red, 10));
        painter->drawText(QPointF(400, 400), "FAILED");
    }

    if (won) {
        QFont font = painter->font();
        font.setPixelSize(150); //改变字体大小
        painter->setFont(font);
        painter->setPen(QPen(Qt::red, 10));
        painter->drawText(QPointF(400, 400), "WON");
    }

    delete painter;
}

Level_1_1::~Level_1_1()
{
    //delete background;
    if (timer != nullptr)
        delete timer;
}

void Level_1_1::init()
{
    PointsBack->setGeometry(120, 0, 123, 34);
    pointsbackground->start();
    PointsBack->show();
    PointsBack->setMovie(pointsbackground);
    PointsFront->setGeometry(170, 10, 65, 30);
    PointsFront->setFont(QFont("Calibri", 16));
    PointsFront->setText("50");
    PointsFront->setAlignment(Qt::AlignHCenter);
    PointsFront->show();
    PointsFront->raise();
    exit->setGeometry(950, 0, 60, 60);
    exit->setFlat(true);
    exit->setIcon(QIcon(":/sources/icon/Leave.png"));
    exit->setIconSize(QSize(60, 60));
    exit->setStyleSheet("background: transparent");
    exit->setCursor(Qt::PointingHandCursor);
    connect(exit, SIGNAL(clicked()), this, SLOT(leave()));
    exit->show();
    exit->raise();

    points = 35;
    cnt_timer = 0;

    Selector* selector0 = new RemoveSelector(this);
    selector0->set_id(0);
    selectors.push_back(selector0);

    Selector* selector1 = new Defender76Selector(this);
    selector1->set_id(1);
    selectors.push_back(selector1);

    Selector* selector2 = new ShooterSelector(this);
    selector2->set_id(2);
    selectors.push_back(selector2);

    Selector* selector3 = new Buff1Selector(this);
    selector3->set_id(3);
    selectors.push_back(selector3);

    Selector* selector4 = new Buff2Selector(this);
    selector4->set_id(4);
    selectors.push_back(selector4);

    Selector* selector5 = new Buff3Selector(this);
    selector5->set_id(5);
    selectors.push_back(selector5);

    Selector* selector6 = new Buff4Selector(this);
    selector6->set_id(6);
    selectors.push_back(selector6);
    //    delete selector0;
    //    delete selector1;
    //    delete selector2;
}

void Level_1_1::onTimer()
{
    remove_dead();

    if (enemies.empty() && mapconfig.generate.empty() && !failed)
        all_cleaned();

    act();
    PointsFront->setText(QString::number(points));

    exit->raise();
    update();

    static int points_recovery_cur = 0, points_recovery_max = 70;

    points_recovery_cur++;
    if (points_recovery_cur >= points_recovery_max) {
        points++;
        points_recovery_cur = 0;
    }

    static int _rk = 0;

    cnt_timer++;

    while (!mapconfig.generate.empty() && (uint64_t)50 * mapconfig.generate.back().first == cnt_timer) {
        QString enemy_name = mapconfig.generate.back().second.first;
        int path_id = mapconfig.generate.back().second.second;

        qDebug() << "generate enemy" << Qt::endl;
        qDebug() << mapconfig.generate.back().first << Qt::endl;
        mapconfig.generate.pop_back();
        Enemy* enemy = nullptr;

        if (enemy_name == "worm") {
            enemy = new Worm(this);
            enemy->rk = ++_rk;
            enemy->scene = this;
            enemy->path = mapconfig.paths[path_id];
            enemy->m_x = enemy->path.back().x();
            enemy->m_y = enemy->path.back().y();
            enemy->x = rect.x() + 10 + square_size.x() * enemy->m_x;
            enemy->y = rect.y() + 10 + square_size.y() * enemy->m_y;
            enemy->move(enemy->x, enemy->y);
            srand((int)time(0)); // 产生随机种子
            if (rand() % 100 > 80) { //speed
                enemy->speed = enemy->speed * 2;
                Worm* worm = (Worm*)enemy;
                delete worm->anim_walk;
                delete worm->anim_attack;
                worm->anim_walk = new QMovie(":/sources/enemies/speedworm.gif");
                worm->anim_attack = new QMovie(":/sources/enemies/speedworm.gif");
                worm->setMovie(worm->anim_walk);
                worm->anim_walk->start();
                worm->anim_attack->start();
                worm->show();
            } else if (rand() % 100 > 60) { //flash
                enemy->flash = true;
                Worm* worm = (Worm*)enemy;
                delete worm->anim_walk;
                delete worm->anim_attack;
                worm->anim_walk = new QMovie(":/sources/enemies/flashworm.gif");
                worm->anim_attack = new QMovie(":/sources/enemies/flashworm.gif");
                worm->setMovie(worm->anim_walk);
                worm->anim_walk->start();
                worm->anim_attack->start();
                worm->show();

            } else if (rand() % 100 > 50) { //speed+flash
                enemy->speed = enemy->speed * 2;
                enemy->flash = true;
                Worm* worm = (Worm*)enemy;
                delete worm->anim_walk;
                delete worm->anim_attack;
                worm->anim_walk = new QMovie(":/sources/enemies/doubleworm.gif");
                worm->anim_attack = new QMovie(":/sources/enemies/doubleworm.gif");
                worm->setMovie(worm->anim_walk);
                worm->anim_walk->start();
                worm->anim_attack->start();
                worm->show();
            }
            enemies.push_back(enemy);
            //            delete enemy;
        }
    }
}

void Level_1_1::leave()
{
    emit toTitle();
}
