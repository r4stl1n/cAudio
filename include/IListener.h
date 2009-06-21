
#ifndef ILISTENER_H
#define ILISTENER_H

namespace cAudio
{
    class IListener
    {
    public:
        virtual ~IListener() {}

	//!Sets the listeners position
        virtual void setPosition(ALfloat x,ALfloat y, ALfloat z) = 0;
	//!Sets the listeners orientation
        virtual void setOrientation(ALfloat x,ALfloat y,ALfloat z,ALfloat upx,ALfloat upy,ALfloat upz) = 0;
    protected:
        IListener() {}
    private:
    };
}
#endif //! ILISTENER_H
