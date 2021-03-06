﻿#include "InputController.h"

InputController::InputController()
{
	_input = NULL;
	_keyboard = NULL;
	ZeroMemory(_keyBuffer, 256);
}

InputController::~InputController()
{
	if (_input != NULL)
		this->_input->Release();
	if (_keyboard != NULL)
		this->_keyboard->Release();
}

InputController* InputController::_instance = nullptr;

InputController* InputController::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new InputController();
	}
	return _instance;
}

void InputController::release()
{
	delete _instance;
	_instance = nullptr;
}

bool InputController::init(HWND hWnd, HINSTANCE hinstance)
{
	this->_hWnd = hWnd;

	HRESULT rs;
	rs = DirectInput8Create(
		hinstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&_input,
		NULL);

	if (rs != DI_OK)
		return false;

	rs = _input->CreateDevice(GUID_SysKeyboard, (LPDIRECTINPUTDEVICEW*)&_keyboard, NULL);
	if (rs != DI_OK)
		return false;

	rs = _keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (rs != DI_OK)
		return false;

	rs = _keyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (rs != DI_OK)
		return false;

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.diph.dwObj = 0;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE;

	rs = _keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (rs != DI_OK)
		return false;

	rs = _keyboard->Acquire();
	if (rs != DI_OK)
		return false;

	return true;
}

void InputController::update()
{
	// Get trạng thái của tất cả các phím
	_keyboard->GetDeviceState(sizeof(this->_keyBuffer), _keyBuffer);

	// Nếu ESC được nhấn thì gửi message để thoát
	if (isKeyDown(DIK_ESCAPE))
	{
		PostMessage(_hWnd, WM_QUIT, 0, 0);
	}

	// Collect all buffered events
	DWORD dw = KEYBOARD_BUFFER_SIZE;
	HRESULT rs = _keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), _keyEvents, &dw, 0);

	// Kiểm tra từng phím xem phím có được nhấn hoặc thả
	int keycode, keystate;
	for (DWORD i = 0; i < dw; i++)
	{
		keycode = _keyEvents[i].dwOfs;
		keystate = _keyEvents[i].dwData;
		if ((keystate & 0x80) > 0)
		{
			KeyEventArg* arg = new KeyEventArg(keycode);
			__raise __eventkeyPressed(arg);
			delete arg;
		}
		else
		{
			KeyEventArg* arg = new KeyEventArg(keycode);
			__raise __eventkeyReleased(arg);
			delete arg;
		}
	}
}

int InputController::isKeyDown(int keycode)
{
	return ((_keyBuffer[keycode] & 0x80) > 0);
}
