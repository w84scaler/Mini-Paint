#pragma once

#include "framework.h"
#include "lab1.h"
#include <vector>

typedef enum {
	ftNone, ftPen , ftRectangle, ftEllipse, ftLine, ftPolyline, ftText, ftPolygon
} FigureType;

class Figure {

public:
	FigureType type;

	virtual void Paint(HDC hdc) {}
	virtual void ClickDown(int x1, int y1, int x2, int y2) {}
	virtual void MouseMove(int x1, int y1) {}
	virtual void ClickUp(int x1, int y1, std::vector<Figure*>& FigureVector) {}
};