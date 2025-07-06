//----------------------------------------
//
//プレイヤー表示処理[player.cpp]
//Author fuma sato
//
//----------------------------------------

#include"player.h"
#include"input.h"
#include"score.h"
#include"attack.h"
#include"particle.h"
#include"scroll.h"
#include"enemy.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTexturePlayer;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;//バッファのポインタ
Player g_Player;//プレイヤー情報
D3DXVECTOR3 g_moveBullet;//弾発射向き・スピード管理

//----------------------
//プレイヤーの初期化処理
//----------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);

	g_Player.nCounterAnim = 0;
	g_Player.nPatternAnim = 0;
	g_Player.pos = D3DXVECTOR3(SCREEN_WIDTH / 4.0f, FIELD_HEIGHT - 1.0f, 0);//初期位置
	g_Player.Oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置
	g_Player.move = D3DXVECTOR3(PLAYER_SPEED_DEF, PLAYER_SPEED_DEF, PLAYER_SPEED_DEF);//プレイヤースピード
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
	g_Player.rotDef = g_Player.rot;//初期保存
	g_Player.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;//対角線の長さ
	g_Player.fLengthDef = g_Player.fLength;//初期保存
	g_Player.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);//角度
	g_Player.bMove = false;//移動しているか
	g_Player.bJump = false;//ジャンプしているか
	g_Player.JumpCount = 0;
	g_Player.bLeft = false;
	for (int i = 0; i < ATTACK_NUM; i++)
	{
		g_Player.bAttack[i] = false;
	}
	for (int i = 0; i < ATTACK_NUM; i++)
	{
		g_Player.Pata[i] = (PATA)-1;
	}
	g_Player.nAttackCount = 0;
	g_Player.nRema = 3;
	g_Player.nLife = PALYER_LIFE;
	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.nCounterState = 0;
	g_Player.bDisp = true;
	g_Player.pBlock = NULL;
	g_Player.pOldBlock = NULL;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		PLAYER_TEX,
		&g_pTexturePlayer
	);

	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	//座標設定
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2 + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2 + cosf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2 + cosf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2 + cosf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX);
	pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX, UV_DEF / V_MAX);

	g_pVtxBuffPlayer->Unlock();//プレイヤーバッファのアンロック
}

//-------------------
//プレイヤー終了処理
//-------------------
void UninitPlayer(void)
{
	SetVibrate(0.0f, CONTROLLER_1);
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//-------------------
//プレイヤー更新処理
//-------------------
void UpdatePlayer(void)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ
	float* pStick;
	float* pMouseMove;

	//移動処理

	//X
	if(IsXInputControllerConnected(CONTROLLER_1))
	{
		//左スティック処理
		pStick = GetJoyStick(STICK_LEFT, CONTROLLER_1);
		g_Player.move.x += *pStick * PLAYER_SPEED_X;
		if (*pStick * PLAYER_SPEED_X > 0)
		{
			g_Player.bLeft = false;
		}
		else
		{
			g_Player.bLeft = true;
		}
	}
	//d
	else if (IsDirectInputControllerConnected(CONTROLLER_1))
	{
		//左スティック処理
		pStick = GetdJoyStick(STICK_LEFT, CONTROLLER_1);
		g_Player.move.x += *pStick * PLAYER_SPEED_X;
		if (*pStick * PLAYER_SPEED_X > 0)
		{
			g_Player.bLeft = false;
		}
		else if(*pStick * PLAYER_SPEED_X < 0)
		{
			g_Player.bLeft = true;
		}
	}

	//マウス処理
	pMouseMove = GetMouseMove();
	g_Player.move.x += *pMouseMove * PLAYER_SPEED_X;

	if (*pMouseMove * PLAYER_SPEED_X>0)
	{
		g_Player.bLeft = false;
	}
	else if (*pMouseMove * PLAYER_SPEED_X < 0)
	{
		g_Player.bLeft = true;
	}

	if (GetKeyboradPress(DIK_A) == true || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) == true)//Aキー
	{
		g_Player.move.x += -PLAYER_SPEED_X;
		g_Player.bLeft = true;
	}
	else if (GetKeyboradPress(DIK_D) == true || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) == true)//Dキー
	{
		g_Player.move.x += PLAYER_SPEED_X;
		g_Player.bLeft = false;
	}

	if (!g_Player.bJump || g_Player.JumpCount > 0)
	{
		if (GetKeyboradTrigger(DIK_SPACE) == true || GetMouseWheel() != 0.0f || GetJoykeyTrigger(JOYKEY_A, CONTROLLER_1) == true)
		{
			g_Player.move.y = PLAYER_JUMP;
			g_Player.JumpCount--;
		}
	}

