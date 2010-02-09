// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

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