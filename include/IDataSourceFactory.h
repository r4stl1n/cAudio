// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef IDATASOURCEFACTORY_H
#define IDATASOURCEFACTORY_H

#include "IDataSource.h"

namespace cAudio
{

class IDataSourceFactory
{
    public:
		IDataSourceFactory() { }
		virtual ~IDataSourceFactory() { }

		virtual IDataSource* CreateDataSource(const char* filename, bool streamingRequested) = 0;
	protected:
	private:
};

};

#endif //! IDATASOURCEFACTORY_H
