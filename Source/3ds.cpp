// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.

#include "stdafx.h"
#include "3DS.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------

#define	SWAP(a,b,c) {c temp = a; a = b; b = temp;}
#define	FOREVER for(;;)

// ----------------------------------------------------------------------------

	C3DS::C3DS()
{
	memset( currentNamedObject, 0, sizeof(currentNamedObject) );

	meshCount = 0;
	meshList = NULL;
}

// ----------------------------------------------------------------------------

	C3DS::~C3DS()
{
	for( int i = 0; i < meshCount; i++ )
	{
		free(meshList[i].vList);
		free(meshList[i].fList);
	}

	free(meshList);
}

// ----------------------------------------------------------------------------

int	C3DS::load(const wchar_t *inName, sD3DS *info)
{
	meshList   = info->mesh;
	meshCount  = info->meshCount;

	FILE	*inFile = _wfopen(inName, _T("rb"));

	if (!inFile)
	{
		return 0;
	}

	if (!read3DS(inFile))
	{
		fclose(inFile);
		return 0;
	}

	// Copy back

	info->mesh  = meshList;

	info->meshCount  = meshCount;

	// Now we need to actually swap the y/z coordinates of our vertices

	for( int i = 0; i < meshCount; i++ )
	{
		for( int j = 0; j < meshList[i].vCount; j++ )
		{
			SWAP(meshList[i].vList[j].y, meshList[i].vList[j].z, float);
		}

		for( j = 0; j < meshList[i].fCount; j++ )
		{
			SWAP(meshList[i].fList[j].normal.y, meshList[i].fList[j].normal.z, float);
		}
	}

	fclose(inFile);
	return 1;
}

// ----------------------------------------------------------------------------

int	C3DS::read3DS(FILE *inFile)
{
	// Get the first chunk... verify the integrity of the file...

	sCHK	chunk;

	if (!getChunkID(inFile, &chunk))
	{
		return 0;
	}

	if (chunk.ID != CHUNK_3DSFILE && chunk.ID != CHUNK_MLIFILE && chunk.ID != CHUNK_PRJFILE)
	{
		return 0;
	}

	return process3DS(inFile, chunk.length);
}

// ----------------------------------------------------------------------------

int	C3DS::getChunkID(FILE *file, sCHK *chunk)
{
	return myRead( chunk, sizeof(sCHK), file );
}

// ----------------------------------------------------------------------------

int	C3DS::process3DS(FILE *inFile, int chunkLength)
{
	int	endPos = chunkLength - CHUNK_HEADER_SIZE + ftell(inFile);

	for(;;)
	{
		// Done?

		if (endPos <= ftell(inFile)) break;

		sCHK	chunk;
		int	retCode = getChunkID(inFile, &chunk);

		// Are we done?

		if (!retCode) break;

		switch(chunk.ID)
		{
			case CHUNK_MESH:
				if (!readMeshChunk(inFile, chunk.length))
				{
					return 0;
				}
				break;

			default:
				skipChunk(inFile, chunk.length);
				break;
		}
	}

	return 1;
}

// ----------------------------------------------------------------------------

int	C3DS::readMeshChunk(FILE *inFile, int chunkLength)
{
	int	endPos = chunkLength - CHUNK_HEADER_SIZE + ftell(inFile);

	for(;;)
	{
		// Done?

		if (endPos <= ftell(inFile)) break;

		sCHK	chunk;
		int	retCode = getChunkID(inFile, &chunk);

		// Make sure it's a sub-chunk...

		if (chunk.ID >> 12 >= 0xB || chunk.ID == CHUNK_MESH)
		{
			backup(inFile);
			return 1;
		}

		// Are we done?

		if (!retCode) break;

		switch(chunk.ID)
		{
			case CHUNK_NAMEDOBJECT:
				if (!readNamedObject(inFile, chunk.length))
				{
					return 0;
				}
				break;

			default:
				skipChunk(inFile, chunk.length);
				break;
		}
	}

	return 1;
}

// ----------------------------------------------------------------------------

int	C3DS::readNamedObject(FILE *inFile, int chunkLength)
{
	int	endPos = chunkLength - CHUNK_HEADER_SIZE + ftell(inFile);

	if (!getString( inFile, currentNamedObject ))
		return 0;

	FOREVER
	{
		// Done?

		if (endPos <= ftell(inFile)) break;

		sCHK	chunk;
		int	retCode = getChunkID(inFile, &chunk);

		// Make sure it's a sub-chunk...

		if (chunk.ID >> 12 != 4 || chunk.ID == CHUNK_NAMEDOBJECT )
		{
			backup(inFile);
			break;
		}

		// Are we done?

		if (!retCode) break;

		switch(chunk.ID)
		{
			case CHUNK_TRIMESH:
				if (!readTriMesh(inFile, chunk.length))
				{
					return 0;
				}
				break;

			default:
				skipChunk(inFile, chunk.length);
				break;
		}
	}

	return 1;
}

