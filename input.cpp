//---------------------------------------
//
//���͏���[input.cpp]
//Author fuma sato
//
//---------------------------------------

#include"input.h"

#define STICK_NUM (32767.0f)
#define DSTICK_NUM (-1.0f)
#define MOUSE_NUM (15.0f)
#define MOUSE_NUM_WHEEL (300.0f)
#define NUM_KEY_MAX (256)//�L�[�̍ő吔
#define NUM_DIJOYKEY_MAX (32)//dinput�{�^���̍ő吔
#define POV_MAX (4)
#define POV_SYS_MAX (8)
#define POV_NUM (4500.0f)
#define RELEASE_TIME (50)//���ߎ���
#define REPEAT_TIME (40)//����������

LPDIRECTINPUT8 g_pinput = NULL;//�C���v�b�g�|�C���^
LPDIRECTINPUTDEVICE8 g_Devkeyboard = NULL;//�L�[�{�[�h�f�o�C�X�|�C���^
BYTE g_akeyState[NUM_KEY_MAX];//�L�[���
LPDIRECTINPUTDEVICE8 g_DevMouse = NULL;//�}�E�X�f�o�C�X�|�C���^
DIMOUSESTATE2 g_MouseState;//�}�E�X���
XINPUT_STATE g_joykeyState[CONTROLLER_MAX] = { NULL };//�p�b�h���
LPDIRECTINPUTDEVICE8 g_DevdJoypad[CONTROLLER_MAX] = { NULL };//�p�b�h�f�o�C�X�|�C���^
DIJOYSTATE g_djoykeyState[CONTROLLER_MAX] = { NULL };//d�p�b�h���
LPDIRECTINPUTEFFECT g_djoyEffect[CONTROLLER_MAX] = { NULL };
char g_ConName[CONTROLLER_MAX][64] = { NULL };
int g_XcontrollerNum = 0;
int g_DcontrollerNum = 0;
bool g_bUnCon = false;
bool g_bX = false;
const DIDEVICEINSTANCE* g_pDiInstance[CONTROLLER_MAX];

//--------------------------
//
//�L�[�{�[�h
//
//--------------------------

