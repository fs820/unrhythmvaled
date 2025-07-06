//---------------------------------------
//
//�e�\������[attack.cpp]
//Author fuma sato
//
//---------------------------------------

#include"attack.h"
#include"effect.h"
#include"particle.h"
#include"enemy.h"
#include"game.h"
#include"input.h"
#include"scroll.h"
#include"block.h"
#include"sound.h"

#define SHORT_DAMAGE (50)//�ߐڃ_���[�W
#define SHORT2_DAMAGE (100)//�ߐڃ_���[�W
#define SHORT3_DAMAGE (200)//�ߐڃ_���[�W
#define SHORT4_DAMAGE (500)//�ߐڃ_���[�W
#define SHORT5_DAMAGE (1000)//�ߐڃ_���[�W
#define LONG_DAMAGE (10)//���u�_���[�W
#define LONG2_DAMAGE (20)//���u�_���[�W
#define LONG3_DAMAGE (30)//���u�_���[�W
#define LONG4_DAMAGE (50)//���u�_���[�W
#define LONG5_DAMAGE (100)//���u�_���[�W
#define SP_DAMAGE (2000)//�Z�_���[�W
#define SP2_DAMAGE (3000)//�Z�_���[�W
#define SP3_DAMAGE (5000)//�Z�_���[�W
#define SP4_DAMAGE (8000)//�Z�_���[�W
#define SP5_DAMAGE (10000)//�Z�_���[�W
#define ATTACKSEARCH_INA_P (0.3f)//�v���C���[�U���e���x
#define ATTACKSEARCH_INA_B (0.1f)//�{�X�U���e���x
#define ATTACKSEARCH_INA_V (0.1f)//VS�U���e���x
#define SEARCH_TIME (20)//�{�X�U���e�U������
#define NOSEARCH_TIME (10)//�v���C���[�U���e�U������܂ł̎���
#define ATTACK_EFFECT_LIFE (2)

//�e�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 Oldpos;//�ʒu
	float fLength;//�傫��
	D3DXVECTOR3 move;//�ړ���
	int nLife;//����
	ATTACKTYPE Type;//�N�̋ʂ�
	ATTACKSTYLE Style;//�ǂ�ȋʂ�
	bool bLeft;
	bool bUse;//�g�p���Ă��邩�ǂ���
}ATTACK;

//�O���[�o��
ATTACK g_aAttack[MAX_ATTACK];//�e�̏��

