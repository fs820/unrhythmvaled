//---------------------------------------
//
//�A�C�e���\������[item.cpp]
//Author fuma sato
//
//---------------------------------------

#include"item.h"
#include"player.h"
#include"score.h"
#include"scroll.h"

//�O���[�o��
LPDIRECT3DTEXTURE9 g_apTextureItem = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;//�o�b�t�@�̃|�C���^
Item g_aItem[MAX_ITEM];//�A�C�e���̏��
bool g_bGoal = false;

//-----------------
//�A�C�e���̏���������
//-----------------
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//�f�o�C�X���擾
	pDevice = GetDevice();

	for (int i = 0; i < MAX_ITEM; i++)
	{
		g_aItem[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aItem[i].fRadius = 0.0f;
		g_aItem[i].Type = TYPE_MAX;
		g_aItem[i].bDisp = false;
		g_aItem[i].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		ITEM_TEX,
		&g_apTextureItem
	);

	g_bGoal = false;

	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);

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
	g_pVtxBuffItem->Unlock();//�o�b�t�@�̃A�����b�N
}

//----------------
//�A�C�e���̏I������
//----------------
void UninitItem(void)
{
	//�e�N�X�`���̔j��
	if (g_apTextureItem != NULL)
	{
		g_apTextureItem->Release();
		g_apTextureItem = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//----------------
//�A�C�e���̍X�V����
//----------------
void UpdateItem(void)
{
	VERTEX_2D* pVtx;
	static D3DXVECTOR3 Oldpos[MAX_ITEM];

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (g_aItem[i].bUse == true)
		{//�A�C�e�����g�p����Ă���
			Oldpos[i] = g_aItem[i].pos;

			Player* pPlayer;
			pPlayer = GetPlayer();
			if (pPlayer->pos.x > g_aItem[i].pos.x - g_aItem[i].fRadius && pPlayer->pos.x< g_aItem[i].pos.x + g_aItem[i].fRadius && pPlayer->pos.y> g_aItem[i].pos.y - g_aItem[i].fRadius && pPlayer->pos.y < g_aItem[i].pos.y + g_aItem[i].fRadius)
			{
				switch (g_aItem[i].Type)
				{
				case TYPE_NORMAL:
					AddScore(SCORE_ITEM);
					break;
				case TYPE_GOAL:
					AddScore(SCORE_GOAL);
					g_bGoal = true;
					break;
				}

				g_aItem[i].bUse = false;
			}

			g_aItem[i].pos.x += g_aItem[i].move.x;
			g_aItem[i].pos.y += g_aItem[i].move.y;

			g_aItem[i].pos -= GetScroll();

			g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

			//���W�ݒ�
			pVtx += i * VT_MAX;//�|���S��1���i�߂�
			pVtx[0].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);

			g_pVtxBuffItem->Unlock();//�o�b�t�@�̃A�����b�N

			if (g_aItem[i].pos.x <= 0 || g_aItem[i].pos.x >= SCREEN_WIDTH || g_aItem[i].pos.y <= 0 || g_aItem[i].pos.y >= SCREEN_HEIGHT)
			{
				g_aItem[i].bDisp = false;
			}
			else
			{
				g_aItem[i].bDisp = true;
			}
		}
	}
}

//----------------
//�A�C�e���̕`�揈��
//----------------
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (g_aItem[i].bUse == true&& g_aItem[i].bDisp == true)
		{//�A�C�e�����g�p����Ă���

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureItem);

			//�A�C�e���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				i * VT_MAX,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}
}

//-------------------
//�A�C�e���ݒu
//-------------------
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE Type)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (g_aItem[i].bUse == false)
		{//�A�C�e�����g�p����Ă��Ȃ�
			g_aItem[i].pos = pos;
			g_aItem[i].move = move;
			g_aItem[i].fRadius = ITEM_SIZE;
			g_aItem[i].Type = Type;

			switch (g_aItem[i].Type)
			{
			case TYPE_NORMAL:
				g_aItem[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			case TYPE_GOAL:
				g_aItem[i].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				break;
			}

			g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�ݒ�
			pVtx += i * VT_MAX;//�|���S��1���i�߂�
			pVtx[0].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);

			pVtx[0].col = g_aItem[i].col;
			pVtx[1].col = g_aItem[i].col;
			pVtx[2].col = g_aItem[i].col;
			pVtx[3].col = g_aItem[i].col;

			g_pVtxBuffItem->Unlock();//�o�b�t�@�̃A�����b�N

			g_aItem[i].bDisp = true;
			g_aItem[i].bUse = true;

			break;
		}
	}
}

//------------------
//�擾
//------------------
Item* GetItem(void)
{
	return &g_aItem[0];
}

//--------------------
//�S�[��
//--------------------
bool Goal(void)
{
	return g_bGoal;
}