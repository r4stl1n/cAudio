// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef CFILESOURCE_H
#define CFILESOURCE_H

#include "../include/IDataSource.h"
#include <string>
#include <iostream>
#include <fstream>

namespace cAudio
{

class cFileSource : public IDataSource
{
    public:
        cFileSource(const char* filename);
        ~cFileSource();

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
		//!Hold if valid        
		bool Valid;
		//!Holds file size
        int Filesize;
		//!File stream
        FILE* pFile;
    private:
};

};

#endif //! CFILESOURCE_H
