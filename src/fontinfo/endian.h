/* x86/x86_64/amd64 are all little-endian */

/* incomplete */
#if \
  defined(_M_IX86)       || \
  defined(_M_X64)        || \
  defined(_M_AMD64)      || \
  defined(__amd64__)     || \
  defined(__amd64)       || \
  defined(__x86_64__)    || \
  defined(__x86_64)      || \
  defined(__i386)        || \
  defined(__i386__)      || \
  defined(__IA32__)      || \
  defined(_M_I86)        || \
  defined(_M_IX86)       || \
  defined(__X86__)       || \
  defined(_X86_)         || \
  defined(__THW_INTEL__) || \
  defined(__I86__)       || \
  defined(__INTEL__)     || \
  defined(__386)
#	define YTR_LE 1
#endif

#if 0 /* FIXME Need to detect big endian compiler guards */
#	define YTR_BE 1
#endif

#if defined(YTR_LE) && defined(YTR_BE)
#	error "Big endian and little endian defined"
#endif-

#if defined(_MSC_VER) /* FIXME Add version guards */
#	define ytr_byteswap_16(x) _byteswap_ushort(x)
#	define ytr_byteswap_32(x) _byteswap_ulong(x)
#	define ytr_byteswap_64(x) _byteswap_uint64(x)
#elif defined(__GNUC__) /* FIXME Add version guards */
#	define ytr_byteswap_16(x) __builtin_bswap16(x)
#	define ytr_byteswap_32(x) __builtin_bswap32(x)
#	define ytr_byteswap_64(x) __builtin_bswap64(x)
#endif

/* FIXME Need to check for header guards that
 * already define the following. In those
 * cases, just do nothing. */

#if YTR_LE
#	define htobe16(x) ytr_byteswap_16(x)
#	define htole16(x) (x)
#	define be16toh(x) ytr_byteswap_16(x)
#	define le16toh(x) (x)
 
#	define htobe32(x) ytr_byteswap_32(x)
#	define htole32(x) (x)
#	define be32toh(x) ytr_byteswap_32(x)
#	define le32toh(x) (x)
 
#	define htobe64(x) ytr_byteswap_64(x)
#	define htole64(x) (x)
#	define be64toh(x) ytr_byteswap_64(x)
#	define le64toh(x) (x)
#elif YTR_BE
#	define htobe16(x) (x)
#	define htole16(x) ytr_byteswap_16(x)
#	define be16toh(x) (x)
#	define le16toh(x) ytr_byteswap_16(x)
 
#	define htobe32(x) (x)
#	define htole32(x) ytr_byteswap_32(x)
#	define be32toh(x) (x)
#	define le32toh(x) ytr_byteswap_32(x)
 
#	define htobe64(x) (x)
#	define htole64(x) ytr_byteswap_64(x)
#	define be64toh(x) (x)
#	define le64toh(x) ytr_byteswap_64(x)
#endif