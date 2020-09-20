#pragma once
#include <Windows.h>
#include <mmsystem.h>

//#include <fstream>
//#include <io
#include "speech.h"

#pragma comment(lib, "winmm.lib")

//#define 

class Voice {
public:
	HWAVEIN hWaveIn;  //�����豸
	WAVEFORMATEX waveform; //����Ƶ�ĸ�ʽ���ṹ��
	BYTE *pBuffer1;//����Ƶʱ�����ݻ���
	WAVEHDR wHdr1; //����Ƶʱ�������ݻ���Ľṹ��
	FILE *pf;

	Voice();
	~Voice();
	int result();
	std::string ASR(aip::Speech* client);
	int record();
	std::string transfer2Chinese(std::string unicode);
	int recognize(std::string result);
	std::string *voiceList = new std::string[25];
	
};