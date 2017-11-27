#include <iostream>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "ConvexPolygon.h"

static const wchar_t CLASS_NAME[] = L"ConvexPolygonWnd";

std::vector<Vec2> gPoints[2];

Vec2 gPointInPolyPoint(0, 0);

static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			{
				SelectObject(hdc, GetStockObject(BLACK_PEN));
				Ellipse(hdc, gPointInPolyPoint.mX - 4, gPointInPolyPoint.mY - 4, 
					gPointInPolyPoint.mX + 4, gPointInPolyPoint.mY + 4);
			}

			COLORREF colors[] = 
			{
				RGB(255, 0, 0),
				RGB(0, 255, 0),
			};

			ConvexPolygon polygons[2];

			for(int i = 0; i < 2; i++)
			{
				std::vector<Vec2>& points = gPoints[i];

				HPEN pen = CreatePen(PS_SOLID, 1, colors[i]);
				SelectObject(hdc, pen);

				for(Vec2 pt : points)
				{
					MoveToEx(hdc, pt.mX - 2, pt.mY, nullptr);
					LineTo(hdc, pt.mX + 3, pt.mY);

					MoveToEx(hdc, pt.mX, pt.mY - 2, nullptr);
					LineTo(hdc, pt.mX, pt.mY + 3);
				}

				if(points.size() >= 3)
				{
					ConvexPolygon& convexHull = polygons[i];
					convexHull.setConvexHull(&*points.begin(), (int)points.size());

					Vec2 moveToPt = convexHull.getVertex(convexHull.getNumVertices() - 1);
					MoveToEx(hdc, moveToPt.mX, moveToPt.mY, nullptr);

					for(int i = 0; i < convexHull.getNumVertices(); i++)
					{
						Vec2 pt = convexHull.getVertex(i);
						LineTo(hdc, pt.mX, pt.mY);
					}

					{
						bool pointInPoly = convexHull.pointInPolygon(gPointInPolyPoint);

						RECT textRect;
						SetRect(&textRect, 0, 20 + i * 20, 200, 200);

						wchar_t txt[32];
						wsprintf(txt, L"Point in Poly %d? %s", i, pointInPoly ? L"Yes" : L"No");

						DrawText(hdc, txt, -1, &textRect, DT_LEFT | DT_LEFT);
					}
				}

				SelectObject(hdc, nullptr);
				DeleteObject(pen);
			}

			if(polygons[0].getNumVertices() && polygons[1].getNumVertices())
			{
				HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				SelectObject(hdc, pen);

				RECT rect;
				SetRect(&rect, 0, 0, 100, 100);
				if(polygons[0].overlaps(polygons[1], hdc))
				{	
					DrawText(hdc, L"Overlap", -1, &rect, DT_TOP | DT_LEFT);
				}
				else
				{
					DrawText(hdc, L"No Overlap", -1, &rect, DT_TOP | DT_LEFT);
				}

				SelectObject(hdc, nullptr);
				DeleteObject(pen);
			}

			EndPaint(hwnd, &ps);
		}
		return 0;

	case WM_LBUTTONUP:
		{
			std::vector<Vec2>& points = gPoints[GetAsyncKeyState(VK_CONTROL) < 0 ? 1 : 0];
			POINTS pt = MAKEPOINTS(lParam);
			points.push_back(Vec2(pt.x, pt.y));
			InvalidateRect(hwnd, nullptr, true);
		}
		return 0;

	case WM_RBUTTONUP:
		{
			POINTS pt = MAKEPOINTS(lParam);
			gPointInPolyPoint = Vec2(pt.x, pt.y);
			InvalidateRect(hwnd, nullptr, true);
		}
		return 0;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int main()
{
	gPoints[0] = 
	{
		Vec2(409, 248),
		Vec2(439, 392),
		Vec2(456, 298), 
		Vec2(532, 334),
		Vec2(545, 222),
		Vec2(569, 300),
		Vec2(579, 148),
		Vec2(631, 307),
		Vec2(691, 316),
		Vec2(733, 351),
		Vec2(744, 9),
		Vec2(764, 148),
		Vec2(807, 46)
	};

	gPoints[1] =
	{
		Vec2(635, 413),
		Vec2(635, 486),
		Vec2(695, 391),
		Vec2(705, 338),
		Vec2(760, 434),
		Vec2(795, 360),
		Vec2(826, 512),
		Vec2(831, 291),
		Vec2(884, 327),
		Vec2(949, 481),
	};


	WNDCLASS wc = { };
    wc.lpfnWndProc = wndProc;
    wc.hInstance = GetModuleHandle(0);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Learn to Program Windows",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr,
        nullptr,
        GetModuleHandle(0),
        nullptr);

    ShowWindow(hwnd, SW_SHOWNORMAL);

	MSG msg = { };
	while(GetMessage(&msg, nullptr, 0, 0))
	{
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}

	return 0;
}