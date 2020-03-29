#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>
#include <QPainter>
#include <vector>
#include <QPoint>
#include <iostream>
#include <algorithm>
#include <queue>
#include <forward_list>

struct edge {
             int maxY, minY;
             int currentX;
             int xIncr;
             };

struct compare
{
    bool operator()(const edge* a, const edge* b)
    {
        return a->minY > b->minY;
    }
};

namespace Ui {
class DrawingArea;
}

class DrawingArea : public QWidget
{
    // Meta-Object Compiler macro
    Q_OBJECT
    //std vector of QPoints type
    using  qPointsT = std::vector<QPoint>;
    using edge = struct edge;

public:

    explicit DrawingArea(QWidget *parent = nullptr);
    ~DrawingArea();

    void clear();
    void setMode(char mode);
private:
    Ui::DrawingArea *ui;
    qPointsT qPoints;
    char mode;

    void scanlineFill(QPainter *paint);
    std::priority_queue<edge*, std::vector<edge*>,compare> createEdges();
    void mousePressEvent(QMouseEvent *event);
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DRAWINGAREA_H
