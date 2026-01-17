#include <iostream>
#include <Windows.h>

using namespace std;

HHOOK hHook;

bool running = true; 


DWORD WINAPI Flood(LPVOID) {
	while (running) {
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 100; i++) {
				cout << "#";
			}
			cout << endl;
		}
		Sleep(100);
	}
	return 0;
}


LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		KBDLLHOOKSTRUCT* P = (KBDLLHOOKSTRUCT*)lParam;


		if (P->vkCode == VK_F4 && (GetAsyncKeyState(VK_MENU) & 0x8000)) {
			return 1;
		}

		if (P->vkCode == 'Q' &&
			(GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
			(GetAsyncKeyState(VK_SHIFT) & 0x8000)) {
			PostQuitMessage(0);
		}
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main() {
	setlocale(LC_ALL, "RU");

	SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)2);
	
	Sleep(2000);

	SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)-1);
	



	HWND console = GetConsoleWindow();

	ShowWindow(console, SW_MAXIMIZE);
	
	SetWindowPos(
		console,
		HWND_TOPMOST,
		0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE
	);

	
	hHook = SetWindowsHookEx(
		WH_KEYBOARD_LL,
		KeyboardProc,
		NULL,
		0
	);


	CreateThread(NULL, 0, Flood, NULL, 0, NULL);


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hHook);

	running = false; 
	 
	return 0;
}