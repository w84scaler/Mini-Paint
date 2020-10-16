// lab1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab1.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

std::vector<Figure*> FigureVector;
std::vector<Figure*> NextFigureVector;
Figure* CurrentFigure;
FigureType CurrentFigureType;
BOOL StartedDrawing;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB1));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   InitMenu(hWnd);
   SetTimer(hWnd, 1, 16, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

VOID InitMenu(HWND hWnd)
{
    HMENU menuv = GetMenu(hWnd);

    HMENU menu = CreatePopupMenu();  
    AppendMenuW(menu, MF_STRING, MENU_PEN, L"&Pen");
    AppendMenuW(menu, MF_STRING, MENU_LINE, L"&Line");
    AppendMenuW(menu, MF_STRING, MENU_POLYLINE, L"&Polyline");
    AppendMenuW(menu, MF_STRING, MENU_RECTANGLE, L"&Rectangle");
    AppendMenuW(menu, MF_STRING, MENU_POLYGON, L"&Polygon");
    AppendMenuW(menu, MF_STRING, MENU_TEXT, L"&Text");
    AppendMenuW(menu, MF_STRING, MENU_ELLIPSE, L"&Ellipse");

    AppendMenuW(menuv, MF_POPUP, (UINT_PTR)menu, L"&Figure");
    AppendMenuW(menuv, MF_STRING, MENU_BACK, L"&Back");
    AppendMenuW(menuv, MF_STRING, MENU_NEXT, L"&Next");
    SetMenu(hWnd, menuv);
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case MENU_BACK:
                if (!FigureVector.empty() && !StartedDrawing) {
                    NextFigureVector.push_back(FigureVector.back());
                    FigureVector.pop_back();
                }
                break;
            case MENU_NEXT:
                if (!NextFigureVector.empty() && !StartedDrawing) {
                    FigureVector.push_back(NextFigureVector.back());
                    NextFigureVector.pop_back();
                }
                break;
            case MENU_PEN:
                CurrentFigureType = ftPen;
                break;
            case MENU_ELLIPSE:
                CurrentFigureType = ftEllipse;
                break;
            case MENU_RECTANGLE:
                CurrentFigureType = ftRectangle;
                break;
            case MENU_LINE:
                CurrentFigureType = ftLine;
                break;
            case MENU_POLYLINE:
                CurrentFigureType = ftPolyline;
                break;
            case MENU_TEXT:
                CurrentFigureType = ftText;
                break;
            case MENU_POLYGON:
                CurrentFigureType = ftPolygon;
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_KEYDOWN:
    {
        InputText(hWnd, message, wParam, lParam);
        InvalidateRect(hWnd, NULL, true);
    }
    break;
    case WM_LBUTTONDOWN:
    {
        onClickDown(hWnd, message, wParam, lParam);
        InvalidateRect(hWnd, NULL, true);
    }
    break;
    case WM_RBUTTONDOWN:
    {
        onRClickDown(hWnd, message, wParam, lParam);
        InvalidateRect(hWnd, NULL, true);
    }
    break;
    case WM_LBUTTONUP:
    {
        onClickUp(hWnd, message, wParam, lParam);
        InvalidateRect(hWnd, NULL, true);
    }
    break;
    case WM_MOUSEMOVE:
    {
        onMouseMove(hWnd, message, wParam, lParam);
        InvalidateRect(hWnd, NULL, true);
    }
    break;
    case WM_ERASEBKGND:
    {
        InvalidateRect(hWnd, NULL, FALSE);
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...

            HDC memDC = CreateCompatibleDC(hdc);

            RECT lpRect;
            GetClientRect(hWnd, &lpRect);
            HBITMAP hBM = CreateCompatibleBitmap(hdc, lpRect.right, lpRect.bottom);
            HANDLE hOld = SelectObject(memDC, hBM);
            RECT r;
            SetRect(&r, 0, 0, lpRect.right, lpRect.bottom);
            FillRect(memDC, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));

            HFONT hFont = CreateFont(24, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
            SelectObject(memDC, hFont);
            SetTextColor(memDC, RGB(0, 0, 0));
            TextOut(memDC, 10, 10, L"sorry 4 this bruh moment", wcslen(L"sorry 4 this bruh moment"));

            for (int i = 0; i < FigureVector.size(); i++) {
                onPaint(memDC, FigureVector[i]);
            }
            if (StartedDrawing) {
                onPaint(memDC, CurrentFigure);
            }

            BitBlt(hdc, 0, 0, lpRect.right, lpRect.bottom, memDC, 0, 0, SRCCOPY);
            SelectObject(memDC, hOld);
            DeleteObject(hBM);
            DeleteDC(memDC);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_TIMER:
        InvalidateRect(hWnd, NULL, true);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void CreateInstance() {
    switch (CurrentFigureType) {
    case ftPen: CurrentFigure = new PenFigure(); break;
    case ftRectangle: CurrentFigure = new RectangleFigure(); break;
    case ftEllipse: CurrentFigure = new EllipseFigure(); break;
    case ftLine: CurrentFigure = new LineFigure(); break;
    case ftPolyline: CurrentFigure = new PolylineFigure(); break;
    case ftText: CurrentFigure = new TextFigure(); break;
    case ftPolygon: CurrentFigure = new PolygonFigure(); break;
    }
    if (CurrentFigureType != ftNone) CurrentFigure->type = CurrentFigureType;
}

void InputText(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    if (StartedDrawing && CurrentFigureType == ftText) {
        dynamic_cast<TextFigure*>(CurrentFigure)->ModifyText(wParam);
    }
}

void onRClickDown(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);

    switch (CurrentFigure->type) {
    case ftPolyline: dynamic_cast<PolylineFigure*>(CurrentFigure)->RClickDown(xPos, yPos); break;
    case ftText: { dynamic_cast<TextFigure*>(CurrentFigure)->FinishText(FigureVector); StartedDrawing = false; } break;
    }
}

void onClickDown(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);

    CreateInstance();
    if (CurrentFigureType != ftNone) {
        CurrentFigure->ClickDown(xPos, yPos, xPos, yPos);
        StartedDrawing = true;
    }
}

void onMouseMove(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (wParam == MK_LBUTTON) {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        if (CurrentFigureType != ftText && CurrentFigureType != ftNone)
            CurrentFigure->MouseMove(xPos, yPos);
    }
}

void onClickUp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);

    if (CurrentFigureType != ftText && CurrentFigureType != ftNone) {
        CurrentFigure->ClickUp(xPos, yPos, FigureVector);
        StartedDrawing = false;
    }
}

void onPaint(HDC hdc, Figure* figure) {
    figure->Paint(hdc);
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
