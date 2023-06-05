#include "Rect.h"

Rect::Rect()
{
}

Rect::~Rect()
{
}

void Rect::Draw(QPainter& painter)
{
	painter.setPen(pen);
	painter.drawRect(start.x(), start.y(),
		end.x() - start.x(), end.y() - start.y());
}
