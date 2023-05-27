#pragma once
#include <iostream>
#include <vector>
using namespace std;


class Object;
class RigidBody;

class WindowClass : public Singleton<WindowClass>
{
private:

	HINSTANCE _hInstance;
	HWND _hWnd;

	WCHAR _title[MAX_LOADSTRING];
	WCHAR _windowClass[MAX_LOADSTRING];

	int _scrSize[2];//0: Width, 1: Height
	RECT _scrArea;

	bool _objectAllInitialized;

	ULONG_PTR _GDIplusToken;//GDI+ token
private:
	LARGE_INTEGER _second;
	LARGE_INTEGER _time;
	double _deltaTime;
	int _scrHalfSize[2];

	Object* flappy;
	//std::vector<pair<Object*, bool>> pipes;// bool : is sprite reverse
	std::vector<Object*> pipes;

	double scrMoveSp = 100;
	int frameCnt;
	double groundPos = 0;
	double flightDis = 0;
	double pipeIntv = 0;

	int max_score = 0;
	int score = 0;

	bool stop = true;
	bool floor = false;


	enum SCENE { TITLE, MAIN, END };
	SCENE scene = TITLE;

	//std::vector<Gdiplus::Bitmap*> imgList;
	std::vector<Image*> imgList;

	//std::vector<RigidBody*> pads;
	//vector<POINT> lines;
public:
	WindowClass();
	~WindowClass();

	bool Initialize(HINSTANCE hInstance, int* scrSize);
	int Run();

	LRESULT AppProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void FrameTimer(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	void Shutdown();
	void Release();


	void ClassLoadString(HINSTANCE hInstance);
	void MyResisterClass(HINSTANCE hInstance);
	bool CreateSystemWindow(int sW, int sH);

	void InitializeControl();

	void Simulating();

	void Flapping();
	void GameOver();
	void GameRestart();
	void GeneratePipe();
	void GenerateBorder();
	void InputAction();
};

