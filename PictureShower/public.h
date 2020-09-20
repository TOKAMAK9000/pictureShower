#pragma once
//#include <string>

class option {
public:
	CString PDuration;
	CString PTransitionMode;
	CString PfilePath;
	CString PStretch;
	CString PVoice_text;
	int PVoice_ins;
	bool PclearPicture;

public:
	option() {
		PDuration = _T("3");
		PTransitionMode = _T("0");
		PfilePath = _T("");
		PclearPicture = 0;
		PStretch = _T("0");
		PVoice_text = _T("");
		PVoice_ins = -1;
	}
};


extern option mOption;