// ----------------------------------------------------------------------------

int	C3DS::readTriMesh(FILE *inFile, int chunkLength)
{
	meshList = (sMSH *) realloc( meshList, (meshCount+1) * sizeof(sMSH) );

	if (!meshList)
	{
		return 0;
	}

	memset( &meshList[meshCount], 0, sizeof(sMSH) );

	strcpy(meshList[meshCount].name, currentNamedObject );

	int	endPos = chunkLength - CHUNK_HEADER_SIZE + ftell(inFile);

	FOREVER
	{
		// Done?

		if (endPos <= ftell(inFile)) break;

		sCHK	chunk;
		int	retCode = getChunkID(inFile, &chunk);

		// Make sure it's a sub-chunk...

		if (chunk.ID >> 8 != 0x41 || chunk.ID == CHUNK_TRIMESH )
		{
			backup(inFile);
			break;
		}

		// Are we done?

		if (!retCode) break;

		switch(chunk.ID)
		{
			case CHUNK_VERTEXLIST:
				if (!readVertexList(inFile, chunk.length))
				{
					return 0;
				}
				break;

			case CHUNK_FACELIST:
				if (!readFaceList(inFile, chunk.length))
				{
					return 0;
				}
				break;

			default:
				skipChunk(inFile, chunk.length);
				break;
		}
	}

	meshCount++;
	return 1;
}

// ----------------------------------------------------------------------------

int	C3DS::readVertexList(FILE *inFile, int chunkLength)
{
	chunkLength = chunkLength;

	if (!myRead(&meshList[meshCount].vCount, sizeof(meshList[meshCount].vCount), inFile ))
	{
		return 0;
	}

	meshList[meshCount].vList = (sP3D *) malloc(sizeof(sP3D) * meshList[meshCount].vCount);

	if (!meshList[meshCount].vList)
	{
		return 0;
	}

	if (!myRead( meshList[meshCount].vList, sizeof(sP3D) * meshList[meshCount].vCount, inFile ))
	{
		return 0;
	}

	return 1;
}

// ----------------------------------------------------------------------------

int	C3DS::readFaceList(FILE *inFile, int chunkLength)
{
	chunkLength = chunkLength;

	if (!myRead(&meshList[meshCount].fCount, sizeof(meshList[meshCount].fCount), inFile ))
	{
		return 0;
	}

	meshList[meshCount].fList = (sFACE *) malloc(sizeof(sFACE) * meshList[meshCount].fCount);

	if (!meshList[meshCount].fList)
	{
		return 0;
	}

	memset( meshList[meshCount].fList, 0, sizeof(sFACE) * meshList[meshCount].fCount );

	for( int i = 0; i < meshList[meshCount].fCount; i++)
	{
		meshList[meshCount].fList[i].materialIndex = 0;

		if (!myRead( &meshList[meshCount].fList[i].a, sizeof(unsigned short), inFile ))
		{
			return 0;
		}

		if (!myRead( &meshList[meshCount].fList[i].b, sizeof(unsigned short), inFile ))
		{
			return 0;
		}

		if (!myRead( &meshList[meshCount].fList[i].c, sizeof(unsigned short), inFile ))
		{
			return 0;
		}

		if (!myRead( &meshList[meshCount].fList[i].flags, sizeof(unsigned short), inFile ))
		{
			return 0;
		}

//		orientFace( &meshList[meshCount].fList[i], &meshList[meshCount] );
	}

	return 1;
}

// ----------------------------------------------------------------------------

void	C3DS::skipChunk(FILE *File, unsigned long length)
{
	fseek( File, length + ftell(File) - CHUNK_HEADER_SIZE, SEEK_SET );
}

// ----------------------------------------------------------------------------

void	C3DS::backup(FILE *File)
{
	fseek( File, ftell(File) - CHUNK_HEADER_SIZE, SEEK_SET );
}

// ----------------------------------------------------------------------------

int	C3DS::myRead( void *buffer, int length, FILE *file)
{
	int	Count = fread(buffer, length, 1, file);

	if (Count == 0 && feof(file))
		return 0;

	if (Count != 1)
	{
		return 0;
	}

	return 1;
}

// ----------------------------------------------------------------------------

int	C3DS::getString( FILE *inFile, char *string )
{
	for( int i = 0; i < DEF_STR_LEN; i++)
	{
		int	chr = fgetc(inFile);

		if (chr == ' ')
			chr = '_';

		string[i] = (char) chr;

		if (!chr)
			return 1;
	}

	return 0;
}
