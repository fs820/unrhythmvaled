//----------------------------------------
//
//�G�\������[enemy.cpp]
//Author fuma sato
//
//----------------------------------------
#include"enemy.h"
#include"score.h"
#include"particle.h"
#include"player.h"
#include"scroll.h"
#include"attack.h"

//�}�N����`
#define ENEMY_BULLET_INTER (10)
#define ENEMY_BULLET_SPEED (10.0f)
#define ENEMY_BULLET_SIZE (100.0f)

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = { NULL };//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;//�o�b�t�@�̃|�C���^
Enemy g_aEnemy[MAX_ENEMY];
int g_nNumEnemy = 0;

//-----------------------
//�G�̏���������
//-----------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX*MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		ENEMY_TEX,
		&g_apTextureEnemy[0]
	);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		ENEMY_TEX2,
		&g_apTextureEnemy[1]
	);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		ENEMY_TEX3,
		&g_apTextureEnemy[2]
	);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		ENEMY_TEX4,
		&g_apTextureEnemy[3]
	);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_aEnemy[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[i].Oldpos = g_aEnemy[i].pos;
		g_aEnemy[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[i].state = ENEMYSTATE_NORMAL;
		g_aEnemy[i].nLife = ENEMY_LIFE;
		g_aEnemy[i].nCounterState = 0;
		g_aEnemy[i].fLength = sqrtf(ENEMY_WIDTH * ENEMY_WIDTH + ENEMY_HEIGHT * ENEMY_HEIGHT) / 2.0f;
		g_aEnemy[i].fLengthDef = g_aEnemy[i].fLength;
		g_aEnemy[i].nType = 0;
		g_aEnemy[i].bMove = false;//�����Ă��邩
		g_aEnemy[i].bJump = false;//�W�����v���Ă��邩
		g_aEnemy[i].JumpCount = 0;
		g_aEnemy[i].bLeft = false;
		g_aEnemy[i].bDisp = true;
		g_aEnemy[i].bUse = false;
	}
	g_nNumEnemy = 0;

	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//���_���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - ENEMY_WIDTH / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 - ENEMY_HEIGHT * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + ENEMY_WIDTH / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 - ENEMY_HEIGHT * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - ENEMY_WIDTH / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 + ENEMY_HEIGHT * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + ENEMY_WIDTH / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 + ENEMY_HEIGHT * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);

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

	g_pVtxBuffEnemy->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
}

