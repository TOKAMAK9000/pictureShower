#pragma once



#include "stdafx.h"
#include "voice.h"


//������
int Voice::result()
{
	record();

	// ����滻�ٶ��ƿ���̨���½��ٶ�����Ӧ�õ� Api Key �� Secret Key
	aip::Speech * client = new aip::Speech("20044209", "lglzeMSty1dzbkbaao7am2Wq", "j2vGj2VZBHcV6IeD2zL9xE7Aht3sYiRf");

	// ��ӡ��ϸ�����������Դ򿪲鿴��ϸ��������
	client->setDebug(false);
	std::string result;

	// ����ʶ�����
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
	waveform.wFormatTag = WAVE_FORMAT_PCM;//������ʽΪPCM
	waveform.nSamplesPerSec = 16000;//�����ʣ�16000��/��
	waveform.wBitsPerSample = 16;//�������أ�16bits/��
	waveform.nChannels = 1;//������������2����
	waveform.nAvgBytesPerSec = 32000;//ÿ��������ʣ�����ÿ���ܒ񼯶����ֽڵ�����
	waveform.nBlockAlign = 2;//һ����Ĵ�С������bit���ֽ�������������
	waveform.cbSize = 0;//һ��Ϊ0

	wait = CreateEvent(NULL, 0, 0, NULL);
	//ʹ��waveInOpen����������Ƶ��
	waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);

	//�����������飨�����ܹ�����������飩����������Ƶ����
	DWORD bufsize = 1024 * 100;//ÿ�ο���10k�Ļ���洢¼������
	int i = 5;
	fopen_s(&pf, "C:\\Users\\TOKAMAK\\Desktop\\te.pcm", "wb");
	while (i--)//¼��20�����������������Ƶ��������紫���ܹ��Ķ�Ϊʵʱ¼�����ŵĻ�����ʵ�ֶԽ�����
	{
		pBuffer1 = new BYTE[bufsize];
		wHdr1.lpData = (LPSTR)pBuffer1;
		wHdr1.dwBufferLength = bufsize;
		wHdr1.dwBytesRecorded = 0;
		wHdr1.dwUser = 0;
		wHdr1.dwFlags = 0;
		wHdr1.dwLoops = 1;
		waveInPrepareHeader(hWaveIn, &wHdr1, sizeof(WAVEHDR));//׼��һ���������ݿ�ͷ����¼��
		waveInAddBuffer(hWaveIn, &wHdr1, sizeof(WAVEHDR));//ָ���������ݿ�Ϊ¼�����뻺��
		waveInStart(hWaveIn);//�_ʼ¼��
		Sleep(1000);//�ȴ�����¼��1s
		waveInReset(hWaveIn);//ֹͣ¼��
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
	voiceList[0] = "ֱ����ʾ";
	voiceList[1] = "�����Ҿ���չ��";
	voiceList[2] = "���ҵ������չ��";
	voiceList[3] = "���ϵ��¾���չ��";
	voiceList[4] = "���µ��Ͼ���չ��";
	voiceList[5] = "������ƽ��չ��";
	voiceList[6] = "���ҵ���ƽ��չ��";
	voiceList[7] = "���ϵ���ƽ��չ��";
	voiceList[8] = "���µ���ƽ��չ��";
	voiceList[9] = "�����������м�չ��";
	voiceList[10] = "�����������м�չ��";
	voiceList[11] = "�м�������չ��";
	voiceList[12] = "�м������ܷŴ�";
	voiceList[13] = "���Ͻ�չ��";
	voiceList[14] = "���½�չ��";
	voiceList[15] = "���½�չ��";
	voiceList[16] = "���Ͻ�չ��";
	voiceList[17] = "���ϽǷŴ�";
	voiceList[18] = "���½ǷŴ�";
	voiceList[19] = "���½ǷŴ�";
	voiceList[20] = "���ϽǷŴ�";
	voiceList[21] = "�������";
	voiceList[22] = "�м�������չ��";
	voiceList[23] = "�������Ұ�Ҷ��չ��";
	voiceList[24] = "���������Ҷ��չ��";
}

Voice::~Voice()
{
	delete[] voiceList;
}
