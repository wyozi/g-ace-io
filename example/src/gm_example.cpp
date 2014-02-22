#include "GarrysMod/Lua/Interface.h"

#ifdef _WIN32
#include <direct.h>
#include <dirent_win.h>
#endif

#ifdef __linux__
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <string.h>
#include <errno.h> 
#include <dirent.h>
#endif

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream> // for ostringstream

using namespace GarrysMod::Lua;

int LuaFunc_ListDir( lua_State* state )
{
	if ( LUA->IsType( 1, Type::STRING ) )
	{
		const char* path = LUA->GetString( 1 );

		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(path)) != NULL) {

			// Files
			LUA->CreateTable();

			// Folders
			LUA->CreateTable();

			// The indices in tables, used for a simple table.insert
			int file_idx = 1;
			int dir_idx = 1;

			while ((ent = readdir (dir)) != NULL) {
				// Ignore symlinks etc

				if (ent->d_type == DT_REG) {
					LUA->PushNumber(file_idx++);
					LUA->PushString(ent->d_name);
					LUA->SetTable(-4);
				}
				else if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
					LUA->PushNumber(dir_idx++);
					LUA->PushString(ent->d_name);
					LUA->SetTable(-3);
				}
			}
			
			closedir (dir);

			return 2;
		} else {
			LUA->PushBool(false);
			LUA->PushString("Failed to list folder");
			return 2;
		}
	}

	return 0;
}

int LuaFunc_ReadFile( lua_State* state )
{
	if ( LUA->IsType( 1, Type::STRING ) )
	{
		std::ifstream t;
		t.open(LUA->GetString(1));

		if (t.fail()) {
			char* err = strerror(errno);
			LUA->PushBool(false);
			LUA->PushString(err);
			return 2;
		}

		// Get length of the file
		int length;
		t.seekg(0, std::ios::end);
		length = t.tellg();
		t.seekg(0, std::ios::beg);

		// Allocate buffer of length
		char* buffer = new char[length];
		t.read(buffer, length);
		t.close();

		LUA->PushString(buffer);

		return 1;
	}
	LUA->PushBool(false);
	LUA->PushString("No filename given");
	return 2;
}

int LuaFunc_DeleteFile( lua_State* state )
{
	if ( LUA->IsType( 1, Type::STRING ) )
	{
		int ret = remove(LUA->GetString(1));

		LUA->PushBool(true);
		return 1;
	}
	return 0;
}

int LuaFunc_WriteToFile( lua_State* state )
{
	if ( LUA->IsType( 1, Type::STRING ) && LUA->IsType( 2, Type::STRING ) )
	{
		std::ofstream t;
		t.open(LUA->GetString(1));

		if (t.fail()) {
			char* err = strerror(errno);
			LUA->PushBool(false);
			LUA->PushString(err);
			return 2;
		}

		t << LUA->GetString(2);
		t.close();

		LUA->PushBool(true);

		return 1;
	}
	LUA->PushBool(false);
	LUA->PushString("No filename/content given");
	return 2;
}

int LuaFunc_IsFolder( lua_State* state )
{
	if ( LUA->IsType( 1, Type::STRING ) )
	{
		struct stat buf;

		int ret = stat(LUA->GetString(1), &buf);

		LUA->PushBool(S_ISDIR(buf.st_mode));
		return 1;
	}
	return 0;
}

int LuaFunc_Exists( lua_State* state )
{
	if ( LUA->IsType( 1, Type::STRING ) )
	{
		struct stat buf;

		int ret = stat(LUA->GetString(1), &buf);

		LUA->PushBool(S_ISDIR(buf.st_mode) || S_ISREG(buf.st_mode));
		return 1;
	}

	return 0;
}

int LuaFunc_CreateFolder( lua_State* state )
{
	if ( LUA->IsType( 1, Type::STRING ) )
	{

#ifdef __linux__
		int ret = mkdir(LUA->GetString(1), 0700);
#endif // __linux__
#ifdef _WIN32
		int ret = mkdir(LUA->GetString(1));
#endif // _WIN32

		if (ret != 0) {
			LUA->PushBool(false);

			std::ostringstream out;  
			out << "Error code: " << ret;
			LUA->PushString(out.str().c_str());
			return 2;
		}

		LUA->PushBool(true);
		return 1;
	}
	
	LUA->PushBool(false);
	LUA->PushString("No name given");
	return 2;
}

GMOD_MODULE_OPEN()
{

	LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );
	
	LUA->PushString( "gaceio" );
	LUA->CreateTable();

	LUA->PushString( "List" );
	LUA->PushCFunction( LuaFunc_ListDir );
	LUA->SetTable( -3 );

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