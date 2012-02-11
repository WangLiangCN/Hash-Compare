/**
 * @file   MPQHash/MPQHash.c
 *
 * @date   Oct 11, 2011
 * @author WangLiang
 * @email  liang.wang@elektrobit.com
 *
 * @brief  Hash method written by Justin Olbrantz, copyright blizzard.
 */

#include <stdlib.h>
#include <ctype.h>
#include "MPQHash.h"

unsigned int cryptTable[0x500];

void PrepareCryptTable()
{
	unsigned int seed = 0x00100001, index1 = 0, index2 = 0, i;
	for( index1 = 0; index1 < 0x100; index1++ )
	{
		for( index2 = index1, i = 0; i < 5; i++, index2 += 0x100 )
		{
			unsigned int temp1, temp2;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp1 = (seed & 0xFFFF) << 0x10;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp2 = (seed & 0xFFFF);
			cryptTable[index2] = ( temp1 | temp2 );
		}
	}
}

unsigned int HashString(const char *lpszFileName, unsigned int dwHashType )
{
	unsigned char *key   = (unsigned char *)lpszFileName;
	unsigned int seed1 = 0x7FED7FED;
	unsigned int seed2 = 0xEEEEEEEE;
	int ch;

	while( *key != 0 )
	{
		ch = toupper(*key++);

		seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}
	return seed1;
}

struct HashItem* InitHashTable(int size)
{
	int i;
	struct HashItem* newhashtable=(struct HashItem*)malloc(sizeof(struct HashItem)*size);
	for (i=0;i<size ;i++ )
	{
		newhashtable[i].bExists=0;
	}
	return newhashtable;
}

int InsertHash(const char *lpszString, struct HashItem *lpTable, unsigned int nTableSize)
{
	int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
	unsigned int nHash = HashString(lpszString, HASH_OFFSET);
	unsigned int nHashA = HashString(lpszString, HASH_A);
	unsigned int nHashB = HashString(lpszString, HASH_B);
	unsigned int nHashStart = nHash % nTableSize;
	unsigned int nHashPos = nHashStart;
	while (lpTable[nHashPos].bExists)
	{
		nHashPos = (nHashPos + 1) % nTableSize;
		if (nHashPos == nHashStart)
			break;
	}
	lpTable[nHashPos].bExists=1;
	lpTable[nHashPos].nHashA=nHashA;
	lpTable[nHashPos].nHashB=nHashB;
	lpTable[nHashPos].pAddr= (char *)lpszString;
	return nHashPos;
}

int GetHashTablePos(const char *lpszString,struct HashItem* lpTable, unsigned int nTableSize)
{
	int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
	unsigned int nHash = HashString(lpszString, HASH_OFFSET);
	unsigned int nHashA = HashString(lpszString, HASH_A);
	unsigned int nHashB = HashString(lpszString, HASH_B);
	unsigned int nHashStart = nHash % nTableSize, nHashPos = nHashStart;

	while (lpTable[nHashPos].bExists)
	{
		if (lpTable[nHashPos].nHashA == nHashA && lpTable[nHashPos].nHashB == nHashB)
			return nHashPos;
		else
			nHashPos = (nHashPos + 1) % nTableSize;

		if (nHashPos == nHashStart)
			break;
	}
	return -1;
}
