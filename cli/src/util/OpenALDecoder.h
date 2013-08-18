// Ŭnicode please 
#ifndef COCOSDENSHION_OPENALLOADER_H
#define COCOSDENSHION_OPENALLOADER_H

#include <vector>
#include <string>
#include <stdio.h>
#include <AL/al.h>


namespace CocosDenshion {;

struct OpenALFile
{
	std::string filename; ///< For logging.
	OpenALFile() {}
	~OpenALFile() { }
};

class OpenALDecoder
{
public:
	enum Format {
		Mp3 = 0,
		Vorbis,
		Wav,
		Raw,
		Flac,
		Midi,
		Aac
	};

	virtual ~OpenALDecoder() {}

	/// Returns true if such format is supported and decoding was successful.
	virtual bool decode(OpenALFile &file, ALuint &result) = 0;
	virtual bool acceptsFormat(Format format) const = 0;

	static const std::vector<OpenALDecoder *> &getDecoders();
	static void installDecoders();
	static void uninstallDecoders();

protected:
	static void addDecoder(OpenALDecoder *decoder);
	bool initALBuffer(ALuint &result, ALenum format,
		const ALvoid* data, ALsizei size, ALsizei freq);

	static std::vector<OpenALDecoder *> _decoders;
};

} // namespace CocosDenshion

#endif // COCOSDENSHION_OPENALLOADER_H
