#pragma once



#include "stdafx.h"
#include "voice.h"


//主函数
int Voice::result()
{
	record();

	// 务必替换百度云控制台中新建百度语音应用的 Api Key 和 Secret Key
	aip::Speech * client = new aip::Speech("20044209", "lglzeMSty1dzbkbaao7am2Wq", "j2vGj2VZBHcV6IeD2zL9xE7Aht3sYiRf");

	// 打印详细请求结果，可以打开查看详细请求内容
	client->setDebug(false);
	std::string result;

	// 语音识别调用
	result = ASR(client);
	delete client;

	std::string text;
	int getins;
	text = transfer2Chinese(result);
	getins = recognize(text);

	return getins;
}


std::string Voice::ASR(aip::Speech * client)
{
	std::map<std::string, std::string> options;
	std::string file_content;
	aip::get_file_content("C:\\Users\\TOKAMAK\\Desktop\\te.pcm", &file_content);
	Json::Value result = client->recognize(file_content, "pcm", 16000, options);
	return result.toStyledString();
}


int Voice::record()
{
	HANDLE wait;
	waveform.wFormatTag = WAVE_FORMAT_PCM;//声音格式为PCM
	waveform.nSamplesPerSec = 16000;//採样率，16000次/秒
	waveform.wBitsPerSample = 16;//採样比特，16bits/次
	waveform.nChannels = 1;//採样声道数，2声道
	waveform.nAvgBytesPerSec = 32000;//每秒的数据率，就是每秒能採集多少字节的数据
	waveform.nBlockAlign = 2;//一个块的大小，採样bit的字节数乘以声道数
	waveform.cbSize = 0;//一般为0

	wait = CreateEvent(NULL, 0, 0, NULL);
	//使用waveInOpen函数开启音频採集
	waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);

	//建立两个数组（这里能够建立多个数组）用来缓冲音频数据
	DWORD bufsize = 1024 * 100;//每次开辟10k的缓存存储录音数据
	int i = 5;
	fopen_s(&pf, "C:\\Users\\TOKAMAK\\Desktop\\te.pcm", "wb");
	while (i--)//录制20左右秒声音，结合音频解码和网络传输能够改动为实时录音播放的机制以实现对讲功能
	{
		pBuffer1 = new BYTE[bufsize];
		wHdr1.lpData = (LPSTR)pBuffer1;
		wHdr1.dwBufferLength = bufsize;
		wHdr1.dwBytesRecorded = 0;
		wHdr1.dwUser = 0;
		wHdr1.dwFlags = 0;
		wHdr1.dwLoops = 1;
		waveInPrepareHeader(hWaveIn, &wHdr1, sizeof(WAVEHDR));//准备一个波形数据块头用于录音
		waveInAddBuffer(hWaveIn, &wHdr1, sizeof(WAVEHDR));//指定波形数据块为录音输入缓存
		waveInStart(hWaveIn);//開始录音
		Sleep(1000);//等待声音录制1s
		waveInReset(hWaveIn);//停止录音
		fwrite(pBuffer1, 1, wHdr1.dwBytesRecorded, pf);
		delete [] pBuffer1;
	}
	fclose(pf);

	waveInClose(hWaveIn);
	return 0;
}


std::string Voice::transfer2Chinese(std::string unicode)
{
	CString m_UnicodeToChinese;
	int i = 0;
	int j = 0;
	int len = 0;

	len = unicode.length();
	if (len <= 0)
	{
		return false;
	}

	int nValue = 0;
	WCHAR * pWchar;
	wchar_t* szHex;
	char strchar[6] = { '0','x','\0' };

	for (i = 0; i < len; i++)
	{
		if (unicode[i] == 'u')
		{
			for (j = 2; j < 6; j++)
			{
				i++;
				strchar[j] = unicode[i];
			}

			USES_CONVERSION;
			szHex = A2W(strchar);

			StrToIntExW(szHex, STIF_SUPPORT_HEX, &nValue);
			pWchar = (WCHAR*)& nValue;

			m_UnicodeToChinese = m_UnicodeToChinese + pWchar;
		}
	}
	//wcout.imbue(locale("chs"));

	std::string res;
	res = CT2A(m_UnicodeToChinese.GetString());
	return res;
}


int Voice::recognize(std::string result)
{
	

	int ins = -1;
	for (int i = 0; i < 24; i++) {
		if (result.find(voiceList[i]) < 25) {
			ins = i;
			break;
		}
		else {
			continue;
		}
	}

	return ins;
}

Voice::Voice() {
	voiceList[0] = "直接显示";
	voiceList[1] = "从左到右卷轴展开";
	voiceList[2] = "从右到左卷轴展开";
	voiceList[3] = "从上到下卷轴展开";
	voiceList[4] = "从下到上卷轴展开";
	voiceList[5] = "从左到右平滑展开";
	voiceList[6] = "从右到左平滑展开";
	voiceList[7] = "从上到下平滑展开";
	voiceList[8] = "从下到上平滑展开";
	voiceList[9] = "左右两侧向中间展开";
	voiceList[10] = "上下两侧向中间展开";
	voiceList[11] = "中间向四周展开";
	voiceList[12] = "中间向四周放大";
	voiceList[13] = "左上角展开";
	voiceList[14] = "左下角展开";
	voiceList[15] = "右下角展开";
	voiceList[16] = "右上角展开";
	voiceList[17] = "左上角放大";
	voiceList[18] = "左下角放大";
	voiceList[19] = "右下角放大";
	voiceList[20] = "右上角放大";
	voiceList[21] = "随机方格";
	voiceList[22] = "中间向两侧展开";
	voiceList[23] = "从左向右百叶窗展开";
	voiceList[24] = "从右向左百叶窗展开";
}

Voice::~Voice()
{
	delete[] voiceList;
}