#ifdef _DEBUG

	if (GetKeyboradTrigger(DIK_I) == true || GetMouseTrigger(MOUSE_B2) == true || GetJoykeyTrigger(JOYKEY_RB, CONTROLLER_1) == true)
	{
		g_Player.bAttack[0] = true;
		for (int i = ATTACK_NUM - 1; i >= 0; i--)
		{
			if (g_Player.bAttack[i])
			{
				switch (i)
				{
				case 0:
					g_Player.nAttackCount = 0;
					SetShortAttack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					g_Player.bAttack[1] = true;
					break;
				case 1:
					g_Player.nAttackCount = 0;
					SetShort2Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					g_Player.bAttack[2] = true;
					break;
				case 2:
					g_Player.nAttackCount = 0;
					SetShort3Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					g_Player.bAttack[3] = true;
					break;
				case 3:
					g_Player.nAttackCount = 0;
					SetShort4Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					g_Player.bAttack[4] = true;
					break;
				case 4:
					SetShort5Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					for (int i = 0; i < ATTACK_NUM; i++)
					{
						g_Player.bAttack[i] = false;
					}
					break;
				}
				g_Player.Pata[i] = PATA_SHORT;
				break;
			}
		}
	}

#endif

	if (GetKeyboradTrigger(DIK_P) == true || GetMouseTrigger(MOUSE_LEFT) == true || GetJoykeyTrigger(JOYKEY_RT, CONTROLLER_1) == true)
	{
		ShortMovePlyer();
		g_Player.bAttack[0] = true;
		for (int i = ATTACK_NUM-1; i >= 0; i--)
		{
			if (g_Player.bAttack[i])
			{
				switch (i)
				{
				case 0:
					g_Player.nAttackCount = 0;
					SetShortAttack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					g_Player.bAttack[1] = true;
					break;
				case 1:
					g_Player.nAttackCount = 0;
					SetShort2Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					g_Player.bAttack[2] = true;
					break;
				case 2:
					g_Player.nAttackCount = 0;
					SetShort3Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					g_Player.bAttack[3] = true;
					break;
				case 3:
					g_Player.nAttackCount = 0;
					SetShort4Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					g_Player.bAttack[4] = true;
					break;
				case 4:
					SetShort5Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					for (int i = 0; i < ATTACK_NUM; i++)
					{
						g_Player.bAttack[i] = false;
					}
					break;
				}
				g_Player.Pata[i] = PATA_SHORT;
				break;
			}
		}
	}
	else if (GetKeyboradTrigger(DIK_O) == true || GetMouseTrigger(MOUSE_RIGHT) == true || GetJoykeyTrigger(JOYKEY_LT, CONTROLLER_1) == true)
	{
		g_Player.bAttack[0] = true;
		for (int i = ATTACK_NUM - 1; i >= 0; i--)
		{
			if (g_Player.bAttack[i])
			{
				switch (i)
				{
				case 0:
					g_Player.nAttackCount = 0;
					SetLongAttack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					g_Player.bAttack[1] = true;
					break;
				case 1:
					g_Player.nAttackCount = 0;
					SetLong2Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					g_Player.bAttack[2] = true;
					break;
				case 2:
					g_Player.nAttackCount = 0;
					SetLong3Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					g_Player.bAttack[3] = true;
					break;
				case 3:
					g_Player.nAttackCount = 0;
					SetLong4Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					g_Player.bAttack[4] = true;
					break;
				case 4:
					SetLong5Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
					for (int i = 0; i < ATTACK_NUM; i++)
					{
						g_Player.bAttack[i] = false;
					}
					break;
				}
				g_Player.Pata[i] = PATA_LONG;
				break;
			}
		}
	}

	if (IsDirectInputControllerConnected(CONTROLLER_1))
	{
		if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_1) == true)
		{
			g_Player.move.x += PLAYER_SPEED_X;
			g_Player.bLeft = false;
		}
		else if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_1) == true)
		{
			g_Player.move.x += -PLAYER_SPEED_X;
			g_Player.bLeft = true;
		}
	}

	if (!strcmp(ControllerName(CONTROLLER_1), ELE_CON))
	{
		if (!g_Player.bJump || g_Player.JumpCount > 0)
		{
			if (GetdJoykeyPress(ELEKEY_A, CONTROLLER_1) == true)
			{
				g_Player.move.y = PLAYER_JUMP;
				g_Player.JumpCount--;
			}
		}

		if (GetdJoykeyTrigger(ELEKEY_RT, CONTROLLER_1) == true)
		{
			ShortMovePlyer();
			g_Player.bAttack[0] = true;
			for (int i = ATTACK_NUM - 1; i >= 0; i--)
			{
				if (g_Player.bAttack[i])
				{
					switch (i)
					{
					case 0:
						g_Player.nAttackCount = 0;
						SetShortAttack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[1] = true;
						break;
					case 1:
						g_Player.nAttackCount = 0;
						SetShort2Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[2] = true;
						break;
					case 2:
						g_Player.nAttackCount = 0;
						SetShort3Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[3] = true;
						break;
					case 3:
						g_Player.nAttackCount = 0;
						SetShort4Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[4] = true;
						break;
					case 4:
						SetShort5Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						for (int i = 0; i < ATTACK_NUM; i++)
						{
							g_Player.bAttack[i] = false;
						}
						break;
					}
					g_Player.Pata[i] = PATA_SHORT;
					break;
				}
			}
		}
		else if (GetdJoykeyTrigger(ELEKEY_LT, CONTROLLER_1) == true)
		{
			g_Player.bAttack[0] = true;
			for (int i = ATTACK_NUM - 1; i >= 0; i--)
			{
				if (g_Player.bAttack[i])
				{
					switch (i)
					{
					case 0:
						g_Player.nAttackCount = 0;
						SetLongAttack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[1] = true;
						break;
					case 1:
						g_Player.nAttackCount = 0;
						SetLong2Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[2] = true;
						break;
					case 2:
						g_Player.nAttackCount = 0;
						SetLong3Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[3] = true;
						break;
					case 3:
						g_Player.nAttackCount = 0;
						SetLong4Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[4] = true;
						break;
					case 4:
						SetLong5Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						for (int i = 0; i < ATTACK_NUM; i++)
						{
							g_Player.bAttack[i] = false;
						}
						break;
					}
					g_Player.Pata[i] = PATA_LONG;
					break;
				}
			}
		}
	}
	else if (!strcmp(ControllerName(CONTROLLER_1), PS_CON))
	{
		if (!g_Player.bJump || g_Player.JumpCount > 0)
		{
			if (GetdJoykeyPress(PSKEY_CR, CONTROLLER_1) == true)
			{
				g_Player.move.y = PLAYER_JUMP;
				g_Player.JumpCount--;
			}
		}

		if (GetdJoykeyTrigger(PSKEY_RT, CONTROLLER_1) == true)
		{
			ShortMovePlyer();
			g_Player.bAttack[0] = true;
			for (int i = ATTACK_NUM - 1; i >= 0; i--)
			{
				if (g_Player.bAttack[i])
				{
					switch (i)
					{
					case 0:
						g_Player.nAttackCount = 0;
						SetShortAttack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[1] = true;
						break;
					case 1:
						g_Player.nAttackCount = 0;
						SetShort2Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[2] = true;
						break;
					case 2:
						g_Player.nAttackCount = 0;
						SetShort3Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[3] = true;
						break;
					case 3:
						g_Player.nAttackCount = 0;
						SetShort4Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[4] = true;
						break;
					case 4:
						SetShort5Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						for (int i = 0; i < ATTACK_NUM; i++)
						{
							g_Player.bAttack[i] = false;
						}
						break;
					}
					g_Player.Pata[i] = PATA_SHORT;
					break;
				}
			}
		}
		else if (GetdJoykeyTrigger(PSKEY_LT, CONTROLLER_1) == true)
		{
			g_Player.bAttack[0] = true;
			for (int i = ATTACK_NUM - 1; i >= 0; i--)
			{
				if (g_Player.bAttack[i])
				{
					switch (i)
					{
					case 0:
						g_Player.nAttackCount = 0;
						SetLongAttack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[1] = true;
						break;
					case 1:
						g_Player.nAttackCount = 0;
						SetLong2Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[2] = true;
						break;
					case 2:
						g_Player.nAttackCount = 0;
						SetLong3Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[3] = true;
						break;
					case 3:
						g_Player.nAttackCount = 0;
						SetLong4Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[4] = true;
						break;
					case 4:
						SetLong5Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						for (int i = 0; i < ATTACK_NUM; i++)
						{
							g_Player.bAttack[i] = false;
						}
						break;
					}
					g_Player.Pata[i] = PATA_LONG;
					break;
				}
			}
		}
	}
	else if (!strcmp(ControllerName(CONTROLLER_1), NIN_CON))
	{
	if (!g_Player.bJump || g_Player.JumpCount > 0)
	{
		if (GetdJoykeyPress(NINKEY_B, CONTROLLER_1) == true)
		{
			g_Player.move.y = PLAYER_JUMP;
			g_Player.JumpCount--;
		}
	}

		if (GetdJoykeyTrigger(NINKEY_RT, CONTROLLER_1) == true)
		{
			ShortMovePlyer();
			g_Player.bAttack[0] = true;
			for (int i = ATTACK_NUM - 1; i >= 0; i--)
			{
				if (g_Player.bAttack[i])
				{
					switch (i)
					{
					case 0:
						g_Player.nAttackCount = 0;
						SetShortAttack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[1] = true;
						break;
					case 1:
						g_Player.nAttackCount = 0;
						SetShort2Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[2] = true;
						break;
					case 2:
						g_Player.nAttackCount = 0;
						SetShort3Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[3] = true;
						break;
					case 3:
						g_Player.nAttackCount = 0;
						SetShort4Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[4] = true;
						break;
					case 4:
						SetShort5Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						for (int i = 0; i < ATTACK_NUM; i++)
						{
							g_Player.bAttack[i] = false;
						}
						break;
					}
					g_Player.Pata[i] = PATA_SHORT;
					break;
				}
			}
		}
		else if (GetdJoykeyTrigger(NINKEY_LT, CONTROLLER_1) == true)
		{
			g_Player.bAttack[0] = true;
			for (int i = ATTACK_NUM - 1; i >= 0; i--)
			{
				if (g_Player.bAttack[i])
				{
					switch (i)
					{
					case 0:
						g_Player.nAttackCount = 0;
						SetLongAttack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[1] = true;
						break;
					case 1:
						g_Player.nAttackCount = 0;
						SetLong2Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[2] = true;
						break;
					case 2:
						g_Player.nAttackCount = 0;
						SetLong3Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[3] = true;
						break;
					case 3:
						g_Player.nAttackCount = 0;
						SetLong4Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[4] = true;
						break;
					case 4:
						SetLong5Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						for (int i = 0; i < ATTACK_NUM; i++)
						{
							g_Player.bAttack[i] = false;
						}
						break;
					}
					g_Player.Pata[i] = PATA_LONG;
					break;
				}
			}
		}
    }
	else if (!IsXInputControllerConnected(CONTROLLER_1) && IsDirectInputControllerConnected(CONTROLLER_1))
	{
	if (!g_Player.bJump || g_Player.JumpCount > 0)
	{
		if (GetdJoykeyPress(DKEY_A, CONTROLLER_1) == true)
		{
			g_Player.move.y = PLAYER_JUMP;
			g_Player.JumpCount--;
		}
	}

		if (GetdJoykeyTrigger(DKEY_RT, CONTROLLER_1) == true)
		{
			ShortMovePlyer();
			g_Player.bAttack[0] = true;
			for (int i = ATTACK_NUM - 1; i >= 0; i--)
			{
				if (g_Player.bAttack[i])
				{
					switch (i)
					{
					case 0:
						g_Player.nAttackCount = 0;
						SetShortAttack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[1] = true;
						break;
					case 1:
						g_Player.nAttackCount = 0;
						SetShort2Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[2] = true;
						break;
					case 2:
						g_Player.nAttackCount = 0;
						SetShort3Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[3] = true;
						break;
					case 3:
						g_Player.nAttackCount = 0;
						SetShort4Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[4] = true;
						break;
					case 4:
						SetShort5Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						for (int i = 0; i < ATTACK_NUM; i++)
						{
							g_Player.bAttack[i] = false;
						}
						break;
					}
					g_Player.Pata[i] = PATA_SHORT;
					break;
				}
			}
		}
		else if (GetdJoykeyTrigger(DKEY_LT, CONTROLLER_1) == true)
		{
			g_Player.bAttack[0] = true;
			for (int i = ATTACK_NUM - 1; i >= 0; i--)
			{
				if (g_Player.bAttack[i])
				{
					switch (i)
					{
					case 0:
						g_Player.nAttackCount = 0;
						SetLongAttack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[1] = true;
						break;
					case 1:
						g_Player.nAttackCount = 0;
						SetLong2Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[2] = true;
						break;
					case 2:
						g_Player.nAttackCount = 0;
						SetLong3Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[3] = true;
						break;
					case 3:
						g_Player.nAttackCount = 0;
						SetLong4Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						g_Player.bAttack[4] = true;
						break;
					case 4:
						SetLong5Attack(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft);
						for (int i = 0; i < ATTACK_NUM; i++)
						{
							g_Player.bAttack[i] = false;
						}
						break;
					}
					g_Player.Pata[i] = PATA_LONG;
					break;
				}
			}
		}
	}

	SpAttackMaster(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2, g_Player.pos.z), g_Player.rot.z, g_Player.fLength, ATTACKTYPE_PLAYER, g_Player.bLeft, &g_Player.Pata[0]);

	if (g_Player.bJump)
	{
		g_Player.move.x *= JUMP_MOVE_INA;
	}

	g_Player.move.y += GRAVITY;

	//過去の位置の保存
	g_Player.Oldpos = g_Player.pos;

	//位置の更新
    g_Player.pos.x += g_Player.move.x;
    g_Player.pos.y += g_Player.move.y;

	g_Player.pos -= GetScroll();

	g_Player.pBlock = NULL;

	g_Player.bJump = !CllisionBlock(&g_Player.pos, &g_Player.Oldpos, &g_Player.move, PLAYER_WIDTH * (g_Player.fLength / g_Player.fLengthDef), PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef), &g_Player.pBlock, true);

	if (g_Player.pBlock != NULL)
	{
		g_Player.pos += g_Player.pBlock->move;
		g_Player.pOldBlock = g_Player.pBlock;
	}

	Enemy *pEnemy;
	pEnemy = GetEnemy();
	for (int i = 0; i < MAX_ENEMY; i++,pEnemy++)
	{
		if (pEnemy->bUse)
		{
			float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
			switch (pEnemy->nType)
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

			if (g_Player.pos.x + PLAYER_WIDTH * (g_Player.fLength / g_Player.fLengthDef) / 2 > pEnemy->pos.x - EnemyWidth / 2 && g_Player.pos.x - PLAYER_WIDTH * (g_Player.fLength / g_Player.fLengthDef) / 2 < pEnemy->pos.x + EnemyWidth / 2)
			{
				if (g_Player.Oldpos.y <= pEnemy->pos.y - EnemyHeight && g_Player.pos.y > pEnemy->pos.y - EnemyHeight + BLOCK_SPEEDY - GetScroll().y + pEnemy->move.y)
				{
					g_Player.bJump = false;
					g_Player.pos.y = pEnemy->pos.y - EnemyHeight + BLOCK_SPEEDY - GetScroll().y + pEnemy->move.y;
					g_Player.move.y = 0.0f;

					g_Player.pos.x += pEnemy->move.x;
					g_Player.pos.y += pEnemy->move.y;
				}
				else if (g_Player.Oldpos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) >= pEnemy->pos.y + EnemyHeight && g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) < pEnemy->pos.y + EnemyHeight + BLOCK_SPEEDY - GetScroll().y + pEnemy->move.y)
				{
					g_Player.pos.y = pEnemy->pos.y + EnemyHeight + BLOCK_SPEEDY - GetScroll().y + pEnemy->move.y + PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef);
					g_Player.move.y = 0.0f;
				}
			}
			if (g_Player.pos.y > pEnemy->pos.y - EnemyHeight && g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) < pEnemy->pos.y)
			{
				if (g_Player.Oldpos.x + PLAYER_WIDTH * (g_Player.fLength / g_Player.fLengthDef) / 2 <= pEnemy->pos.x - EnemyWidth / 2 && g_Player.pos.x + PLAYER_WIDTH * (g_Player.fLength / g_Player.fLengthDef) / 2 > pEnemy->pos.x - EnemyWidth / 2 + BLOCK_SPEEDX - GetScroll().x + pEnemy->move.x)
				{
					g_Player.pos.x = pEnemy->pos.x - EnemyWidth / 2 + BLOCK_SPEEDX - GetScroll().x + pEnemy->move.x - PLAYER_WIDTH * (g_Player.fLength / g_Player.fLengthDef) / 2;
					g_Player.move.x = 0.0f;
				}
				else if (g_Player.Oldpos.x - PLAYER_WIDTH * (g_Player.fLength / g_Player.fLengthDef) / 2 >= pEnemy->pos.x + EnemyWidth / 2 && g_Player.pos.x - PLAYER_WIDTH * (g_Player.fLength / g_Player.fLengthDef) / 2 < pEnemy->pos.x + EnemyWidth / 2 + BLOCK_SPEEDX - GetScroll().x + pEnemy->move.x)
				{
					g_Player.pos.x = pEnemy->pos.x + EnemyWidth / 2 + BLOCK_SPEEDX - GetScroll().x + pEnemy->move.x + PLAYER_WIDTH * (g_Player.fLength / g_Player.fLengthDef) / 2;
					g_Player.move.x = 0.0f;
				}
			}
		}
	}

	if (!g_Player.bJump)
	{
		g_Player.JumpCount = JUMP_NUM;
	}
	else
	{
		if (g_Player.JumpCount == JUMP_NUM)
		{
			g_Player.JumpCount--;
		}
	}

	//if (g_Player.pos.x > SCREEN_WIDTH + g_Player.fLength)
	//{
	//	g_Player.pos.x = -g_Player.fLength;
	//}
	//else if (g_Player.pos.x < -g_Player.fLength)
	//{
	//	g_Player.pos.x = SCREEN_WIDTH + g_Player.fLength;
	//}

	if (g_Player.pos.x + PLAYER_WIDTH < 0 || g_Player.pos.x - PLAYER_WIDTH > SCREEN_WIDTH || g_Player.pos.y < 0 || g_Player.pos.y - PLAYER_HEIGHT > SCREEN_HEIGHT)//画面内
	{
		g_Player.bDisp = false;
	}
	else
	{
		g_Player.bDisp = true;
	}

	//移動量の更新(減衰)
	g_Player.move.x += (PLAYER_SPEED_DEF - g_Player.move.x) * PLAYER_INA;

	if (g_Player.move.x > 0.0001f || g_Player.move.x < -0.0001f)
	{
		g_Player.bMove = true;
	}
	else
	{
		g_Player.bMove = false;
	}

	if (g_Player.nAttackCount>=COMBO_TIME)
	{
		for (int i = 0; i < ATTACK_NUM; i++)
		{
			g_Player.bAttack[i] = false;
		}
		for (int i = 0; i < ATTACK_NUM; i++)
		{
			g_Player.Pata[i] = (PATA)-1;
		}
	}

	g_Player.nAttackCount++;

	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

	//座標設定
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2 + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2 + cosf(g_Player.rot.z - (D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2 + cosf(g_Player.rot.z - g_Player.fAngle) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2 + cosf(g_Player.rot.z + g_Player.fAngle) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;

	g_pVtxBuffPlayer->Unlock();//プレイヤーバッファのアンロック

	g_Player.nCounterAnim++;
	if ((g_Player.nCounterAnim % UP_TIME) == 0)//更新期間
	{
		g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % U_MAX * !g_Player.bJump * g_Player.bMove + g_Player.bLeft * U_MAX + g_Player.bJump;

		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player.nPatternAnim, UV_DEF / V_MAX * (g_Player.nPatternAnim / U_MAX));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player.nPatternAnim / U_MAX));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player.nPatternAnim, UV_DEF / V_MAX * (g_Player.nPatternAnim / U_MAX) + UV_DEF / V_MAX);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player.nPatternAnim / U_MAX) + UV_DEF / V_MAX);

		g_pVtxBuffPlayer->Unlock();//プレイヤーバッファのアンロック
	}

	if (g_Player.pos.y - PLAYER_HEIGHT > SCREEN_HEIGHT && g_Player.state != PLAYERSTATE_DIE)
	{
		if (g_Player.nRema <= 0)
		{
			SetParticle(D3DXVECTOR3(g_Player.pos.x, SCREEN_HEIGHT, g_Player.pos.z), g_Player.fLength);
			if (GetMode() == MODE_GAME)
			{
				AddScore(-SCORE_DIE);
			}
			g_Player.state = PLAYERSTATE_DIE;
		}
		else
		{
			SetParticle(D3DXVECTOR3(g_Player.pos.x, SCREEN_HEIGHT, g_Player.pos.z), g_Player.fLength);
			if (GetMode() == MODE_GAME)
			{
				AddScore(-SCORE_DIE);
			}
			g_Player.state = PLAYERSTATE_APPEAR;
			g_Player.nCounterState = 0;
			g_Player.nRema--;
			g_Player.nLife = PALYER_LIFE;
			if (g_Player.pOldBlock != NULL)
			{
				g_Player.pos = D3DXVECTOR3(g_Player.pOldBlock->pos.x, g_Player.pOldBlock->pos.y - BLOCK_HEIGHT_DEF * 2.0f, 0.0f);//再開位置
			}
			else
			{
				g_Player.pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f);//初期位置
			}
		}
		SetVibrate(1.0f, CONTROLLER_1);
	}


	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:
		SetVibrate(0.0f, CONTROLLER_1);

		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_pVtxBuffPlayer->Unlock();//プレイヤーバッファのアンロック
		break;
	case PLAYERSTATE_APPEAR:
		g_Player.nCounterState++;
		if (g_Player.nCounterState >= 60)
		{
			g_Player.state = PLAYERSTATE_NORMAL;
			g_Player.bDisp = true;
		}
		else if (g_Player.nCounterState % 5 == 0)
		{
			g_Player.bDisp = !g_Player.bDisp;
		}
		break;
	case PLAYERSTATE_DAMAGE:
		g_Player.nCounterState--;
		if (g_Player.nCounterState <= 0)
		{
			g_Player.state = PLAYERSTATE_NORMAL;

			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_pVtxBuffPlayer->Unlock();//プレイヤーバッファのアンロック
		}

		break;
	case PLAYERSTATE_DIE:
		g_Player.bDisp = false;
		break;
	}
}

