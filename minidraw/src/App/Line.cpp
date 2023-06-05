#include "Line.h"

Line::Line()
{
}

Line::~Line()
{
}

void Line::Draw(QPainter& painter)
{
	painter.setPen(pen);
	painter.drawLine(start, end);
}
