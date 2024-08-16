#include "_findfirst.h" // For linux
#include "FileOperations.h"
#include <stdio.h>
#include <string.h>
#ifdef _WIN32 
// For mkdir
#include <direct.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#include "LinuxStrings.h"
#include "_findfirst.h"
#endif


bool WriteFileWithDirectories( const char *path, char *data, unsigned dataLength )
{
	int index;
	FILE *fp;
	char *pathCopy;
#ifndef _WIN32

	char *systemCommand;
#endif

	if ( path == 0 || path[ 0 ] == 0 )
		return false;

#ifndef _WIN32

	systemCommand = new char [ strlen( path ) + 1 + 6 ];

#endif

	pathCopy = new char [ strlen( path ) + 1 ];

	strcpy( pathCopy, path );

	index = 0;

	while ( pathCopy[ index ] )
	{
		if ( pathCopy[ index ] == '/' || pathCopy[ index ] == '\\')
		{
			pathCopy[ index ] = 0;
#ifdef _WIN32
			_mkdir( pathCopy );
#else

			mkdir( pathCopy, 0744 );
#endif

			pathCopy[ index ] = '/';
		}

		index++;
	}

	if (data && dataLength)
	{
		fp = fopen( path, "wb" );

		if ( fp == 0 )
		{
			delete [] pathCopy;
#ifndef _WIN32
			delete [] systemCommand;
#endif
			return false;
		}

		fwrite( data, 1, dataLength, fp );

		fclose( fp );
	}
	else
	{
#ifdef _WIN32
		_mkdir( pathCopy );
#else
		mkdir( pathCopy, 0744 );
#endif
	}

	delete [] pathCopy;
#ifndef _WIN32
	delete [] systemCommand;
#endif


	return true;
}
bool IsSlash(unsigned char c)
{
	return c=='/' || c=='\\';
}

void AddSlash( char *input )
{
	if (input==0 || input[0]==0)
		return;

	int lastCharIndex=(int) strlen(input)-1;
	if (input[lastCharIndex]=='\\')
		input[lastCharIndex]='/';
	else if (input[lastCharIndex]!='/')
	{
		input[lastCharIndex+1]='/';
		input[lastCharIndex+2]=0;
	}
}
bool DirectoryExists(const char *directory)
{
	_finddata_t fileInfo;
	intptr_t dir;
	char baseDirWithStars[560];
	strcpy(baseDirWithStars, directory);
	AddSlash(baseDirWithStars);
	strcat(baseDirWithStars, "*.*");
	dir=_findfirst(baseDirWithStars, &fileInfo );
	if (dir==-1)
		return false;
	_findclose(dir);
	return true;
}
void QuoteIfSpaces(char *str)
{
	unsigned i;
	bool hasSpace=false;
	for (i=0; str[i]; i++)
	{
		if (str[i]==' ')
		{
			hasSpace=true;
			break;
		}
	}
	if (hasSpace)
	{
		int len=(int)strlen(str);
		memmove(str+1, str, len);
		str[0]='\"';
		str[len]='\"';
		str[len+1]=0;
	}
}
