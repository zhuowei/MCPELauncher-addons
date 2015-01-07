#pragma once

class SoundDesc {
public:
	void* soundData;
	size_t length; // without header
	int channels; // 1
	int channels2; // 2
	int sampleRate; // 44100
	int wtf2; // 0x38f6
	std::string wtf3; // ???
	void* soundDataWithoutHeader; // basically soundData + 0x10
};
