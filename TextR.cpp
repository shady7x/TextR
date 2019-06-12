#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <sstream>

using namespace std;

const char* tesscmd = "\"C:\\Program Files (x86)\\Tesseract-OCR\\tesseract.exe\" C:\\Users\\Oleg\\Desktop\\TextR\\textin.png C:\\Users\\Oleg\\Desktop\\TextR\\textout";
const string bf = "0123456789ABCDEF";

void send_keys(string text)
{
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	HKL kbl = GetKeyboardLayout(0);
	bool holding_shift = false;
	for (unsigned int i = 0; i < (int)text.size(); ++i)
	{
		if (GetAsyncKeyState(VK_END))
		{
			break;
		}
		ip.ki.dwFlags = 0;
		if (isupper(text[i]))
		{
			ip.ki.wVk = 0xA0;
			SendInput(1, &ip, sizeof(ip));
			Sleep(1);
			holding_shift = true;
		}
		ip.ki.wVk = VkKeyScanExA(text[i], kbl);
		SendInput(1, &ip, sizeof(ip));
		Sleep(1);
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(ip));
		if (holding_shift)
		{
			ip.ki.wVk = 0xA0;
			ip.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &ip, sizeof(ip));
			Sleep(1);
			holding_shift = false;
		}
		Sleep(11);
	}
}

int main()
{
	cout << "executing cmd -> " << tesscmd << endl;
	system(tesscmd);
	cout << "finished <-\n\n";
	string text = "";
	bool streak2 = false;
	char ch;
	fstream fin("C:\\Users\\Oleg\\Desktop\\TextR\\textout.txt", fstream::in);
	while (fin >> noskipws >> ch) 
	{
		if (bf[ch >> 4] == '0' && bf[ch & 15] == 'A')
		{
			if (streak2 == false)
			{
				text += ' ';
				streak2 = true;
			}
		}
		else
		{
			text += ch;
			streak2 = false;
		}
	}
	text.erase((int)text.size() - 1, 2);
	cout << text << "\n\nHit N to start copying text..\n";
	while (!GetAsyncKeyState(0x4E));
	HWND window = FindWindow(L"Chrome_WidgetWin_1", NULL);
	HWND chrome = GetWindow(window, GW_HWNDNEXT);
	if (window == NULL)
	{
		cout << "couldnt find window\n\n";
		return 132;
	}
	else
	{
		cout << "found window -> 0x" << window << endl;
		SetForegroundWindow(chrome);
		Sleep(1000);
		//type_char_by_char(window, text);
		send_keys(text);
		cout << "\n\nfinished typing text\n";
	}
    return 0;
}
