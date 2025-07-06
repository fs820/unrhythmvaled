//---------------------------------------
//
//�e�\������[block.cpp]
//Author fuma sato
//
//---------------------------------------

#include"block.h"
#include"player.h"
#include"game.h"
#include"input.h"
#include"scroll.h"
#include"particle.h"

//�O���[�o��
LPDIRECT3DTEXTURE9 g_apTextureBlock = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;//�o�b�t�@�̃|�C���^
Block g_aBlock[MAX_BLOCK];//�e�̏��

//-----------------
//�e�̏���������
//-----------------
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//�f�o�C�X���擾
	pDevice = GetDevice();

	for (int i = 0; i < MAX_BLOCK; i++)
	{//��񏉊���
		g_aBlock[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[i].fWidth = 0.0f;
		g_aBlock[i].fHeight = 0.0f;
		g_aBlock[i].bDisp = true;
		g_aBlock[i].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		BLOCK_TEX,
		&g_apTextureBlock
	);

	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BLOCK; i++)
	{//�e�̐������J��Ԃ�
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2, g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2, g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2, g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2, g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2, 0.0f);

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
	g_pVtxBuffBlock->Unlock();//�o�b�t�@�̃A�����b�N
}

//----------------
//�e�̏I������
//----------------
void UninitBlock(void)
{
	//�e�N�X�`���̔j��
	if (g_apTextureBlock != NULL)
	{
		g_apTextureBlock->Release();
		g_apTextureBlock = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//----------------
//�e�̍X�V����
//----------------
void UpdateBlock(void)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_BLOCK; i++)
	{//�u���b�N�̐������J��Ԃ�
		if (g_aBlock[i].bUse == true)
		{//�u���b�N���g�p����Ă���

			g_aBlock[i].pos.x += g_aBlock[i].move.x;
			g_aBlock[i].pos.y += g_aBlock[i].move.y;

			g_aBlock[i].pos -= GetScroll();

			if (g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 < 0 || g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 > SCREEN_WIDTH || g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 < 0 || g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 > SCREEN_HEIGHT)
			{
				g_aBlock[i].bDisp = false;
			}
			else
			{
				g_aBlock[i].bDisp = true;
			}

			g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

			//���W�ݒ�
			pVtx += i * VT_MAX;
			pVtx[0].pos = D3DXVECTOR3(g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2, g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2, g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2, g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2, g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2, 0.0f);

			//�e�N�X�`��
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aBlock[i].fWidth / BLOCK_HEIGHT_DEF, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aBlock[i].fHeight / BLOCK_HEIGHT_DEF);
			pVtx[3].tex = D3DXVECTOR2(g_aBlock[i].fWidth / BLOCK_HEIGHT_DEF, g_aBlock[i].fHeight / BLOCK_HEIGHT_DEF);

			g_pVtxBuffBlock->Unlock();//�o�b�t�@�̃A�����b�N
		}
	}
}

//----------------
//�e�̕`�揈��
//----------------
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		if (g_aBlock[i].bUse && g_aBlock[i].bDisp)
		{//�u���b�N���g�p����Ă���
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureBlock);
			//�u���b�N�̕`��
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
//���ʏ�e
//-------------------
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		if (g_aBlock[i].bUse == false)
		{//�u���b�N���g�p����Ă��Ȃ�
			g_aBlock[i].pos = pos;
			g_aBlock[i].fWidth = fWidth;
			g_aBlock[i].fHeight = fHeight;

			g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

			//���W�ݒ�
			pVtx += i * VT_MAX;
			pVtx[0].pos = D3DXVECTOR3(g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2, g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2, g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2, g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2, g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2, 0.0f);

			//�e�N�X�`��
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aBlock[i].fWidth / BLOCK_HEIGHT_DEF, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aBlock[i].fHeight / BLOCK_HEIGHT_DEF);
			pVtx[3].tex = D3DXVECTOR2(g_aBlock[i].fWidth / BLOCK_HEIGHT_DEF, g_aBlock[i].fHeight / BLOCK_HEIGHT_DEF);

			g_pVtxBuffBlock->Unlock();//�o�b�t�@�̃A�����b�N

			g_aBlock[i].bDisp = true;
			g_aBlock[i].bUse = true;

			break;
		}
	}
}