//--------------------
//����������
//--------------------
HRESULT InitKeyboard(HINSTANCE hInstanse, HWND hWnd)
{
	//�C���v�b�g�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create
	(
		hInstanse,
		DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&g_pinput,
		NULL
	)))
	{
		return E_FAIL;
	}

	//�C���v�b�g�f�o�C�X�̍쐬
	if (FAILED(g_pinput->CreateDevice(GUID_SysKeyboard, &g_Devkeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�t�H�[�}�b�g�쐬
	if (FAILED(g_Devkeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	if (FAILED(g_Devkeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	g_Devkeyboard->Acquire();//�A�N�Z�X�����̎擾

	return S_OK;
}

//----------------------
//�I������
//----------------------
void UninitKeyborad(void)
{
	//�L�[�{�[�h�f�o�C�X�̔j��
	if (g_Devkeyboard != NULL)
	{
		g_Devkeyboard->Unacquire();
		g_Devkeyboard->Release();
		g_Devkeyboard = NULL;
	}
	//�C���v�b�g�̔j��
	if (g_pinput != NULL)
	{
		g_pinput->Release();
		g_pinput = NULL;
	}
}

//-------------------------
//�X�V����
//-------------------------
void UpdateKeyborad(void)
{
	BYTE akeystate[NUM_KEY_MAX];//�ꎞ���͊i�[
	int nCntkey;//�L�[���̓J�E���g

	if (SUCCEEDED(g_Devkeyboard->GetDeviceState(sizeof(akeystate), &akeystate[0])))
	{
		for (nCntkey = 0; nCntkey < NUM_KEY_MAX; nCntkey++)//�L�[���͕��J��Ԃ�
		{
			g_akeyState[nCntkey] = akeystate[nCntkey];//�ꎞ�I�ȏ��𐳋K���ɓn��
		}
	}
	else
	{
		g_Devkeyboard->Acquire();//�A�N�Z�X�����̎擾
	}
}

//------------------------------
//�L�[��������
//------------------------------
bool GetKeyboradPress(int nkey)
{
	return (g_akeyState[nkey] & 0x80) ? true : false;
}

//------------------------------
//�L�[���͎��̏���
//------------------------------
bool GetKeyboradTrigger(int nkey)
{
	static bool bOldkey[NUM_KEY_MAX] = { false };

	if (bOldkey[nkey]==false&&((g_akeyState[nkey] & 0x80)?true:false)==true)
	{
     	bOldkey[nkey] = true;
		return true;
	}
	else if(bOldkey[nkey] ==true&&((g_akeyState[nkey] & 0x80) ? true : false) == false)
	{
		bOldkey[nkey] = false;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//�L�[�b�������ۂ̏���
//------------------------------
bool GetKeyboradRelease(int nkey)
{
	static bool bOldkey[NUM_KEY_MAX] = { false };
	static int KeyPutTime[NUM_KEY_MAX] = { 0 };

	if (bOldkey[nkey] == true && ((g_akeyState[nkey] & 0x80) ? true : false) == false)
	{
		bOldkey[nkey] = false;
		if (KeyPutTime[nkey] >=RELEASE_TIME)
		{
			KeyPutTime[nkey] = 0;
			return true;
		}
		else
		{
			KeyPutTime[nkey] = 0;
			return false;
		}
	}
	else if (bOldkey[nkey] == false && ((g_akeyState[nkey] & 0x80) ? true : false) == true)
	{
		bOldkey[nkey] = true;
		return false;
	}
	else if (bOldkey[nkey] == true && ((g_akeyState[nkey] & 0x80) ? true : false) == true)
	{
		KeyPutTime[nkey]++;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//�L�[����������
//------------------------------
bool GetKeyboradRepeat(int nkey)
{
	static bool bOldkey[NUM_KEY_MAX] = { false };
	static int KeyPutTime[NUM_KEY_MAX] = { 0 };

	if (bOldkey[nkey] == true && ((g_akeyState[nkey] & 0x80) ? true : false) == true)
	{
		return true;
	}
	else if (bOldkey[nkey] == false && ((g_akeyState[nkey] & 0x80) ? true : false) == true)
	{
		KeyPutTime[nkey]++;

		if (KeyPutTime[nkey] >=REPEAT_TIME)
		{
			KeyPutTime[nkey] = 0;
			bOldkey[nkey] = true;
		}
		return false;
	}
	else
	{
		bOldkey[nkey] = false;
		KeyPutTime[nkey] = 0;
		return false;
	}
}

//--------------------------
//
//�}�E�X
//
//--------------------------

//--------------------
//����������
//--------------------
HRESULT InitMouse(HINSTANCE hInstanse, HWND hWnd)
{
	//�C���v�b�g�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create
	(
		hInstanse,
		DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&g_pinput,
		NULL
	)))
	{
		return E_FAIL;
	}

	//�C���v�b�g�f�o�C�X�̍쐬
	if (FAILED(g_pinput->CreateDevice(GUID_SysMouse, &g_DevMouse, NULL)))
	{
		return E_FAIL;
	}

	//�t�H�[�}�b�g�쐬
	if (FAILED(g_DevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	if (FAILED(g_DevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	g_DevMouse->Acquire();//�A�N�Z�X�����̎擾

	return S_OK;
}

//----------------------
//�I������
//----------------------
void UninitMouse(void)
{
	//�L�[�{�[�h�f�o�C�X�̔j��
	if (g_DevMouse != NULL)
	{
		g_DevMouse->Unacquire();
		g_DevMouse->Release();
		g_DevMouse = NULL;
	}
	//�C���v�b�g�̔j��
	if (g_pinput != NULL)
	{
		g_pinput->Release();
		g_pinput = NULL;
	}
}

//-------------------------
//�X�V����
//-------------------------
void UpdateMouse(void)
{
	DIMOUSESTATE2 MouseState;//�}�E�X���//�ꎞ���͊i�[

	if (SUCCEEDED(g_DevMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
	{
		g_MouseState = MouseState;//�ꎞ�I�ȏ��𐳋K���ɓn��
	}
	else
	{
		g_DevMouse->Acquire();//�A�N�Z�X�����̎擾
	}
}

//------------------------------
//�L�[��������
//------------------------------
bool GetMousePress(int nkey)
{
	return (g_MouseState.rgbButtons[nkey] & 0x80) ? true : false;
}

//------------------------------
//�L�[���͎��̏���
//------------------------------
bool GetMouseTrigger(int nkey)
{
	static bool bOldMouse[MOUSE_MAX] = { false };

	if (bOldMouse[nkey] == false && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		bOldMouse[nkey] = true;
		return true;
	}
	else if (bOldMouse[nkey] == true && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == false)
	{
		bOldMouse[nkey] = false;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//�L�[�b�������ۂ̏���
//------------------------------
bool GetMouseRelease(int nkey)
{
	static bool bOldMouse[MOUSE_MAX] = { false };
	static int MousePutTime[MOUSE_MAX] = { 0 };

	if (bOldMouse[nkey] == true && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == false)
	{
		bOldMouse[nkey] = false;
		if (MousePutTime[nkey] >= RELEASE_TIME)
		{
			MousePutTime[nkey] = 0;
			return true;
		}
		else
		{
			MousePutTime[nkey] = 0;
			return false;
		}
	}
	else if (bOldMouse[nkey] == false && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		bOldMouse[nkey] = true;
		return false;
	}
	else if (bOldMouse[nkey] == true && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		MousePutTime[nkey]++;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//�L�[����������
//------------------------------
bool GetMouseRepeat(int nkey)
{
	static bool bOldMouse[MOUSE_MAX] = { false };
	static int MousePutTime[MOUSE_MAX] = { 0 };
	
	if (bOldMouse[nkey] == true && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		return true;
	}
	else if (bOldMouse[nkey] == false && ((g_MouseState.rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		MousePutTime[nkey]++;

		if (MousePutTime[nkey] >= REPEAT_TIME)
		{
			MousePutTime[nkey] = 0;
			bOldMouse[nkey] = true;
		}
		return false;
	}
	else
	{
		bOldMouse[nkey] = false;
		MousePutTime[nkey] = 0;
		return false;
	}
}

//--------------
//�}�E�X�ړ�
//--------------
float* GetMouseMove(void)
{
	static float MouseMove[2] = { 0.0f };

	MouseMove[0]=(float)g_MouseState.lX;
	MouseMove[1]=(float)g_MouseState.lY;

	MouseMove[0] /= MOUSE_NUM;
	MouseMove[1] /= MOUSE_NUM;

	return &MouseMove[0];
}

//----------------
//�L�[��������
//----------------
float GetMouseWheel(void)
{
	float MouseWheel = 0.0f;

	MouseWheel = (float)g_MouseState.lZ;

	MouseWheel /= MOUSE_NUM_WHEEL;

	return MouseWheel;
}

//--------------------------
//
//�p�b�h
//
//--------------------------

//--------------------------------
//����������
//--------------------------------
HRESULT InitJoypad(void)
{
	memset(&g_joykeyState,0,sizeof(XINPUT_STATE)* CONTROLLER_MAX);

	XInputEnable(true);

	return S_OK;
}

//------------------------
//�I������
//------------------------
void UninitJoypad(void)
{
	XInputEnable(false);
}

//------------------------
//�X�V����
//------------------------
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState[CONTROLLER_MAX];
	int ControllerNum = 0;

	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (XInputGetState(i, &joykeyState[i]) == ERROR_SUCCESS)
		{
			g_joykeyState[i] = joykeyState[i];
		}
		else
		{
			ControllerNum++;
		}
	}

	g_XcontrollerNum = CONTROLLER_MAX - ControllerNum;
}

//--------------------------
//�L�[��������
//--------------------------
bool GetJoykeyPress(JOYKEY key, CONTROLLER Controller)
{
	BYTE Trigger = 0;

	if (Controller==CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}
			
			if (g_joykeyState[i].Gamepad.wButtons & (0x01 << key) || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			{
				return true;
			}
		}

		return false;
	}

	if (key==JOYKEY_LT||key==JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	return (g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)|| Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true: false;
}

//--------------------------
//�L�[���͏���
//--------------------------
bool GetJoykeyTrigger(JOYKEY key, CONTROLLER Controller)
{
	static bool bOldJoy[CONTROLLER_MAX][JOYKEY_MAX] = { false };
	BYTE Trigger = 0;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}

			if (bOldJoy[i][key] == false && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				bOldJoy[i][key] = true;
				return true;
			}
			else if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == false && Trigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				bOldJoy[i][key] = false;
			}
		}

		return false;
	}

	if (key == JOYKEY_LT || key == JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	if (bOldJoy[Controller][key] == false && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true|| Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		bOldJoy[Controller][key] = true;
		return true;
	}
	else if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == false&& Trigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		bOldJoy[Controller][key] = false;
		return false;
	}
	else
	{
		return false;
	}
}


//----------------------------
//�L�[��������
//----------------------------
bool GetJoykeyRelease(JOYKEY key, CONTROLLER Controller)
{
	static bool bOldJoy[CONTROLLER_MAX][JOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][JOYKEY_MAX] = { 0 };
	BYTE Trigger = 0;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}

			if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == false && Trigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				bOldJoy[i][key] = false;
				if (KeyPutTime[i][key] >= RELEASE_TIME)
				{
					KeyPutTime[i][key] = 0;
					return true;
				}
				else
				{
					KeyPutTime[i][key] = 0;
				}
			}
			else if (bOldJoy[i][key] == false && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				bOldJoy[i][key] = true;
			}
			else if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				KeyPutTime[i][key]++;
			}
		}

		return false;
	}

	if (key == JOYKEY_LT || key == JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == false && Trigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		bOldJoy[Controller][key] = false;
		if (KeyPutTime[Controller][key] >= RELEASE_TIME)
		{
			KeyPutTime[Controller][key] = 0;
			return true;
		}
		else
		{
			KeyPutTime[Controller][key] = 0;
			return false;
		}
	}
	else if (bOldJoy[Controller][key] == false && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		bOldJoy[Controller][key] = true;
		return false;
	}
	else if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		KeyPutTime[Controller][key]++;
		return false;
	}
	else
	{
		return false;
	}
}

//----------------------------
//�L�[����������
//----------------------------
bool GetJoykeyRepeat(JOYKEY key, CONTROLLER Controller)
{
	static bool bOldJoy[CONTROLLER_MAX][JOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][JOYKEY_MAX] = { 0 };
	BYTE Trigger = 0;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}

			if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				return true;
			}
			else if (bOldJoy[i][key] == false && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				KeyPutTime[i][key]++;

				if (KeyPutTime[i][key] >= REPEAT_TIME)
				{
					KeyPutTime[i][key] = 0;
					bOldJoy[i][key] = true;
				}
			}
			else
			{
				bOldJoy[i][key] = false;
				KeyPutTime[i][key] = 0;
			}
		}
		return false;
	}

	if (key == JOYKEY_LT || key == JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		return true;
	}
	else if (bOldJoy[Controller][key] == false && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		KeyPutTime[Controller][key]++;

		if (KeyPutTime[Controller][key] >= REPEAT_TIME)
		{
			KeyPutTime[Controller][key] = 0;
			bOldJoy[Controller][key] = true;
		}
		return false;
	}
	else
	{
		bOldJoy[Controller][key] = false;
		KeyPutTime[Controller][key] = 0;
		return false;
	}
}

//-----------------
//�X�e�B�b�N����
//-----------------
float *GetJoyStick(STICK Stick, CONTROLLER Controller)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//-----------------------------
//
//-----------------------------
bool JoyStickPress(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;

	if (Controller==CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (fStick > STICK_DED)
				{
					return true;
				}
				break;
			case DIRESTICK_DOWN:
				if (fStick < -STICK_DED)
				{
					return true;
				}
				break;
			case DIRESTICK_LEFT:
				if (fStick < -STICK_DED)
				{
					return true;
				}
				break;
			case DIRESTICK_RIGHT:
				if (fStick > STICK_DED)
				{
					return true;
				}
				break;
			}
		}
		return false;
	}

	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		return fStick > STICK_DED;
		break;
	case DIRESTICK_DOWN:
		return fStick < -STICK_DED;
		break;
	case DIRESTICK_LEFT:
		return fStick < -STICK_DED;
		break;
	case DIRESTICK_RIGHT:
		return fStick > STICK_DED;
		break;
	}

	return false;
}

//-----------------------------
//
//-----------------------------
bool JoyStickTrigger(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;
	static bool bOldStick[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = {false};

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			case DIRESTICK_DOWN:
				if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
					break;
			case DIRESTICK_LEFT:
				if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			case DIRESTICK_RIGHT:
				if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			}
		}
		return false;
	}


	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_DOWN:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_LEFT:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_RIGHT:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	}

	return false;
}

