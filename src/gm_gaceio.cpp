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
	Bootil::String::List list;
	int files = Bootil::File::GetFilesInFolder(folder, list, false);

	LUA->CreateTable();
	int i = 1;
	for (auto f = list.begin(); f != list.end(); ++f) {
		LUA->PushNumber(i);
		LUA->PushString((*f).c_str());
		LUA->SetTable(-3);

		i++;
	}
	
	return 1;
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

int LuaFunc_DeleteFile( lua_State* state )
{
	if ( !LUA->IsType( 1, Type::STRING ) )
	{
		LUA->PushBool(false);
		LUA->PushString("First parameter not a string");
		return 2;
	}

	if (!Bootil::File::RemoveFile(LUA->GetString(1))) {
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

	LUA_TABLE_SET_CFUNC( "List", LuaFunc_ListDir );

	LUA->PushString( "Read" );
	LUA->PushCFunction( LuaFunc_ReadFile );
	LUA->SetTable( -3 );

	LUA->PushString( "Write" );
	LUA->PushCFunction( LuaFunc_WriteToFile );
	LUA->SetTable( -3 );

	LUA->PushString( "Delete" );
	LUA->PushCFunction( LuaFunc_DeleteFile );
	LUA->SetTable( -3 );

	LUA->PushString( "IsFolder" );
	LUA->PushCFunction( LuaFunc_IsFolder );
	LUA->SetTable( -3 );

	LUA->PushString( "Exists" );
	LUA->PushCFunction( LuaFunc_Exists );
	LUA->SetTable( -3 );

	LUA->PushString( "CreateFolder" );
	LUA->PushCFunction( LuaFunc_CreateFolder );
	LUA->SetTable( -3 );

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
