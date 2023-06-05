#include "Poly.h"


Poly::Poly() {
}

Poly::Poly(Poly& other) {
	for (int i = 0; i < pointlist.size(); i++) {
		pointlist.push_back(other.pointlist[i]);
	}
}

Poly::~Poly() {

}

void Poly::Draw(QPainter& painter) {
	if (pointlist.size() <= 1) return;
	painter.setPen(pen);


	//drawPolygon
	//painter.drawPolygon(QPolygon(pointlist));

	//drawline
	for (int i = 1; i < pointlist.size(); i++) {
		painter.drawLine(pointlist[i - 1], pointlist[i]);
	}
}


void Poly::add_end(QPoint s) {
	pointlist.push_back(s);
}

void Poly::pop_end() {
	pointlist.pop_back();
}

int Poly::listlen() {
	return pointlist.size();
}

QPoint Poly::endpoint() {
	assert(pointlist.size() >= 1);
	return pointlist.back();
}