//-----------------------------
//
//-----------------------------
bool JoyStickRelease(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;
	static bool bOldStick[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { false };
	static int nStickTime[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			case DIRESTICK_DOWN:
				if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			case DIRESTICK_LEFT:
				if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			case DIRESTICK_RIGHT:
				if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			}
		}
		return false;
	}


	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_DOWN:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_LEFT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_RIGHT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	}

	return false;
}

//-----------------------------
//
//-----------------------------
bool JoyStickRepeat(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;
	static bool bOldStick[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { false };
	static int nStickTime[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbLX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_joykeyState[i].Gamepad.sThumbRX;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			case DIRESTICK_DOWN:
				if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			case DIRESTICK_LEFT:
				if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			case DIRESTICK_RIGHT:
				if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			}
		}
		return false;
	}

	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	case DIRESTICK_DOWN:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	case DIRESTICK_LEFT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	case DIRESTICK_RIGHT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	}

	return false;
}

//----------------
//�U��
//----------------
void VibrateController(WORD wLeftMotorSpeed, WORD wRightMotorSpeed, CONTROLLER Controller)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	vibration.wLeftMotorSpeed = wLeftMotorSpeed;  // ���̐U�����[�^�[�̑��x (0-65535)
	vibration.wRightMotorSpeed = wRightMotorSpeed; // �E�̐U�����[�^�[�̑��x (0-65535)

	XInputSetState((DWORD)Controller, &vibration);
}

