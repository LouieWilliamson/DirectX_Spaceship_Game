#pragma once
#include <dinput.h>

class Input
{
public:
	Input();
	~Input();
	HRESULT Initialise(HINSTANCE hInst, HWND hWnd);
	void ReadInputStates();
	bool isKeyPressed(unsigned char DI_keycode);
private:
	IDirectInput8* m_direct_input;
	IDirectInputDevice8* m_keyboard_device;
	unsigned char m_keyboard_state[256];
};

