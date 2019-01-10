
#ifndef _JC_PROTOCOL_H_
#define _JC_PROTOCOL_H_
#include "jcbase.h"

#ifdef __cplusplus
extern "C" {
#endif

	DLL_EXPORT unsigned long dataunpack(unsigned char *in, unsigned long inlen,unsigned char *out);
	DLL_EXPORT unsigned long datapack(unsigned char *in, unsigned long len,unsigned char *out);

#ifdef __cplusplus
}
#endif

#endif

