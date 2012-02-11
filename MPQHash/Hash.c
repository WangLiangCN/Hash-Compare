/**
 * @file   MPQHash/Hash.c
 *
 * @date   Oct 11, 2011
 * @author WangLiang
 * @email  liang.wang@elektrobit.com
 *
 * @brief  Create a hash table from string array or list, search string by hash table.
 */

#include "Hash.h"

/**
 * @brief Create hash information from list.
 *
 * @param itemNum Number of items in array.
 * @param list Pointer pointed to list which will create hash information.
 * @param GetNextStr Method of how to get string from list.
 * @return Pointer to created hash information.
 */
HashInf *HashFromList(int itemNum, char *(GetNextStr)(void **), void *list)
{
	char *str = NULL;
	int hashTableIndex;

	PrepareCryptTable();
	HashInf *pHashInf = (HashInf *)malloc(sizeof(HashInf));
	pHashInf->pHashTable = InitHashTable(itemNum * ZOOM_TIMES_PREVENT_CONFLICT);
	pHashInf->nTableSize = itemNum * ZOOM_TIMES_PREVENT_CONFLICT;

	// Get every string in list and add them to hash table.
	while(NULL != (str = (*GetNextStr)(&list)))
	{
		InsertHash(str, pHashInf->pHashTable, pHashInf->nTableSize);
	}

	return pHashInf;
}

/**
 * @brief Create hash information from a array.
 *
 * @param itemNum Number of items in array.
 * @param pArray Pointer pointed to array which will create hash information.
 * @return Pointer to created hash information.
 */
HashInf *HashFromArray(int itemNum, char **pArray)
{
	PrepareCryptTable();
	HashInf *pHashInf = (HashInf *)malloc(sizeof(HashInf));
	pHashInf->pHashTable = InitHashTable(itemNum * ZOOM_TIMES_PREVENT_CONFLICT);
	pHashInf->nTableSize = itemNum * ZOOM_TIMES_PREVENT_CONFLICT;

	// Add each string to hash table.
	for (int i=0; i<itemNum; ++i)
	{
		InsertHash(pArray[i], pHashInf->pHashTable, pHashInf->nTableSize);
	}
	return pHashInf;
}

/**
 * @brief Delete created hash information.
 *
 * @param pHashInf Pointer to which hash information you want to delete.
 */
void DeleteHashInf(HashInf **pHashInf)
{
	if (IS_NOT_FREED(*pHashInf))
	{
		SECURE_FREE((*pHashInf)->pHashTable);
		FREE(*pHashInf);
	}
}

/**
 * @brief Get real string address, the string must be in array or list which created hash information.
 *
 * @param pHashInf Pointer to which hash information you want to delete.
 * @param str Which string you want to find.
 * @return Real string address you want to search.
 */
void *GetStringAddress(HashInf *pHashInf, const char *str)
{
	int position = GetHashTablePos(str, pHashInf->pHashTable, pHashInf->nTableSize);
	if (-1 != position)
	{
		return ((pHashInf->pHashTable)[position].pAddr);
	}
	else
	{
		return NULL;
	}
}
