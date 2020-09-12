#pragma once
//#include <string>

class option {
public:
	CString PDuration;
	CString PTransitionMode;
	CString PfilePath;
	CString PStretch;
	CString PVoice;
	bool PclearPicture;

public:
	option() {
		PDuration = _T("3");
		PTransitionMode = _T("0");
		PfilePath = _T("");
		PclearPicture = 0;
		PStretch = _T("0");
		PVoice = _T("");
	}
};


extern option mOption;