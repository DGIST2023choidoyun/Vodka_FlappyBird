#include "framework.h"
#include "Resource.h"


#include <vector>
#include <cmath>
#include "Vector.h"

#include "Collider.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "Object.h"

#include "WindowClass.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return WindowClass::Instance()->AppProc(hWnd, message, wParam, lParam);
}

WindowClass::WindowClass()
{
	_hInstance = NULL;
	_hWnd = NULL;

	memset(_title, 0, MAX_LOADSTRING);
	memset(_windowClass, 0, MAX_LOADSTRING);

	_scrSize[0] = _scrSize[1] = 0;
	_scrHalfSize[0] = _scrHalfSize[1] = 0;

	_objectAllInitialized = false;

	
	GdiplusStartupInput gdiplusStartInput;
	GdiplusStartup(&_GDIplusToken, &gdiplusStartInput, NULL);
}

WindowClass::~WindowClass()
{
	Shutdown();
}

bool WindowClass::Initialize(HINSTANCE hInstance, int* scrSize)
{
	_hInstance = hInstance;


	ClassLoadString(hInstance);

	MyResisterClass(hInstance);

	_scrSize[0] = scrSize[0] + 16;
	_scrSize[1] = scrSize[1] + 59;
	_scrHalfSize[0] = _scrSize[0] / 2;
	_scrHalfSize[1] = _scrSize[1] / 2;

	_scrArea.left = 0;
	_scrArea.right = _scrSize[0];
	_scrArea.top = 0;
	_scrArea.bottom = _scrSize[1];

	/*imgList.push_back(Gdiplus::Bitmap::FromResource(hInstance, MAKEINTRESOURCE(FBBackground)));
	imgList.push_back(Gdiplus::Bitmap::FromResource(hInstance, MAKEINTRESOURCE(Ground)));
	imgList.push_back(Gdiplus::Bitmap::FromResource(hInstance, MAKEINTRESOURCE(Title)));
	imgList.push_back(Gdiplus::Bitmap::FromResource(hInstance, MAKEINTRESOURCE(FBBackground)));
	imgList.push_back(Gdiplus::Bitmap::FromResource(hInstance, MAKEINTRESOURCE(Flappy)));
	imgList.push_back(Gdiplus::Bitmap::FromResource(hInstance, MAKEINTRESOURCE(PipeUp)));
	imgList.push_back(Gdiplus::Bitmap::FromResource(hInstance, MAKEINTRESOURCE(PipeDown)));*/

	imgList.push_back(Image::FromFile(L"FBbackground.png"));
	imgList.push_back(Image::FromFile(L"Ground.png"));
	imgList.push_back(Image::FromFile(L"Title2.png"));
	imgList.push_back(Image::FromFile(L"ScoreBoard.png"));
	imgList.push_back(Image::FromFile(L"flappy2.png"));
	imgList.push_back(Image::FromFile(L"pipeUp.png"));
	imgList.push_back(Image::FromFile(L"pipeDown.png"));
	imgList.push_back(Image::FromFile(L"SilverMD.png"));
	imgList.push_back(Image::FromFile(L"GoldMD.png"));
	imgList.push_back(Image::FromFile(L"GameOver.png"));
	imgList.push_back(Image::FromFile(L"MenuBtn.png"));
	imgList.push_back(Image::FromFile(L"OKBtn.png"));


	if (!CreateSystemWindow(_scrSize[0], _scrSize[1]))
	{
		MessageBox(NULL, L"Failed creating window.", L"Error", MB_OK);
		return false;
	}


	flappy = new Object;
	flappy->rigidbody = new RigidBody;
	flappy->rigidbody->pos = VECTOR2<double>{ _scrSize[0] * 0.3, _scrSize[1] * 0.5 - 43};
	flappy->rigidbody->SetCOM(54, 43);

	flappy->rigidbody->type = Collider::TYPE::BOX;
	flappy->rigidbody->origin = VECTOR2<double>{ 33, 41.5 };
	flappy->rigidbody->wh = VECTOR2<double>{ 45, 30 };

	flappy->sprite = new Sprite;
	flappy->sprite->fileIndex = 4;
	flappy->sprite->size = DOUBLE_PAIR<int>{ 336, 87 };
	flappy->sprite->hvFrames = DOUBLE_PAIR<int>{ 4, 1 };

	GenerateBorder();

	_objectAllInitialized = true;

	return true;
}

