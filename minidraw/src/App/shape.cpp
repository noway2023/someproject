#include "Shape.h"

Shape::Shape()
{
}

Shape::~Shape()
{
}

void Shape::set_start(QPoint s)
{
	start = s;
}

QPoint Shape::get_start()
{
	return start;
}

QPoint Shape::get_end()
{
	return end;
}

void Shape::set_end(QPoint e)
{
	end = e;
}

void Shape::add_end(QPoint e)
{
	end = e;
}


void Shape::set_width(int width)
{
	pen.setWidth(width);
}

void Shape::set_color(QColor c)
{
	pen.setColor(c);
}
