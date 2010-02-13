// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef IMANAGEREVENTHANDLER_H
#define IMANAGEREVENTHANDLER_H

namespace cAudio{

	class IManagerEventHandler
	{

	public:
		//! This function is called on manager initialization
		virtual void onInit() = 0;
		//! This function is called on manager update
		virtual void onUpdate() = 0;
		//! This function is called on manager release
		virtual void onRelease() = 0;
		//! This function is called on source creation
		virtual void onSourceCreate() = 0;
		//! This function is called on decoder registration
		virtual void onDecoderRegister() = 0;
		//! This function is called on data source registration
		virtual void onDataSourceRegister() = 0;

	};

};

#endif //! IMANAGEREVENTHANDLER_H