int WindowClass::Run()
{
	MSG msg;

	QueryPerformanceFrequency(&_second);
	QueryPerformanceCounter(&_time);

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;


			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	Shutdown();

	return (int)msg.wParam;
}

void WindowClass::Simulating()
{
	RigidBody::Frame(_deltaTime);

	if (flappy->rigidbody->pos.y >= 500)
	{
		floor = true;
		flappy->rigidbody->pos.y = 500;
		//return;
	}

	if (!stop)
	{

		if (scene == MAIN)
		{

			if (Collider* coll = flappy->rigidbody->Collision())
			{
				if (!coll->isTrigger)
				{
					
					PlaySound(L"Punch.wav", NULL, SND_ASYNC);
					GameOver();
				}

				else
				{
					score++;
					PlaySound(L"Score.wav", NULL, SND_ASYNC);

					coll->wh = VECTOR2_ZERO_D;
					coll->origin = VECTOR2_ZERO_D;

				}

				return;
			}

			if (Object::list.size() > 4)
			{
				if (Object::list[3]->rigidbody->pos.x < -78)
				{

					// Always object #1 & 2 are both pipe;

					// Erased from vector => useless => destructor
					RigidBody::list.erase(RigidBody::list.begin() + 3);
					Object::list.erase(Object::list.begin() + 3);

					RigidBody::list.erase(RigidBody::list.begin() + 3);
					Object::list.erase(Object::list.begin() + 3);

					RigidBody::list.erase(RigidBody::list.begin() + 3);


				}

			}


			groundPos += _deltaTime * scrMoveSp;
			flightDis += _deltaTime * scrMoveSp;
			if (groundPos >= 168)
			{
				groundPos = 0;
			}

			if (flightDis - pipeIntv >= 255)
			{
				pipeIntv = flightDis;
				GeneratePipe();
			}
		}
	}
}

void WindowClass::GeneratePipe()
{
	Object* pipeUp;
	Object* pipeDown;
	RigidBody* scorePad;

	pipeUp = new Object;
	pipeDown = new Object;
	scorePad = new RigidBody;
	
	// Object member
	pipeUp->rigidbody = new RigidBody;
	pipeDown->rigidbody = new RigidBody;
	pipeUp->sprite = new Sprite;
	pipeDown->sprite = new Sprite;
	/*pipeUp->scrOutDisable = pipeDown->scrOutDisable = true;
	pipeUp->scrBorder = pipeDown->scrBorder = VECTOR2<double>{ (double)_scrSize[0], (double)_scrSize[1]};*/

	// Collider member
	pipeUp->rigidbody->type = pipeDown->rigidbody->type = scorePad->type = Collider::TYPE::BOX;
	pipeUp->rigidbody->origin = pipeDown->rigidbody->origin = VECTOR2<double>{ 39, 240 };
	pipeUp->rigidbody->wh = pipeDown->rigidbody->wh = VECTOR2<double>{ 78, 480 };

	scorePad->origin = VECTOR2<double>{ 5, 90 };
	scorePad->wh = VECTOR2<double>{ 10, 180 };


	//Rigidbody member
	pipeUp->rigidbody->rotation = pipeDown->rigidbody->rotation = false;
	pipeUp->rigidbody->drag = pipeDown->rigidbody->drag = scorePad->drag = false;
	pipeUp->rigidbody->vel.x = pipeDown->rigidbody->vel.x = -scrMoveSp;
	pipeUp->rigidbody->pos.x = pipeDown->rigidbody->pos.x = 939; // 900 + 78 / 2
	pipeUp->rigidbody->pos.y = rand() % 244 + 1 + 40 - 480;
	pipeDown->rigidbody->pos.y = pipeUp->rigidbody->pos.y + 480 + 180;
	scorePad->pos.x = pipeUp->rigidbody->pos.x + 78;
	scorePad->pos.y = pipeUp->rigidbody->pos.y + 480;
	scorePad->vel.x = -scrMoveSp;
	scorePad->isTrigger = true;


	//Sprite member
	pipeUp->sprite->fileIndex = 5;
	pipeDown->sprite->fileIndex = 6;
	pipeUp->sprite->size = pipeDown->sprite->size = DOUBLE_PAIR<int>{ 78, 480 };


}

