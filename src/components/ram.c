/* See LICENSE file for copyright and license details. */
#include <stdio.h>

#include "../slstatus.h"
#include "../util.h"

#if defined(__linux__)
	#include <stdint.h>

	#define RAM_CONFIG
	#include "../config.h"

	static int
	_ram_info(uintmax_t *t, uintmax_t *f, uintmax_t *u, int *p)
	{
		uintmax_t total, free, buffers, cached, used;

		if (pscanf("/proc/meminfo",
                           "MemTotal: %ju kB\n"
                           "MemFree: %ju kB\n"
                           "MemAvailable: %ju kB\n"
                           "Buffers: %ju kB\n"
                           "Cached: %ju kB\n",
                           &total, &free, &buffers, &buffers, &cached) != 5)
                        return -1;

		if (total == 0)
			return -1;

		used = total - free - buffers - cached;

		*t = total;
		*f = free;
		*u = used;
		*p = 100 * used / total;
		return 0;
	}

	const char *
	ram_free(const char *unused)
	{
		uintmax_t null, f;
		int inull;

		if (_ram_info(&null, &f, &null, &inull) < 0)
			return NULL;

		return fmt_human(f * 1024, 1024);
	}

	const char *
	ram_perc(const char *unused)
	{
		uintmax_t null;
		int p;

		if (_ram_info(&null, &null, &null, &p) < 0)
			return NULL;

		return bprintf("%d", p);
	}
	
	const char *
	ram_perc_di(const char *unused)
	{
		uintmax_t null;
		int p;

		if (_ram_info(&null, &null, &null, &p) < 0)
			return NULL;

		return iprintf(rdis, LEN(rdis), p);
	}

	const char *
	ram_total(const char *unused)
	{
		uintmax_t t, null;
		int inull;

		if (_ram_info(&t, &null, &null, &inull) < 0)
			return NULL;

		return fmt_human(t * 1024, 1024);
	}

	const char *
	ram_used(const char *unused)
	{
		uintmax_t null, u;
		int inull;

		if (_ram_info(&null, &null, &u, &inull) < 0)
			return NULL;

		return fmt_human(u * 1024, 1024);
	}
#elif defined(__OpenBSD__)
	#include <stdlib.h>
	#include <sys/sysctl.h>
	#include <sys/types.h>
	#include <unistd.h>

	#define LOG1024 10
	#define pagetok(size, pageshift) (size_t)(size << (pageshift - LOG1024))

	inline int
	load_uvmexp(struct uvmexp *uvmexp)
	{
		int uvmexp_mib[] = {CTL_VM, VM_UVMEXP};
		size_t size;

		size = sizeof(*uvmexp);

		if (sysctl(uvmexp_mib, 2, uvmexp, &size, NULL, 0) >= 0)
			return 1;

		return 0;
	}

	const char *
	ram_free(const char *unused)
	{
		struct uvmexp uvmexp;
		int free_pages;

		if (!load_uvmexp(&uvmexp))
			return NULL;

		free_pages = uvmexp.npages - uvmexp.active;
		return fmt_human(pagetok(free_pages, uvmexp.pageshift) *
				 1024, 1024);
	}

	const char *
	ram_perc(const char *unused)
	{
		struct uvmexp uvmexp;
		int percent;

		if (!load_uvmexp(&uvmexp))
			return NULL;

		percent = uvmexp.active * 100 / uvmexp.npages;
		return bprintf("%d", percent);
	}

	const char *
	ram_total(const char *unused)
	{
		struct uvmexp uvmexp;

		if (!load_uvmexp(&uvmexp))
			return NULL;

		return fmt_human(pagetok(uvmexp.npages,
					 uvmexp.pageshift) * 1024, 1024);
	}

	const char *
	ram_used(const char *unused)
	{
		struct uvmexp uvmexp;

		if (!load_uvmexp(&uvmexp))
			return NULL;

		return fmt_human(pagetok(uvmexp.active,
					 uvmexp.pageshift) * 1024, 1024);
	}
#elif defined(__FreeBSD__)
	#include <sys/sysctl.h>
	#include <sys/vmmeter.h>
	#include <unistd.h>
	#include <vm/vm_param.h>

	const char *
	ram_free(const char *unused) {
		struct vmtotal vm_stats;
		int mib[] = {CTL_VM, VM_TOTAL};
		size_t len;

		len = sizeof(struct vmtotal);
		if (sysctl(mib, 2, &vm_stats, &len, NULL, 0) < 0
		    || !len)
			return NULL;

		return fmt_human(vm_stats.t_free * getpagesize(), 1024);
	}

	const char *
	ram_total(const char *unused) {
		unsigned int npages;
		size_t len;

		len = sizeof(npages);
		if (sysctlbyname("vm.stats.vm.v_page_count",
		                 &npages, &len, NULL, 0) < 0 || !len)
			return NULL;

		return fmt_human(npages * getpagesize(), 1024);
	}

	const char *
	ram_perc(const char *unused) {
		unsigned int npages;
		unsigned int active;
		size_t len;

		len = sizeof(npages);
		if (sysctlbyname("vm.stats.vm.v_page_count",
		                 &npages, &len, NULL, 0) < 0 || !len)
			return NULL;

		if (sysctlbyname("vm.stats.vm.v_active_count",
		                 &active, &len, NULL, 0) < 0 || !len)
			return NULL;

		return bprintf("%d", active * 100 / npages);
	}

	const char *
	ram_used(const char *unused) {
		unsigned int active;
		size_t len;

		len = sizeof(active);
		if (sysctlbyname("vm.stats.vm.v_active_count",
		                 &active, &len, NULL, 0) < 0 || !len)
			return NULL;

		return fmt_human(active * getpagesize(), 1024);
	}
#endif