//---------------------
//�G�̏I������
//---------------------
void UninitEnemy(void)
{
	for (int i = 0; i < NUM_ENEMY; i++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureEnemy[i] != NULL)
		{
			g_apTextureEnemy[i]->Release();
			g_apTextureEnemy[i] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//-----------------------
//�G�̍X�V����
//-----------------------
void UpdateEnemy(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	static int aEnemyBulletCnt[MAX_ENEMY] = { 0 };

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_aEnemy[i].bUse == true)
		{
			//�ߋ��̈ʒu�̕ۑ�
			g_aEnemy[i].Oldpos = g_aEnemy[i].pos;

			float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
			switch (g_aEnemy[i].nType)
			{
			case 0:
				EnemyWidth = ENEMY_WIDTH;
				EnemyHeight = ENEMY_HEIGHT;
				break;
			case 1:
				EnemyWidth = ENEMY_WIDTH2;
				EnemyHeight = ENEMY_HEIGHT2;
				break;
			case 2:
				EnemyWidth = ENEMY_WIDTH3;
				EnemyHeight = ENEMY_HEIGHT3;
				break;
			case 3:
				EnemyWidth = ENEMY_WIDTH4;
				EnemyHeight = ENEMY_HEIGHT4;
				break;
			}

			if (g_aEnemy[i].bDisp)
			{
				switch (g_aEnemy[i].state)
				{
				case ENEMYSTATE_NORMAL:
					aEnemyBulletCnt[i]++;
					if (aEnemyBulletCnt[i] >= ENEMY_BULLET_INTER)
					{
						aEnemyBulletCnt[i] = 0;
						//�e�̐ݒ�
						int nRand = 0;
						switch (g_aEnemy[i].nType)
						{
						case 0:
							nRand = rand() % 3;
							switch (nRand)
							{
							case 0:
								if (!g_aEnemy[i].bJump || g_aEnemy[i].JumpCount > 0)
								{
									g_aEnemy[i].move.y = ENEMY_JUMP;
									g_aEnemy[i].JumpCount--;
								}
								break;
							case 1:
								nRand = rand() % (int)(ENEMY_MAX_SPEED * 200.0f + 1.0f);
								g_aEnemy[i].move.x += ((float)nRand / (ENEMY_MAX_SPEED * 2.0f)) - ENEMY_MAX_SPEED;
								break;
							case 2:
								SetShortAttack(D3DXVECTOR3(g_aEnemy[i].pos.x, g_aEnemy[i].pos.y - EnemyHeight / 2, g_aEnemy[i].pos.z), D3DX_PI, g_aEnemy[i].fLength, ATTACKTYPE_ENEMY, g_aEnemy[i].bLeft);
								break;
							}
							break;
						case 1:
							nRand = rand() % 3;
							switch (nRand)
							{
							case 0:
								if (!g_aEnemy[i].bJump || g_aEnemy[i].JumpCount > 0)
								{
									g_aEnemy[i].move.y = ENEMY_JUMP;
									g_aEnemy[i].JumpCount--;
								}
								break;
							case 1:
								nRand = rand() % (int)(ENEMY_MAX_SPEED * 200.0f + 1.0f);
								g_aEnemy[i].move.x += ((float)nRand / (ENEMY_MAX_SPEED * 2.0f)) - ENEMY_MAX_SPEED;
								break;
							case 2:
								SetLongAttack(D3DXVECTOR3(g_aEnemy[i].pos.x, g_aEnemy[i].pos.y - EnemyHeight / 2, g_aEnemy[i].pos.z), D3DX_PI, g_aEnemy[i].fLength, ATTACKTYPE_ENEMY, g_aEnemy[i].bLeft);
								break;
							}
							break;
						case 2:
							nRand = rand() % 5;
							switch (nRand)
							{
							case 0:
								if (!g_aEnemy[i].bJump || g_aEnemy[i].JumpCount > 0)
								{
									g_aEnemy[i].move.y = ENEMY_JUMP;
									g_aEnemy[i].JumpCount--;
								}
								break;
							case 1:
								nRand = rand() % (int)(ENEMY_MAX_SPEED * 200.0f + 1.0f);
								g_aEnemy[i].move.x += ((float)nRand / (ENEMY_MAX_SPEED * 2.0f)) - ENEMY_MAX_SPEED;
								break;
							case 2:
								SetShort5Attack(D3DXVECTOR3(g_aEnemy[i].pos.x, g_aEnemy[i].pos.y - EnemyHeight / 2, g_aEnemy[i].pos.z), D3DX_PI, g_aEnemy[i].fLength, ATTACKTYPE_ENEMY, g_aEnemy[i].bLeft);
								break;
							case 3:
								SetLong5Attack(D3DXVECTOR3(g_aEnemy[i].pos.x, g_aEnemy[i].pos.y - EnemyHeight / 2, g_aEnemy[i].pos.z), D3DX_PI, g_aEnemy[i].fLength, ATTACKTYPE_ENEMY, g_aEnemy[i].bLeft);
								break;
							case 4:
								SetSpAttack(D3DXVECTOR3(g_aEnemy[i].pos.x, g_aEnemy[i].pos.y - EnemyHeight / 2, g_aEnemy[i].pos.z), D3DX_PI, g_aEnemy[i].fLength, ATTACKTYPE_ENEMY, g_aEnemy[i].bLeft);
								break;
							}
							break;
						case 3:
							nRand = rand() % 100;
							if (nRand < 20)
							{
								if (!g_aEnemy[i].bJump || g_aEnemy[i].JumpCount > 0)
								{
									g_aEnemy[i].move.y = ENEMY_JUMP;
									g_aEnemy[i].JumpCount--;
								}
							}
							else if (nRand < 70)
							{
								nRand = rand() % (int)(ENEMY_MAX_SPEED * 200.0f + 1.0f);
								g_aEnemy[i].move.x += ((float)nRand / (ENEMY_MAX_SPEED * 2.0f)) - ENEMY_MAX_SPEED;
							}
							else if (nRand < 80)
							{
								SetSpAttack(D3DXVECTOR3(g_aEnemy[i].pos.x, g_aEnemy[i].pos.y - EnemyHeight / 2, g_aEnemy[i].pos.z), D3DX_PI, g_aEnemy[i].fLength, ATTACKTYPE_ENEMY, g_aEnemy[i].bLeft);
							}
							else if (nRand < 90)
							{
								SetSp2Attack(D3DXVECTOR3(g_aEnemy[i].pos.x, g_aEnemy[i].pos.y - EnemyHeight / 2, g_aEnemy[i].pos.z), D3DX_PI, g_aEnemy[i].fLength, ATTACKTYPE_ENEMY, g_aEnemy[i].bLeft);
							}
							else if (nRand < 95)
							{
								SetSp3Attack(D3DXVECTOR3(g_aEnemy[i].pos.x, g_aEnemy[i].pos.y - EnemyHeight / 2, g_aEnemy[i].pos.z), D3DX_PI, g_aEnemy[i].fLength, ATTACKTYPE_ENEMY, g_aEnemy[i].bLeft);
							}
							else
							{
								SetSp4Attack(D3DXVECTOR3(g_aEnemy[i].pos.x, g_aEnemy[i].pos.y - EnemyHeight / 2, g_aEnemy[i].pos.z), D3DX_PI, g_aEnemy[i].fLength, ATTACKTYPE_ENEMY, g_aEnemy[i].bLeft);
							}
							break;
						}
					}
					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[i].nCounterState--;
					if (g_aEnemy[i].nCounterState <= 0)
					{
						g_aEnemy[i].state = ENEMYSTATE_NORMAL;

						g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
						pVtx += i * VT_MAX;
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						g_pVtxBuffEnemy->Unlock();//�o�b�t�@�̃A�����b�N
					}
				}
			}

			if (g_aEnemy[i].move.x < 0.0f)
			{
				g_aEnemy[i].bLeft = true;
			}
			else
			{
				g_aEnemy[i].bLeft = false;
			}

			if (g_aEnemy[i].bJump)
			{
				g_aEnemy[i].move.x *= ENEMY_JUMP_MOVE_INA;
			}

			g_aEnemy[i].move.y += GRAVITY;

			//�ʒu�̍X�V
			g_aEnemy[i].pos.x += g_aEnemy[i].move.x;
			g_aEnemy[i].pos.y += g_aEnemy[i].move.y;

			g_aEnemy[i].pos -= GetScroll();

			g_aEnemy[i].bJump = !CllisionBlock(&g_aEnemy[i].pos, &g_aEnemy[i].Oldpos, &g_aEnemy[i].move, EnemyWidth * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef), EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef), &g_aEnemy->pBlock, false);

			if (!g_aEnemy[i].bJump)
			{
				g_aEnemy[i].JumpCount = ENEMY_JUMP_NUM;
			}
			else
			{
				if (g_aEnemy[i].JumpCount == ENEMY_JUMP_NUM)
				{
					g_aEnemy[i].JumpCount--;
				}
			}

			//�ړ��ʂ̍X�V(����)
			g_aEnemy[i].move.x += (ENEMY_SPEED_DEF - g_aEnemy[i].move.x) * ENEMY_INA;

			if (g_aEnemy[i].move.x > 0.0001f || g_aEnemy[i].move.x < -0.0001f)
			{
				g_aEnemy[i].bMove = true;
			}
			else
			{
				g_aEnemy[i].bMove = false;
			}

			Player* pPlayer;
			pPlayer = GetPlayer();
			if (g_aEnemy[i].pos.x + PLAYER_WIDTH * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 > pPlayer->pos.x - EnemyWidth / 2 && g_aEnemy[i].pos.x - PLAYER_WIDTH * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 < pPlayer->pos.x + EnemyWidth / 2)
			{
				if (g_aEnemy[i].Oldpos.y <= pPlayer->pos.y - EnemyHeight && g_aEnemy[i].pos.y > pPlayer->pos.y - EnemyHeight + BLOCK_SPEEDY - GetScroll().y + pPlayer->move.y)
				{
					g_aEnemy[i].bJump = false;
					g_aEnemy[i].pos.y = pPlayer->pos.y - EnemyHeight + BLOCK_SPEEDY - GetScroll().y + pPlayer->move.y;
					g_aEnemy[i].move.y = 0.0f;

					g_aEnemy[i].pos.x += pPlayer->move.x;
					g_aEnemy[i].pos.y += pPlayer->move.y;
				}
				else if (g_aEnemy[i].Oldpos.y - PLAYER_HEIGHT * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) >= pPlayer->pos.y + EnemyHeight && g_aEnemy[i].pos.y - PLAYER_HEIGHT * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) < pPlayer->pos.y + EnemyHeight + BLOCK_SPEEDY - GetScroll().y + pPlayer->move.y)
				{
					g_aEnemy[i].pos.y = pPlayer->pos.y + EnemyHeight + BLOCK_SPEEDY - GetScroll().y + pPlayer->move.y + PLAYER_HEIGHT * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef);
					g_aEnemy[i].move.y = 0.0f;
				}
			}
			if (g_aEnemy[i].pos.y > pPlayer->pos.y - EnemyHeight && g_aEnemy[i].pos.y - PLAYER_HEIGHT * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) < pPlayer->pos.y)
			{
				if (g_aEnemy[i].Oldpos.x + PLAYER_WIDTH * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 <= pPlayer->pos.x - EnemyWidth / 2 && g_aEnemy[i].pos.x + PLAYER_WIDTH * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 > pPlayer->pos.x - EnemyWidth / 2 + BLOCK_SPEEDX - GetScroll().x + pPlayer->move.x)
				{
					g_aEnemy[i].pos.x = pPlayer->pos.x - EnemyWidth / 2 + BLOCK_SPEEDX - GetScroll().x + pPlayer->move.x - PLAYER_WIDTH * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2;
					g_aEnemy[i].move.x = 0.0f;
				}
				else if (g_aEnemy[i].Oldpos.x - PLAYER_WIDTH * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 >= pPlayer->pos.x + EnemyWidth / 2 && g_aEnemy[i].pos.x - PLAYER_WIDTH * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 < pPlayer->pos.x + EnemyWidth / 2 + BLOCK_SPEEDX - GetScroll().x + pPlayer->move.x)
				{
					g_aEnemy[i].pos.x = pPlayer->pos.x + EnemyWidth / 2 + BLOCK_SPEEDX - GetScroll().x + pPlayer->move.x + PLAYER_WIDTH * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2;
					g_aEnemy[i].move.x = 0.0f;
				}
			}

			for (int i2 = 0; i2 < MAX_ENEMY; i2++)
			{
				if (g_aEnemy[i2].bUse)
				{
					float EnemyWidth2 = 0.0f, EnemyHeight2 = 0.0f;
					switch (g_aEnemy[i2].nType)
					{
					case 0:
						EnemyWidth2 = ENEMY_WIDTH;
						EnemyHeight2 = ENEMY_HEIGHT;
						break;
					case 1:
						EnemyWidth2 = ENEMY_WIDTH2;
						EnemyHeight2 = ENEMY_HEIGHT2;
						break;
					case 2:
						EnemyWidth2 = ENEMY_WIDTH3;
						EnemyHeight2 = ENEMY_HEIGHT3;
						break;
					case 3:
						EnemyWidth2 = ENEMY_WIDTH4;
						EnemyHeight2 = ENEMY_HEIGHT4;
						break;
					}

					if (g_aEnemy[i].pos.x + EnemyWidth / 2 > g_aEnemy[i2].pos.x - EnemyWidth2 / 2 && g_aEnemy[i].pos.x - EnemyWidth / 2 < g_aEnemy[i2].pos.x + EnemyWidth2 / 2)
					{
						if (g_aEnemy[i].Oldpos.y <= g_aEnemy[i2].pos.y - EnemyHeight2 && g_aEnemy[i].pos.y > g_aEnemy[i2].pos.y - EnemyHeight2 + BLOCK_SPEEDY - GetScroll().y)
						{
							g_aEnemy[i].bJump = false;
							g_aEnemy[i].pos.y = g_aEnemy[i2].pos.y - EnemyHeight2 + BLOCK_SPEEDY - GetScroll().y;
							g_aEnemy[i].move.y = 0.0f;

							g_aEnemy[i].pos.x += g_aEnemy[i2].move.x;
							g_aEnemy[i].pos.y += g_aEnemy[i2].move.y;
						}
						else if (g_aEnemy[i].Oldpos.y - EnemyHeight >= g_aEnemy[i2].pos.y + EnemyHeight2 && g_aEnemy[i].pos.y - EnemyHeight < g_aEnemy[i2].pos.y + EnemyHeight2 + BLOCK_SPEEDY - GetScroll().y)
						{
							g_aEnemy[i].pos.y = g_aEnemy[i2].pos.y + EnemyHeight2 + BLOCK_SPEEDY - GetScroll().y + EnemyHeight;
							g_aEnemy[i].move.y = 0.0f;
						}
					}
					if (g_aEnemy[i].pos.y > g_aEnemy[i2].pos.y - EnemyHeight2 && g_aEnemy[i].pos.y - EnemyHeight < g_aEnemy[i2].pos.y)
					{
						if (g_aEnemy[i].Oldpos.x + EnemyWidth / 2 <= g_aEnemy[i2].pos.x - EnemyWidth2 / 2 && g_aEnemy[i].pos.x + EnemyWidth / 2 > g_aEnemy[i2].pos.x - EnemyWidth2 / 2 + BLOCK_SPEEDX - GetScroll().x)
						{
							g_aEnemy[i].pos.x = g_aEnemy[i2].pos.x - EnemyWidth2 / 2 + BLOCK_SPEEDX - GetScroll().x - EnemyWidth / 2;
							g_aEnemy[i].move.x = 0.0f;
						}
						else if (g_aEnemy[i].Oldpos.x - EnemyWidth / 2 >= g_aEnemy[i2].pos.x + EnemyWidth2 / 2 && g_aEnemy[i].pos.x - EnemyWidth / 2 < g_aEnemy[i2].pos.x + EnemyWidth2 / 2 + BLOCK_SPEEDX - GetScroll().x)
						{
							g_aEnemy[i].pos.x = g_aEnemy[i2].pos.x + EnemyWidth2 / 2 + BLOCK_SPEEDX - GetScroll().x + EnemyWidth / 2;
							g_aEnemy[i].move.x = 0.0f;
						}
					}

					if (g_aEnemy[i].pos.x + EnemyWidth / 2 < 0 || g_aEnemy[i].pos.x - EnemyWidth / 2 > SCREEN_WIDTH || g_aEnemy[i].pos.y < 0 || g_aEnemy[i].pos.y - EnemyHeight > SCREEN_HEIGHT)
					{
						g_aEnemy[i].bDisp = false;
					}
					else
					{
						g_aEnemy[i].bDisp = true;
					}

					g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N
					pVtx += i * VT_MAX;

					//���_���W�ݒ�
					pVtx[0].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - EnemyWidth / 2, g_aEnemy[i].pos.y - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + EnemyWidth / 2, g_aEnemy[i].pos.y - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - EnemyWidth / 2, g_aEnemy[i].pos.y - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 + EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + EnemyWidth / 2, g_aEnemy[i].pos.y - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 + EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);

					if (g_aEnemy[i].bLeft)
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

					g_pVtxBuffEnemy->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N
					pVtx += i2 * VT_MAX;

					//���_���W�ݒ�
					pVtx[0].pos = D3DXVECTOR3(g_aEnemy[i2].pos.x - EnemyWidth2 / 2, g_aEnemy[i2].pos.y - EnemyHeight2 * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 - EnemyHeight2 * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aEnemy[i2].pos.x + EnemyWidth2 / 2, g_aEnemy[i2].pos.y - EnemyHeight2 * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 - EnemyHeight2 * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aEnemy[i2].pos.x - EnemyWidth2 / 2, g_aEnemy[i2].pos.y - EnemyHeight2 * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 + EnemyHeight2 * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aEnemy[i2].pos.x + EnemyWidth2 / 2, g_aEnemy[i2].pos.y - EnemyHeight2 * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 + EnemyHeight2 * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);

					if (g_aEnemy[i2].bLeft)
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

					g_pVtxBuffEnemy->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
			}
		}
	}
}