void WindowClass::GenerateBorder()
{
	RigidBody* borderUp = new RigidBody;
	RigidBody* borderDown = new RigidBody;

	borderUp->rotation = borderDown->rotation = false;
	borderUp->drag = borderDown->drag = false;
	borderUp->pos.x = borderDown->pos.x = _scrSize[0] * 0.3 - 15;
	borderUp->pos.y = -5;
	borderDown->pos.y = 500;

	borderUp->type = borderDown->type = Collider::TYPE::BOX;
	borderUp->wh.x = borderDown->wh.x = 30;
	borderUp->wh.y = borderDown->wh.y = 5;

}

void WindowClass::InputAction()
{
	switch (scene)
	{
	case WindowClass::TITLE:
		Flapping();
		GameRestart();

		break;
	case WindowClass::MAIN:
		Flapping();

		//PlaySound(L"Twit.wav", NULL, SND_ASYNC);

		frameCnt = 0;
		break;
	case WindowClass::END:
		if (floor)
		{
			Flapping();
			GameRestart();
		}
		break;
	default:
		break;
	}
}

void WindowClass::Flapping()
{
	flappy->rigidbody->vel.y = -700;
	flappy->rigidbody->rad = 0.78;
	flappy->rigidbody->avel = 0;
}

void WindowClass::GameOver()
{
	stop = true;

	scene = END;

	if (max_score < score)
		max_score = score;

	flappy->rigidbody->SetConstForce(0, 4000);

	if (RigidBody::list.size() > 3)
	{
		std::vector<RigidBody*>::iterator iter;
		for (iter = RigidBody::list.begin() + 3; iter != RigidBody::list.end(); iter++)
		{
			(*iter)->enabled = false;
		}
	}

	SetTimer(_hWnd, 13, 1000, NULL);

}

void WindowClass::GameRestart()
{
	stop = false;
	scene = MAIN;
	floor = false;

	
	score = 0;

	Object::list.clear();
	RigidBody::list.clear();

	flappy->rigidbody->SetConstForce(0, 2000);
	flappy->rigidbody->pos = VECTOR2<double>{ _scrSize[0] * 0.3, _scrSize[1] * 0.5 - 43 };

	Object::list.push_back(flappy);
	RigidBody::list.push_back(flappy->rigidbody);
	GenerateBorder();

}

LRESULT WindowClass::AppProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		SetTimer(hWnd, 10, 1, NULL);

		InitializeControl();

		GdiplusStartupInput gdiplusStartInput;
		GdiplusStartup(&_GDIplusToken, &gdiplusStartInput, NULL);

		break;
	}

	case WM_DESTROY:
	{

		PostQuitMessage(0);
		break;
	}
	case WM_TIMER:
	{
		FrameTimer(hWnd, message, wParam, lParam);
	}
		break;
	case WM_CLOSE:
	{
		PostQuitMessage(0);

		break;
	}
	case WM_GETMINMAXINFO://Set minimum of window
	{
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = _scrSize[0];
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = _scrSize[1];
		((MINMAXINFO*)lParam)->ptMaxTrackSize.x = _scrSize[0];
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y = _scrSize[1];
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);//Start Painting

