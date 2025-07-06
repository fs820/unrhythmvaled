//---------------------------------------
//
//�G�t�F�N�g�\������[effect.cpp]
//Author fuma sato
//
//---------------------------------------

#include"effect.h"
#include"attack.h"
#include"player.h"
#include"enemy.h"
#include"scroll.h"

//�e�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fRadius;
	float fAngle;
	D3DXVECTOR3 rot;
	EFFECTTYPE Type;
	int nLife;//����
	bool bLeft;
	bool bUse;//�g�p���Ă��邩�ǂ���
}Effect;

//�O���[�o��
LPDIRECT3DTEXTURE9 g_apTextureEffect[EFFECTTYPE_MAX] = { NULL };//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;//�o�b�t�@�̃|�C���^
Effect g_aEffect[MAX_EFFECT];//�e�̏��
int g_nLifeDef[MAX_EFFECT] = { 0 };

//-----------------
//�e�̏���������
//-----------------
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//�f�o�C�X���擾
	pDevice = GetDevice();

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		g_aEffect[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[i].fRadius = 0.0f;
		g_aEffect[i].fAngle = 0.0f;
		g_aEffect[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[i].Type = EFFECTTYPE_NORMAL;
		g_aEffect[i].nLife = 0;
		g_aEffect[i].bLeft = false;
		g_aEffect[i].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT,
		&g_apTextureEffect[0]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_SHORT,
		&g_apTextureEffect[1]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_SHORT2,
		&g_apTextureEffect[2]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_SHORT3,
		&g_apTextureEffect[3]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_SHORT4,
		&g_apTextureEffect[4]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_SHORT5,
		&g_apTextureEffect[5]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_LONG,
		&g_apTextureEffect[6]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_LONG2,
		&g_apTextureEffect[7]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_LONG3,
		&g_apTextureEffect[8]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_LONG4,
		&g_apTextureEffect[9]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_LONG5,
		&g_apTextureEffect[10]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_SP,
		&g_apTextureEffect[11]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_SP2,
		&g_apTextureEffect[12]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_SP3,
		&g_apTextureEffect[13]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_SP4,
		&g_apTextureEffect[14]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_SP5,
		&g_apTextureEffect[15]
	);

	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius * sinf(g_aEffect[i].rot.z - (D3DX_PI - g_aEffect[i].fAngle)), g_aEffect[i].pos.y + g_aEffect[i].fRadius * cosf(g_aEffect[i].rot.z - (D3DX_PI - g_aEffect[i].fAngle)), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius * sinf(g_aEffect[i].rot.z - (D3DX_PI + g_aEffect[i].fAngle)), g_aEffect[i].pos.y + g_aEffect[i].fRadius * cosf(g_aEffect[i].rot.z - (D3DX_PI + g_aEffect[i].fAngle)), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius * sinf(g_aEffect[i].rot.z - g_aEffect[i].fAngle), g_aEffect[i].pos.y + g_aEffect[i].fRadius * cosf(g_aEffect[i].rot.z - g_aEffect[i].fAngle), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius * sinf(g_aEffect[i].rot.z + g_aEffect[i].fAngle), g_aEffect[i].pos.y + g_aEffect[i].fRadius * cosf(g_aEffect[i].rot.z + g_aEffect[i].fAngle), 0.0f);

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

		pVtx += VT_MAX;//�|���S��1���i�߂�
	}
	g_pVtxBuffEffect->Unlock();//�o�b�t�@�̃A�����b�N
}

