#ifndef IDATASOURCE_H
#define IDATASOURCE_H

namespace cAudio
{

class IDataSource
{
    public:
        IDataSource() {  }
        virtual ~IDataSource() {  }

        //!Returns whether the source is valid (in case of an error, like the file couldn't be found)
        virtual bool isValid() = 0;

        //!Get the current location in the data stream
        virtual int getCurrentPos() = 0;

        //!Get the total size of the data stream
        virtual int getSize() = 0;

        //!Read out a section of the data stream
        virtual int read(void* output, int size) = 0;

        //!Seek to a position in the data stream
        virtual bool seek(int amount, bool relative) = 0;
};

};

#endif //! IDATASOURCE_H
