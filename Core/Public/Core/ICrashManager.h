/***********************************************************************************
*   Copyright 2021 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_CRASH_MANAGER_H
#define CORE_I_CRASH_MANAGER_H 1

#include "Interface.h"

namespace greaper
{
    class ICrashManager : public TInterface<ICrashManager>
    {
    public:
        static constexpr Uuid InterfaceUUID = Uuid{ 0xF58F9031, 0xDFD24F1D, 0x86C87C5C, 0xAC411A8F };
        static constexpr StringView InterfaceName = StringView{ "CrashManager" };

        virtual ~ICrashManager() = default;

        virtual int32 ReportSEHCrash(void* exceptionData) = 0;

        virtual void ReportCrash(const String& type, const String& description, const String& function = {},
            const String& file = {}, uint32 line = 0) = 0;

        virtual String GetStackTrace() = 0;
    };
}

#endif /* CORE_I_CRASH_MANAGER_H */