#pragma once

#include "Elli.h"

class Circle : public Elli {
public:
	Circle();
	~Circle();

	void Draw(QPainter& painter);
};