// Ŭnicode please 
#include "stdafx.h"
#include "OpenALDecoder.h"
#include <sys/stat.h>
#include <AL/alut.h>

namespace CocosDenshion {;
static int checkALError(const char *funcName)
{
	int err = alGetError();

	if (err != AL_NO_ERROR)
	{
		switch (err)
		{
		case AL_INVALID_NAME:
			fprintf(stderr, "AL_INVALID_NAME in %s\n", funcName);
			break;

		case AL_INVALID_ENUM:
			fprintf(stderr, "AL_INVALID_ENUM in %s\n", funcName);
			break;

		case AL_INVALID_VALUE:
			fprintf(stderr, "AL_INVALID_VALUE in %s\n", funcName);
			break;

		case AL_INVALID_OPERATION:
			fprintf(stderr, "AL_INVALID_OPERATION in %s\n", funcName);
			break;

		case AL_OUT_OF_MEMORY:
			fprintf(stderr, "AL_OUT_OF_MEMORY in %s\n", funcName);
			break;
		}
	}

	return err;
}

class AlutDecoder : public OpenALDecoder
{
	bool decode(OpenALFile &file, ALuint &result)
	{
		result = alutCreateBufferFromFile(file.filename.c_str());
		if (AL_NONE == result) {
			puts(alutGetErrorString(alutGetError()));
			return false;
		}
		return true;
	}

	bool acceptsFormat(Format format) const
	{
		return Wav == format || Raw == format;
	}
};

class DataRaii
{
public:
	char *data;
	size_t size;

	DataRaii() : data(0), size(0) {}
	~DataRaii() { delete [] data; }
};


std::vector<OpenALDecoder *> OpenALDecoder::_decoders;

void OpenALDecoder::installDecoders()
{
	addDecoder(new AlutDecoder());
}
void OpenALDecoder::uninstallDecoders()
{
	for(auto decoder : _decoders) {
		delete(decoder);
	}
	_decoders.clear();
}

void OpenALDecoder::addDecoder(OpenALDecoder *decoder)
{
	if (decoder)
		_decoders.push_back(decoder);
}

bool OpenALDecoder::initALBuffer(ALuint &result, ALenum format,
								 const ALvoid *data, ALsizei size, ALsizei freq)
{
	// Load audio data into a buffer.
	alGenBuffers(1, &result);

	if (checkALError("initALBuffer:alGenBuffers") != AL_NO_ERROR)
	{
		fprintf(stderr, "Couldn't generate OpenAL buffer\n");
		return false;
	}

	alBufferData(result, format, data, size, freq);
	checkALError("initALBuffer:alBufferData");
	return true;
}

const std::vector<OpenALDecoder *> &OpenALDecoder::getDecoders()
{
	return _decoders;
}


} // namespace CocosDenshion
