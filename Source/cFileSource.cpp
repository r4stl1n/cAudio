#include "../Headers/cFileSource.h"
#include <cstring>

namespace cAudio
{

//!Init Takes a string for file name
cFileSource::cFileSource(const std::string& filename) : pFile(NULL), Valid(false), Filesize(0)
{
    if(filename.length() != 0)
    {
		pFile = fopen(filename.c_str(),"rb");
		if(pFile)
			Valid = true;
    }

    if(Valid)
    {
        fseek(pFile, 0, SEEK_END);
        Filesize = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
    }
}

cFileSource::~cFileSource()
{
    fclose(pFile);
}

//!Returns true if the FileSource is valid
bool cFileSource::isValid()
{
    return Valid;
}

//!Returns the current position of the file stream.
int cFileSource::getCurrentPos()
{
    return ftell(pFile);
}

//!Returns the file size
int cFileSource::getSize()
{
    return Filesize;
}

//!Read current FileSource Data
int cFileSource::read(void* output, int size)
{
	return fread(output, sizeof(char), size, pFile);
}

//!Seek the file stream
bool cFileSource::seek(int amount, bool relative)
{
    if(relative == true)
    {
        int oldamount = ftell(pFile);
        fseek(pFile, amount, SEEK_CUR);

        //check against the absolute position
        if(oldamount+amount != ftell(pFile))
            return false;
    }
    else
    {
        fseek(pFile, amount, SEEK_SET);
        if(amount != ftell(pFile))
            return false;
    }

    return true;
}

};
