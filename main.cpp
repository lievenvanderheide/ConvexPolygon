#include <iostream>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Geom/ConvexPolygon.h"

static const wchar_t CLASS_NAME[] = L"ConvexPolygonWnd";

enum Preset
{
	PRESET_EMPTY,
	PRESET_OVERLAP,
	PRESET_NO_OVERLAP,
};

std::vector<Vec2> gPoints[2];

static void openOverlapPreset()
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
}

static void openNoOverlapPreset()
{
	gPoints[0] =
	{	
		Vec2(454, 514),
		Vec2(462, 396),
		Vec2(574, 441),
		Vec2(620, 473),
		Vec2(620, 567),
		Vec2(660, 420),
		Vec2(717, 512),
		Vec2(761, 422),
		Vec2(805, 524),
		Vec2(836, 329),
		Vec2(867, 607),
	};


	gPoints[1] =
	{
		Vec2(274, 551),
		Vec2(321, 414),
		Vec2(351, 400),
		Vec2(389, 269),
		Vec2(418, 293),
		Vec2(419, 346),
		Vec2(439, 308),
		Vec2(447, 265),
		Vec2(485, 314),
		Vec2(505, 254),
		Vec2(509, 201),
		Vec2(631, 90),
	};
}

static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

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
				if(polygons[0].overlaps(polygons[1]))
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
		break;

	case WM_LBUTTONUP:
		{
			POINTS pt = MAKEPOINTS(lParam);
			gPoints[0].push_back(Vec2(pt.x, pt.y));
			InvalidateRect(hwnd, nullptr, true);
		}
		break;

	case WM_RBUTTONUP:
		{
			POINTS pt = MAKEPOINTS(lParam);
			gPoints[1].push_back(Vec2(pt.x, pt.y));
			InvalidateRect(hwnd, nullptr, true);
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case PRESET_EMPTY:
			gPoints[0].clear();
			gPoints[1].clear();
			break;

		case PRESET_OVERLAP:
			openOverlapPreset();
			break;

		case PRESET_NO_OVERLAP:
			openNoOverlapPreset();
			break;
		}

		InvalidateRect(hwnd, nullptr, true);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int main()
{

	WNDCLASS wc = { };
    wc.lpfnWndProc = wndProc;
    wc.hInstance = GetModuleHandle(0);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

	HMENU presetsMenu = CreateMenu();
	AppendMenu(presetsMenu, MF_STRING, PRESET_EMPTY, L"Empty");
	AppendMenu(presetsMenu, MF_STRING, PRESET_OVERLAP, L"Overlap");
	AppendMenu(presetsMenu, MF_STRING, PRESET_NO_OVERLAP, L"No Overlap");

    HMENU menu = CreateMenu();
	AppendMenu(menu, MF_STRING | MF_POPUP, (UINT_PTR)presetsMenu, L"Open");

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Geometry Workbench",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr,
        menu,
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