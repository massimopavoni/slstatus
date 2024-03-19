/* See LICENSE file for copyright and license details. */
#include <stdint.h>

#if defined(__linux__)
	/* dynamic icon */
	struct dynico {
	        const int lvl;
	        const char *ico;
	        const char *colb;
	        const char *cole;
	};
#endif

extern char buf[1024];

#define LEN(x) (sizeof(x) / sizeof((x)[0]))

extern char *argv0;

void warn(const char *, ...);
void die(const char *, ...);

int esnprintf(char *str, size_t size, const char *fmt, ...);
const char *bprintf(const char *fmt, ...);
const char *fmt_human(uintmax_t num, int base);
int pscanf(const char *path, const char *fmt, ...);
