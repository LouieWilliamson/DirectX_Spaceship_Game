#include "Input.h"



Input::Input()
{
}

//Cleans up pointers and unacquires the acquired keyboard
Input::~Input()
{
	if (m_keyboard_device)
	{
		m_keyboard_device->Unacquire();
		m_keyboard_device->Release();
	}

	if (m_direct_input) m_direct_input->Release();
}

//Initialises DirectInput
HRESULT Input::Initialise(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;
	ZeroMemory(m_keyboard_state, sizeof(m_keyboard_state));

	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& m_direct_input, NULL);
	if (FAILED(hr)) return hr;

	hr = m_direct_input->CreateDevice(GUID_SysKeyboard, &m_keyboard_device, NULL);
	if (FAILED(hr)) return hr;

	hr = m_keyboard_device->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) return hr;

	hr = m_keyboard_device->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) return hr;

	hr = m_keyboard_device->Acquire();
	if (FAILED(hr)) return hr;

	return S_OK;
}
//Gets the state of all the keys (pressed or not)
void Input::ReadInputStates()
{
	HRESULT hr;

	hr = m_keyboard_device->GetDeviceState(sizeof(m_keyboard_state), (LPVOID)& m_keyboard_state);

	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_keyboard_device->Acquire();
		}
	}
}
//Returns true if the given key is being pressed
bool Input::isKeyPressed(unsigned char DI_keycode)
{
	return m_keyboard_state[DI_keycode] & 0x80;
}
