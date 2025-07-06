//---------------------------------------
//
//�J�[�\������[cursor.cpp]
//Author fuma sato
//
//---------------------------------------

#include"cursor.h"
#include"input.h"
#include"fade.h"
#include"game.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureCursor;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCursor = NULL;//�o�b�t�@�̃|�C���^
CURSOR g_Cursor;//�v���C���[���

//-------------------
//����������
//-------------------
void InitCursor(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCursor,
		NULL
	);

	g_Cursor.nCounterAnim = 0;
	g_Cursor.nPatternAnim = 0;
	g_Cursor.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);//�����ʒu
	g_Cursor.move = D3DXVECTOR3(CURSOR_SPEED_DEF, CURSOR_SPEED_DEF, CURSOR_SPEED_DEF);//�v���C���[�X�s�[�h
	g_Cursor.state = CURSORSTATE_NONE;
	g_Cursor.bUse = true;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_CURSOR,
		&g_pTextureCursor
	);

	g_pVtxBuffCursor->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	//���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Cursor.pos.x - CURSOR_WIDTH / 2, g_Cursor.pos.y - CURSOR_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_WIDTH / 2, g_Cursor.pos.y - CURSOR_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Cursor.pos.x - CURSOR_WIDTH / 2, g_Cursor.pos.y + CURSOR_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_WIDTH / 2, g_Cursor.pos.y + CURSOR_HEIGHT / 2, 0.0f);

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//�e�N�X�`��
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_CU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX_CU);
	pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_CU, UV_DEF / V_MAX_CU);

	g_pVtxBuffCursor->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
}

//------------------
//�I������
//------------------
void UninitCursor(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureCursor != NULL)
	{
		g_pTextureCursor->Release();
		g_pTextureCursor = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCursor != NULL)
	{
		g_pVtxBuffCursor->Release();
		g_pVtxBuffCursor = NULL;
	}
}

//-------------------
//�X�V����
//-------------------
void UpdateCursor(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	float* pMouseMove;
	float* Stick;
	float* dStick;
	FADE fade;
	MODE mode;
	GAMESTATE state;
	GAMESTATE Tstate;

	g_Cursor.move.x = 0;
	g_Cursor.move.y = 0;

	//�ړ�����
	pMouseMove = GetMouseMove();
	Stick = GetJoyStick(STICK_RIGHT, CONTROLLER_1);
	dStick = GetdJoyStick(STICK_RIGHT, CONTROLLER_1);
	g_Cursor.move.x += *pMouseMove * CURSOR_SPEED_X;
	g_Cursor.move.y += *(pMouseMove+1) * CURSOR_SPEED_Y;
	//g_Cursor.move.x += *Stick * CURSOR_SPEED_X;
	//g_Cursor.move.y += *(Stick + 1) * CURSOR_SPEED_Y;
	//g_Cursor.move.x += *dStick * CURSOR_SPEED_X;
	//g_Cursor.move.y += *(dStick + 1) * CURSOR_SPEED_Y;

	//�ʒu�̍X�V
	g_Cursor.pos.x += g_Cursor.move.x;
	g_Cursor.pos.y += g_Cursor.move.y;

	g_Cursor.nCounterAnim++;
	if ((g_Cursor.nCounterAnim % UP_TIME) == 0)//�X�V����
	{
		g_Cursor.nPatternAnim = (g_Cursor.nPatternAnim + 1) % (U_MAX_CU * V_MAX_CU);

		g_pVtxBuffCursor->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

	    //���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Cursor.pos.x - CURSOR_WIDTH / 2, g_Cursor.pos.y - CURSOR_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_WIDTH / 2, g_Cursor.pos.y - CURSOR_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Cursor.pos.x - CURSOR_WIDTH / 2, g_Cursor.pos.y + CURSOR_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_WIDTH / 2, g_Cursor.pos.y + CURSOR_HEIGHT / 2, 0.0f);

		//�e�N�X�`��
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_CU * g_Cursor.nPatternAnim, UV_DEF / V_MAX_CU * (g_Cursor.nPatternAnim / U_MAX_CU));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_CU * g_Cursor.nPatternAnim + UV_DEF / U_MAX_CU, UV_DEF / V_MAX_CU * (g_Cursor.nPatternAnim / U_MAX_CU));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_CU * g_Cursor.nPatternAnim, UV_DEF / V_MAX_CU * (g_Cursor.nPatternAnim / U_MAX_CU) + UV_DEF / V_MAX_CU);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_CU * g_Cursor.nPatternAnim + UV_DEF / U_MAX_CU, UV_DEF / V_MAX_CU * (g_Cursor.nPatternAnim / U_MAX_CU) + UV_DEF / V_MAX_CU);


		if (pVtx[3].pos.x >= SCREEN_WIDTH+CURSOR_WIDTH)//��ʉE�[
		{
			g_Cursor.pos.x = SCREEN_WIDTH + CURSOR_WIDTH;//��ʍ��[
		}
		else if (pVtx[0].pos.x <= -CURSOR_WIDTH)//��ʍ��[
		{
			g_Cursor.pos.x = -CURSOR_WIDTH;//��ʉE�[
		}

		if (pVtx[3].pos.y >= SCREEN_HEIGHT+CURSOR_HEIGHT)//��ʉ��[
		{
			g_Cursor.pos.y = SCREEN_HEIGHT + CURSOR_HEIGHT;//��ʏ�[
		}
		else if (pVtx[0].pos.y <= -CURSOR_HEIGHT)//��ʏ�[
		{
			g_Cursor.pos.y = -CURSOR_HEIGHT;//��ʉ��[
		}

		fade = GetFade();
		mode = GetMode();
		state = GetGameState();
		//Tstate = GetTutoState();
		switch (g_Cursor.state)
		{
		case CURSORSTATE_NONE:
			if (fade == FADE_NONE && ((mode != MODE_GAME&&mode!=MODE_TUTO && mode != MODE_VS) || state == GAMESTATE_PAUSE/* || Tstate == GAMESTATE_PAUSE*/) && mode != MODE_DEMO||Edit())
			{
			    g_Cursor.state = CURSORSTATE_NORMAL;
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
		case CURSORSTATE_NORMAL:
			if (fade != FADE_NONE || ((mode == MODE_GAME || mode == MODE_TUTO || mode == MODE_VS) && state != GAMESTATE_PAUSE && !Edit()/*&& Tstate != GAMESTATE_PAUSE*/) || mode == MODE_DEMO)
			{
				g_Cursor.state = CURSORSTATE_NONE;
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			}
			break;
		}

		g_pVtxBuffCursor->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	}
}

//------------------
//�`�揈��
//------------------
void DrawCursor(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

    //�f�o�C�X�̎擾
	pDevice = GetDevice();

	if (g_Cursor.bUse)
	{
		//���_�o�b�t�@
		pDevice->SetStreamSource(0, g_pVtxBuffCursor, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureCursor);

		//�v���C���[�̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//�^�C�v
			0,//�n�܂�̔ԍ�
			2//�|���S���̌�
		);
	}
}

//-------------------
//�o�b�t�@�̎擾
//-------------------
PDIRECT3DVERTEXBUFFER9 GetCursorBuffer(void)
{
	return g_pVtxBuffCursor;
}

//----------------------
//���̎擾
//----------------------
CURSOR* GetCursorIn(void)
{
	return &g_Cursor;
}

//----
//�g�p
//----
void CrCursor(bool bUse)
{
	g_Cursor.bUse = bUse;
}