//----------------
//�e�̏I������
//----------------
void UninitEffect(void)
{
	for (int i = 0; i < EFFECTTYPE_MAX; i++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureEffect[i] != NULL)
		{
			g_apTextureEffect[i]->Release();
			g_apTextureEffect[i] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//----------------
//�e�̍X�V����
//----------------
void UpdateEffect(void)
{
	VERTEX_2D* pVtx;
	static D3DXVECTOR3 Oldpos[MAX_EFFECT];

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (g_aEffect[i].bUse == true)
		{//�e���g�p����Ă���
			Oldpos[i] = g_aEffect[i].pos;

			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

			//���W�ݒ�
			pVtx += i * VT_MAX;//�|���S��1���i�߂�
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius * sinf(g_aEffect[i].rot.z - (D3DX_PI - g_aEffect[i].fAngle)), g_aEffect[i].pos.y + g_aEffect[i].fRadius * cosf(g_aEffect[i].rot.z - (D3DX_PI - g_aEffect[i].fAngle)), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius * sinf(g_aEffect[i].rot.z - (D3DX_PI + g_aEffect[i].fAngle)), g_aEffect[i].pos.y + g_aEffect[i].fRadius * cosf(g_aEffect[i].rot.z - (D3DX_PI + g_aEffect[i].fAngle)), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius * sinf(g_aEffect[i].rot.z - g_aEffect[i].fAngle), g_aEffect[i].pos.y + g_aEffect[i].fRadius * cosf(g_aEffect[i].rot.z - g_aEffect[i].fAngle), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius * sinf(g_aEffect[i].rot.z + g_aEffect[i].fAngle), g_aEffect[i].pos.y + g_aEffect[i].fRadius * cosf(g_aEffect[i].rot.z + g_aEffect[i].fAngle), 0.0f);

			BYTE alpha = (BYTE)(((float)g_aEffect[i].nLife) / ((float)g_nLifeDef[i]) * 255.0f);
			pVtx[0].col = (pVtx[0].col & 0x00FFFFFF) | (alpha << 24);
			pVtx[1].col = (pVtx[1].col & 0x00FFFFFF) | (alpha << 24);
			pVtx[2].col = (pVtx[2].col & 0x00FFFFFF) | (alpha << 24);
			pVtx[3].col = (pVtx[3].col & 0x00FFFFFF) | (alpha << 24);

			g_pVtxBuffEffect->Unlock();//�o�b�t�@�̃A�����b�N

			g_aEffect[i].pos.x += g_aEffect[i].move.x;
			g_aEffect[i].pos.y += g_aEffect[i].move.y;

			g_aEffect[i].pos -= GetScroll();

			g_aEffect[i].nLife--;
			g_aEffect[i].fRadius *= ((float)g_aEffect[i].nLife) / ((float)g_nLifeDef[i]);

			if (g_aEffect[i].nLife <= 0)
			{
				g_aEffect[i].bUse = false;
			}

			if (g_aEffect[i].pos.x <= 0 || g_aEffect[i].pos.x >= SCREEN_WIDTH || g_aEffect[i].pos.y <= 0 || g_aEffect[i].pos.y >= SCREEN_HEIGHT)
			{
				g_aEffect[i].bUse = false;
			}
		}
	}
}

//----------------
//�e�̕`�揈��
//----------------
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���Z�u�����h�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // �A���t�@�u�����h��L����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); // ���Z�u�����h
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // �\�[�X�u�����h: �A���t�@
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);     // �f�X�g�u�����h: ���Z�p

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (g_aEffect[i].bUse == true)
		{//�e���g�p����Ă���
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEffect[(int)g_aEffect[i].Type]);

			//�e�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				i * VT_MAX,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}

	// ���Z�u�����h�̃��Z�b�g�i�ʏ�̃A���t�@�u�����h�ɖ߂��j
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); // ���̃u�����h�I�y���[�V�����ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // �\�[�X�u�����h: �ʏ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // �f�X�g�u�����h: �ʏ�
}

//-------------------
//���ʏ�e
//-------------------
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nLife, EFFECTTYPE Type, bool bLeft)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (g_aEffect[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aEffect[i].pos = pos;
			g_aEffect[i].move = move;
			g_aEffect[i].fRadius = fRadius;
			g_aEffect[i].fAngle = atan2f(g_aEffect[i].fRadius, g_aEffect[i].fRadius);
			g_aEffect[i].rot = rot;
			g_aEffect[i].col = col;
			g_aEffect[i].Type = Type;
			if (g_aEffect[i].Type == EFFECTTYPE_NORMAL)
			{
				g_aEffect[i].fRadius *= EFFECT_INA;
			}

			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�ݒ�
			pVtx += i * VT_MAX;//�|���S��1���i�߂�
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius * sinf(g_aEffect[i].rot.z - (D3DX_PI - g_aEffect[i].fAngle)), g_aEffect[i].pos.y + g_aEffect[i].fRadius * cosf(g_aEffect[i].rot.z - (D3DX_PI - g_aEffect[i].fAngle)), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius * sinf(g_aEffect[i].rot.z - (D3DX_PI + g_aEffect[i].fAngle)), g_aEffect[i].pos.y + g_aEffect[i].fRadius * cosf(g_aEffect[i].rot.z - (D3DX_PI + g_aEffect[i].fAngle)), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius * sinf(g_aEffect[i].rot.z - g_aEffect[i].fAngle), g_aEffect[i].pos.y + g_aEffect[i].fRadius * cosf(g_aEffect[i].rot.z - g_aEffect[i].fAngle), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[i].pos.x + g_aEffect[i].fRadius * sinf(g_aEffect[i].rot.z + g_aEffect[i].fAngle), g_aEffect[i].pos.y + g_aEffect[i].fRadius * cosf(g_aEffect[i].rot.z + g_aEffect[i].fAngle), 0.0f);


			pVtx[0].col = g_aEffect[i].col;
			pVtx[1].col = g_aEffect[i].col;
			pVtx[2].col = g_aEffect[i].col;
			pVtx[3].col = g_aEffect[i].col;

			if (bLeft && rot.z == 0.0f)
			{
				//�e�N�X�`��
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			else
			{
				//�e�N�X�`��
				pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
			}

			g_pVtxBuffEffect->Unlock();//�o�b�t�@�̃A�����b�N

			g_aEffect[i].nLife = nLife;
			g_nLifeDef[i] = g_aEffect[i].nLife;
			g_aEffect[i].bLeft = bLeft;
			g_aEffect[i].bUse = true;

			break;
		}
	}
}