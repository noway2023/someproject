#pragma once

#include <ui_viewwidget.h>

#include "Shape.h"
#include "Line.h"
#include "Rect.h"
#include "Poly.h"
#include "Elli.h"
#include "Circle.h" 
#include "Clean.h"
#include "Freeline.h"
#include "Curve.h"

#include <qevent.h>
#include <qpainter.h>
#include <QWidget>

#include <vector>

class ViewWidget : public QWidget
{
	Q_OBJECT

public:
	ViewWidget(QWidget* parent = 0);
	~ViewWidget();

private:
	Ui::ViewWidget ui;

private:
	bool draw_status_;
	bool ispoly_;
	bool dclick;
	bool ismedium;
	bool isthird;
	bool isfree;
	bool iscurve;
	QPoint start_point_;
	QPoint end_point_;
	int penwidth;
	QColor pencolor;
	Shape::Type type_;
	Shape* shape_;
	Poly* shapepoly;
	std::vector<Shape*> shape_list_;
	std::vector<Shape*> back_list_;


public:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);
public:
	void paintEvent(QPaintEvent*);
signals:
public slots:
	void setLine();
	void setRect();
	void setEllipse();
	void setCircle();
	void setPolygon();
	void setFreeline();
	void setCurve();
	void setClean();
	void setUndo();
	void setBack();
	void setPenwidthshort();
	void setPenwidthmedium(); 
	void setPenwidthlong();
	void receiveColor(QColor color);
};


