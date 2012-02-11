/**
 * @file   NormalHash/HashMethod.h
 *
 * @date   Oct 11, 2011
 * @author WangLiang
 * @email  liang.wang@elektrobit.com
 *
 * @brief  Frequently used hash method.
 */

#ifndef SIMPLEHASHMETHOD_H_
#define SIMPLEHASHMETHOD_H_

// SDB Hash Function
unsigned int SDBMHash(const char *str);

// RS Hash Function
unsigned int RSHash(const char *str);

// JS Hash Function
unsigned int JSHash(const char *str);

// P. J. Weinberger Hash Function
unsigned int PJWHash(const char *str);

// ELF Hash Function
unsigned int ELFHash(const char *str);

// BKDR Hash Function
unsigned int BKDRHash(const char *str);

// DJB Hash Function
unsigned int DJBHash(const char *str);

// AP Hash Function
unsigned int APHash(const char *str);

#endif /* SIMPLEHASHMETHOD_H_ */
