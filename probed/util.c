/**
 * Contains various utilities.
 * 
 * \author Anders Berggren <anders@halon.se>
 * \author Lukas Garberg <lukas@spritelink.net>
 * \file util.c
 */

#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include "probed.h"

/** 
 * Create string containing IP address.
 *
 * Place human-readable IPv6 or IPv6-mapped-IPv4 address into
 * string 's' with size INET6_ADDRSTRLEN.
 *
 * \param[in] a sockaddr_in6 to fetch address from.
 * \param[out] s Buffer where the string is written.
 * \return Status; 0 on success, <0 on failure.
 */
int addr2str(addr_t *a, /*@out@*/ char *s) {
	if (inet_ntop(AF_INET6, &(a->sin6_addr), s, INET6_ADDRSTRLEN) == NULL) {
		syslog(LOG_ERR, "inet_ntop: %s", strerror(errno));
		return -1;
	}
	return 0;
}

/**
 * Prints a string.
 *
 * Just print string, without respect to return value.
 *
 * \param[in] str String to print.
 */
void p(char *str) {
	(void)puts(str);
}

/**
 * Enable debug logging.
 *
 * Configures the syslog handler to enable/disable logging of debug
 * messages.
 *
 * \param[in] enabled Boolean to determine to enable or disable.
 */
void debug(int enabled) {
	if (enabled == 1) 
		(void)setlogmask(LOG_UPTO(LOG_DEBUG));
	else 
		(void)setlogmask(LOG_UPTO(LOG_INFO));
	return;
}

/*@ -type Disable fucked up 'Arrow access field of non-struct...' */
/**
 * Calculates the difference between two timespec.
 *
 * Calculates the value \p end - \p beg.
 * 
 * \param[out] r Pointer to timespec where result will be written.
 * \param[in] end Time #1.
 * \param[in] beg Time #2.
 */
void diff_ts (/*@out@*/ ts_t *r, ts_t *end, ts_t *beg) {
	memset(r, 0, sizeof *r);
	if ((end->tv_nsec - beg->tv_nsec) < 0) {
		r->tv_sec = end->tv_sec - beg->tv_sec - 1;
		r->tv_nsec = 1000000000 + end->tv_nsec - beg->tv_nsec;
	} else {
		r->tv_sec = end->tv_sec - beg->tv_sec;
		r->tv_nsec = end->tv_nsec - beg->tv_nsec;
	}
}

/**
 * Calculates the difference between two timeval.
 *
 * Calculates the value \p end - \p beg.
 * 
 * \param[out] r Pointer to timeval where result will be written.
 * \param[in] end Time #1.
 * \param[in] beg Time #2.
 */
void diff_tv (struct timeval *r, struct timeval *end, struct timeval *beg) {
	if ((end->tv_usec - beg->tv_usec) < 0) {
		r->tv_sec = end->tv_sec - beg->tv_sec - 1;
		r->tv_usec = 1000000 + end->tv_usec - beg->tv_usec;
	} else {
		r->tv_sec = end->tv_sec - beg->tv_sec;
		r->tv_usec = end->tv_usec - beg->tv_usec;
	}
}

/** 
 * Compare two timespec.
 *
 * Returns -1 if \p t1 < \p t2, 1 if \p t1 > \p t2 and 0 if \p t1 == \p t2
 *
 * \param[in] t1 Time #1.
 * \param[in] t2 Time #2.
 * \return Comparison result.
 */
int cmp_ts(struct timespec *t1, struct timespec *t2) {

	if (t1->tv_sec < t2->tv_sec) {
		return -1;
	} else if (t1->tv_sec > t2->tv_sec) {
		return 1;
	} else if (t1->tv_nsec < t2->tv_nsec) {
		return -1;
	} else if (t1->tv_nsec > t2->tv_nsec) {
		return 1;
	} else { /* equal */
		return 0;
	}

}

/** 
 * Compare two timeval.
 *
 * Returns -1 if \p t1 < \p t2, 1 if \p t1 > \p t2 and 0 if \p t1 == \p t2
 *
 * \param[in] t1 Time #1.
 * \param[in] t2 Time #2.
 * \return Comparison result.
 */
int cmp_tv(struct timeval *t1, struct timeval *t2) {

	if (t1->tv_sec < t2->tv_sec) {
		return -1;
	} else if (t1->tv_sec > t2->tv_sec) {
		return 1;
	} else if (t1->tv_usec < t2->tv_usec) {
		return -1;
	} else if (t1->tv_usec > t2->tv_usec) {
		return 1;
	} else { /* equal */
		return 0;
	}

}
