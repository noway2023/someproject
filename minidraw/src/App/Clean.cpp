#include "Clean.h"

Clean::Clean(QWidget* w) :widget(w)
{
}

Clean::~Clean()
{
}

void Clean::Draw(QPainter& painter)
{
	painter.fillRect(widget->rect(), widget->palette().color(QPalette::Background));
}