/**
 * @file   MPQHash/MPQHash.h
 *
 * @date   Oct 11, 2011
 * @author WangLiang
 * @email  liang.wang@elektrobit.com
 *
 * @brief  Hash method written by Justin Olbrantz, copyright blizzard.
 */

#ifndef MPQHASH_H_
#define MPQHASH_H_

struct HashItem
{
	int bExists;
	unsigned int HashKey;
	unsigned int nHashA;
	unsigned int nHashB;
	void *pAddr;
};

/**
 * @brief Create crypt table, call it before initialize hash table.
 */
void PrepareCryptTable();

/**
 * @brief Hash a string.
 */
unsigned int HashString(const char *lpszFileName, unsigned int dwHashType);

/**
 * @brief Initialize hash table.
 * @param size Size of hash table.
 * return Created hash table.
 */
struct HashItem* InitHashTable(int size);

/**
 * @brief Insert a string into hash table.
 */
int InsertHash(const char *lpszString, struct HashItem *lpTable, unsigned int nTableSize);

/**
 * @brief Search a string in hash table.
 */
int GetHashTablePos(const char *lpszString,struct HashItem* lpTable, unsigned int nTableSize);

#endif /* MPQHASH_H_ */
