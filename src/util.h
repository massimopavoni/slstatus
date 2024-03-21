/* See LICENSE file for copyright and license details. */
#include <stdint.h>

extern char buf[1024];

#define LEN(x) (sizeof(x) / sizeof((x)[0]))

extern char *argv0;

void warn(const char *, ...);
void die(const char *, ...);

int esnprintf(char *str, size_t size, const char *fmt, ...);
const char *bprintf(const char *fmt, ...);
const char *fmt_human(uintmax_t num, int base);

#if defined(__linux__)
	/* dynamic icon */
	struct dynico {
	        const uintmax_t lvl;
	        const char *ico;
	        const char *colb;
	        const char *cole;
	};

	const char *iprintf(const struct dynico *dis, size_t dislen, uintmax_t value);
#endif

int pscanf(const char *path, const char *fmt, ...);
