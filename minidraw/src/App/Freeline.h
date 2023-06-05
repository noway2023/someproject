#include "Shape.h"

class Freeline : public Shape {
public:
	Freeline();
	~Freeline();

	void Draw(QPainter& painter);
	void pop_end();
	void add_end(QPoint s);
	int listlen();
	QPoint endpoint();

private:
	QVector<QPoint> pointlist;
};