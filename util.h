#ifndef CC200_UTIL_H
#define CC200_UTIL_H

#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>

#define CC200_UNUSED(NAME) do {                                        \
	((void) sizeof(NAME));                                         \
} while (0)

#define CC200_PRINT_ARGS(FORMAT, ...)                                  \
	"%d:%s:%d:%s: " FORMAT "\n",                                   \
	nodeinfo.nodenumber,                                           \
	__FILE__,                                                      \
	__LINE__,                                                      \
	__func__,                                                      \
	__VA_ARGS__ 

#define CC200_PRINT(FORMAT, ...) do {                                  \
	char filename[32];                                             \
	FILE *log;                                                     \
	snprintf(filename, 32, "hammertime.%d.log", getpid());         \
	log = fopen(filename, "a");                                    \
	if (log) {                                                     \
		fprintf(log, CC200_PRINT_ARGS(FORMAT, __VA_ARGS__));   \
		fflush(log);                                           \
		fclose(log);                                           \
	}                                                              \
	printf(CC200_PRINT_ARGS(FORMAT, __VA_ARGS__));                 \
	fprintf(stderr, CC200_PRINT_ARGS(FORMAT, __VA_ARGS__));        \
	fflush(stdout);                                                \
	fflush(stderr);                                                \
} while (0)

#define CC200_CHECK_BODY do {                                          \
	CC200_PRINT(                                                   \
		"%s: %s",                                              \
		cnet_errname[cnet_errno],                              \
		cnet_errstr[cnet_errno]                                \
	);                                                             \
	exit(0);                                                       \
} while (0)

#define CC200_CHECK(CALL) do {                                         \
	if ((CALL) != 0)                                               \
		CC200_CHECK_BODY;                                      \
} while (0)

#define CC200_CHECK0(CALL) do {                                        \
	if ((CALL) == 0)                                               \
		CC200_CHECK_BODY;                                      \
} while (0)

#define CC200_SEQ	"#%" PRIu8
#define CC200_NODE	"@%" PRId32
#define CC200_LINK	"%%%d"
#define CC200_TO	" -> "
#define CC200_FROM	" <- "

#endif
