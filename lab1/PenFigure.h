#pragma once

#include "Figure.h"
#include "framework.h"

class PenFigure : public Figure {

public:
	std::vector<POINT> points;

	void Paint(HDC hdc) override {
		MoveToEx(hdc, points[0].x, points[0].y, NULL);
		for (int i = 1; i < points.size(); i++) {
			LineTo(hdc, points[i].x, points[i].y);
		}
	}

	void ClickDown(int x1, int y1, int x2, int y2) override {
		POINT point;
		point.x = x1;
		point.y = y1;
		points.push_back(point);
	}

	void MouseMove(int x1, int y1) override {
		POINT point;
		point.x = x1;
		point.y = y1;
		points.push_back(point);
	}

	void ClickUp(int x1, int y1, std::vector<Figure*>& FigureVector) override {
		POINT point;
		point.x = x1;
		point.y = y1;
		points.push_back(point);
		if (point.x == points[0].x && point.y == points[0].y) return;
		FigureVector.push_back(this);
	}
};