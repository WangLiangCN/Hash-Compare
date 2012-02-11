/**
 * @file   NormalHash/Hash.h
 *
 * @date   Oct 12, 2011
 * @author WangLiang
 * @email  liang.wang@elektrobit.com
 *
 * @brief  Create a hash table from string array or list, search string by hash table.
 */

#ifndef HASH2_H_
#define HASH2_H_

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

#include "../CProjectDfn.h"
#include "HashMethod.h"
#include "list.h"

/**
 * @brief Hash information for each item.
 */
typedef struct HashItem
{
	void *item;              ///< Address of item.
	unsigned int HashKey;    ///< Hash key.
	struct list_head node;   ///< node pointer, next and previous node address.
}HashItem;

//! Hash table made up by many hash items.
typedef HashItem* HashTable;

/**
 * @brief Hash information.
 */
typedef struct HashTableInf
{
	unsigned int (*HashMethod)(const char *);   ///< Which hash method used in hash table.
	int nTableSize;                             ///< Size of hash table.
	HashTable *pHashTable;                      ///< Pointer pointed to hash table.
}HashInf;

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
		               unsigned int (*HashMethod)(const char *));

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
		              unsigned int (*HashMethod)(const char *));

/**
 * @brief Delete created hash information.
 *
 * @param pHashInf Pointer to which hash information you want to delete.
 */
void DeleteHashInf(HashInf **pHashInf);

/**
 * @brief Get real string address, the string must be in array or list which created hash information.
 *
 * @param pHashInf Pointer to which hash information you want to delete.
 * @param str Which string you want to find.
 * @return Real string address you want to search.
 */
void *GetStringAddress(HashInf *pHashInf, const char *str);

#endif /* HASH2_H_ */
