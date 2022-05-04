// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include "PoeItem.h"
#include <thread>
#include "ModFilter.h"
#include <list>

using namespace std;

void toClipboard(HWND hwnd, const std::string &s);

HHOOK mouseHook;
void ProcessID(POINT pt);
void ProcessIDCtrl(POINT pt);
list<PoeItem*> _poeItems;
const int _poeItems_limit = 32;
PoeItem* _mapItem;

LRESULT __stdcall MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		if (wParam == WM_LBUTTONUP && (GetAsyncKeyState(VK_LSHIFT) & 0x8000))
		{
			MSLLHOOKSTRUCT* Lpar = (MSLLHOOKSTRUCT*)lParam;
#ifdef _DEBUG
			cout << Lpar->pt.x << " " << Lpar->pt.y << "\n";
#endif
			thread t1(ProcessID, Lpar->pt);
			t1.detach();
		}
		else if (wParam == WM_LBUTTONUP && (GetAsyncKeyState(VK_LCONTROL) & 0x8000))
		{
			MSLLHOOKSTRUCT* Lpar = (MSLLHOOKSTRUCT*)lParam;
#ifdef _DEBUG
			cout << Lpar->pt.x << " " << Lpar->pt.y << "\n";
#endif
			thread t1(ProcessIDCtrl, Lpar->pt);
			t1.detach();
		}
		//Lpar->pt.x;
		
	}
	return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

void ProcessIDCtrl(POINT pt)
{
	//Wait before sending Ctrl+C depends on your ping.
	Sleep(110);
	keybd_event(VK_LCONTROL, 0x9d, KEYEVENTF_EXTENDEDKEY | 0, 0);
	keybd_event(0x43, 0x9e, KEYEVENTF_EXTENDEDKEY | 0, 0);
	keybd_event(0x43, 0x9e, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	keybd_event(VK_LCONTROL, 0x9d, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	Sleep(50); // Seems like this is needed since CTRL+C is not instant in poe's case.
	
	HANDLE clip = NULL;
	if (OpenClipboard(NULL))
		clip = GetClipboardData(CF_TEXT);

	bool isMap = PoeItem::IsItemMapType((char*)clip);

#ifdef _DEBUG
    std::cout << "IsMap : " << std::boolalpha << isMap << '\n';
	std::cout << "length : " << strlen((char*)clip) << '\n';
#endif

	return;
}

void ProcessID(POINT pt)
{
	//Wait before sending Ctrl+C depends on your ping.
	Sleep(110);
	keybd_event(VK_LCONTROL, 0x9d, KEYEVENTF_EXTENDEDKEY | 0, 0);
	keybd_event(0x43, 0x9e, KEYEVENTF_EXTENDEDKEY | 0, 0);
	keybd_event(0x43, 0x9e, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	keybd_event(VK_LCONTROL, 0x9d, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	Sleep(50); // Seems like this is needed since CTRL+C is not instant in poe's case.

	HANDLE clip = NULL;
	if (OpenClipboard(NULL))
		clip = GetClipboardData(CF_TEXT);

	PoeItem* item = new PoeItem(string((char*)clip), pt);

	if (item->GetItemType() == ItemType::Map)
		_mapItem = item;
	else if (_poeItems.size() == _poeItems_limit)
	{
		delete _poeItems.front();
		_poeItems.pop_front();
		_poeItems.push_back(item);
	}
	else
		_poeItems.push_back(item);

#ifdef _DEBUG
	cout << "size: " << _poeItems.size() << endl;
#endif
	return;
}

void SetHook()
{
	if (!(mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0)))
	{
#ifdef _DEBUG
		cout << "Failed to install mouse hook!" << endl;
#endif
	}
}

void ReleaseHook()
{
	UnhookWindowsHookEx(mouseHook);
}


void toClipboard(HWND hwnd, const std::string &s) {
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

int main()
{
	SetConsoleCtrlHandler(NULL, 0);	
	/*string testplus = "+55 maximum Life";
	string testpercent = "55% increased life";
	string testpluspercent = "+55% to some resistance";
	string testto = "Adds 1 to 44 Lightning Damage";

	cout << ReadValuePlus(testplus) << "\n";
	cout << ReadValuePercent(testpercent) << "\n";
	cout << ReadValuePlusPercent(testpluspercent) << "\n";
	cout << ReadValueTo(testto) << "\n";*/

	if (!ReadFilters())
	{
		throw std::invalid_argument("Filter file read error");
		return 0;

	}
	SetHook();
	//Start Render Thread
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
#ifdef _DEBUG
		std::cout << "Hello World!\n";
#endif
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
	
}