//--------------------
//�擾
//--------------------
Block* GetBlock(void)
{
	return &g_aBlock[0];
}

//-------------------
//�����蔻��
//-------------------
bool CllisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight, Block** pBlock, bool bPlayer)
{
	bool bLanding = false;//���n���Ă��邩
	bool bY = fabs(pMove->y) > fabs(pMove->x);

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		if (g_aBlock[i].bUse)
		{
			if (bY)
			{
				if (pPos->y > g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 && pPos->y - fHeight < g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2)
				{
					if (pPosOld->x + fWidth / 2 <= g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 && pPos->x + fWidth / 2 > g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x)
					{
						pPos->x = g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x - fWidth / 2;
						pMove->x = 0.0f;
					}
					else if (pPosOld->x - fWidth / 2 >= g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 && pPos->x - fWidth / 2 < g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x)
					{
						pPos->x = g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x + fWidth / 2;
						pMove->x = 0.0f;
					}
				}
				if (pPos->x + fWidth / 2 > g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 && pPos->x - fWidth / 2 < g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2)
				{
					if (pPosOld->y <= g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 && pPos->y > g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y)
					{
						bLanding = true;
						pPos->y = g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y;
						pMove->y = 0.0f;

						if ((i == 117 || i == 149 || i == 150 || i == 152) && bPlayer)
						{
							g_aBlock[117].move.x = BLOCK_SPEEDX;
							g_aBlock[149].move.x = BLOCK_SPEEDX;
							g_aBlock[150].move.x = BLOCK_SPEEDX;
							g_aBlock[152].move.x = BLOCK_SPEEDX;
						}
						else if (i == 151 && bPlayer)
						{
							g_aBlock[i].move.y = BLOCK_SPEEDY;
						}

						if (bPlayer)
						{
							if (i >= 64 && i <= 69)
							{
								SetScroll(D3DXVECTOR3(SCROLL_X, 0.0f, 0.0f));
							}
							if ((i >= 71 && i <= 77) || (i >= 108 && i <= 112))
							{
								SetScroll(D3DXVECTOR3(SCROLL_X, SCROLL_Y, 0.0f));
							}
							if (i == 132 || i == 134 || i == 108 || i == 112 || (i >= 137 && i <= 139) || i == 141 || i == 142)
							{
								SetScroll(D3DXVECTOR3(SCROLL_X, 0.0f, 0.0f));
							}
							if (i == 151)
							{
								SetScroll(D3DXVECTOR3(0.0f, SCROLL_Y * 3.0f, 0.0f));
							}
							if (i == 158 || i == 163 || i == 167)
							{
								SetScroll(D3DXVECTOR3(SCROLL_X, 0.0f, 0.0f));
							}
						}

						if (pBlock != NULL)
						{
							*pBlock = &g_aBlock[i];
						}
					}
					else if (pPosOld->y - fHeight >= g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 && pPos->y - fHeight < g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y)
					{
						pPos->y = g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y + fHeight;
						pMove->y = 0.0f;
					}
				}
			}
			else
			{
				if (pPos->x + fWidth / 2 > g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 && pPos->x - fWidth / 2 < g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2)
				{
					if (pPosOld->y <= g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 && pPos->y > g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y)
					{
						bLanding = true;
						pPos->y = g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y;
						pMove->y = 0.0f;

						if ((i == 117 || i == 149 || i == 150 || i == 152) && bPlayer)
						{
							g_aBlock[117].move.x = BLOCK_SPEEDX;
							g_aBlock[149].move.x = BLOCK_SPEEDX;
							g_aBlock[150].move.x = BLOCK_SPEEDX;
							g_aBlock[152].move.x = BLOCK_SPEEDX;
						}
						else if (i == 151 && bPlayer)
						{
							g_aBlock[i].move.y = BLOCK_SPEEDY;
						}

						if (bPlayer)
						{
							if (i >= 64 && i <= 69)
							{
								SetScroll(D3DXVECTOR3(SCROLL_X, 0.0f, 0.0f));
							}
							if ((i >= 71 && i <= 77) || (i >= 108 && i <= 112))
							{
								SetScroll(D3DXVECTOR3(SCROLL_X, SCROLL_Y, 0.0f));
							}
							if (i == 132 || i == 134 || i == 108 || i == 112 || (i >= 137 && i <= 139) || i == 141 || i == 142)
							{
								SetScroll(D3DXVECTOR3(SCROLL_X, 0.0f, 0.0f));
							}
							if (i == 151)
							{
								SetScroll(D3DXVECTOR3(0.0f, SCROLL_Y * 3.0f, 0.0f));
							}
							if (i == 158 || i == 163 || i == 167)
							{
								SetScroll(D3DXVECTOR3(SCROLL_X, 0.0f, 0.0f));
							}
						}

						if (pBlock != NULL)
						{
							*pBlock = &g_aBlock[i];
						}
					}
					else if (pPosOld->y - fHeight >= g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 && pPos->y - fHeight < g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y)
					{
						pPos->y = g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y + fHeight;
						pMove->y = 0.0f;
					}
				}
				if (pPos->y > g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 && pPos->y - fHeight < g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2)
				{
					if (pPosOld->x + fWidth / 2 <= g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 && pPos->x + fWidth / 2 > g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x)
					{
						pPos->x = g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x - fWidth / 2;
						pMove->x = 0.0f;
					}
					else if (pPosOld->x - fWidth / 2 >= g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 && pPos->x - fWidth / 2 < g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x)
					{
						pPos->x = g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x + fWidth / 2;
						pMove->x = 0.0f;
					}
				}
			}
		}
	}

	return bLanding;
}

