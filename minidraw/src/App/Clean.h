#pragma once

#include "Shape.h"
#include <QWidget>
class Clean : public Shape {
public:
	Clean(QWidget* w);
	~Clean();

	void Draw(QPainter& painter);
private:
	QWidget* widget;
};
