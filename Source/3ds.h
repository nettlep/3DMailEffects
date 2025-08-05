// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.

#ifndef	_H_3DS
#define	_H_3DS

#pragma pack(1)

// ----------------------------------------------------------------------------

typedef struct
{
	float	x, y, z;
} sP3D;

// ----------------------------------------------------------------------------

typedef struct
{
	unsigned short	a, b, c, flags;
	unsigned short	materialIndex;
	sP3D		normal;
} sFACE;

// ----------------------------------------------------------------------------

typedef struct
{
	unsigned short	ID;
	unsigned long	length;
} sCHK;

// ----------------------------------------------------------------------------

typedef struct
{
	unsigned short	vCount;
	sP3D		*vList;

	unsigned short	fCount;
	sFACE		*fList;

	char		name[80];
} sMSH;

// ----------------------------------------------------------------------------

typedef struct
{
	unsigned short	meshCount;
	sMSH		*mesh;
} sD3DS;

// ----------------------------------------------------------------------------

#pragma pack()

class	C3DS
{
private:
	enum	{DEF_STR_LEN = 512};

	enum	{CHUNK_HEADER_SIZE = 6};

	enum	{CHUNK_3DSFILE     = 0x4D4D};	// 3DS file descriptor
	enum	{CHUNK_MLIFILE     = 0x3DAA};	// MLI file descriptor
	enum	{CHUNK_PRJFILE     = 0xC23D};	// PRJ file descriptor
	enum	{CHUNK_MESH        = 0x3D3D};	// Mesh chunk
	enum	{CHUNK_NAMEDOBJECT = 0x4000};	// Named Object
	enum	{CHUNK_TRIMESH     = 0x4100};	// Tri-Mesh
	enum	{CHUNK_VERTEXLIST  = 0x4110};	// Vertex Coordinate List Chunk
	enum	{CHUNK_FACELIST    = 0x4120};	// Face List Chunk

	char	currentNamedObject[80];

	// Material, Tri-Mesh, Light, and Camera information
	
	unsigned short	meshCount;
	sMSH		*meshList;

	int		read3DS(FILE *InFile);
	int		process3DS(FILE *InFile, int ChunkLength);
	int		readMeshChunk(FILE *InFile, int ChunkLength);
	int		readNamedObject(FILE *InFile, int ChunkLength);
	int		readTriMesh(FILE *InFile, int ChunkLength);
	int		readVertexList(FILE *InFile, int ChunkLength);
	int		readFaceList(FILE *InFile, int ChunkLength);
	int		getChunkID(FILE *File, sCHK *Chunk);
	void		skipChunk(FILE *File, unsigned long Length);
	void		backup(FILE *File);
	int		myRead(void *Buffer, int Length, FILE *File);
	int		getString(FILE *InFile, char *String);

public:
			C3DS();
			~C3DS();

	int		load(const wchar_t *InName, sD3DS *Info);
};

#endif
