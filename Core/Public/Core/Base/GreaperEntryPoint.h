/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#define CreateGreaperEntrypoint(nspace)											  \
BEGIN_C																			  \
namespace nspace::_Impl															  \
{																				  \
	DLLEXPORT void* CDECL _Greaper();											  \
}																				  \
END_C																			  \
namespace nspace																  \
{																				  \
	extern greaper::IGreaperLibrary* GetCurrentLibrary()						  \
	{																			  \
		return static_cast<greaper::IGreaperLibrary*>(_Impl::_Greaper());		  \
	}																			  \
}


//#define GetGreaperEntrypoint(nspace)											  \
//BEGIN_C																		  \
//namespace nspace																  \
//{																				  \
//	DLLIMPORT void* CDECL _Greaper();											  \
//}																				  \
//END_C																			  \
//namespace nspace																  \
//{																				  \
//	extern greaper::IGreaperLibrary* GetCurrentLibrary()						  \
//	{																			  \
//		return static_cast<greaper::IGreaperLibrary*>(_Greaper());				  \
//	}																			  \
//}

/*
#ifdef IS_GREAPER_MAIN_LIB
CreateGreaperEntrypoint(greaper)
#else
GetGreaperEntrypoint(greaper)
#endif
*/