//-----------------------------
//
//dinput�p�b�h
//
//-----------------------------

//--------------------
//����������
//--------------------
HRESULT InitdJoypad(HINSTANCE hInstanse, HWND hWnd)
{
	//�C���v�b�g�I�u�W�F�N�g�̍쐬
	DirectInput8Create
	(
		hInstanse,
		DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&g_pinput,
		NULL
	);

	// DirectInput�f�o�C�X���
	g_pinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, NULL, DIEDFL_ATTACHEDONLY);

	return S_OK;
}

//----------------------
//�I������
//----------------------
void UninitdJoypad(void)
{
	//�G�t�F�N�g�f�o�C�X�̔j��
	VibratedControllerStop(CONTROLLER_MAX);
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		//�L�[�{�[�h�f�o�C�X�̔j��
		if (g_DevdJoypad[i] != NULL)
		{
			g_DevdJoypad[i]->Unacquire();
			g_DevdJoypad[i]->Release();
			g_DevdJoypad[i] = NULL;
		}
	}
	//�C���v�b�g�̔j��
	if (g_pinput != NULL)
	{
		g_pinput->Release();
		g_pinput = NULL;
	}
}

//-------------------------
//�X�V����
//-------------------------
void UpdatedJoypad(void)
{
	DIJOYSTATE  djoykeyState[CONTROLLER_MAX];//�ꎞ���͊i�[
	int ControllerNum = 0;

	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (g_DevdJoypad[i] != NULL)
		{
			if (SUCCEEDED(g_DevdJoypad[i]->GetDeviceState(sizeof(DIJOYSTATE), &djoykeyState[i])))
			{
				g_djoykeyState[i] = djoykeyState[i];//�ꎞ�I�ȏ��𐳋K���ɓn��
			}
			else
			{
				ControllerNum++;
			}
		}
		else
		{
			ControllerNum++;
		}
	}

	if (g_DcontrollerNum!=(CONTROLLER_MAX-ControllerNum))
	{
		SetUnCon(true);
		InputReSet();
		// DirectInput�f�o�C�X���
		g_pinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, NULL, DIEDFL_ATTACHEDONLY);
	}
}

//------------------------------
//�L�[��������
//------------------------------
bool GetdJoykeyPress(int nkey, CONTROLLER Controller)
{
	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false)
			{
				return true;
			}
		}

		return false;
	}

	return (g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false;
}