//-----------------
//�e�̏���������
//-----------------
void InitAttack(void)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{//��񏉊���
		g_aAttack[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[i].Oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[i].fLength = 0.0f;
		g_aAttack[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[i].nLife = SHORT_ATTACK_LIFE;
		g_aAttack[i].Type = ATTACKTYPE_PLAYER;
		g_aAttack[i].Style = STYLE_SHORT;
		g_aAttack[i].bLeft = false;
		g_aAttack[i].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
	}
}

//----------------
//�e�̍X�V����
//----------------
void UpdateAttack(void)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{//�e�̐������J��Ԃ�
		if (g_aAttack[i].bUse == true)
		{//�e���g�p����Ă���
			g_aAttack[i].Oldpos = g_aAttack[i].pos;

			if (g_aAttack[i].Type == ATTACKTYPE_PLAYER)
			{//�v���C���[�̒e
				Enemy* pEnemy;
				//Boss* pBoss;
				Player* pPlayer;

				//���擾
				pEnemy = GetEnemy();
				//pBoss = GetBoss();
				pPlayer = GetPlayer();
				float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
				for (int i2 = 0; i2 < MAX_ENEMY; i2++, pEnemy++)
				{//�G�̐������J��Ԃ�
					if (pEnemy->bUse && pEnemy->bDisp)
					{//�G���g�p����Ă���
						switch (pEnemy->nType)
						{//�G�̎�ނɂ���ĕς���
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

						if (pEnemy->pos.x - EnemyWidth / 2 <= g_aAttack[i].pos.x + g_aAttack[i].fLength && pEnemy->pos.x + EnemyWidth / 2 >= g_aAttack[i].pos.x - g_aAttack[i].fLength && pEnemy->pos.y - EnemyHeight <= g_aAttack[i].pos.y + g_aAttack[i].fLength && pEnemy->pos.y >= g_aAttack[i].pos.y - g_aAttack[i].fLength)
						{//�G�ɓ�������
							int nDamage;

							switch (g_aAttack[i].Style)
							{//�_���[�W�v�Z
							case STYLE_SHORT:
								nDamage = (int)((float)SHORT_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_SHORT2:
								nDamage = (int)((float)SHORT2_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_SHORT3:
								nDamage = (int)((float)SHORT3_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_SHORT4:
								nDamage = (int)((float)SHORT4_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_SHORT5:
								nDamage = (int)((float)SHORT5_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_LONG:
								nDamage = (int)((float)LONG_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_LONG2:
								nDamage = (int)((float)LONG2_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_LONG3:
								nDamage = (int)((float)LONG3_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_LONG4:
								nDamage = (int)((float)LONG4_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_LONG5:
								nDamage = (int)((float)LONG5_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_SP:
								nDamage = (int)((float)SP_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_SP2:
								nDamage = (int)((float)SP2_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_SP3:
								nDamage = (int)((float)SP3_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_SP4:
								nDamage = (int)((float)SP4_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case STYLE_SP5:
								nDamage = (int)((float)SP5_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							}

							HitEnemy(i2, nDamage, g_aAttack[i].pos);


							if (!(g_aAttack[i].Style >= STYLE_SP))
							{
								g_aAttack[i].bUse = false;
							}
						}
					}
				}
			}
			else if (g_aAttack[i].Type == ATTACKTYPE_ENEMY)
			{//�G�̒e
				Player* pPlayer;

				pPlayer = GetPlayer();
				if (pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_DIE)
				{
					if (pPlayer->pos.x - PLAYER_WIDTH * (pPlayer->fLength / pPlayer->fLengthDef) / 2 <= g_aAttack[i].pos.x + g_aAttack[i].fLength && pPlayer->pos.x + PLAYER_WIDTH * (pPlayer->fLength / pPlayer->fLengthDef) / 2 >= g_aAttack[i].pos.x - g_aAttack[i].fLength && pPlayer->pos.y - PLAYER_HEIGHT * (pPlayer->fLength / pPlayer->fLengthDef) <= g_aAttack[i].pos.y + g_aAttack[i].fLength && pPlayer->pos.y >= g_aAttack[i].pos.y - g_aAttack[i].fLength)
					{
						int nDamage = 0;

						switch (g_aAttack[i].Style)
						{
						case STYLE_SHORT:
							nDamage = SHORT_DAMAGE;
							break;
						case STYLE_SHORT2:
							nDamage = SHORT2_DAMAGE;
							break;
						case STYLE_SHORT3:
							nDamage = SHORT3_DAMAGE;
							break;
						case STYLE_SHORT4:
							nDamage = SHORT4_DAMAGE;
							break;
						case STYLE_SHORT5:
							nDamage = SHORT5_DAMAGE;
							break;
						case STYLE_LONG:
							nDamage = LONG_DAMAGE;
							break;
						case STYLE_LONG2:
							nDamage = LONG2_DAMAGE;
							break;
						case STYLE_LONG3:
							nDamage = LONG3_DAMAGE;
							break;
						case STYLE_LONG4:
							nDamage = LONG4_DAMAGE;
							break;
						case STYLE_LONG5:
							nDamage = LONG5_DAMAGE;
							break;
						case STYLE_SP:
							nDamage = SP_DAMAGE;
							break;
						case STYLE_SP2:
							nDamage = SP2_DAMAGE;
							break;
						case STYLE_SP3:
							nDamage = SP3_DAMAGE;
							break;
						case STYLE_SP4:
							nDamage = SP4_DAMAGE;
							break;
						case STYLE_SP5:
							nDamage = SP5_DAMAGE;
							break;
						}

						HitPlayer(nDamage, g_aAttack[i].pos);

						if (!(g_aAttack[i].Style >= STYLE_SP))
						{
							g_aAttack[i].bUse = false;
						}
					}
				}
			}

			if (g_aAttack[i].nLife <= 0)
			{
				g_aAttack[i].bUse = false;
			}

			g_aAttack[i].nLife--;

			g_aAttack[i].pos.x += g_aAttack[i].move.x;
			g_aAttack[i].pos.y += g_aAttack[i].move.y;

			g_aAttack[i].pos -= GetScroll();

			for (int i2 = 0; i2 < MAX_ATTACK; i2++)
			{
				if (g_aAttack[i2].bUse && g_aAttack[i].Type != g_aAttack[i2].Type)
				{
					float Xlong = 0.0f, Ylong = 0.0f, Long = 0.0f, Angle = 0.0f;
					D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					Xlong = g_aAttack[i].pos.x - g_aAttack[i2].pos.x;
					Ylong = g_aAttack[i].pos.y - g_aAttack[i2].pos.y;
					Long = sqrtf(Xlong * Xlong + Ylong * Ylong);
					Angle = atan2f(Ylong, Xlong);
					Rot = D3DXVECTOR3(sinf(Angle), cosf(Angle), 0.0f);
					if (g_aAttack[i].fLength + g_aAttack[i2].fLength > Long)
					{
						if (g_aAttack[i].Style> g_aAttack[i2].Style)
						{
							g_aAttack[i2].bUse = false;
						}
						else if (g_aAttack[i].Style < g_aAttack[i2].Style)
						{
							g_aAttack[i].bUse = false;
						}
						else
						{
							g_aAttack[i].bUse = false;
							g_aAttack[i2].bUse = false;
						}
						SetParticle(D3DXVECTOR3(g_aAttack[i].pos.x + Rot.x * Long / 2, g_aAttack[i].pos.y + Rot.y * Long / 2, Rot.z), Long);
					}
				}
			}

			if (!(g_aAttack[i].Style >= STYLE_SP))
			{
				CllisionBlockA(&g_aAttack[i].pos, &g_aAttack[i].Oldpos, g_aAttack[i].fLength, &g_aAttack[i].bUse);
			}

			D3DXCOLOR col;
			switch (g_aAttack[i].Style)
			{
			case STYLE_SHORT:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_SHORT2:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_SHORT3:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_SHORT4:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_SHORT5:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_LONG:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_LONG2:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_LONG3:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_LONG4:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_LONG5:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_SP:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_SP2:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_SP3:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_SP4:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case STYLE_SP5:
				col = D3DXCOLOR(0.8f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			}
			SetEffect(g_aAttack[i].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, atan2f(g_aAttack[i].move.y, g_aAttack[i].move.x)), col, g_aAttack[i].fLength, ATTACK_EFFECT_LIFE, (EFFECTTYPE)(g_aAttack[i].Style + 1), g_aAttack[i].bLeft);
		}
	}
}

//-------------------
//�Z
//-------------------
void SpAttackMaster(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft, PATA* Pata)
{
	PATA* PataDef = Pata;
	FILE* pFile;
	char aFile[64] = { "\0" };
	int aData[ATTACK_NUM] = { -1,-1,-1,-1,-1 };
	bool OK = false;

	for (int i = 0; i < SP_MAX; i++)
	{
		//������̑��
		snprintf(&aFile[0], sizeof(aFile), SP_FILE, i);
		pFile = fopen(aFile, "r");

		if (pFile != NULL)
		{
			for (int i2 = 0; i2 < ATTACK_NUM; i2++, Pata++)
			{
				//�f�[�^�̓ǂݍ���
				fscanf(pFile, "%d", &aData[i2]);//�t�@�C�����疼�O��ǂݍ���

				if ((PATA)aData[i2] != *Pata)
				{
					break;
				}
				else if (i2 == ATTACK_NUM - 1)
				{
					Pata = PataDef;
					for (int i = 0; i < ATTACK_NUM; i++, Pata++)
					{
						*Pata = (PATA)-1;
					}
					OK = true;
				}
			}

			fclose(pFile);

			if (OK)
			{
				switch (i)
				{
				case 0:
					SetSpAttack(pos, Playerrot, fLengthPlayer, type, bLeft);
					break;
				case 1:
					SetSp2Attack(pos, Playerrot, fLengthPlayer, type, bLeft);
					break;
				case 2:
					SetSp3Attack(pos, Playerrot, fLengthPlayer, type, bLeft);
					break;
				case 3:
					SetSp4Attack(pos, Playerrot, fLengthPlayer, type, bLeft);
					break;
				case 4:
					SetSp5Attack(pos, Playerrot, fLengthPlayer, type, bLeft);
					break;
				}
				break;
			}
		}
		Pata = PataDef;
	}
}

//-------------------
//�ߐ�1
//-------------------
void SetShortAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{
		if (g_aAttack[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			if (bLeft)
			{
				g_aAttack[i].pos.x -= fLengthPlayer;
			}
			else
			{
				g_aAttack[i].pos.x += fLengthPlayer;
			}
			g_aAttack[i].fLength = fLengthPlayer;
			g_aAttack[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aAttack[i].nLife = SHORT_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i].Style = STYLE_SHORT;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			PlaySound(SOUND_LABEL_SHORT);
			break;
		}
	}
}

//-------------------
//�ߐ�2
//-------------------
void SetShort2Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{
		if (g_aAttack[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			if (bLeft)
			{
				g_aAttack[i].pos.x -= fLengthPlayer;
			}
			else
			{
				g_aAttack[i].pos.x += fLengthPlayer;
			}
			g_aAttack[i].fLength = fLengthPlayer;
			g_aAttack[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aAttack[i].nLife = SHORT_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i].Style = STYLE_SHORT2;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			PlaySound(SOUND_LABEL_SHORT2);
			break;
		}
	}
}

//-------------------
//�ߐ�3
//-------------------
void SetShort3Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{
		if (g_aAttack[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			if (bLeft)
			{
				g_aAttack[i].pos.x -= fLengthPlayer;
			}
			else
			{
				g_aAttack[i].pos.x += fLengthPlayer;
			}
			g_aAttack[i].fLength = fLengthPlayer;
			g_aAttack[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aAttack[i].nLife = SHORT_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i].Style = STYLE_SHORT3;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			PlaySound(SOUND_LABEL_SHORT3);
			break;
		}
	}
}

//-------------------
//�ߐ�4
//-------------------
void SetShort4Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{
		if (g_aAttack[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			if (bLeft)
			{
				g_aAttack[i].pos.x -= fLengthPlayer;
			}
			else
			{
				g_aAttack[i].pos.x += fLengthPlayer;
			}
			g_aAttack[i].fLength = fLengthPlayer;
			g_aAttack[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aAttack[i].nLife = SHORT_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i].Style = STYLE_SHORT4;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			PlaySound(SOUND_LABEL_SHORT4);
			break;
		}
	}
}

//-------------------
//�ߐ�5
//-------------------
void SetShort5Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{
		if (g_aAttack[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			if (bLeft)
			{
				g_aAttack[i].pos.x -= fLengthPlayer;
			}
			else
			{
				g_aAttack[i].pos.x += fLengthPlayer;
			}
			g_aAttack[i].fLength = fLengthPlayer;
			g_aAttack[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aAttack[i].nLife = SHORT_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i].Style = STYLE_SHORT5;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			PlaySound(SOUND_LABEL_SHORT5);
			break;
		}
	}
}

//-----------------
//���u1
//-----------------
void SetLongAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{
		if (g_aAttack[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			if (bLeft)
			{
				g_aAttack[i].pos.x -= fLengthPlayer;
			}
			else
			{
				g_aAttack[i].pos.x += fLengthPlayer;
			}
			g_aAttack[i].fLength = fLengthPlayer;
			g_aAttack[i].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * LONG_ATTACK_SPEED, cosf(Playerrot + D3DX_PI * 0.5f) * LONG_ATTACK_SPEED, 0.0f);
			if (bLeft)
			{
				g_aAttack[i].move = -g_aAttack[i].move;
			}
			g_aAttack[i].nLife = LONG_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i].Style = STYLE_LONG;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			PlaySound(SOUND_LABEL_LONG);
			break;
		}
	}
}

//-----------------
//���u2
//-----------------
void SetLong2Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{
		if (g_aAttack[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			if (bLeft)
			{
				g_aAttack[i].pos.x -= fLengthPlayer;
			}
			else
			{
				g_aAttack[i].pos.x += fLengthPlayer;
			}
			g_aAttack[i].fLength = fLengthPlayer;
			g_aAttack[i].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * LONG_ATTACK_SPEED, cosf(Playerrot + D3DX_PI * 0.5f) * LONG_ATTACK_SPEED, 0.0f);
			if (bLeft)
			{
				g_aAttack[i].move = -g_aAttack[i].move;
			}
			g_aAttack[i].nLife = LONG_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i].Style = STYLE_LONG2;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			PlaySound(SOUND_LABEL_LONG2);
			break;
		}
	}
}

//-----------------
//���u3
//-----------------
void SetLong3Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{
		if (g_aAttack[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			if (bLeft)
			{
				g_aAttack[i].pos.x -= fLengthPlayer;
			}
			else
			{
				g_aAttack[i].pos.x += fLengthPlayer;
			}
			g_aAttack[i].fLength = fLengthPlayer;
			g_aAttack[i].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * LONG_ATTACK_SPEED, cosf(Playerrot + D3DX_PI * 0.5f) * LONG_ATTACK_SPEED, 0.0f);
			if (bLeft)
			{
				g_aAttack[i].move = -g_aAttack[i].move;
			}
			g_aAttack[i].nLife = LONG_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i].Style = STYLE_LONG3;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			PlaySound(SOUND_LABEL_LONG3);
			break;
		}
	}
}

//-----------------
//���u4
//-----------------
void SetLong4Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{
		if (g_aAttack[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			if (bLeft)
			{
				g_aAttack[i].pos.x -= fLengthPlayer;
			}
			else
			{
				g_aAttack[i].pos.x += fLengthPlayer;
			}
			g_aAttack[i].fLength = fLengthPlayer;
			g_aAttack[i].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * LONG_ATTACK_SPEED, cosf(Playerrot + D3DX_PI * 0.5f) * LONG_ATTACK_SPEED, 0.0f);
			if (bLeft)
			{
				g_aAttack[i].move = -g_aAttack[i].move;
			}
			g_aAttack[i].nLife = LONG_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i].Style = STYLE_LONG4;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			PlaySound(SOUND_LABEL_LONG4);
			break;
		}
	}
}

//-----------------
//���u5
//-----------------
void SetLong5Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{
		if (g_aAttack[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			if (bLeft)
			{
				g_aAttack[i].pos.x -= fLengthPlayer;
			}
			else
			{
				g_aAttack[i].pos.x += fLengthPlayer;
			}
			g_aAttack[i].fLength = fLengthPlayer;
			g_aAttack[i].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * LONG_ATTACK_SPEED, cosf(Playerrot + D3DX_PI * 0.5f) * LONG_ATTACK_SPEED, 0.0f);
			if (bLeft)
			{
				g_aAttack[i].move = -g_aAttack[i].move;
			}
			g_aAttack[i].nLife = LONG_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i].Style = STYLE_LONG5;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			PlaySound(SOUND_LABEL_LONG5);
			break;
		}
	}
}

//-------------------
//�Z1
//-------------------
void SetSpAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{
		if (g_aAttack[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			if (bLeft)
			{
				g_aAttack[i].pos.x += fLengthPlayer;
			}
			else
			{
				g_aAttack[i].pos.x -= fLengthPlayer;
			}
			g_aAttack[i].fLength = fLengthPlayer;
			g_aAttack[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aAttack[i].nLife = SP_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i].Style = STYLE_SP;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			PlaySound(SOUND_LABEL_SP);
			break;
		}
	}
}

//-------------------
//�Z2
//-------------------
void SetSp2Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK - 2; i++)
	{
		if (g_aAttack[i].bUse == false && g_aAttack[i + 1].bUse == false && g_aAttack[i + 2].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			g_aAttack[i + 1].pos = pos;
			g_aAttack[i + 2].pos = pos;
			if (bLeft)
			{
				g_aAttack[i].pos.x -= fLengthPlayer;
				g_aAttack[i + 1].pos.x -= fLengthPlayer;
				g_aAttack[i + 2].pos.x -= fLengthPlayer;
			}
			else
			{
				g_aAttack[i].pos.x += fLengthPlayer;
				g_aAttack[i + 1].pos.x += fLengthPlayer;
				g_aAttack[i + 2].pos.x += fLengthPlayer;
			}
			g_aAttack[i].fLength = fLengthPlayer;
			g_aAttack[i + 1].fLength = fLengthPlayer;
			g_aAttack[i + 2].fLength = fLengthPlayer;
			g_aAttack[i].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * SP2_ATTACK_SPEED, cosf(Playerrot + D3DX_PI * 0.5f) * SP2_ATTACK_SPEED, 0.0f);
			g_aAttack[i + 1].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.25f) * SP2_ATTACK_SPEED, cosf(Playerrot + D3DX_PI * 0.25f) * SP2_ATTACK_SPEED, 0.0f);
			g_aAttack[i + 2].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.75f) * SP2_ATTACK_SPEED, cosf(Playerrot + D3DX_PI * 0.75f) * SP2_ATTACK_SPEED, 0.0f);
			if (bLeft)
			{
				g_aAttack[i].move = -g_aAttack[i].move;
				g_aAttack[i + 1].move = -g_aAttack[i + 1].move;
				g_aAttack[i + 2].move = -g_aAttack[i + 2].move;
			}
			g_aAttack[i].nLife = SP2_ATTACK_LIFE;
			g_aAttack[i + 1].nLife = SP2_ATTACK_LIFE;
			g_aAttack[i + 2].nLife = SP2_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i + 1].Type = type;
			g_aAttack[i + 2].Type = type;
			g_aAttack[i].Style = STYLE_SP2;
			g_aAttack[i + 1].Style = STYLE_SP2;
			g_aAttack[i + 2].Style = STYLE_SP2;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i + 1].bLeft = bLeft;
			g_aAttack[i + 2].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			g_aAttack[i + 1].bUse = true;
			g_aAttack[i + 2].bUse = true;
			PlaySound(SOUND_LABEL_SP2);
			break;
		}
	}
}

