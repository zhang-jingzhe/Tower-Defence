#include "selector.h"
#include "scene.h"

Selector::Selector(QWidget* parent)
    : BaseClass(parent)
{
    setCursor(Qt::PointingHandCursor);

    front = new QWidget(this);
    front->setStyleSheet("background-color: rgba(0, 0, 0, 50%);");
    front->show();
    front->raise();

    back = new QWidget(this);
    back->setStyleSheet("background-color: rgba(0, 0, 0, 50%);");
    back->show();
    back->raise();

    frontText = new QLabel(this);
    frontText->setText("50");
    frontText->setGeometry(-10, 5, 40, 20);
    frontText->setAlignment(Qt::AlignHCenter);
    frontText->setFont(QFont("Calibri", 11));
    frontText->show();
    frontText->raise();
    show();
    raise();
}

Selector::~Selector()
{
    delete back;
    delete front;
    delete frontText;
}

void Selector::act()
{
    if (cd_cur > 0)
        cd_cur--;
    change_progress();
    raise();
}

void Selector::set_id(int id)
{
    setGeometry(125, 40 + 50 * id, 50, 50);
}

void Selector::change_progress()
{
    front->setGeometry(0, 6, 50, 50 * this->cd_cur / this->cd_max);
    if (scene->points >= cost)
        back->setGeometry(0, 0, 0, 0);
    else
        back->setGeometry(0, 6, 50, 50);
}

void Selector::mousePressEvent(QMouseEvent* event)
{
    if (scene->cur_selector != nullptr)
        scene->cur_selector->move(scene->cur_selector->selector_pos_origin);

    if (event->button() == Qt::LeftButton) {
        if (scene->points < this->cost || front->height() > 0) {
            scene->cur_selector = nullptr;
            return;
        }
        status = 0;
        selector_pos_origin = pos();
        scene->cur_selector = this;
    } else {
        scene->cur_selector = nullptr;
    }
}

RemoveSelector::RemoveSelector(QWidget* parent)
    : Selector(parent)
{
    setMovie(anim);
    anim->start();
    frontText->hide();
    cost = 0;
    cd_cur = cd_max = 1;
    agent_id = 0;
}

RemoveSelector::~RemoveSelector()
{
    delete anim;
}

Defender76Selector::Defender76Selector(QWidget* parent)
    : Selector(parent)
{
    setMovie(anim);
    anim->start();
    anim->stop();
    frontText->setText("10");
    cost = 10;
    cd_cur = cd_max = 100;
    agent_id = 1;
    available = string("_1");
}

Defender76Selector::~Defender76Selector()
{
    delete anim;
}

ShooterSelector::ShooterSelector(QWidget* parent)
    : Selector(parent)
{
    setMovie(anim);
    anim->start();
    anim->stop();
    frontText->setText("12");
    cost = 12;
    cd_cur = cd_max = 100;
    agent_id = 2;
    available = string("#");
}

ShooterSelector::~ShooterSelector()
{
    delete anim;
}

Buff1Selector::Buff1Selector(QWidget* parent)
    : Selector(parent)
{
    setMovie(anim);
    anim->start();
    anim->stop();
    frontText->setText("");
    cost = 0;
    cd_cur = 0;
    cd_max = 100;
    agent_id = 3;
}

Buff1Selector::~Buff1Selector()
{
    delete anim;
}

Buff2Selector::Buff2Selector(QWidget* parent)
    : Selector(parent)
{
    setMovie(anim);
    anim->start();
    anim->stop();
    frontText->setText("");
    cost = 0;
    cd_cur = 0;
    cd_max = 100;
    agent_id = 4;
}

Buff2Selector::~Buff2Selector()
{
    delete anim;
}

Buff3Selector::Buff3Selector(QWidget* parent)
    : Selector(parent)
{
    setMovie(anim);
    anim->start();
    anim->stop();
    frontText->setText("");
    cost = 0;
    cd_cur = 0;
    cd_max = 100;
    agent_id = 5;
}

Buff3Selector::~Buff3Selector()
{
    delete anim;
}

Buff4Selector::Buff4Selector(QWidget* parent)
    : Selector(parent)
{
    setMovie(anim);
    anim->start();
    anim->stop();
    frontText->setText("");
    cost = 0;
    cd_cur = 0;
    cd_max = 100;
    agent_id = 6;
}

Buff4Selector::~Buff4Selector()
{
    delete anim;
}
