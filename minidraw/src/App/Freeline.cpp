#include "Freeline.h"

Freeline::Freeline() {
}

Freeline::~Freeline() {
}

void Freeline::Draw(QPainter& painter) {
	if (pointlist.size() <= 1) return;
	painter.setPen(pen);
	
	for (int i = 1; i < pointlist.size(); i++) {
		painter.drawLine(pointlist[i - 1], pointlist[i]);
	}
}


void Freeline::add_end(QPoint s) {
	pointlist.push_back(s);
}

void Freeline::pop_end() {
	pointlist.pop_back();
}

int Freeline::listlen() {
	return pointlist.size();
}

QPoint Freeline::endpoint() {
	assert(pointlist.size() >= 1);
	return pointlist.back();
}

