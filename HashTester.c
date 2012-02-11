/**
 *  @file   HashTester.c
 *
 *  @date   Oct 11, 2011
 *  @author WangLiang
 *  @email  liang.wang@elektrobit.com
 *
 *  @brief  Test hash function.
 */

#define TEST_MPQHASH
//#define TEST_LIST_HASH

#include "CProjectDfn.h"
#ifdef TEST_LIST_HASH
#include "NormalHash/Hash.h"
#else
#include "MPQHash/Hash.h"
#endif
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>

typedef struct TestStruct
{
	char *str;
	int private;
	struct TestStruct *next;
}TestStruct;

#define ITEM_NUM 999999
#define STR_LEN 63
#define FIND_THIS_NODE_IN_LIST 987654
#ifdef TEST_LIST_HASH
#define DEFAULT_HASH_METHOD BKDRHash
#endif

char *GetNextStr(void **pNode)
{
	TestStruct **node = (TestStruct **)pNode;
	(*node) = (*node)->next;
	return (NULL != *node) ? (*node)->str : NULL;
}

char *rand_str(int strLen);

int TestHashList()
{
	struct timeval startTime, endTime;
	unsigned long long costTime = 0ULL;

	TestStruct head;
	TestStruct *currNode = &head;
	TestStruct *nextNode = NULL;

	char findStr[STR_LEN+1];
	char *findResult;

	head.str = (char *)malloc(sizeof(char)*(STR_LEN+1));
	strcpy(head.str, "I AM Head!");

	for (int i=0; i<ITEM_NUM-1; ++i)
	{
		TestStruct *tmpStruct = (TestStruct *)malloc(sizeof(TestStruct));
		tmpStruct->next = NULL;
		char *randStr = rand_str(STR_LEN);
		tmpStruct->str = randStr;
		currNode->next = tmpStruct;
		currNode = currNode->next;
		if (FIND_THIS_NODE_IN_LIST == i+1)
		{
			printf("The %d node string:%s\n", FIND_THIS_NODE_IN_LIST, randStr);
			strcpy(findStr, randStr);
		}
	}

	// Normal search
	gettimeofday(&startTime,NULL);
	char *str = NULL;
	void *pHead = &head;
	while(NULL != (str = GetNextStr(&pHead)))
	{
		if (0 == strcmp(str, findStr))
			break;
	}
	gettimeofday(&endTime,NULL);
	costTime = 1000 * 1000 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	printf("Normal list searched str:%s, used %llu us.\n", (NULL != str) ? str : "string NOT found!", costTime);

	// Hash search
	gettimeofday(&startTime,NULL);
#ifdef TEST_LIST_HASH
	HashInf *pHashInf = HashFromList(ITEM_NUM, &head, GetNextStr, DEFAULT_HASH_METHOD);
#else
	HashInf *pHashInf = HashFromList(ITEM_NUM, GetNextStr, &head);
#endif
	gettimeofday(&endTime,NULL);
	costTime = 1000 * 1000 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	printf("Create hash table cost %llu us.\n", costTime);

	gettimeofday(&startTime,NULL);
	findResult = (char *)GetStringAddress(pHashInf, findStr);
	gettimeofday(&endTime,NULL);
	costTime = 1000 * 1000 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	printf("Hash searched str:%s, cost %llu us.\n", NULL != findResult ? findResult : "string NOT found!", costTime);
	DeleteHashInf(&pHashInf);

	currNode = head.next;
	while(NULL != currNode)
	{
		nextNode = currNode->next;
		FREE(currNode->str);
		FREE(currNode);
		currNode = nextNode;
	}
	FREE(head.str);

	return 0;
}

int TestHashArray()
{
	struct timeval startTime, endTime;
	unsigned long long costTime = 0ULL;

	char findStr[STR_LEN+1];
	char **array = (char **)malloc(sizeof(char *)*(ITEM_NUM));
	for (int i=0; i<ITEM_NUM; ++i)
	{
		char *randStr = rand_str(STR_LEN);
		array[i] = randStr;
		if (FIND_THIS_NODE_IN_LIST == i+1)
		{
			printf("The %d node string:%s\n", FIND_THIS_NODE_IN_LIST, randStr);
			strcpy(findStr, randStr);
		}
	}

	// Normal search
	gettimeofday(&startTime,NULL);
	int i=0;
	for (i=0; i<ITEM_NUM; ++i)
	{
		if (0 == strcmp(findStr, array[i]))
			break;
	}
	gettimeofday(&endTime,NULL);
	costTime = 1000 * 1000 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	printf("Normal array searched str:%s, used %llu us.\n", (ITEM_NUM != i) ? array[i] : "string NOT found!", costTime);

	// Hash table search
	gettimeofday(&startTime,NULL);
#ifdef TEST_LIST_HASH
	HashInf *pHashInf = HashFromArray(ITEM_NUM, array, DEFAULT_HASH_METHOD);
#else
	HashInf *pHashInf = HashFromArray(ITEM_NUM, array);
#endif
	gettimeofday(&endTime,NULL);
	costTime = 1000 * 1000 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	printf("Create hash table cost %llu us.\n", costTime);

	gettimeofday(&startTime,NULL);
	char *findResult = (char *)GetStringAddress(pHashInf, findStr);
	gettimeofday(&endTime,NULL);
	costTime = 1000 * 1000 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	printf("Hash searched str:%s, used %llu us.\n", NULL != findResult ? findResult : "string NOT found!", costTime);

	DeleteHashInf(&pHashInf);
	for (int i=0; i<ITEM_NUM; ++i)
	{
		FREE(array[i]);
	}
	FREE(array);

	return 0;
}

int main()
{
	//TestHashList();
	TestHashArray();

	return 0;
}

char *rand_str(int strLen)
{
  char *str = (char *)malloc(strLen + 1);

  srand(time(NULL) + rand());    //初始化随机数的种子
  int i;
  for (i = 0; i  < strLen; i++)
  {
    str[i] = rand() %94 + 32;      //控制得到的随机数为可显示字符
  }
  str[i] = '\0';

  return str;
}
