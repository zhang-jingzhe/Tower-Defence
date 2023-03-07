#include "unit.h"

BaseClass::BaseClass(QWidget* parent)
    : QLabel(parent)
{
    setMouseTracking(true);
    this->scene = (Scene*)this->parent();
    exist = true;
}

Unit::Unit(QWidget* parent)
    : BaseClass(parent)
{
    aimed.clear();
    exist = true;
    target = nullptr;

    setGeometry(0, 0, 90, 90);

    hpBar = new QLabel(this);
    hpBar->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");
    hpBar->setFrameShape(QFrame::Box);
    hpBar->setGeometry(10, 5, 60, 7);
    hpBar->raise();
    hpBarfront = new QLabel(this);
    hpBarfront->setStyleSheet("background-color: rgba(200, 0, 0, 70%);");
    hpBarfront->setGeometry(10, 5, 60, 7);
    hpBarfront->raise();
}

Unit::~Unit()
{
    delete hpBar;
    delete hpBarfront;
}
