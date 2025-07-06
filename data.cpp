//---------------------------------------
//
//�Z�[�u�E���[�h����[data.cpp]
//Author fuma sato
//
//---------------------------------------

#include"data.h"

//------------------
//�\�[�g�֐�
//------------------
int *Soat(int* pData)
{
	//�ϐ���錾
	int nRam = 0;

	for (int i = 0; i < MAX_DATA; i++)
	{//��r�Ώ�1
		for (int i2 = i + 1; i2 < MAX_DATA+1; i2++)
		{//��r�Ώ�2
			if (pData[i] < pData[i2])
			{
				//�傫���ق��ɓ���ւ���
				nRam = pData[i];
				pData[i] = pData[i2];
				pData[i2] = nRam;
			}
		}
	}
	return &pData[0];
}
//---------------------------------------
//�Z�[�u����
//---------------------------------------
void SaveData(int* pData)
{
	FILE* pFile;//�t�@�C���|�C���^��錾

	pFile = fopen(DATA_FILE, "wb");//�t�@�C�����J��
	if (pFile != NULL)
	{//�J������
		fwrite(pData, sizeof(int), MAX_DATA,pFile);//���l����������

		fclose(pFile);//�t�@�C�������
	}
	else
	{//�J���Ȃ�����
		HWND hWnd;
		hWnd = GethWnd();
		ReleaseCursor();
		while (ShowCursor(TRUE) < 0);
		MessageBox(hWnd, "�Z�[�u�G���[", "�Z�[�u�ł��Ȃ�������", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
	}
}

//---------------------------------------
//���[�h����
//---------------------------------------
int *LoadData(void)
{
	FILE* pFile;//�t�@�C���|�C���^��錾
	static int aData[MAX_DATA] = { 0 };

	for (int i = 0; i < MAX_DATA; i++)
	{
		aData[i] = 0;
	}

	pFile = fopen(DATA_FILE, "rb");//�t�@�C�����J��
	if (pFile != NULL)
	{//�J������
		fread(&aData[0], sizeof(int), MAX_DATA, pFile);//���l����������

		fclose(pFile);//�t�@�C�������
		return &aData[0];
	}
	else
	{//�J���Ȃ�����
		HWND hWnd;
		hWnd = GethWnd();
		ReleaseCursor();
		while (ShowCursor(TRUE) < 0);
		MessageBox(hWnd, "���[�h�G���[", "���[�h�ł��Ȃ�������", MB_OK | MB_ICONERROR);
		PostMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
		return &aData[0];
	}
}