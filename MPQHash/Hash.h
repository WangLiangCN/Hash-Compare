/**
 * @file   MPQHash/Hash.h
 *
 * @date   Oct 11, 2011
 * @author WangLiang
 * @email  liang.wang@elektrobit.com
 *
 * @brief  Create a hash table from string array or list, search string by hash table.
 */

#ifndef HASH_H_
#define HASH_H_

#include "../CProjectDfn.h"
#include "MPQHash.h"

/**
 * @brief To prevent conflicts, hash table is bigger than items.
 * Undefined behavior when it smaller than 1.
 */
#define ZOOM_TIMES_PREVENT_CONFLICT 1.5

typedef struct HashItem HashItem;

/**
 * @brief Hash information, including hash table and it's size.
 */
typedef struct HashTableInf
{
	HashItem *pHashTable;
	int nTableSize;
}HashInf;

/**
 * @brief Create hash information from list.
 *
 * @param itemNum Number of items in array.
 * @param list Pointer pointed to list which will create hash information.
 * @param GetNextStr Method of how to get string from list.
 * @return Pointer to created hash information.
 */
HashInf *HashFromList(int itemNum, char *(GetNextStr)(void **), void *list);

/**
 * @brief Create hash information from a array.
 *
 * @param itemNum Number of items in array.
 * @param pArray Pointer pointed to array which will create hash information.
 * @return Pointer to created hash information.
 */
HashInf *HashFromArray(int itemNum, char **pArray);

/**
 * @brief Delete created hash information.
 *
 * @param pHashInf Pointer to which hash information you want to delete.
 */
void DeleteHashInf(HashInf **pHashTable);

/**
 * @brief Get real string address, the string must be in array or list which created hash information.
 *
 * @param pHashInf Pointer to which hash information you want to delete.
 * @param str Which string you want to find.
 * @return Real string address you want to search.
 */
void *GetStringAddress(HashInf *pHashTable, const char *str);

#endif /* HASH_H_ */
