#ifndef __STDINC_HPP__
#define __STDINC_HPP__

// OS X doesn't include the endian conversion functions we need, so
// I've resorted to this.  https://gist.github.com/panzi/6856583
#if defined(__linux__)
#include <endian.h>
#elif defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)
#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)
#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)

#define __BYTE_ORDER    BYTE_ORDER
#define __BIG_ENDIAN    BIG_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN
#define __PDP_ENDIAN    PDP_ENDIAN
#endif

#define RED_STA "\33[0;31m"
#define RED_END "\33[0m"
#define YEL_STA "\33[0;33m"
#define YEL_END "\33[0m"
#define STDMSG "====C++ MSPY==>"
#define NOTICE(msg_) (cout << YEL_STA << STDMSG << msg_ <<YEL_END << endl)
#define debugPrintFunction(msg_, fname_, lno_) (cout << RED_STA << msg_ << ":" << fname_ << ":" << lno_ << RED_END << endl)


#endif //__STDINC_HPP__