//--------------------
//�G�̕`�揈��
//--------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_aEnemy[i].bUse && g_aEnemy[i].bDisp)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[i].nType]);

			//�w�i�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				VT_MAX * i,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}
}

//-------------------
//�G�ݒ�
//-------------------
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_aEnemy[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aEnemy[i].pos = pos;
			g_aEnemy[i].Oldpos = g_aEnemy[i].pos;
			g_aEnemy[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[i].nType = nType;
			g_aEnemy[i].state = ENEMYSTATE_NORMAL;
			switch (g_aEnemy[i].nType)
			{
			case 0:
				g_aEnemy[i].nLife = ENEMY_LIFE;
				break;
			case 1:
				g_aEnemy[i].nLife = ENEMY_LIFE2;
				break;
			case 2:
				g_aEnemy[i].nLife = ENEMY_LIFE3;
				break;
			case 3:
				g_aEnemy[i].nLife = ENEMY_LIFE4;
				break;
			}
			g_aEnemy[i].nCounterState = 0;
			g_aEnemy[i].bMove = false;//�����Ă��邩
			g_aEnemy[i].bJump = false;//�W�����v���Ă��邩
			g_aEnemy[i].JumpCount = 0;
			g_aEnemy[i].bLeft = false;
			g_aEnemy[i].bDisp = true;
			g_aEnemy[i].bUse = true;

			//���_���W�ݒ�

			VERTEX_2D* pVtx;

			float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
			switch (g_aEnemy[i].nType)
			{
			case 0:
				EnemyWidth = ENEMY_WIDTH;
				EnemyHeight = ENEMY_HEIGHT;
				break;
			case 1:
				EnemyWidth = ENEMY_WIDTH2;
				EnemyHeight = ENEMY_HEIGHT2;
				break;
			case 2:
				EnemyWidth = ENEMY_WIDTH3;
				EnemyHeight = ENEMY_HEIGHT3;
				break;
			case 3:
				EnemyWidth = ENEMY_WIDTH4;
				EnemyHeight = ENEMY_HEIGHT4;
				break;
			}

			g_aEnemy[i].fLength = sqrtf(EnemyWidth * EnemyWidth + EnemyHeight * EnemyHeight) / 2.0f;
			g_aEnemy[i].fLengthDef = g_aEnemy[i].fLength;

			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N
			pVtx += i * VT_MAX;

			//���_���W�ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - EnemyWidth / 2, g_aEnemy[i].pos.y - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + EnemyWidth / 2, g_aEnemy[i].pos.y - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - EnemyWidth / 2, g_aEnemy[i].pos.y - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 + EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + EnemyWidth / 2, g_aEnemy[i].pos.y - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 + EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);

			g_pVtxBuffEnemy->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

			g_nNumEnemy++;
			break;
		}
	}
}

