#ifndef _GDS_TYPES_H
#define _GDS_TYPES_H

/*
 * We need byte swapping functions.
 * If these exist for a particular platform, use the (presumably) optimised
 * versions provided. Otherwise, use the network functions htonl() and htons().
 */

#if defined(linux)
#	include <endian.h>
#	include <byteswap.h>
#	define endian_swap_long(A) bswap_32((A))
#	define endian_swap_short(A) bswap_16((A))
#elif defined(WIN32)
#	define __LITTLE_ENDIAN 1234
#	define __BIG_ENDIAN 4321
#	define __BYTE_ORDER __LITTLE_ENDIAN
	long endian_swap_long(long value);
	short endian_swap_short(short value);
#else
	/* Generic includes for Unix-alike systems */
	/* Includes at least Solaris, FreeBSD, HP-UX */
#	include <arpa/nameser_compat.h>
#	include <netinet/in.h>
#	define endian_swap_long(A) htonl((A))
#	define endian_swap_short(A) htons((A))
#endif

#endif

