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
	HWAVEIN hWaveIn;  //输入设备
	WAVEFORMATEX waveform; //採集音频的格式，结构体
	BYTE *pBuffer1;//採集音频时的数据缓存
	WAVEHDR wHdr1; //採集音频时包括数据缓存的结构体
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