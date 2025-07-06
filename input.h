//---------------------------------------
//
//���͏����̒�`�E�錾[input.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _INPUT_H_
#define _INPUT_H_
#include"main.h"

#define STICK_DED (0.8f)//�f�b�h�]�[��
#define VIBRATION_MAX (65535)//�o�C�u���[�V�����l

//POV
#define POV_UP (0.0f)//��
#define POV_RIGHTUP (4500.0f)//�E��
#define POV_RIGHT (9000.0f)//�E
#define POV_RIGHTDOWN (13500.0f)//�E��
#define POV_DOWN (18000.0f)//��
#define POV_LEFTDOWN (22500.0f)//����
#define POV_LEFT (27000.0f)//��
#define POV_LEFTUP (31500.0f)//����

//�}�E�X�{�^���̎��
typedef enum
{
	MOUSE_LEFT = 0,
	MOUSE_RIGHT,
	MOUSE_SENTER,
	MOUSE_B1,
	MOUSE_B2,
	MOUSE_B3,
	MOUSE_B4,
	MOUSE_B5,
	MOUSE_MAX
}MOUSE;

//�R���g���[���[�̎��
typedef enum
{
	CONTROLLER_1 = 0,
	CONTROLLER_2,
	CONTROLLER_3,
	CONTROLLER_4,
	CONTROLLER_MAX
}CONTROLLER;

//�R���g���[���[�^�C�v
typedef enum
{
	CONTYPE_X = 0,
	CONTYPE_D,
	CONTYPE_MAX
}CONTYPE;

//X�p�b�h�{�^���̎��
typedef enum
{
	JOYKEY_UP = 0,
	JOYKEY_DOWN,
	JOYKEY_LEFT,
	JOYKEY_RIGHT,
	JOYKEY_START,
	JOYKEY_BACK,
	JOYKEY_L3,
	JOYKEY_R3,
	JOYKEY_LB,
	JOYKEY_RB,
	JOYKEY_LT,
	JOYKEY_RT,
	JOYKEY_A,
	JOYKEY_B,
	JOYKEY_X,
	JOYKEY_Y,
	JOYKEY_MAX
}JOYKEY;

//ELE�p�b�h�{�^���̎��
typedef enum
{
	ELEKEY_A = 0,
	ELEKEY_B,
	ELEKEY_X,
	ELEKEY_Y,
	ELEKEY_LB,
	ELEKEY_RB,
	ELEKEY_LT,
	ELEKEY_RT,
	ELEKEY_L3,
	ELEKEY_R3,
	ELEKEY_BACK,
	ELEKEY_START,
	ELEKEY_SENTER,
	ELEKEY_MAX
}ELEKEY;

//PS�p�b�h�{�^���̎��
typedef enum
{
	PSKEY_SQ = 0,
	PSKEY_CR,
	PSKEY_CI,
	PSKEY_TRA,
	PSKEY_LB,
	PSKEY_RB,
	PSKEY_LT,
	PSKEY_RT,
	PSKEY_BACK,
	PSKEY_START,
	PSKEY_L3,
	PSKEY_R3,
	PSKEY_SENTER,
	PSKEY_PAD,
	PSKEY_MAX
}PSKEY;