//------------------------------
//�L�[���͎��̏���
//------------------------------
bool GetdJoykeyTrigger(int nkey, CONTROLLER Controller)
{
	static bool bOldkey[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { false };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (bOldkey[i][nkey] == false && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == true)
			{
				bOldkey[i][nkey] = true;
				return true;
			}
			else if (bOldkey[i][nkey] == true && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == false)
			{
				bOldkey[i][nkey] = false;
			}
		}

		return false;
	}

	if (bOldkey[Controller][nkey] == false && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		bOldkey[Controller][nkey] = true;
		return true;
	}
	else if (bOldkey[Controller][nkey] == true && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == false)
	{
		bOldkey[Controller][nkey] = false;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//�L�[�b�������ۂ̏���
//------------------------------
bool GetdJoykeyRelease(int nkey, CONTROLLER Controller)
{
	static bool bOldkey[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (bOldkey[i][nkey] == true && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == false)
			{
				bOldkey[i][nkey] = false;
				if (KeyPutTime[i][nkey] >= RELEASE_TIME)
				{
					KeyPutTime[i][nkey] = 0;
					return true;
				}
				else
				{
					KeyPutTime[i][nkey] = 0;
				}
			}
			else if (bOldkey[i][nkey] == false && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == true)
			{
				bOldkey[i][nkey] = true;
			}
			else if (bOldkey[i][nkey] == true && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == true)
			{
				KeyPutTime[i][nkey]++;
			}
		}

		return false;
	}

	if (bOldkey[Controller][nkey] == true && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == false)
	{
		bOldkey[Controller][nkey] = false;
		if (KeyPutTime[Controller][nkey] >= RELEASE_TIME)
		{
			KeyPutTime[Controller][nkey] = 0;
			return true;
		}
		else
		{
			KeyPutTime[Controller][nkey] = 0;
			return false;
		}
	}
	else if (bOldkey[Controller][nkey] == false && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		bOldkey[Controller][nkey] = true;
		return false;
	}
	else if (bOldkey[Controller][nkey] == true && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		KeyPutTime[Controller][nkey]++;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//�L�[����������
//------------------------------
bool GetdJoykeyRepeat(int nkey, CONTROLLER Controller)
{
	static bool bOldkey[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (bOldkey[i][nkey] == true && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == true)
			{
				return true;
			}
			else if (bOldkey[i][nkey] == false && ((g_djoykeyState[i].rgbButtons[nkey] & 0x80) ? true : false) == true)
			{
				KeyPutTime[i][nkey]++;

				if (KeyPutTime[i][nkey] >= REPEAT_TIME)
				{
					KeyPutTime[i][nkey] = 0;
					bOldkey[i][nkey] = true;
				}
			}
			else
			{
				bOldkey[i][nkey] = false;
				KeyPutTime[i][nkey] = 0;
			}
		}

		return false;
	}

	if (bOldkey[Controller][nkey] == true && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		return true;
	}
	else if (bOldkey[Controller][nkey] == false && ((g_djoykeyState[Controller].rgbButtons[nkey] & 0x80) ? true : false) == true)
	{
		KeyPutTime[Controller][nkey]++;

		if (KeyPutTime[Controller][nkey] >= REPEAT_TIME)
		{
			KeyPutTime[Controller][nkey] = 0;
			bOldkey[Controller][nkey] = true;
		}
		return false;
	}
	else
	{
		bOldkey[Controller][nkey] = false;
		KeyPutTime[Controller][nkey] = 0;
		return false;
	}
}

//-----------------
//�X�e�B�b�N����
//-----------------
float* GetdJoyStick(STICK Stick, CONTROLLER Controller)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_djoykeyState[Controller].lX;
		fStick[1] = (float)g_djoykeyState[Controller].lY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_djoykeyState[Controller].lZ;
		fStick[1] = (float)g_djoykeyState[Controller].lRz;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	fStick[0] += DSTICK_NUM;
	fStick[1] += DSTICK_NUM;

	return &fStick[0];
}

//
//
//
bool dJoyStickPress(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;
			fStick += DSTICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (fStick < -STICK_DED)
				{
					return true;
				}
				break;
			case DIRESTICK_DOWN:
				if (fStick > STICK_DED)
				{
					return true;
				}
				break;
			case DIRESTICK_LEFT:
				if (fStick < -STICK_DED)
				{
					return true;
				}
				break;
			case DIRESTICK_RIGHT:
				if (fStick > STICK_DED)
				{
					return true;
				}
				break;
			}
		}
		return false;
	}
	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;
	fStick += DSTICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		return fStick > STICK_DED;
		break;
	case DIRESTICK_DOWN:
		return fStick < -STICK_DED;
		break;
	case DIRESTICK_LEFT:
		return fStick < -STICK_DED;
		break;
	case DIRESTICK_RIGHT:
		return fStick > STICK_DED;
		break;
	}

	return false;
}

//
//
//
bool dJoyStickTrigger(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;
	static bool bOldStick[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { false };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;
			fStick += DSTICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			case DIRESTICK_DOWN:
				if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			case DIRESTICK_LEFT:
				if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			case DIRESTICK_RIGHT:
				if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
				}
				break;
			}
		}
		return false;
	}


	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;
	fStick += DSTICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_DOWN:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_LEFT:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_RIGHT:
		if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			return false;
		}
		else
		{
			return false;
		}
		break;
	}

	return false;
}

