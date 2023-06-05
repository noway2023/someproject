#include "Shape.h"

class Curve : public Shape {
public:
	Curve();
	~Curve();
	void add_end(QPoint s);
	void Draw(QPainter& painter);

private:
	QVector<QPoint> pointlist;
	QVector<QPoint> controlpoints;
};