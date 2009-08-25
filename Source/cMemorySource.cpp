#include "../Headers/cMemorySource.h"
#include <cstring>
#include <iostream>

namespace cAudio
{

cMemorySource::cMemorySource(const void* data, int size, bool copy) : Data(NULL), Size(0), Valid(false), Pos(0)
{
    if(data && size > 0)
    {
        Size = size;
        if(copy)
        {
            Data = new char[Size];
			if(Data)
				memcpy(Data, data, Size);
        }
        else
        {
            Data = (char*)data;
        }
		if(Data)
			Valid = true;
    }
}

cMemorySource::~cMemorySource()
{
    delete[] Data;
}

//!Returns true if the DataStream is valid
bool cMemorySource::isValid()
{
    return Valid;
}

//!Returns the current position of the data stream.
int cMemorySource::getCurrentPos()
{
    return Pos;
}

//!Returns the data stream size
int cMemorySource::getSize()
{
    return Size;
}

//!Read current Data Stream Data
int cMemorySource::read(void* output, int size)
{
 
    //memset(output, 0, size);
    if(Pos+size <= Size)
    {
        memcpy(output, Data+Pos, size);
        Pos += size;
        return size;
    }
    else
    {
        int extra = (Pos+size) - Size;
        int copied = size - extra;
        memcpy(output, Data+Pos, copied);
        Pos = Size;
        return copied;
    }
}

//!Seek the data stream
bool cMemorySource::seek(int amount, bool relative)
{
    if(relative)
    {
        Pos += amount;
        if(Pos > Size)
        {
            Pos = Size;
            return false;
        }
    }
    else
    {
        Pos = amount;
        if(Pos > Size)
        {
            Pos = Size;
            return false;
        }
    }

    return true;
}

};
