//------------------------------------------
//
//�Q�[�����C���̏���[Pause.cpp]
//Author fuma sato
//
//------------------------------------------

#include"pause.h"
#include"game.h"
#include"fade.h"
#include"input.h"
#include"sound.h"
#include"cursor.h"

#define SELECT_WIDTH (512)
#define SELECT_HEIGHT (128)

typedef enum
{
	SELECT_RESTART = 0,
	SELECT_RETRY,
	SELECT_BACK,
	SELECT_EXIT,
	SELECT_MAX
}SELECT;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTexturePause[SELECT_MAX] = { NULL };//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;//�o�b�t�@�̃|�C���^
SELECT g_SelectNew = SELECT_RESTART;

//--------------------
//����������
//--------------------
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^
	D3DXVECTOR3 posSelect;//�X�R�A�̈ʒu

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * SELECT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_RESTART,
		&g_apTexturePause[0]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_RETRY,
		&g_apTexturePause[1]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_BACKTITLE,
		&g_apTexturePause[2]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EXIT,
		&g_apTexturePause[3]
	);

	posSelect = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SELECT_HEIGHT * 1.5f, 0.0f);

	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	for (int i = 0; i < SELECT_MAX; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[1].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[2].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[3].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`��
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += VT_MAX;

	}

	g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

	PlaySound(SOUND_LABEL_BGM);
}

//------------------
//�I������
//------------------
void UninitPause(void)
{
	for (int i = 0; i < SELECT_MAX; i++)
	{
		//�e�N�X�`���̔j��
		if (g_apTexturePause[i] != NULL)
		{
			g_apTexturePause[i]->Release();
			g_apTexturePause[i] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//--------------
//�X�V����
//--------------
void UpdatePause(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^

	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	pVtx += VT_MAX * g_SelectNew;

	//�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

	if (GetKeyboradTrigger(DIK_BACK) == true || GetJoykeyTrigger(JOYKEY_B, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_RIGHT) == true)
	{
		SetGameState(GAMESTATE_NORMAL);
		PlaySound(SOUND_LABEL_BGM4);
	}
	else if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_A, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_SENTER) == true)
	{
		FADE fade;
		switch (g_SelectNew)
		{
		case SELECT_RESTART:
			SetGameState(GAMESTATE_NORMAL);
			PlaySound(SOUND_LABEL_BGM4);
			break;
		case SELECT_RETRY:
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				//�T�E���h
				StopSound(SOUND_LABEL_BGM4);
				//�ؑ�
				SetFade(MODE_GAME);
			}
			break;
		case SELECT_BACK:
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				//�T�E���h
				StopSound(SOUND_LABEL_BGM4);
				//�ؑ�
				SetFade(MODE_TITLE);
			}
			break;
		case SELECT_EXIT:
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				//�T�E���h
				StopSound(SOUND_LABEL_BGM4);
				//�ؑ�
				HWND hWnd;
				hWnd = GethWnd();
				PostMessage(hWnd, WM_QUIT, 0, 0);
			}
			break;
		}
	}
	else if (GetMouseTrigger(MOUSE_LEFT) == true)
	{
		CURSOR* pCursor;
		pCursor = GetCursorIn();

		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

		for (int i = 0; i < SELECT_MAX; i++)
		{
			if (pCursor->pos.x >= pVtx[0].pos.x && pCursor->pos.x <= pVtx[3].pos.x && pCursor->pos.y >= pVtx[0].pos.y && pCursor->pos.y <= pVtx[3].pos.y)
			{
				FADE fade;
				switch (i)
				{
				case SELECT_RESTART:
					SetGameState(GAMESTATE_NORMAL);
					PlaySound(SOUND_LABEL_BGM4);
					break;
				case SELECT_RETRY:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound(SOUND_LABEL_BGM4);
						//�ؑ�
						SetFade(MODE_GAME);
					}
					break;
				case SELECT_BACK:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound(SOUND_LABEL_BGM4);
						//�ؑ�
						SetFade(MODE_TITLE);
					}
					break;
				case SELECT_EXIT:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound(SOUND_LABEL_BGM4);
						//�ؑ�
						HWND hWnd;
						hWnd = GethWnd();
						PostMessage(hWnd, WM_QUIT, 0, 0);
					}
					break;
				}
			}
			pVtx += VT_MAX;
		}

		g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	}
	else if (GetKeyboradTrigger(DIK_UP) == true || GetKeyboradTrigger(DIK_W) == true || GetJoykeyTrigger(JOYKEY_UP, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_UP, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() > 0.0f)
	{
		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

		pVtx += VT_MAX * g_SelectNew;

		//�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

		switch (g_SelectNew)
		{
		case SELECT_RESTART:
			g_SelectNew = SELECT_EXIT;
			break;
		case SELECT_RETRY:
			g_SelectNew = SELECT_RESTART;
			break;
		case SELECT_BACK:
			g_SelectNew = SELECT_RETRY;
			break;
		case SELECT_EXIT:
			g_SelectNew = SELECT_BACK;
			break;
		}

		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

		pVtx += VT_MAX * g_SelectNew;

		//�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	}
	else if (GetKeyboradTrigger(DIK_DOWN) == true || GetKeyboradTrigger(DIK_S) == true || GetJoykeyTrigger(JOYKEY_DOWN, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() < 0.0f)
	{
		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

		pVtx += VT_MAX * g_SelectNew;

		//�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

		switch (g_SelectNew)
		{
		case SELECT_RESTART:
			g_SelectNew = SELECT_RETRY;
			break;
		case SELECT_RETRY:
			g_SelectNew = SELECT_BACK;
			break;
		case SELECT_BACK:
			g_SelectNew = SELECT_EXIT;
			break;
		case SELECT_EXIT:
			g_SelectNew = SELECT_RESTART;
			break;
		}
		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

		pVtx += VT_MAX * g_SelectNew;

		//�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	}

	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{
			if (GetdJoykeyTrigger(ELEKEY_BACK, (CONTROLLER)i) == true)
			{
				SetGameState(GAMESTATE_NORMAL);
				PlaySound(SOUND_LABEL_BGM4);
			}
			else if (GetdJoykeyTrigger(ELEKEY_A, (CONTROLLER)i) == true)
			{
				FADE fade;
				switch (g_SelectNew)
				{
				case SELECT_RESTART:
					SetGameState(GAMESTATE_NORMAL);
					PlaySound(SOUND_LABEL_BGM4);
					break;
				case SELECT_RETRY:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound(SOUND_LABEL_BGM4);
						//�ؑ�
						SetFade(MODE_GAME);
					}
					break;
				case SELECT_BACK:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound(SOUND_LABEL_BGM4);
						//�ؑ�
						SetFade(MODE_TITLE);
					}
					break;
				case SELECT_EXIT:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound(SOUND_LABEL_BGM4);
						//�ؑ�
						HWND hWnd;
						hWnd = GethWnd();
						PostMessage(hWnd, WM_QUIT, 0, 0);
					}
					break;
				}
			}
			else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
			{
				g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * g_SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

				switch (g_SelectNew)
				{
				case SELECT_RESTART:
					g_SelectNew = SELECT_EXIT;
					break;
				case SELECT_RETRY:
					g_SelectNew = SELECT_RESTART;
					break;
				case SELECT_BACK:
					g_SelectNew = SELECT_RETRY;
					break;
				case SELECT_EXIT:
					g_SelectNew = SELECT_BACK;
					break;
				}

				g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * g_SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
			}
			else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
			{
				VERTEX_2D* pVtx;//���_���|�C���^

				g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * g_SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

				switch (g_SelectNew)
				{
				case SELECT_RESTART:
					g_SelectNew = SELECT_RETRY;
					break;
				case SELECT_RETRY:
					g_SelectNew = SELECT_BACK;
					break;
				case SELECT_BACK:
					g_SelectNew = SELECT_EXIT;
					break;
				case SELECT_EXIT:
					g_SelectNew = SELECT_RESTART;
					break;
				}
				g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * g_SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		{
			if (GetdJoykeyTrigger(PSKEY_BACK, (CONTROLLER)i) == true)
			{
				SetGameState(GAMESTATE_NORMAL);
				PlaySound(SOUND_LABEL_BGM4);
			}
			else if (GetdJoykeyTrigger(PSKEY_CI, (CONTROLLER)i) == true)
			{
				FADE fade;
				switch (g_SelectNew)
				{
				case SELECT_RESTART:
					SetGameState(GAMESTATE_NORMAL);
					PlaySound(SOUND_LABEL_BGM4);
					break;
				case SELECT_RETRY:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound(SOUND_LABEL_BGM4);
						//�ؑ�
						SetFade(MODE_GAME);
					}
					break;
				case SELECT_BACK:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound(SOUND_LABEL_BGM4);
						//�ؑ�
						SetFade(MODE_TITLE);
					}
					break;
				case SELECT_EXIT:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound(SOUND_LABEL_BGM4);
						//�ؑ�
						HWND hWnd;
						hWnd = GethWnd();
						PostMessage(hWnd, WM_QUIT, 0, 0);
					}
					break;
				}
			}
			else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
			{
				g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * g_SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

				switch (g_SelectNew)
				{
				case SELECT_RESTART:
					g_SelectNew = SELECT_EXIT;
					break;
				case SELECT_RETRY:
					g_SelectNew = SELECT_RESTART;
					break;
				case SELECT_BACK:
					g_SelectNew = SELECT_RETRY;
					break;
				case SELECT_EXIT:
					g_SelectNew = SELECT_BACK;
					break;
				}

				g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * g_SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
			}
			else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
			{
				g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * g_SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

				switch (g_SelectNew)
				{
				case SELECT_RESTART:
					g_SelectNew = SELECT_RETRY;
					break;
				case SELECT_RETRY:
					g_SelectNew = SELECT_BACK;
					break;
				case SELECT_BACK:
					g_SelectNew = SELECT_EXIT;
					break;
				case SELECT_EXIT:
					g_SelectNew = SELECT_RESTART;
					break;
				}
				g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * g_SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
			}
			else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
			{
				if (GetdJoykeyTrigger(NINKEY_�|, (CONTROLLER)i) == true)
				{
					SetGameState(GAMESTATE_NORMAL);
					PlaySound(SOUND_LABEL_BGM4);
				}
				else if (GetdJoykeyTrigger(NINKEY_A, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (g_SelectNew)
					{
					case SELECT_RESTART:
						SetGameState(GAMESTATE_NORMAL);
						PlaySound(SOUND_LABEL_BGM4);
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM4);
							//�ؑ�
							SetFade(MODE_GAME);
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM4);
							//�ؑ�
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM4);
							//�ؑ�
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * g_SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (g_SelectNew)
					{
					case SELECT_RESTART:
						g_SelectNew = SELECT_EXIT;
						break;
					case SELECT_RETRY:
						g_SelectNew = SELECT_RESTART;
						break;
					case SELECT_BACK:
						g_SelectNew = SELECT_RETRY;
						break;
					case SELECT_EXIT:
						g_SelectNew = SELECT_BACK;
						break;
					}

					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * g_SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
				{
					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * g_SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (g_SelectNew)
					{
					case SELECT_RESTART:
						g_SelectNew = SELECT_RETRY;
						break;
					case SELECT_RETRY:
						g_SelectNew = SELECT_BACK;
						break;
					case SELECT_BACK:
						g_SelectNew = SELECT_EXIT;
						break;
					case SELECT_EXIT:
						g_SelectNew = SELECT_RESTART;
						break;
					}
					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * g_SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
			}
			else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
			{
				if (GetdJoykeyTrigger(DKEY_BACK, (CONTROLLER)i) == true)
				{
					SetGameState(GAMESTATE_NORMAL);
					PlaySound(SOUND_LABEL_BGM4);
				}
				else if (GetdJoykeyTrigger(DKEY_A, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (g_SelectNew)
					{
					case SELECT_RESTART:
						SetGameState(GAMESTATE_NORMAL);
						PlaySound(SOUND_LABEL_BGM4);
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM4);
							//�ؑ�
							SetFade(MODE_GAME);
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM4);
							//�ؑ�
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound(SOUND_LABEL_BGM4);
							//�ؑ�
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * g_SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (g_SelectNew)
					{
					case SELECT_RESTART:
						g_SelectNew = SELECT_EXIT;
						break;
					case SELECT_RETRY:
						g_SelectNew = SELECT_RESTART;
						break;
					case SELECT_BACK:
						g_SelectNew = SELECT_RETRY;
						break;
					case SELECT_EXIT:
						g_SelectNew = SELECT_BACK;
						break;
					}

					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * g_SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, (CONTROLLER)i))
				{
					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * g_SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (g_SelectNew)
					{
					case SELECT_RESTART:
						g_SelectNew = SELECT_RETRY;
						break;
					case SELECT_RETRY:
						g_SelectNew = SELECT_BACK;
						break;
					case SELECT_BACK:
						g_SelectNew = SELECT_EXIT;
						break;
					case SELECT_EXIT:
						g_SelectNew = SELECT_RESTART;
						break;
					}
					g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * g_SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					g_pVtxBuffPause->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
			}
		}
	}
}

//-------------------
//�`�揈��
//-------------------
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < SELECT_MAX; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTexturePause[i]);

		//�w�i�̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//�^�C�v
			i * VT_MAX,//�n�܂�̔ԍ�
			2//�|���S���̌�
		);
	}
}