#ifndef WIDGET_H
#define WIDGET_H

#include "scene.h"
#include <QPainter>
#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();
    //void paintEvent(QPaintEvent*);
    Scene* current_scene;

private:
    Ui::Widget* ui;
public slots:
    void start_1_1();
    void back();
};
#endif // WIDGET_H