//NIN�p�b�h�{�^���̎��
typedef enum
{
	NINKEY_B = 0,
	NINKEY_A,
	NINKEY_Y,
	NINKEY_X,
	NINKEY_LB,
	NINKEY_RB,
	NINKEY_LT,
	NINKEY_RT,
	NINKEY_�|,
	NINKEY_�{,
	NINKEY_L3,
	NINKEY_R3,
	NINKEY_HOME,
	NINKEY_CAP,
	NINKEY_MAX
}NINKEY;

//D�p�b�h�{�^���̎��
typedef enum
{
	DKEY_A = 0,
	DKEY_B,
	DKEY_X,
	DKEY_Y,
	DKEY_LB,
	DKEY_RB,
	DKEY_LT,
	DKEY_RT,
	DKEY_L3,
	DKEY_R3,
	DKEY_BACK,
	DKEY_START,
	DKEY_SENTER,
	DKEY_B1,
	DKEY_B2,
	DKEY_MAX
}DKEY;

//�p�b�h�X�e�B�b�N�̎��
typedef enum
{
	STICK_LEFT = 0,
	STICK_RIGHT,
	STICK_MAX
}STICK;

//�X�e�B�b�N�����̎��
typedef enum
{
	DIRESTICK_UP = 0,
	DIRESTICK_DOWN,
	DIRESTICK_LEFT,
	DIRESTICK_RIGHT,
	DIRESTICK_MAX
}DIRESTICK;

//�L�[�{�[�h
HRESULT InitKeyboard(HINSTANCE hInstanse, HWND hWnd);//����������
void UninitKeyborad(void);//�I������
void UpdateKeyborad(void);//�X�V����
bool GetKeyboradPress(int nkey);//�L�[��������
bool GetKeyboradTrigger(int nkey);//�L�[���͏���
bool GetKeyboradRelease(int nkey);//�L�[��������
bool GetKeyboradRepeat(int nkey);//�L�[����������

//�}�E�X
HRESULT InitMouse(HINSTANCE hInstanse, HWND hWnd);//����������
void UninitMouse(void);//�I������
void UpdateMouse(void);//�X�V����
bool GetMousePress(int nkey);//�L�[��������
bool GetMouseTrigger(int nkey);//�L�[���͏���
bool GetMouseRelease(int nkey);//�L�[��������
bool GetMouseRepeat(int nkey);//�L�[����������
float *GetMouseMove(void);//�}�E�X����
float GetMouseWheel(void);//�z�C�[������

//Xinput
HRESULT InitJoypad(void);//����������
void UninitJoypad(void);//�I������
void UpdateJoypad(void);//�X�V����
bool GetJoykeyPress(JOYKEY key, CONTROLLER Controller);//�L�[��������
bool GetJoykeyTrigger(JOYKEY key, CONTROLLER Controller);//�L�[���͏���
bool GetJoykeyRelease(JOYKEY key, CONTROLLER Controller);//�L�[��������
bool GetJoykeyRepeat(JOYKEY key, CONTROLLER Controller);//�L�[����������
float *GetJoyStick(STICK Stick, CONTROLLER Controller);//�X�e�B�b�N����
bool JoyStickPress(DIRESTICK DireStick ,STICK Stick, CONTROLLER Controller);//�X�e�B�b�N��������
bool JoyStickTrigger(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//�X�e�B�b�N���͏���
bool JoyStickRelease(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//�X�e�B�b�N��������
bool JoyStickRepeat(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//�X�e�B�b�N����������
void VibrateController(WORD wLeftMotorSpeed, WORD wRightMotorSpeed, CONTROLLER Controller);//�o�C�u���[�V��������

//dinput�p�b�h
HRESULT InitdJoypad(HINSTANCE hInstanse, HWND hWnd);//����������
void UninitdJoypad(void);//�I������
void UpdatedJoypad(void);//�X�V����
bool GetdJoykeyPress(int nkey, CONTROLLER Controller);//�L�[��������
bool GetdJoykeyTrigger(int nkey, CONTROLLER Controller);//�L�[���͏���
bool GetdJoykeyRelease(int nkey, CONTROLLER Controller);//�L�[��������
bool GetdJoykeyRepeat(int nkey, CONTROLLER Controller);//�L�[����������
float* GetdJoyStick(STICK Stick, CONTROLLER Controller);//�X�e�B�b�N����
bool dJoyStickPress(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//�X�e�B�b�N����
bool dJoyStickTrigger(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//�X�e�B�b�N���͏���
bool dJoyStickRelease(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//�X�e�B�b�N��������
bool dJoyStickRepeat(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//�X�e�B�b�N����������
float GetdJoySlider(int nSlider, CONTROLLER Controller);//�X���C�_�[������
bool GetdJoyPov(float fData,int nPov, CONTROLLER Controller);//POV��������
bool GetdJoyPovTrigger(float fData, int nPov, CONTROLLER Controller);//POV���͏���
bool GetdJoyPovRelease(float fData, int nPov, CONTROLLER Controller);//POV��������
bool GetdJoyPovRepeat(float fData, int nPov, CONTROLLER Controller);//POV����������
void VibratedController(LONG lMotorPower, CONTROLLER Controller);//�o�C�u���[�V��������
void VibratedControllerStop(CONTROLLER Controller);//�o�C�u���[�V������~����

void SetVibrate(float fPower, CONTROLLER Controller);//�o�C�u���[�V�����ݒ�

//�ڑ��m�F
bool IsXInputControllerConnected(CONTROLLER Controller);//Xinput�ڑ��m�F
bool IsDirectInputControllerConnected(CONTROLLER Controller);//dinpit�ڑ��m�F
char *ControllerName(CONTROLLER Controller);//�R���g���[���[�C���X�^���X�l�[���擾
int ControllerNum(CONTYPE Contype);//�R���g���[���[���擾
BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);//�R���g���[���[�񋓃R�[���o�b�N�֐�
int XNum(void);//Xinput�ڑ����擾

LPDIRECTINPUT8 GetInput(void);//���̓|�C���^�擾
void SetUnCon(bool bUnCon);

void InputReSet(void);

const DIDEVICEINSTANCE* GetDiInstance(CONTROLLER Controller);
XINPUT_STATE GetXstate(CONTROLLER Controller);
DIJOYSTATE Getdstate(CONTROLLER Controller);

#endif _INPUT_H_