//-------------------
//プレイヤー描画処理
//-------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	if (g_Player.bDisp)
	{
		//頂点バッファ
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);

		if (GetMode() != MODE_VS)
		{
			//プレイヤーの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				0,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}
}

//------------------------
//攻撃移動処理
//------------------------
void ShortMovePlyer(void)
{
	Enemy* pEnemy;
	float Xlong = 0.0f, Ylong = 0.0f;
	pEnemy = GetEnemy();
	for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
	{
		if (pEnemy->bUse && pEnemy->bDisp)
		{
			float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
			switch (pEnemy->nType)
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
			Xlong = pEnemy->pos.x - g_Player.pos.x;
			Ylong = pEnemy->pos.y - EnemyHeight / 2 - g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) * 0.5f;
			g_Player.move += D3DXVECTOR3(sinf(atan2f(Xlong, Ylong)) * ATTACK_MOVE_INA, cosf(atan2f(Xlong, Ylong)) * ATTACK_MOVE_INA, 0.0f);
		}
	}
}

//-------------------
//ヒット処理
//-------------------
void HitPlayer(int nDamage, D3DXVECTOR3 pos)
{
	if (g_Player.state!=PLAYERSTATE_DIE&& g_Player.state != PLAYERSTATE_APPEAR)
	{
		VERTEX_2D* pVtx;

		g_Player.nLife -= nDamage;

		if (g_Player.nLife <= 0)
		{
			if (g_Player.nRema <= 0)
			{
				SetParticle(g_Player.pos, g_Player.fLength);
				if (GetMode() == MODE_GAME)
				{
					AddScore(-nDamage * SCORE_DIE);
				}
				g_Player.state = PLAYERSTATE_DIE;
			}
			else
			{
				SetParticle(g_Player.pos, g_Player.fLength);
				if (GetMode() == MODE_GAME)
				{
					AddScore(-nDamage * SCORE_DIE);
				}
				g_Player.state = PLAYERSTATE_APPEAR;
				g_Player.nCounterState = 0;
				g_Player.nRema--;
				g_Player.nLife = PALYER_LIFE;
			}
			SetVibrate(1.0f, CONTROLLER_1);
		}
		else
		{
			float Xlong = 0.0f, Ylong = 0.0f;
			Xlong = g_Player.pos.x - pos.x;
			Ylong = g_Player.pos.y - PLAYER_HEIGHT * (g_Player.fLength / g_Player.fLengthDef) / 2 - pos.y;
			float Vector = atan2f(Xlong, Ylong);
			g_Player.move = D3DXVECTOR3(sinf(Vector) * HIT_VECTOR, cosf(Vector) * HIT_VECTOR, 0.0f);

			g_Player.state = PLAYERSTATE_DAMAGE;
			g_Player.nCounterState = 5;
			if (GetMode()==MODE_GAME)
			{
				AddScore(-nDamage * SCORE_MISS);
			}

			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffPlayer->Unlock();//バッファのアンロック
		}
	}
}

//-------------------------
//プレイヤーバッファの取得
//-------------------------
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer(void)
{
	return g_pVtxBuffPlayer;
}

//-------------------------
//プレイヤー取得
//-------------------------
Player* GetPlayer(void)
{
	return &g_Player;
}