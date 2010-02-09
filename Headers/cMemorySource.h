// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef CMEMORYSOURCE_H
#define CMEMORYSOURCE_H

#include "../include/IDataSource.h"

namespace cAudio
{

//!Class used to read from a memory source.
//!If copy is true, then cMemorySource will make a copy of the data.
//!Otherwise, cMemorySource will take care of deleting the data array for you.
class cMemorySource : public IDataSource
{
    public:
        cMemorySource(const void* data, int size, bool copy);
        ~cMemorySource();

        //!Returns whether the source is valid (in case of an error, like the file couldn't be found)
        virtual bool isValid();

        //!Get the current location in the data stream
        virtual int getCurrentPos();

        //!Get the total size of the data stream
        virtual int getSize();

        //!Read out a section of the data stream
        virtual int read(void* output, int size);

        //!Seek to a position in the data stream
        virtual bool seek(int amount, bool relative);
    protected:
        char* Data;
        int Size;
        bool Valid;
        int Pos;
    private:
};

};

#endif //! CMEMORYSOURCE_H