//-------------------
//�Z3
//-------------------
void SetSp3Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{
		if (g_aAttack[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			if (bLeft)
			{
				g_aAttack[i].pos.x += fLengthPlayer;
			}
			else
			{
				g_aAttack[i].pos.x -= fLengthPlayer;
			}
			g_aAttack[i].fLength = fLengthPlayer * 2.0f;
			g_aAttack[i].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * SP3_ATTACK_SPEED, cosf(Playerrot + D3DX_PI * 0.5f) * SP3_ATTACK_SPEED, 0.0f);
			if (bLeft)
			{
				g_aAttack[i].move = -g_aAttack[i].move;
			}
			g_aAttack[i].nLife = SP3_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i].Style = STYLE_SP3;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			PlaySound(SOUND_LABEL_SP3);
			break;
		}
	}
}

//-------------------
//�Z4
//-------------------
void SetSp4Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	float fAll = 0.0f;
	bool OK = false;

	for (int i = 0; i < MAX_ATTACK - ALL_ATTACK; i++)
	{
		for (int i2 = 0; i2 < ALL_ATTACK; i2++)
		{
			if (g_aAttack[i + i2].bUse == true)
			{
				break;
			}
			if (i2 == ALL_ATTACK - 1)
			{
				OK = true;
			}
		}
		if (OK == true)
		{
			for (int i2 = 0; i2 < ALL_ATTACK; i2++)
			{
				if (g_aAttack[i + i2].bUse == false)
				{//�e���g�p����Ă��Ȃ�
					g_aAttack[i + i2].pos = pos;
					g_aAttack[i + i2].fLength = fLengthPlayer;
					g_aAttack[i + i2].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * fAll) * SP4_ATTACK_SPEED, cosf(Playerrot + D3DX_PI * fAll) * SP4_ATTACK_SPEED, 0.0f);
					g_aAttack[i + i2].nLife = SP4_ATTACK_LIFE;
					g_aAttack[i + i2].Type = type;
					g_aAttack[i + i2].Style = STYLE_SP4;
					g_aAttack[i + i2].bLeft = bLeft;
					g_aAttack[i + i2].bUse = true;
					fAll += 2.0f / (float)ALL_ATTACK;
				}
			}
			PlaySound(SOUND_LABEL_SP4);
			break;
		}
	}
}

//-------------------
//�Z5
//-------------------
void SetSp5Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft)
{
	for (int i = 0; i < MAX_ATTACK; i++)
	{
		if (g_aAttack[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aAttack[i].pos = pos;
			g_aAttack[i].fLength = fLengthPlayer * 10.0f;
			g_aAttack[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aAttack[i].nLife = SP5_ATTACK_LIFE;
			g_aAttack[i].Type = type;
			g_aAttack[i].Style = STYLE_SP5;
			g_aAttack[i].bLeft = bLeft;
			g_aAttack[i].bUse = true;
			PlaySound(SOUND_LABEL_SP5);
			break;
		}
	}
}