/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_GREAPER_LIBRARY_H
#define CORE_GREAPER_LIBRARY_H 1

#include "Library.h"
#include "Uuid.h"
#include "Result.h"

namespace greaper
{
	class IInterface;
	class IGreaperLibrary
	{
	protected:
		virtual EmptyResult RegisterProperty(SPtr<IProperty> property) = 0;

	public:
		static constexpr Uuid LibraryUUID = Uuid{  };
		static constexpr StringView LibraryName = StringView{ "Unknown Greaper Library" };

		virtual ~IGreaperLibrary() = default;

		virtual void InitLibrary(PLibrary lib, WPtr<IApplication> app) = 0;

		virtual void InitManagers() = 0;

		virtual void InitProperties() = 0;

		virtual void InitReflection() = 0;

		virtual void DeinitReflection() = 0;

		virtual void DeinitProperties() = 0;

		virtual void DeinitManagers() = 0;

		virtual void DeinitLibrary() = 0;

		INLINE void Initialize(PLibrary lib, WPtr<IApplication> app)
		{
			InitLibrary(lib, app);
			InitManagers();
			InitProperties();
			InitReflection();
		}

		INLINE void Deinitialize()
		{
			DeinitReflection();
			DeinitProperties();
			DeinitManagers();
			DeinitLibrary();
		}

		virtual int GetLibraryVersion()const = 0;

		virtual const Uuid& GetLibraryUuid()const = 0;

		virtual const StringView& GetLibraryName()const = 0;

		virtual WPtr<IApplication> GetApplication()const = 0;

		virtual WPtr<Library> GetOSLibrary()const = 0;

		virtual CRange<SPtr<IInterface>> GetManagers()const = 0;

		virtual CRange<SPtr<IProperty>> GetPropeties()const = 0;

		virtual Result<WPtr<IProperty>> GetProperty(const StringView& name)const = 0;

		virtual void LogVerbose(const String& message) = 0;

		virtual void Log(const String& message) = 0;

		virtual void LogWarning(const String& message) = 0;

		virtual void LogError(const String& message) = 0;

		virtual void LogCritical(const String& message) = 0;

		template<class T, class _Alloc_>
		friend Result<SPtr<TProperty<T>>> CreateProperty(const WPtr<IGreaperLibrary>&, StringView, T, StringView,
			bool, bool, TPropertyValidator<T>*);
	};

	using PGreaperLib = SPtr<IGreaperLibrary>;
	using WGreaperLib = WPtr<IGreaperLibrary>;

	template<class T>
	struct ValidGreaperLibrary
	{
		static_assert(std::is_base_of_v<IGreaperLibrary, T>, "Trying to validate a GreaperLibrary that does not derive from IGreaperLibrary");
		static constexpr bool UUIDValid = T::LibraryUUID != IGreaperLibrary::LibraryUUID;
		static constexpr bool NameValid = T::LibraryName != IGreaperLibrary::LibraryName;

		static constexpr bool Valid = UUIDValid && NameValid;
	};
}

#endif /* CORE_GREAPER_LIBRARY_H */