#pragma region DoubleBuffering

		HDC memoryDC = CreateCompatibleDC(hdc);
		HBITMAP memBit = CreateCompatibleBitmap(hdc, _scrSize[0], _scrSize[1]);//Create bitmap big as window
		HBITMAP oldBit = (HBITMAP)SelectObject(memoryDC, memBit);//Connect bitmap to memory DC and Extend memory DC

		PatBlt(memoryDC, 0, 0, _scrArea.right, _scrArea.bottom, WHITENESS);//Memory DC's white background color
#pragma endregion

		Graphics grabbed(memoryDC);


		grabbed.DrawImage(imgList[0], 0, 0, 900, 504);

		auto iter = Object::list.begin();
		for (; iter != Object::list.end(); iter++)
		{
			if (!(*iter)->enabled)
				continue;

			Image* objImg = imgList[(*iter)->sprite->fileIndex];

			VECTOR2<double> pos = (*iter)->rigidbody->pos;
			VECTOR2<double> com = (*iter)->rigidbody->com;
			double angle = (*iter)->rigidbody->rad;

			DOUBLE_PAIR<int> hvFrames = (*iter)->sprite->hvFrames;
			DOUBLE_PAIR<int> size = (*iter)->sprite->size;
			DOUBLE_PAIR<int> imgSlicedSize = { size.value[0] / hvFrames.value[0], size.value[1] / hvFrames.value[1] }; // Slice original image

			RectF rtTarget(pos.x - com.x, pos.y - com.y, imgSlicedSize.value[0], imgSlicedSize.value[1]);
			Matrix mat;
			mat.RotateAt(-angle * RAD2DEG, PointF(pos.x, pos.y)); // Different logic with ccw, I think;
			grabbed.SetTransform(&mat);


			//Draw image along frames
			grabbed.DrawImage(objImg, rtTarget, ((*iter)->sprite->frame % hvFrames.value[0]) * imgSlicedSize.value[0], ((*iter)->sprite->frame % hvFrames.value[1]) * imgSlicedSize.value[1], imgSlicedSize.value[0], imgSlicedSize.value[1], UnitPixel);
			
#pragma region Draw Systemical object data visibly

			//Ellipse(memoryDC, pos.x - 2, pos.y - 2, pos.x + 2, pos.y + 2);
			//Ellipse(memoryDC, pos.x - com.x - 2, pos.y - com.y - 2, pos.x - com.x + 2, pos.y - com.y + 2);

			//VECTOR2<double> org = (*iter)->rigidbody->origin;
			//VECTOR2<double> wh = (*iter)->rigidbody->wh;
			//VECTOR2<double> cnt = (*iter)->rigidbody->ColliderCenter();
			//Pen pen(Color(0, 0, 0));
			//grabbed.DrawRectangle(&pen, pos.x + (org - com).x - wh.x * 0.5, pos.y + (org - com).y - wh.y * 0.5, (int)wh.x, wh.y);

			//VECTOR2<double> w1, h1;
			//w1 = VECTOR2_X_D.Rotation(angle) * wh.x * 0.5;
			//h1 = VECTOR2_Y_D.Rotation(angle) * wh.y * 0.5;
			//com.Rotate(angle);

			//Ellipse(memoryDC, cnt.x - 2, cnt.y - 2, cnt.x + 2, cnt.y + 2);
			//Ellipse(memoryDC, cnt.x - w1.x + h1.x - 2, cnt.y - w1.y + h1.y - 2, cnt.x - w1.x + h1.x + 2, cnt.y - w1.y + h1.y + 2);
			//Ellipse(memoryDC, cnt.x + w1.x + h1.x - 2, cnt.y + w1.y + h1.y - 2, cnt.x + w1.x + h1.x + 2, cnt.y + w1.y + h1.y + 2);
			//Ellipse(memoryDC, cnt.x - w1.x - h1.x - 2, cnt.y - w1.y - h1.y - 2, cnt.x - w1.x - h1.x + 2, cnt.y - w1.y - h1.y + 2);
			//Ellipse(memoryDC, cnt.x + w1.x - h1.x - 2, cnt.y + w1.y - h1.y - 2, cnt.x + w1.x - h1.x + 2, cnt.y + w1.y - h1.y + 2);

			//Ellipse(memoryDC, pos.x - com.x - 2, pos.y - com.y - 2, pos.x - com.x + 2, pos.y - com.y + 2);

			//MoveToEx(memoryDC, 50, 50, nullptr);
			//LineTo(memoryDC, 50 + w1.x, 50 + w1.y);

			//MoveToEx(memoryDC, 50, 50, nullptr);
			//LineTo(memoryDC, 50 + h1.x, 50 + h1.y);

			//MoveToEx(memoryDC, 50, 50, nullptr);
			//LineTo(memoryDC, 50 + w1.x + h1.x, 50 + w1.y + h1.y);

			//MoveToEx(memoryDC, 50, 50, nullptr);
			//LineTo(memoryDC, 50 + w1.x - h1.x, 50 + w1.y - h1.y);

			//MoveToEx(memoryDC, 50, 50, nullptr);
			//LineTo(memoryDC, 50 - w1.x - h1.x, 50 - w1.y - h1.y);

			//MoveToEx(memoryDC, 50, 50, nullptr);
			//LineTo(memoryDC, 50 - w1.x + h1.x, 50 - w1.y + h1.y);

#pragma endregion

		}

		grabbed.ResetTransform();

		/*if (RigidBody::list.size() > 1)
		{
			auto tter = RigidBody::list.begin() + 1;
			for (; tter != RigidBody::list.end(); tter++)
			{
				Pen pen(Color(0, 0, 0));

				VECTOR2<double> pos = (*tter)->pos;
				VECTOR2<double> wh = (*tter)->wh;

				grabbed.DrawRectangle(&pen, (int)pos.x, pos.y, wh.x, wh.y);
			}
		}*/

		for (int i = 0; i < 7; i++)
		{
			grabbed.DrawImage(imgList[1], -INT(groundPos) + 168 * i, 504, 168, 56);
		}

		switch (scene)
		{
		case WindowClass::TITLE:
		{
			grabbed.DrawImage(imgList[2], 182, 175, 534, 96);
			SetTextColor(memoryDC, RGB(255, 255, 255));
			SetBkColor(memoryDC, RGB(0, 0, 0));
			TextOut(memoryDC, 440, 275, L"Press Spacebar or Mouse Click To Start...", 41);
		}
			break;
		case WindowClass::MAIN:
		{
			HBRUSH oldB, newB = CreateSolidBrush(RGB(0, 0, 0));
			oldB = (HBRUSH)SelectObject(memoryDC, newB);

			TCHAR scoreStr[4];
			memset(scoreStr, 0, sizeof(TCHAR) * 4);
			_swprintf(scoreStr, L"%d", score);
			 
			Rectangle(memoryDC, 420, 32, 520, 52);
			SetTextColor(memoryDC, RGB(255, 255, 255));
			SetBkColor(memoryDC, RGB(0, 0, 0));
			TextOut(memoryDC, 430, 34, scoreStr, lstrlen(scoreStr));

			SelectObject(memoryDC, oldB);

		}
		break;
		case WindowClass::END:
			if (floor)
			{

				grabbed.DrawImage(imgList[3], 289, 165, 339, 171);
				grabbed.DrawImage(imgList[9], 314, 98, 288, 63);

				TCHAR scoreStr[4], bst[4];
				memset(scoreStr, 0, sizeof(TCHAR) * 4);
				memset(bst, 0, sizeof(TCHAR) * 4);

				_swprintf(scoreStr, L"%d", score);
				_swprintf(bst, L"%d", max_score);

				SetTextColor(memoryDC, RGB(255, 255, 255));
				SetBkColor(memoryDC, RGB(0, 0, 0));
				TextOut(memoryDC, 560, 220, scoreStr, lstrlen(scoreStr));
				TextOut(memoryDC, 560, 285, bst, lstrlen(bst));

				if (score == max_score)
				{
					grabbed.DrawImage(imgList[8], 329, 228, 66, 66);
				}
				else if (score >= max_score * 0.8)
				{
					grabbed.DrawImage(imgList[7], 329, 228, 66, 66);
				}
			}
			break;
		}


		BitBlt(hdc, 0, 0, _scrArea.right, _scrArea.bottom, memoryDC, 0, 0, SRCCOPY);

		SelectObject(memoryDC, oldBit);
		DeleteObject(memBit);
		DeleteDC(memoryDC);
		EndPaint(hWnd, &ps);

		break;
	}

	case WM_COMMAND:
	{


		break;
	}
	case WM_KEYDOWN:
		if (wParam == VK_SPACE)
		{
			InputAction();
		}
	case WM_LBUTTONDOWN:
		InputAction();
	}


	return DefWindowProc(hWnd, message, wParam, lParam);
}