//
//
//
bool dJoyStickRelease(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;
	static bool bOldStick[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { false };
	static int nStickTime[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;
			fStick += DSTICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			case DIRESTICK_DOWN:
				if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
			case DIRESTICK_LEFT:
				if (bOldStick[i][Stick][DireStick] == true && fStick >= -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			case DIRESTICK_RIGHT:
				if (bOldStick[i][Stick][DireStick] == true && fStick <= STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = false;
					if (nStickTime[i][Stick][DireStick] >= RELEASE_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						return true;
					}
					else
					{
						nStickTime[i][Stick][DireStick] = 0;
					}
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					bOldStick[i][Stick][DireStick] = true;
				}
				else if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;
				}
				break;
			}
		}
		return false;
	}


	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;
	fStick += DSTICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_DOWN:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_LEFT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick >= -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	case DIRESTICK_RIGHT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick <= STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = false;
			if (nStickTime[Controller][Stick][DireStick] >= RELEASE_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return true;
			}
			else
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				return false;
			}
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			bOldStick[Controller][Stick][DireStick] = true;
			return false;
		}
		else if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;
			return false;
		}
		else
		{
			return false;
		}
		break;
	}

	return false;
}

//
//
//
bool dJoyStickRepeat(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller)
{
	float fStick = 0.0f;
	static bool bOldStick[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { false };
	static int nStickTime[CONTROLLER_MAX][STICK_MAX][DIRESTICK_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			switch (Stick)
			{
			case STICK_LEFT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lY;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lX;
					break;
				}
				break;
			case STICK_RIGHT:
				switch (DireStick)
				{
				case DIRESTICK_UP:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_DOWN:
					fStick = (float)g_djoykeyState[i].lRz;
					break;
				case DIRESTICK_LEFT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				case DIRESTICK_RIGHT:
					fStick = (float)g_djoykeyState[i].lZ;
					break;
				}
				break;
			}

			fStick /= STICK_NUM;
			fStick += DSTICK_NUM;

			switch (DireStick)
			{
			case DIRESTICK_UP:
				if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			case DIRESTICK_DOWN:
				if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			case DIRESTICK_LEFT:
				if (bOldStick[i][Stick][DireStick] == true && fStick < -STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick < -STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			case DIRESTICK_RIGHT:
				if (bOldStick[i][Stick][DireStick] == true && fStick > STICK_DED)
				{
					return true;
				}
				else if (bOldStick[i][Stick][DireStick] == false && fStick > STICK_DED)
				{
					nStickTime[i][Stick][DireStick]++;

					if (nStickTime[i][Stick][DireStick] >= REPEAT_TIME)
					{
						nStickTime[i][Stick][DireStick] = 0;
						bOldStick[i][Stick][DireStick] = true;
					}
				}
				else
				{
					bOldStick[i][Stick][DireStick] = false;
					nStickTime[i][Stick][DireStick] = 0;
				}
				break;
			}
		}
		return false;
	}

	switch (Stick)
	{
	case STICK_LEFT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lY;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lX;
			break;
		}
		break;
	case STICK_RIGHT:
		switch (DireStick)
		{
		case DIRESTICK_UP:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_DOWN:
			fStick = (float)g_djoykeyState[Controller].lRz;
			break;
		case DIRESTICK_LEFT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		case DIRESTICK_RIGHT:
			fStick = (float)g_djoykeyState[Controller].lZ;
			break;
		}
		break;
	}

	fStick /= STICK_NUM;
	fStick += DSTICK_NUM;

	switch (DireStick)
	{
	case DIRESTICK_UP:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	case DIRESTICK_DOWN:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	case DIRESTICK_LEFT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick < -STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick < -STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	case DIRESTICK_RIGHT:
		if (bOldStick[Controller][Stick][DireStick] == true && fStick > STICK_DED)
		{
			return true;
		}
		else if (bOldStick[Controller][Stick][DireStick] == false && fStick > STICK_DED)
		{
			nStickTime[Controller][Stick][DireStick]++;

			if (nStickTime[Controller][Stick][DireStick] >= REPEAT_TIME)
			{
				nStickTime[Controller][Stick][DireStick] = 0;
				bOldStick[Controller][Stick][DireStick] = true;
			}
			return false;
		}
		else
		{
			bOldStick[Controller][Stick][DireStick] = false;
			nStickTime[Controller][Stick][DireStick] = 0;
			return false;
		}
		break;
	}
	return false;
}

//----------------------
//�X���C�_�[��
//----------------------
float GetdJoySlider(int nSlider, CONTROLLER Controller)
{
	float fSlider = 0.0f;

	fSlider = (float)g_djoykeyState[Controller].rglSlider[nSlider];

	fSlider /= STICK_NUM;

	fSlider += DSTICK_NUM;

	return fSlider;
}

//------------------------
//�n�b�g�X�C�b�`
//------------------------
bool GetdJoyPov(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (fPov==fData)
			{
				return true;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	return (fPov == fData) ? true : false;
}

//-------------------------
//
//-------------------------
bool GetdJoyPovTrigger(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				return true;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				return false;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		return true;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		return false;
	}
	else
	{
		return false;
	}
}

//-----------------------
//
//-----------------------
bool GetdJoyPovRelease(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };
	static int PovPutTime[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				if (PovPutTime[i][nPov][(int)(fData / POV_NUM)] >= RELEASE_TIME)
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					return true;
				}
				else
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					return false;
				}
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				return false;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
			{
				PovPutTime[i][nPov][(int)(fData / POV_NUM)]++;
				return false;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		if (PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] >= RELEASE_TIME)
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			return true;
		}
		else
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			return false;
		}
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		return false;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
	{
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)]++;
		return false;
	}
	else
	{
		return false;
	}
}

