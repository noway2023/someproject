#include "Circle.h"

Circle::Circle() {
}

Circle::~Circle() {
}

void Circle::Draw(QPainter& painter) {
	painter.setPen(pen);
	double dx = end.x() - start.x();
	double dy = end.y() - start.y();
	if (dx < dy) dy = dx;
	else dx = dy;
	painter.drawEllipse(start.x(), start.y(),
		dx, dy);
}