void WindowClass::FrameTimer(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 11://Get deltaTime
	{

		LARGE_INTEGER ct;
		QueryPerformanceCounter(&ct);
		_deltaTime = (ct.QuadPart - _time.QuadPart) / (float)_second.QuadPart;

		_time = ct;

		Simulating();

		InvalidateRect(hWnd, &_scrArea, FALSE);

		break;
	}
	case 12:
	{
		if (flappy->rigidbody->vel.y <= 0)
		{
			flappy->sprite->frame++;
		}
		else
		{
			if (frameCnt >= 12)
			{
				flappy->sprite->frame = 1;
			}
			else
			{
				frameCnt++;
				flappy->sprite->frame++;
			}
		}

		InvalidateRect(hWnd, &_scrArea, FALSE);

	}
	case 13:
	{
		PlaySound(L"Failed.wav", NULL, SND_ASYNC);
		KillTimer(_hWnd, 13);

	}
	case 10:
	{
		if (_objectAllInitialized)
		{
			SetTimer(hWnd, 11, 1, NULL);
			SetTimer(hWnd, 12, 60, NULL);
			KillTimer(_hWnd, 10);

			SetWindowText(_hWnd, L"Flappy Bird in Vodka - 2023 cdy");
		}
	}
	break;
	}
}