//-------------------
//�q�b�g����
//-------------------
void HitEnemy(int nCntEnemy, int nDamage, D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		SetParticle(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].fLength);
		g_aEnemy[nCntEnemy].bUse = false;
		g_nNumEnemy--;
		AddScore(nDamage*SCORE_DES);
	}
	else
	{
		float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
		switch (g_aEnemy[nCntEnemy].nType)
		{
		case 0:
			EnemyHeight = ENEMY_HEIGHT;
			break;
		case 1:
			EnemyHeight = ENEMY_HEIGHT2;
			break;
		case 2:
			EnemyHeight = ENEMY_HEIGHT3;
			break;
		case 3:
			EnemyHeight = ENEMY_HEIGHT4;
			break;
		}

		float Xlong = 0.0f, Ylong = 0.0f;
		Xlong = g_aEnemy[nCntEnemy].pos.x - pos.x;
		Ylong = g_aEnemy[nCntEnemy].pos.y - EnemyHeight / 2 - pos.y;
		float Vector = atan2f(Xlong, Ylong);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(sinf(Vector) * HIT_VECTOR, cosf(Vector) * HIT_VECTOR, 0.0f);

		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 5;
		AddScore(nDamage*SCORE_HIT);

		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += nCntEnemy * VT_MAX;
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		g_pVtxBuffEnemy->Unlock();//�o�b�t�@�̃A�����b�N
	}
}

