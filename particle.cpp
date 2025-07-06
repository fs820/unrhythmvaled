//---------------------------------------
//
//�e�\������[particle.cpp]
//Author fuma sato
//
//---------------------------------------

#include"particle.h"
#include"attack.h"
#include"player.h"
#include"enemy.h"
#include"effect.h"
#include"sound.h"

//�e�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	float fLength;
	int nLife;//����
	bool bUse;//�g�p���Ă��邩�ǂ���
}PARTICLE;

//�O���[�o��
PARTICLE g_aParticle[MAX_PARTICLE];//�e�̏��

//-----------------
//�e�̏���������
//-----------------
void InitParticle(void)
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		g_aParticle[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[i].fLength = 0.0f;
		g_aParticle[i].nLife = PARTICLE_LIFE;
		g_aParticle[i].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
	}
}

//----------------
//�e�̏I������
//----------------
void UninitParticle(void)
{

}

//----------------
//�e�̍X�V����
//----------------
void UpdateParticle(void)
{

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (g_aParticle[i].bUse == true)
		{//�e���g�p����Ă���

			for (int i2 = 0; i2 < PARTICLE_NUM; i2++)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f), move = D3DXVECTOR3(0.0f,0.0f,0.0f);
				D3DCOLOR col = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
				float fRadius=0.0f,fangle=0.0f,fspeed=0.0f;
				int nLife=0;
				pos=g_aParticle[i].pos;
				fangle = (float)(rand() % 629 - 314) / 100.0f;
				fspeed = (float)(rand() % 100) / 10.0f + 1.0f;
				move.x = sinf(fangle) * fspeed;
				move.y = cosf(fangle) * fspeed;
				col = D3DXCOLOR((float)(rand() % 101) / 100.0f, (float)(rand() % 101) / 100.0f, (float)(rand() % 101) / 100.0f, (float)(rand() % 51) / 100.0f);
				fRadius = (float)(rand() % (int)(g_aParticle[i].fLength+1.0f) * 2.0f);
				nLife = g_aParticle[i].nLife;

				SetEffect(pos, move, D3DXVECTOR3(0.0f, 0.0f, atan2f(move.y, move.x)), col, fRadius, nLife, EFFECTTYPE_NORMAL, NULL);
			}

			g_aParticle[i].nLife--;
			g_aParticle[i].fLength *= ((float)g_aParticle[i].nLife) / ((float)PARTICLE_LIFE);

			if (g_aParticle[i].nLife <= 0)
			{
				g_aParticle[i].bUse = false;
			}
		}
	}
}

//-------------------
//���ʏ�e
//-------------------
void SetParticle(D3DXVECTOR3 pos, float fLength)
{

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (g_aParticle[i].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aParticle[i].pos = pos;
			g_aParticle[i].fLength=fLength;
			g_aParticle[i].nLife = PARTICLE_LIFE;
			g_aParticle[i].bUse = true;

			break;
		}
	}
	PlaySound(SOUND_LABEL_EXPLOSION);
}