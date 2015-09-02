#ifndef _LISTS_H_
#define _LISTS_H_


typedef struct _CompoundType {

	char * pcData;	

	struct _CompoundType * pNext; 

} CompoundType, *pCompoundType;

typedef struct _UrlChainType {

	/* Either <pcData> contains _integral ULR-injection string ... */
	char * pcData;

	/* either _composite ULR-injection is stored in few <pcCompData> fields. */
	struct _CompoundType * pCompound;
	
	unsigned long uloDataAddr; /* Opt'l */

	struct _UrlChainType * pNextChain; 

} UrlChainType, *pUrlChainType;

#define CreateUrl(x) _CreateUrl(__func__, (x))

#define AppendUrl(x, y) _AppendUrl(__func__, (x), (y))

#define DeleteCompound(x) _DeleteCompound(__func__, (x))

#define DeleteUrl(x) _DeleteUrl(__func__, (x))

#define DisplayString(x) _DisplayString(__func__, (x))

#define DisplayCompound(x) _DisplayCompound(__func__, (x))

#define DisplayEntireUrl(x) _DisplayEntireUrl(__func__, (x))


#endif /* _LISTS_H_ */