//---------------------
//
//---------------------
bool GetdJoyPovRepeat(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };
	static int PovPutTime[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
			{
				return true;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				PovPutTime[i][nPov][(int)(fData / POV_NUM)]++;

				if (PovPutTime[i][nPov][(int)(fData / POV_NUM)] >= REPEAT_TIME)
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				}
				return false;
			}
			else
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
			}


		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
	{
		return true;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)]++;

		if (PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] >= REPEAT_TIME)
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		}
		return false;
	}
	else
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
		return false;
	}
}

//-------------
//�U��
//-------------
void VibratedController(LONG lMotorPower, CONTROLLER Controller)
{
	// �t�H�[�X�t�B�[�h�o�b�N�̃G�t�F�N�g���쐬����
	DIEFFECT effect;
	ZeroMemory(&effect, sizeof(DIEFFECT));
	effect.dwSize = sizeof(DIEFFECT);
	effect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	effect.dwDuration = INFINITE;  // �����ɑ����U��
	effect.dwGain = DI_FFNOMINALMAX;
	effect.dwTriggerButton = DIEB_NOTRIGGER;

	// ���ƕ����̐ݒ�
	DWORD rgdwAxes[1] = { DIJOFS_X };  // X���̐U��
	LONG rglDirection[1] = { 0 };      // �����͐������i0�j
	effect.cAxes = 1;                  // 1��
	effect.rgdwAxes = rgdwAxes;        // �g�p���鎲
	effect.rglDirection = rglDirection;// �U���̕���

	DICONSTANTFORCE cf;
	cf.lMagnitude = lMotorPower;  // �U���̋��� (0 - DI_FFNOMINALMAX)

	effect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
	effect.lpvTypeSpecificParams = &cf;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (SUCCEEDED(g_DevdJoypad[i]->CreateEffect(GUID_ConstantForce, &effect, &g_djoyEffect[i], NULL)))
			{
				if (FAILED(g_djoyEffect[i]->Start(1, 0)))
				{
					g_djoyEffect[i]->Release();
				}
			}
		}
	}
	else if (SUCCEEDED(g_DevdJoypad[Controller]->CreateEffect(GUID_ConstantForce, &effect, &g_djoyEffect[Controller], NULL)))
	{
		if (FAILED(g_djoyEffect[Controller]->Start(1, 0)))
		{
			g_djoyEffect[Controller]->Release();
		}
	}
}

//---------
//�U��
//---------
void VibratedControllerStop(CONTROLLER Controller)
{
	if (Controller==CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (g_djoyEffect[i] != NULL)
			{
				g_djoyEffect[i]->Stop();
				g_djoyEffect[i]->Release();
				g_djoyEffect[i] = NULL;
			}
		}
	}
	else if (g_djoyEffect[Controller] != NULL)
	{
		g_djoyEffect[Controller]->Stop();
		g_djoyEffect[Controller]->Release();
		g_djoyEffect[Controller] = NULL;
	}
}


//------------------------
//
//�U��
//
//------------------------
void SetVibrate(float fPower, CONTROLLER Controller)
{
	static bool bVibate = false;
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	if (g_DcontrollerNum>=(Controller+1)||Controller==CONTROLLER_MAX)
	{
		if (bVibate)
		{
			if (Controller == CONTROLLER_MAX)
			{
				for (int i = 0; i < g_DcontrollerNum; i++)
				{
					if (XInputGetState((CONTROLLER)i, &state) == ERROR_SUCCESS)
					{
						VibrateController((WORD)(VIBRATION_MAX * fPower), (WORD)(VIBRATION_MAX * fPower), (CONTROLLER)i);
					}
					else
					{
						VibratedControllerStop((CONTROLLER)i);
					}
				}
			}
			else
			{
				if (XInputGetState(Controller, &state) == ERROR_SUCCESS)
				{
					VibrateController((WORD)(VIBRATION_MAX * fPower), (WORD)(VIBRATION_MAX * fPower), Controller);
				}
				else
				{
					VibratedControllerStop(Controller);
				}
			}
			bVibate = false;
		}
		else
		{
			if (Controller == CONTROLLER_MAX)
			{
				for (int i = 0; i < g_DcontrollerNum; i++)
				{
					if (XInputGetState(i, &state) == ERROR_SUCCESS)
					{
						VibrateController((WORD)(VIBRATION_MAX * fPower), (WORD)(VIBRATION_MAX * fPower), (CONTROLLER)i);
					}
					else
					{
						VibratedController((LONG)(DI_FFNOMINALMAX * fPower), (CONTROLLER)i);
						bVibate = true;
					}
				}
			}

			if (XInputGetState(Controller, &state) == ERROR_SUCCESS)
			{
				VibrateController((WORD)(VIBRATION_MAX * fPower), (WORD)(VIBRATION_MAX * fPower), Controller);
			}
			else
			{
				VibratedController((LONG)(DI_FFNOMINALMAX * fPower), Controller);
				bVibate = true;
			}

		}
	}
}

//--------------------------------
//�ڑ��m�F
//--------------------------------

//-----------
//Xinput
//-----------
bool IsXInputControllerConnected(CONTROLLER Controller)
{
	XINPUT_STATE joykeyState[CONTROLLER_MAX];

	if (Controller==CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (XInputGetState(i, &joykeyState[i]) == ERROR_SUCCESS)
			{
				return true;
			}
		}

		return false;
	}

	return XInputGetState(Controller, &joykeyState[Controller]) == ERROR_SUCCESS;
}