//-------------------
//�����蔻��
//-------------------
void CllisionBlockA(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fLength, bool* pUse)
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		if (g_aBlock[i].bUse)
		{
			if (pPos->x + fLength / 2 > g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 && pPos->x - fLength / 2 < g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2)
			{
				if (pPosOld->y - fLength / 2 <= g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 && pPos->y - fLength / 2 > g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y)
				{
					*pUse = false;
					SetParticle(*pPos, fLength);
				}
				else if (pPosOld->y - fLength / 2 >= g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 && pPos->y - fLength / 2 < g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y)
				{
					*pUse = false;
					SetParticle(*pPos, fLength);
				}
			}
			if (pPos->y - fLength / 2 > g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 && pPos->y - fLength / 2 < g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2)
			{
				if (pPosOld->x + fLength / 2 <= g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 && pPos->x + fLength / 2 > g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x)
				{
					*pUse = false;
					SetParticle(*pPos, fLength);
				}
				else if (pPosOld->x - fLength / 2 >= g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 && pPos->x - fLength / 2 < g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x)
				{
					*pUse = false;
					SetParticle(*pPos, fLength);
				}
			}
		}
	}
}

////-------------------
////�����蔻��
////-------------------
//bool CllisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight, Block** pBlock, bool bPlayer)
//{
//	bool bLanding = false;//���n���Ă��邩
//	bool bY = fabs(pMove->y) > fabs(pMove->x);
//	float Xmove = (pPos->x - pPosOld->x) * CLLISION_INA, Ymove = (pPos->y - pPosOld->y) * CLLISION_INA;
//
//	for (int i = 0; i < MAX_BLOCK; i++)
//	{
//		if (g_aBlock[i].bUse)
//		{
//			if (bY)
//			{
//				for (int i2 = 1; i2 <= CLLISION_NUM; i2++)
//				{
//					if (pPosOld->y + Ymove * i2 + g_aBlock[i].move.y - GetScroll().y > g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y && pPosOld->y + Ymove * i2 - fHeight + g_aBlock[i].move.y - GetScroll().y < g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y)
//					{
//						if (pPosOld->x + fWidth / 2 + g_aBlock[i].move.x - GetScroll().x <= g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x && pPosOld->x + Xmove * i2 + fWidth / 2 + g_aBlock[i].move.x - GetScroll().x > g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x)
//						{
//							pPos->x = g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x - fWidth / 2;
//							pMove->x = 0.0f;
//							break;
//						}
//						else if (pPosOld->x - fWidth / 2 + g_aBlock[i].move.x - GetScroll().x >= g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x && pPosOld->x + Xmove * i2 - fWidth / 2 + g_aBlock[i].move.x - GetScroll().x < g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x)
//						{
//							pPos->x = g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x + fWidth / 2;
//							pMove->x = 0.0f;
//							break;
//						}
//					}
//				}
//				for (int i2 = 1; i2 <= CLLISION_NUM; i2++)
//				{
//					if (pPosOld->x + Xmove * i2 + fWidth / 2 + g_aBlock[i].move.x - GetScroll().x > g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 && pPosOld->x + Xmove * i2 - fWidth / 2 + g_aBlock[i].move.x - GetScroll().x < g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2)
//					{
//						if (pPosOld->y + g_aBlock[i].move.y - GetScroll().y <= g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y && pPosOld->y + Ymove * i2 + g_aBlock[i].move.y - GetScroll().y > g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y)
//						{
//							bLanding = true;
//							if (bPlayer)
//							{
//								if (i >= 64 && i <= 69)
//								{
//									SetScroll(D3DXVECTOR3(SCROLL_X, 0.0f, 0.0f));
//								}
//								if ((i >= 71 && i <= 77) || (i >= 108 && i <= 112))
//								{
//									SetScroll(D3DXVECTOR3(SCROLL_X, SCROLL_Y, 0.0f));
//								}
//								if (i == 132 || i == 134 || i == 108 || i == 112 || (i >= 137 && i <= 139) || i == 141 || i == 142)
//								{
//									SetScroll(D3DXVECTOR3(SCROLL_X, 0.0f, 0.0f));
//								}
//								if (i == 151)
//								{
//									SetScroll(D3DXVECTOR3(0.0f, SCROLL_Y * 3.0f, 0.0f));
//								}
//								if (i == 158 || i == 163 || i == 167)
//								{
//									SetScroll(D3DXVECTOR3(SCROLL_X, 0.0f, 0.0f));
//								}
//							}
//							pPos->y = g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y;
//							pMove->y = 0.0f;
//
//							if ((i == 117 || i == 149 || i == 150 || i == 152) && bPlayer)
//							{
//								g_aBlock[117].move.x = BLOCK_SPEEDX;
//								g_aBlock[149].move.x = BLOCK_SPEEDX;
//								g_aBlock[150].move.x = BLOCK_SPEEDX;
//								g_aBlock[152].move.x = BLOCK_SPEEDX;
//							}
//							else if (i == 151 && bPlayer)
//							{
//								g_aBlock[i].move.y = BLOCK_SPEEDY;
//							}
//
//							if (pBlock != NULL)
//							{
//								*pBlock = &g_aBlock[i];
//							}
//							break;
//						}
//						else if (pPosOld->y - fHeight + g_aBlock[i].move.y - GetScroll().y >= g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y && pPosOld->y + Ymove * i2 - fHeight + g_aBlock[i].move.y - GetScroll().y < g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y)
//						{
//							pPos->y = g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y + fHeight;
//							pMove->y = 0.0f;
//							break;
//						}
//					}
//				}
//			}
//			else
//			{
//				for (int i2 = 1; i2 <= CLLISION_NUM; i2++)
//				{
//					if (pPosOld->x + Xmove * i2 + fWidth / 2 + g_aBlock[i].move.x - GetScroll().x > g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 && pPosOld->x + Xmove * i2 - fWidth / 2 + g_aBlock[i].move.x - GetScroll().x < g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2)
//					{
//						if (pPosOld->y + g_aBlock[i].move.y - GetScroll().y <= g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y && pPosOld->y + Ymove * i2 + g_aBlock[i].move.y - GetScroll().y > g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y)
//						{
//							bLanding = true;
//							if (bPlayer)
//							{
//								if (i >= 64 && i <= 69)
//								{
//									SetScroll(D3DXVECTOR3(SCROLL_X, 0.0f, 0.0f));
//								}
//								if ((i >= 71 && i <= 77) || (i >= 108 && i <= 112))
//								{
//									SetScroll(D3DXVECTOR3(SCROLL_X, SCROLL_Y, 0.0f));
//								}
//								if (i == 132 || i == 134 || i == 108 || i == 112 || (i >= 137 && i <= 139) || i == 141 || i == 142)
//								{
//									SetScroll(D3DXVECTOR3(SCROLL_X, 0.0f, 0.0f));
//								}
//								if (i == 151)
//								{
//									SetScroll(D3DXVECTOR3(0.0f, SCROLL_Y * 3.0f, 0.0f));
//								}
//								if (i == 158 || i == 163 || i == 167)
//								{
//									SetScroll(D3DXVECTOR3(SCROLL_X, 0.0f, 0.0f));
//								}
//							}
//							pPos->y = g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y;
//							pMove->y = 0.0f;
//
//							if ((i == 117 || i == 149 || i == 150 || i == 152) && bPlayer)
//							{
//								g_aBlock[117].move.x = BLOCK_SPEEDX;
//								g_aBlock[149].move.x = BLOCK_SPEEDX;
//								g_aBlock[150].move.x = BLOCK_SPEEDX;
//								g_aBlock[152].move.x = BLOCK_SPEEDX;
//							}
//							else if (i == 151 && bPlayer)
//							{
//								g_aBlock[i].move.y = BLOCK_SPEEDY;
//							}
//
//							if (pBlock != NULL)
//							{
//								*pBlock = &g_aBlock[i];
//							}
//							break;
//						}
//						else if (pPosOld->y - fHeight + g_aBlock[i].move.y - GetScroll().y >= g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y && pPosOld->y + Ymove * i2 - fHeight + g_aBlock[i].move.y - GetScroll().y < g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y)
//						{
//							pPos->y = g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y + fHeight;
//							pMove->y = 0.0f;
//							break;
//						}
//					}
//				}
//				for (int i2 = 1; i2 <= CLLISION_NUM; i2++)
//				{
//					if (pPosOld->y + Ymove * i2 + g_aBlock[i].move.y - GetScroll().y > g_aBlock[i].pos.y - g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y && pPosOld->y + Ymove * i2 - fHeight + g_aBlock[i].move.y - GetScroll().y < g_aBlock[i].pos.y + g_aBlock[i].fHeight / 2 + g_aBlock[i].move.y - GetScroll().y)
//					{
//						if (pPosOld->x + fWidth / 2 + g_aBlock[i].move.x - GetScroll().x <= g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x && pPosOld->x + Xmove * i2 + fWidth / 2 + g_aBlock[i].move.x - GetScroll().x > g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x)
//						{
//							pPos->x = g_aBlock[i].pos.x - g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x - fWidth / 2;
//							pMove->x = 0.0f;
//							break;
//						}
//						else if (pPosOld->x - fWidth / 2 + g_aBlock[i].move.x - GetScroll().x >= g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x && pPosOld->x + Xmove * i2 - fWidth / 2 + g_aBlock[i].move.x - GetScroll().x < g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x)
//						{
//							pPos->x = g_aBlock[i].pos.x + g_aBlock[i].fWidth / 2 + g_aBlock[i].move.x - GetScroll().x + fWidth / 2;
//							pMove->x = 0.0f;
//							break;
//						}
//					}
//				}
//			}
//		}
//	}
//	return bLanding;
//}