void WindowClass::Shutdown()
{
	GdiplusShutdown(_GDIplusToken);

	KillTimer(_hWnd, 11);
	KillTimer(_hWnd, 12);

	Release();

	DestroyWindow(_hWnd);
	_hWnd = NULL;

	UnregisterClass(_windowClass, _hInstance);

	_hInstance = NULL;
}

void WindowClass::Release()
{

	if (flappy)
	{
		flappy->Release();
		delete flappy;
		flappy = nullptr;
	}

}

void WindowClass::ClassLoadString(HINSTANCE hInstance)
{
	LoadStringW(hInstance, IDS_APP_TITLE, _title, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY202311189CDYVODKAFLAPPYBIRD, _windowClass, MAX_LOADSTRING);
}

void WindowClass::MyResisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY202311189CDYVODKAFLAPPYBIRD));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY202311189CDYVODKAFLAPPYBIRD);
	wcex.lpszClassName = _windowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);
}

bool WindowClass::CreateSystemWindow(int sW, int sH)
{
	_hWnd = CreateWindowW(_windowClass, _title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, 0, sW, sH, nullptr, nullptr, _hInstance, nullptr);//forbid maximizing

	if (!_hWnd)
		return false;

	ShowWindow(_hWnd, SW_SHOW);
	UpdateWindow(_hWnd);

	return true;
}


void WindowClass::InitializeControl()
{
}
