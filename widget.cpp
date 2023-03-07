#include "widget.h"
#include "level_1_1.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setMouseTracking(true); //track mouse when mouse isn`t pressed
    this->setFixedSize(800, 600);
    current_scene = new StartScene(this);
    connect(current_scene, SIGNAL(to_1_1()), this, SLOT(start_1_1()));
}

Widget::~Widget()
{
    delete ui;
}

//void Widget::paintEvent(QPaintEvent*)
//{
//    QPainter painter(this);
//    painter.drawLine(QPoint(0, 0), QPoint(100, 100));
//}

void Widget::start_1_1()
{
    setFixedSize(1000, 750);
    delete current_scene;
    current_scene = new Level_1_1(this);
    connect(current_scene, SIGNAL(toTitle()), this, SLOT(back()));
}

void Widget::back()
{
    setFixedSize(800, 600);
    delete current_scene;
    current_scene = new StartScene(this);
    connect(current_scene, SIGNAL(to_1_1()), this, SLOT(start_1_1()));
}