//---------------------
//�G���擾
//---------------------
Enemy* GetEnemy(void)
{
	return &g_aEnemy[0];
}

//----------------------
//�G�̐��̎擾
//----------------------
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}



//-----------------------
//�G�̍X�V����
//-----------------------
void UpdateEditEnemy(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	static int aEnemyBulletCnt[MAX_ENEMY] = { 0 };

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_aEnemy[i].bUse == true)
		{
			float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
			switch (g_aEnemy[i].nType)
			{
			case 0:
				EnemyWidth = ENEMY_WIDTH;
				EnemyHeight = ENEMY_HEIGHT;
				break;
			case 1:
				EnemyWidth = ENEMY_WIDTH2;
				EnemyHeight = ENEMY_HEIGHT2;
				break;
			case 2:
				EnemyWidth = ENEMY_WIDTH3;
				EnemyHeight = ENEMY_HEIGHT3;
				break;
			case 3:
				EnemyWidth = ENEMY_WIDTH4;
				EnemyHeight = ENEMY_HEIGHT4;
				break;
			}

			if (g_aEnemy[i].pos.x + EnemyWidth / 2 < 0 || g_aEnemy[i].pos.x - EnemyWidth / 2 > SCREEN_WIDTH || g_aEnemy[i].pos.y < 0 || g_aEnemy[i].pos.y - EnemyHeight > SCREEN_HEIGHT)
			{
				g_aEnemy[i].bDisp = false;
			}
			else
			{
				g_aEnemy[i].bDisp = true;
			}

			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N
			pVtx += i * VT_MAX;

			//���_���W�ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - EnemyWidth / 2, g_aEnemy[i].pos.y - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + EnemyWidth / 2, g_aEnemy[i].pos.y - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - EnemyWidth / 2, g_aEnemy[i].pos.y - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 + EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + EnemyWidth / 2, g_aEnemy[i].pos.y - EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2 + EnemyHeight * (g_aEnemy[i].fLength / g_aEnemy[i].fLengthDef) / 2, 0.0f);

			g_pVtxBuffEnemy->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
		}
	}
}