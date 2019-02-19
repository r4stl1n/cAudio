// Copyright (c) 2008-2011 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones, Murat (wolfmanfx) Sari
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#include "cFileSource.h"
#include "cUtils.h"
#include <algorithm>

#if CAUDIO_COMPILE_WITH_FILE_SOURCE == 1

#include <cstring>

namespace cAudio
{

std::unordered_map<std::string, std::vector<char>> cFileSource::cache;

cFileSource::cFileSource(const char* filename)
{
	cAudioString safeFilename = fromUTF8(filename);
	if(safeFilename.length() != 0)
	{
		if (cache.count(safeFilename) == 0) { // Is this thread-safe?
			std::ifstream f(safeFilename, std::ios::binary);
			f.unsetf(std::ios_base::skipws);
			cache.emplace(safeFilename, std::vector<char>(std::istreambuf_iterator<char>{f}, {}));
		}

		data = &cache[safeFilename];
		Valid = true;
    }
}

bool cFileSource::isValid()
{
    return Valid;
}

int cFileSource::getCurrentPos()
{
    return currentPos;
}

int cFileSource::getSize()
{
    return data->size();
}

int cFileSource::read(void* output, int size)
{
	const int bytesToRead = std::min(size, getSize() - currentPos);
	memcpy(output, &(*data)[currentPos], bytesToRead);
	currentPos += bytesToRead;
	return bytesToRead;
}

bool cFileSource::seek(int amount, bool relative)
{
	int desiredPosition = relative ? amount + currentPos : amount;
	currentPos = std::min(std::max(desiredPosition, 0), getSize());
	return desiredPosition < getSize() && desiredPosition >= 0;
}

};

#endif
