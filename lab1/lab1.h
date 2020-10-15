#pragma once

#include "resource.h"

#include "Figure.h"
#include "PenFigure.h"
#include "EllipseFigure.h"
#include "LineFigure.h"
#include "PolylineFigure.h"
#include "PolygoneFigure.h"
#include "RectangleFigure.h"
#include "TextFigure.h"

#include <windowsx.h>
#include <vector>
#include <string>

#define MENU_PEN 3227
#define MENU_LINE 3228
#define MENU_POLYLINE 3229
#define MENU_RECTANGLE 3230
#define MENU_POLYGON 3231
#define MENU_TEXT 3232
#define MENU_ELLIPSE 3233

VOID InitMenu(HWND hWnd);

void InputText(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void CreateInstance();

void onClickDown(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void onRClickDown(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void onMouseMove(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void onClickUp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void onPaint(HDC hdc, Figure* figure);