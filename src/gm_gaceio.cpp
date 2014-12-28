#include "GarrysMod/Lua/Interface.h"
#include "Bootil/Bootil.h"

using namespace GarrysMod::Lua;

int LuaFunc_ListDir( lua_State* state )
{
	if ( !LUA->IsType( 1, Type::STRING ) )
	{
		LUA->PushBool(false);
		LUA->PushString("First parameter not a string");
		return 2;
	}

	const Bootil::BString& folder = LUA->GetString(1);
	Bootil::String::List files;
	Bootil::String::List folders;
	Bootil::File::Find(&files, &folders, folder + "/*", false);

	// Files
	LUA->CreateTable();
	int i = 1;

	BOOTIL_FOREACH (f, files, Bootil::String::List) {
		LUA->PushNumber(i);
		LUA->PushString((*f).c_str());
		LUA->SetTable(-3);

		i++;
	}

	// Folders
	LUA->CreateTable();
	i = 1;

	BOOTIL_FOREACH (f, folders, Bootil::String::List) {
		LUA->PushNumber(i);
		LUA->PushString((*f).c_str());
		LUA->SetTable(-3);

		i++;
	}
	
	return 2;
}

int LuaFunc_ReadFile( lua_State* state )
{
	if ( !LUA->IsType( 1, Type::STRING ) )
	{
		LUA->PushBool(false);
		LUA->PushString("First parameter not a string");
		return 2;
	}

	Bootil::BString out;
	if (!Bootil::File::Read(LUA->GetString(1), out)) {
		LUA->PushBool(false);
		LUA->PushString(Bootil::Platform::LastError().c_str());
		return 2;
	}
	
	LUA->PushString(out.c_str());
	return 1;
}

int LuaFunc_Delete( lua_State* state )
{
	if ( !LUA->IsType( 1, Type::STRING ) )
	{
		LUA->PushBool(false);
		LUA->PushString("First parameter not a string");
		return 2;
	}

	const Bootil::BString& targ = LUA->GetString(1);

	bool success;
	if (Bootil::File::IsFolder(targ)) {
		success = Bootil::File::RemoveFolder(targ); // TODO should we recursively remove (2nd param)?
	}
	else {
		success = Bootil::File::RemoveFile(LUA->GetString(1));
	}

	if (!success) {
		LUA->PushBool(false);
		LUA->PushString(Bootil::Platform::LastError().c_str());
		return 2;
	}

	return 0;
}

int LuaFunc_WriteToFile( lua_State* state )
{
	if ( !LUA->IsType( 1, Type::STRING ) )
	{
		LUA->PushBool(false);
		LUA->PushString("First parameter not a string");
		return 2;
	}
	if ( !LUA->IsType( 2, Type::STRING ) )
	{
		LUA->PushBool(false);
		LUA->PushString("Second parameter not a string");
		return 2;
	}
	
	if (!Bootil::File::Write(LUA->GetString(1), LUA->GetString(2))) {
		LUA->PushBool(false);
		LUA->PushString(Bootil::Platform::LastError().c_str());
		return 2;
	}

	return 0;
}

int LuaFunc_IsFolder( lua_State* state )
{
	if ( !LUA->IsType( 1, Type::STRING ) )
	{
		LUA->PushBool(false);
		LUA->PushString("First parameter not a string");
		return 2;
	}

	LUA->PushBool(Bootil::File::IsFolder(LUA->GetString(1)));
	return 1;
}

int LuaFunc_Exists( lua_State* state )
{
	if ( !LUA->IsType( 1, Type::STRING ) )
	{
		LUA->PushBool(false);
		LUA->PushString("First parameter not a string");
		return 2;
	}

	// Bootil::File::Exists actually returns a boolean
	LUA->PushBool(Bootil::File::Exists(LUA->GetString(1)));
	return 1;
}

int LuaFunc_CreateFolder( lua_State* state )
{
	
	if ( !LUA->IsType( 1, Type::STRING ) )
	{
		LUA->PushBool(false);
		LUA->PushString("First parameter not a string");
		return 2;
	}
	
	if (!Bootil::File::CreateFolder(LUA->GetString(1))) {
		LUA->PushBool(false);
		LUA->PushString(Bootil::Platform::LastError().c_str());
		return 2;
	}

	return 0;
}

int FileLastModified(const Bootil::BString& strFileName) {
	struct stat fileStat;
	int err = stat( strFileName.c_str(), &fileStat );
	if ( err != 0 ) { return 0; }
	return fileStat.st_mtime;
}


int LuaFunc_Time( lua_State* state )
{
	if ( !LUA->IsType( 1, Type::STRING ) )
	{
		LUA->PushBool(false);
		LUA->PushString("First parameter not a string");
		return 2;
	}

	LUA->PushNumber(FileLastModified(LUA->GetString(1)));
	return 1;
}

int LuaFunc_Size( lua_State* state )
{
	if ( !LUA->IsType( 1, Type::STRING ) )
	{
		LUA->PushBool(false);
		LUA->PushString("First parameter not a string");
		return 2;
	}

	LUA->PushNumber(Bootil::File::Size(LUA->GetString(1)));
	return 1;
}

int LuaFunc_CRC( lua_State* state )
{
	if ( !LUA->IsType( 1, Type::STRING ) )
	{
		LUA->PushBool(false);
		LUA->PushString("First parameter not a string");
		return 2;
	}

	LUA->PushNumber(Bootil::File::CRC(LUA->GetString(1)));
	return 1;
}

#define LUA_TABLE_SET_CFUNC(name, func) \
	LUA->PushString( name ); \
	LUA->PushCFunction( func ); \
	LUA->SetTable( -3 ); \

GMOD_MODULE_OPEN()
{

	LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );

	LUA->PushString( "gaceio" );
	LUA->CreateTable();

	// File IO

	LUA_TABLE_SET_CFUNC("List", LuaFunc_ListDir);
	LUA_TABLE_SET_CFUNC("Read", LuaFunc_ReadFile);
	LUA_TABLE_SET_CFUNC("Write", LuaFunc_WriteToFile);
	LUA_TABLE_SET_CFUNC("Delete", LuaFunc_Delete);
	LUA_TABLE_SET_CFUNC("IsDir", LuaFunc_IsFolder);
	LUA_TABLE_SET_CFUNC("Exists", LuaFunc_Exists);
	LUA_TABLE_SET_CFUNC("CreateDir", LuaFunc_CreateFolder);
	LUA_TABLE_SET_CFUNC("Time", LuaFunc_Time);
	LUA_TABLE_SET_CFUNC("Size", LuaFunc_Size);
	LUA_TABLE_SET_CFUNC("CRC", LuaFunc_CRC);

	LUA->SetTable( -3 );

	return 0;
}

//
// Called when your module is closed
//
GMOD_MODULE_CLOSE()
{
	return 0;
}
