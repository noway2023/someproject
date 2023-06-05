#pragma once

#include "Shape.h"

class Poly : public Shape {
public:
	Poly();
	Poly(Poly&);
	~Poly();

	void Draw(QPainter& painter);
	void add_end(QPoint s);
	void pop_end();
	int listlen();
	QPoint endpoint();


private:
	QVector<QPoint> pointlist;
};


