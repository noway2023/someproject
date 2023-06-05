#pragma once

#include <QtGui>

class Shape
{
public:
	Shape();
	virtual ~Shape();
	virtual void Draw(QPainter& paint) = 0;
	void set_start(QPoint s);
	QPoint get_start();
	QPoint get_end();
	void set_end(QPoint e);
	virtual void add_end(QPoint e);
	void set_width(int width);
	void set_color(QColor c);


public:
	enum Type
	{
		kDefault = 0,
		kClean = 1,
		kLine = 2,
		kRect = 3,
		kEllipse = 4,
		kPolygon = 5,
		kCircle = 6,
		kCurve = 7,
		kFreeline = 8,
	};

protected:
	QPoint start;
	QPoint end;
	QPen pen;
};

