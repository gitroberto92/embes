#pragma once

class IAudioDataReceiver
{
public:
	virtual bool AppendAudioData(unsigned char* p_pData, int p_nBytes) = 0;
	virtual bool IsRunningState() = 0;
};
