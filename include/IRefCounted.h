#ifndef IREFCOUNTED_H
#define IREFCOUNTED_H

namespace cAudio
{
	class IRefCounted
	{
	public:
		IRefCounted() : RefCount(1) { }
		virtual ~IRefCounted() { }

		void grab()
		{
			++RefCount; 
		}

		bool drop()
		{
			--RefCount;
			if (RefCount < 1)
			{
				delete this;
				return true;
			}
			return false;
		}

		int getReferenceCount() const
		{
			return RefCount;
		}

	private:
		int RefCount;
	};	
}
#endif //! IREFCOUNTED_H