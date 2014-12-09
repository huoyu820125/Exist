# Microsoft Developer Studio Project File - Name="cpu" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=cpu - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cpu.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cpu.mak" CFG="cpu - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cpu - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "cpu - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cpu - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\Micro-Development-Kit\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\cpu.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy ..\Micro-Development-Kit\lib\mdk.lib ..\lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "cpu - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\Micro-Development-Kit\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\cpu_d.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy ..\Micro-Development-Kit\lib\mdk_d.lib ..\lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "cpu - Win32 Release"
# Name "cpu - Win32 Debug"
# Begin Group "common"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\common\common.cpp
# End Source File
# Begin Source File

SOURCE=..\common\common.h
# End Source File
# Begin Source File

SOURCE=..\common\MD5Helper.cpp
# End Source File
# Begin Source File

SOURCE=..\common\MD5Helper.h
# End Source File
# Begin Source File

SOURCE=..\common\Protocol.cpp
# End Source File
# Begin Source File

SOURCE=..\common\Protocol.h
# End Source File
# End Group
# Begin Group "source"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "frame"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\source\frame\ConnectPool.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\frame\ConnectPool.h
# End Source File
# Begin Source File

SOURCE=.\source\frame\Container.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\frame\Container.h
# End Source File
# Begin Source File

SOURCE=.\source\frame\Element.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\frame\Element.h
# End Source File
# Begin Source File

SOURCE=.\source\frame\ExistType.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\frame\ExistType.h
# End Source File
# Begin Source File

SOURCE=.\source\frame\IOBus.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\frame\IOBus.h
# End Source File
# Begin Source File

SOURCE=.\source\frame\Key.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\frame\Key.h
# End Source File
# Begin Source File

SOURCE=.\source\frame\KVData.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\frame\KVData.h
# End Source File
# Begin Source File

SOURCE=.\source\frame\Value.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\frame\Value.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\source\Double.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\Double.h
# End Source File
# Begin Source File

SOURCE=..\include\Exist\Exist.h
# End Source File
# Begin Source File

SOURCE=.\source\Float.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\Float.h
# End Source File
# Begin Source File

SOURCE=.\source\Int16.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\Int16.h
# End Source File
# Begin Source File

SOURCE=.\source\Int32.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\Int32.h
# End Source File
# Begin Source File

SOURCE=.\source\Int64.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\Int64.h
# End Source File
# Begin Source File

SOURCE=.\source\Int8.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\Int8.h
# End Source File
# Begin Source File

SOURCE=.\source\Map.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\Map.h
# End Source File
# Begin Source File

SOURCE=.\source\Stream.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\Stream.h
# End Source File
# Begin Source File

SOURCE=.\source\UInt16.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\UInt16.h
# End Source File
# Begin Source File

SOURCE=.\source\UInt32.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\UInt32.h
# End Source File
# Begin Source File

SOURCE=.\source\UInt64.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\UInt64.h
# End Source File
# Begin Source File

SOURCE=.\source\UInt8.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\UInt8.h
# End Source File
# Begin Source File

SOURCE=.\source\Vector.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Exist\Vector.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\makefile
# End Source File
# End Target
# End Project