//------------
//dinput
//------------
bool IsDirectInputControllerConnected(CONTROLLER Controller)
{
	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (g_DevdJoypad[i] != NULL)
			{
				return true;
			}
		}

		return false;
	}

	return g_DevdJoypad[Controller] != NULL;
}

//------------------
//�R���g���[���[��
//------------------
char *ControllerName(CONTROLLER Controller)
{
	return &g_ConName[Controller][0];
}

//------------------
//�R���g���[���[��
//------------------
int ControllerNum(CONTYPE Contype)
{
	if (Contype==CONTYPE_X)
	{
		return g_XcontrollerNum;
	}
	else
	{
		return g_DcontrollerNum;
	}
}

//---------------------------
//�f�o�C�X���擾
//---------------------------
BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	static int nControllerNum = 0;
	static int XcontrollerNum = 0;

	if (g_bUnCon)
	{
		nControllerNum = 0;
		XcontrollerNum = 0;
		g_DcontrollerNum = 0;
		g_XcontrollerNum = 0;
		g_bX = false;
		g_bUnCon = false;
		//�G�t�F�N�g�f�o�C�X�̔j��
		VibratedControllerStop(CONTROLLER_MAX);
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			//�L�[�{�[�h�f�o�C�X�̔j��
			if (g_DevdJoypad[i] != NULL)
			{
				g_DevdJoypad[i]->Unacquire();
				g_DevdJoypad[i]->Release();
				g_DevdJoypad[i] = NULL;
			}
		}
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			g_ConName[i][0] = '\0';
		}
	}

	if ((nControllerNum+XcontrollerNum)<=CONTROLLER_MAX)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		g_pDiInstance[nControllerNum] = pdidInstance;

		if (XInputGetState(XcontrollerNum, &state) == ERROR_SUCCESS)
		{
			if (SUCCEEDED(g_pinput->CreateDevice(pdidInstance->guidInstance, &g_DevdJoypad[XcontrollerNum], NULL)))
			{//�C���v�b�g�f�o�C�X�̍쐬
				HINSTANCE hInstanse;
				HWND hWnd;

				hInstanse = GethInstanse();
				hWnd = GethWnd();

				//�t�H�[�}�b�g�쐬
				g_DevdJoypad[nControllerNum]->SetDataFormat(&c_dfDIJoystick);

				g_DevdJoypad[nControllerNum]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE));

				g_DevdJoypad[nControllerNum]->Acquire();//�A�N�Z�X�����̎擾

				strcpy(&g_ConName[nControllerNum][0], pdidInstance->tszProductName);
				XcontrollerNum++;
			}
		}
		else if (SUCCEEDED(g_pinput->CreateDevice(pdidInstance->guidInstance, &g_DevdJoypad[nControllerNum + XNum()], NULL)))
		{//�C���v�b�g�f�o�C�X�̍쐬
			HINSTANCE hInstanse;
			HWND hWnd;

			hInstanse = GethInstanse();
			hWnd = GethWnd();

			//�t�H�[�}�b�g�쐬
			g_DevdJoypad[nControllerNum + XNum()]->SetDataFormat(&c_dfDIJoystick);

			g_DevdJoypad[nControllerNum + XNum()]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE));

			g_DevdJoypad[nControllerNum + XNum()]->Acquire();//�A�N�Z�X�����̎擾

			strcpy(&g_ConName[nControllerNum + XNum()][0], pdidInstance->tszProductName);
			nControllerNum++;
		}
	}
	g_DcontrollerNum = nControllerNum+XcontrollerNum;

	// �񋓂𑱍s
	return DIENUM_CONTINUE;
}

//----------
//
//----------
int XNum(void)
{
	int XNum = 0;
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (XInputGetState(i, &state) == ERROR_SUCCESS)
		{
			XNum++;
		}
	}
	return XNum;
}

LPDIRECTINPUT8 GetInput(void)
{
	return g_pinput;
}

void SetUnCon(bool bUnCon)
{
	g_bUnCon = bUnCon;
}

//-----------------------
//�R���g���[���[�Ǘ�������
//-----------------------
void InputReSet(void)
{
	g_bUnCon = false;
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		g_ConName[i][0] = '\0';
	}
	g_XcontrollerNum = 0;
	g_DcontrollerNum = 0;
    g_bX = false;
	//�G�t�F�N�g�f�o�C�X�̔j��
	VibratedControllerStop(CONTROLLER_MAX);
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		//�L�[�{�[�h�f�o�C�X�̔j��
		if (g_DevdJoypad[i] != NULL)
		{
			g_DevdJoypad[i]->Unacquire();
			g_DevdJoypad[i]->Release();
			g_DevdJoypad[i] = NULL;
		}
	}
}

//------------------------------------------------
//�R���g���[���[���擾
//------------------------------------------------
const DIDEVICEINSTANCE* GetDiInstance(CONTROLLER Controller)
{
	return g_pDiInstance[Controller];
}

//----------------------------------------------
//Xinput�R���g���[���[���擾
//----------------------------------------------
XINPUT_STATE GetXstate(CONTROLLER Controller)
{
	return g_joykeyState[Controller];
}

//----------------------------------------------
//dinput�R���g���[���[���擾
//----------------------------------------------
DIJOYSTATE Getdstate(CONTROLLER Controller)
{
	return g_djoykeyState[Controller];
}