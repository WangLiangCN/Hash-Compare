/**
 * @file   NormalHash/Hash.c
 *
 * @date   Oct 12, 2011
 * @author WangLiang
 * @email  liang.wang@elektrobit.com
 *
 * @brief  Create a hash table from string array or list, search string by hash table.
 */

/**
 * Data structure:
 *
 * +--------+         Key0   Key1   Key2   Key3   Key4   Key5    ...   Key size
 * |        |       +------+------+------+------+------+------+------+----------+
 * |  Hash  |  -->  | head | head | head | head | head | head |  ... |   head   |
 * |  table |       +------+------+------+------+------+------+------+----------+
 * |        |           |      |      |      |      |      |      |        |
 * +--------+        +----+ +----+  NULL  +----+ +----+  NULL    ...    +----+
 * |        |        |List| |List|        |List| |List|                 |List|
 * |  size  |        |Node| |Node|        |Node| |Node|                 |Node|
 * | method |        +----+ +----+        +----+ +----+                 +----+
 * |        |           |      |             |      |                      |
 * +--------+        +----+  NULL          NULL  +----+                  NULL
 *                   |List|                      |List|
 *                   |Node|                      |Node|
 *                   +----+                      +----+
 *                      |                           |
 *                    NULL                       +----+
 *                                               |List|
 *                                               |Node|
 *                                               +----+
 *                                                  |
 *                                                NULL
 */

#include "Hash.h"

/**
 * @brief Create hash table.
 * @param size Size of hash table.
 * @return created hash table, nothing in table.
 */
static HashTable *InitHashTable(int size)
{
	HashTable *hashTable = (HashTable *)malloc(sizeof(HashItem *) * size);
	for (int i=0; i<size ;i++ )
	{
		hashTable[i] = NULL;
	}
	return hashTable;
}

/**
 * @brief Insert a string to hash table.
 *
 * @param hashTable Which hash table to insert.
 * @param nTableSize Size of hash table.
 * @param HashMethod Which hash method is going to use to add hash table.
 * @param str Which string want to insert into hash table.
 */
static void InsertHash(HashTable *hashTable, unsigned int nTableSize,
		               unsigned int (*HashMethod)(const char *), const char *str)
{
	unsigned int nHash = (*HashMethod)(str);
	unsigned int position = nHash % nTableSize;

	HashItem *pHashItem = (HashItem *)malloc(sizeof(HashItem));
	pHashItem->item = (char *)str;
	pHashItem->HashKey = nHash;

	if (NULL == hashTable[position])
	{
		INIT_LIST_HEAD(&(pHashItem->node));
		hashTable[position] = pHashItem;
	}
	else
	{
		list_add(&(pHashItem->node), &(hashTable[position]->node));
	}
}

/**
 * @brief Create hash information from a array.
 *
 * @param itemNum Number of items in array.
 * @param pArray Pointer pointed to array which will create hash information.
 * @param HashMethod Which hash method will be used to create hash table, method listed in HashMethod.h.
 * @see HashMethod.h
 * @return Pointer to created hash information.
 */
HashInf *HashFromArray(int itemNum, char **pArray,
		               unsigned int (*HashMethod)(const char *))
{
	HashInf *hashInf = (HashInf *)malloc(sizeof(HashInf));
	hashInf->pHashTable = InitHashTable(itemNum);
	hashInf->nTableSize = itemNum;
	hashInf->HashMethod = HashMethod;

	// Add each string to hash table.
	for (int i=0; i<itemNum; ++i)
	{
		InsertHash(hashInf->pHashTable, hashInf->nTableSize, hashInf->HashMethod, pArray[i]);
	}
	return hashInf;
}

/**
 * @brief Create hash information from list.
 *
 * @param itemNum Number of items in array.
 * @param list Pointer pointed to list which will create hash information.
 * @param GetNextStr Method of how to get string from list.
 * @param HashMethod Which hash method will be used to create hash table, method listed in HashMethod.h.
 * @see HashMethod.h
 * @return Pointer to created hash information.
 */
HashInf *HashFromList(int itemNum, void *list,
		              char *(GetNextStr)(void **),
		              unsigned int (*HashMethod)(const char *))
{
	char *str = NULL;
	int hashTableIndex;

	HashInf *hashInf = (HashInf *)malloc(sizeof(HashInf));
	hashInf->pHashTable = InitHashTable(itemNum);
	hashInf->nTableSize = itemNum;
	hashInf->HashMethod = HashMethod;

	// Get every string in list and add them to hash table.
	while(NULL != (str = (*GetNextStr)(&list)))
	{
		InsertHash(hashInf->pHashTable, hashInf->nTableSize, hashInf->HashMethod, str);
	}

	return hashInf;
}

/**
 * @brief Delete created hash information.
 *
 * @param pHashInf Pointer to which hash information you want to delete.
 */
void DeleteHashInf(HashInf **pHashInf)
{
	struct list_head *currNode, *nextNode;
	HashItem *pHashItem;

	if (IS_NOT_FREED(*pHashInf))
	{
		HashTable *pHashTable = (*pHashInf)->pHashTable;

		// Free each list in hash table.
		for (int i=0; i< (*pHashInf)->nTableSize; ++i)
		{
			if (IS_NOT_NULL(pHashTable[i]))
			{
				list_for_each_safe(currNode, nextNode, &(pHashTable[i]->node))
				{
					pHashItem = list_entry(currNode, struct HashItem, node);
					list_del(currNode);
					FREE(pHashItem);
				}
				// Free list head in hash table.
				FREE(pHashTable[i]);
			}
		}
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
	unsigned int nHash = (*(pHashInf->HashMethod))(str);
	unsigned int position = nHash % pHashInf->nTableSize;

	// Check if there is a list.
	if (NULL == pHashInf->pHashTable[position])
	{
		return NULL;
	}

	// Check if the head is the string you want to search.
	HashItem *pHashItem = pHashInf->pHashTable[position];
	if ((nHash == pHashItem->HashKey) && (0 == strcmp(str, (char *)pHashItem->item)))
	{
		return pHashItem->item;
	}

	// Read the whole list and find out where is the string.
	struct list_head *pos;
	list_for_each(pos, &(pHashInf->pHashTable[position]->node))
	{
		pHashItem= list_entry(pos, HashItem, node);
		if ((nHash == pHashItem->HashKey) && (0 == strcmp(str, (char *)pHashItem->item)))
		{
			return pHashItem->item;
		}
	}

	return NULL;
}
