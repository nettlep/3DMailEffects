// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// CSum.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>
#include <string.h>


int main(int argc, char* argv[])
{
	FILE *	fp = NULL;

	try
	{
		// Validate arguments

		if (argc != 2) throw "Either no filename was given, or too many args given";

		char	*fname = argv[1];
		printf("Checksumming file: %s...", fname);

		// Read the file

		fp = fopen(fname, "rb");
		if (!fp) throw "Unable to open file";

		// Get the file length

		fseek(fp, 0L, SEEK_END);
		unsigned int	length = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		// Allocation length is multiples of four (for simpler checksumming)

		unsigned int	allocLength = length;
		if (allocLength & 3) allocLength += 4 - (allocLength & 3);

		// Allocate the RAM

		unsigned int *	buf = new unsigned int[allocLength / 4];
		if (!buf) throw "Unable to allocate RAM for file";
		memset(buf, 0, allocLength);

		// Read it in

		if (fread(buf, 1, length, fp) != length) throw "Unable to read input file";

		// Close the file

		fclose(fp);
		fp = NULL;

		// Checksum the data

		unsigned int	cksum = 0;
		for (unsigned int i = 0; i < allocLength / 4; ++i)
		{
			cksum += buf[i];
		}

		// Open the file for writing

		fp = fopen(fname, "wb");
		if (!fp) throw "Unable to open file for writing, file may have been deleted";

		// Write the data

		if (fwrite(buf, 1, length, fp) != length) throw "Unable to write file, file may be damaged.";

		// Write the magic number & checksum

		int	magic = 0xab9834fe;
		if (fwrite(&magic, 1, 4, fp) != 4) throw "Unable to write magic, file may be damaged.";
		if (fwrite(&cksum, 1, 4, fp) != 4) throw "Unable to write checksum, file may be damaged.";

		// Done

		fclose(fp);
	}
	catch (const char * errString)
	{
		printf("CKSUM ERROR: %s\n", errString);
		if (fp) fclose(fp);
		return 1;
	}

	printf("Done\n");

	return 0;
}
