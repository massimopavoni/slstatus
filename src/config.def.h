/* See LICENSE file for copyright and license details. */

#if defined(SLSTATUS_CONFIG)
	/* interval between updates (in ms) */
	const unsigned int interval = 500;

	/* text to show if no value can be retrieved */
	static const char unknown_str[] = "n/a";

	/* maximum output string length */
	#define MAXLEN 2048

	/*
	 * function            description                     argument (example)
	 *
	 * battery_perc        battery percentage              battery name (BAT0)
	 *                                                     NULL on OpenBSD/FreeBSD
	 * battery_perc_di     battery percentage              battery name (BAT0)
	 *                     dynamic icon only               LINUX ONLY
	 * battery_remaining   battery remaining HH:MM         battery name (BAT0)
	 *                                                     NULL on OpenBSD/FreeBSD
	 * battery_state       battery charging state          battery name (BAT0)
	 *                                                     NULL on OpenBSD/FreeBSD
	 * cat                 read arbitrary file             path
	 * cpu_freq            cpu frequency in MHz            NULL
	 * cpu_perc            cpu usage in percent            NULL
	 * cpu_perc_di         cpu usage in percent            NULL
	 *                     dynamic icon only               LINUX ONLY
	 * datetime            date and time                   format string (%F %T)
	 * disk_free           free disk space in GB           mountpoint path (/)
	 * disk_perc           disk usage in percent           mountpoint path (/)
	 * disk_total          total disk space in GB          mountpoint path (/)
	 * disk_used           used disk space in GB           mountpoint path (/)
	 * entropy             available entropy               NULL
	 * gid                 GID of current user             NULL
	 * hostname            hostname                        NULL
	 * ipv4                IPv4 address                    interface name (eth0)
	 * ipv6                IPv6 address                    interface name (eth0)
	 * kernel_release      `uname -r`                      NULL
	 * keyboard_indicators caps/num lock indicators        format string (c?n?)
	 *                                                     see keyboard_indicators.c
	 * keymap              layout (variant) of current     NULL
	 *                     keymap
	 * load_avg            load average                    NULL
	 * netspeed_rx         receive network speed           interface name (wlan0)
	 * netspeed_tx         transfer network speed          interface name (wlan0)
	 * num_files           number of files in a directory  path
	 *                                                     (/home/foo/Inbox/cur)
	 * ram_free            free memory in GB               NULL
	 * ram_perc            memory usage in percent         NULL
	 * ram_perc_di         memory usage in percent         NULL
	 *                     dynamic icon only               LINUX ONLY
	 * ram_total           total memory size in GB         NULL
	 * ram_used            used memory in GB               NULL
	 * run_command         custom shell command            command (echo foo)
	 * swap_free           free swap in GB                 NULL
	 * swap_perc           swap usage in percent           NULL
	 * swap_total          total swap size in GB           NULL
	 * swap_used           used swap in GB                 NULL
	 * temp                temperature in degree celsius   sensor file
	 *                                                     (/sys/class/thermal/...)
	 *                                                     NULL on OpenBSD
	 *                                                     thermal zone on FreeBSD
	 *                                                     (tz0, tz1, etc.)
	 * temp_di             temperature in degree celsius   sensor file
	 *                     dynamic icon only               LINUX ONLY
	 * uid                 UID of current user             NULL
	 * up                  interfaace is running           interface name (eth0)
	 * uptime              system uptime                   NULL
	 * username            username of current user        NULL
	 * vol_perc            OSS/ALSA volume in percent      mixer file (/dev/mixer)
	 *                                                     NULL on OpenBSD/FreeBSD
	 * wifi_perc           WiFi signal in percent          interface name (wlan0)
	 * wifi_perc_di        WiFi signal in percent          interface name (wlan0)
	 *                     dynamic icon only               LINUX ONLY
	 * wifi_essid          WiFi ESSID                      interface name (wlan0)
	 */
	static const struct arg args[] = {
		/* function          format              argument */
		{ cpu_perc_di,       " %s",              NULL },
		{ cpu_perc,          " %2s%% |",         NULL },
		{ temp_di,           " %s",              "/sys/class/thermal/thermal_zone8/temp" },
		{ temp,              " %2s°C |",         "/sys/class/thermal/thermal_zone8/temp" },
		{ ram_perc_di,       " %s",              NULL },
		{ ram_used,          " %6s |",           NULL },
		{ wifi_perc_di,      " %s",              "wlan0" },
		{ wifi_essid,        " %.3s |",          "wlan0" },
		{ battery_perc_di,   " %s",              "BAT0" },
		{ battery_perc,      " %2s%% |",         "BAT0" },
		{ keymap,            " %s |",            NULL },
		{ datetime,          " %s |",            "%A %d %B %Y | %T" },
	};
#endif

#if defined(__linux__)
	/* enable dynamic icons
	 * color support needed (status2d patch for dwm): leave colors NULL if not available */
	#if defined(BATTERY_CONFIG)
		/* battery dynamic icons */
	        const struct dynico bdis[] = {
			/* level  icon  begin color code  end color code */
	                {  10,    "󰁺",  "^c#ff0000^",     "^d^" },
			{  20,    "󰁻",  "^c#ff3900^",     "^d^" },
	                {  30,    "󰁼",  "^c#ff7100^",     "^d^" },
	                {  40,    "󰁽",  "^c#ffaa00^",     "^d^" },
	                {  50,    "󰁾",  "^c#ffe300^",     "^d^" },
			{  60,    "󰁿",  "^c#e3ff00^",     "^d^" },
			{  70,    "󰂀",  "^c#aaff00^",     "^d^" },
			{  80,    "󰂁",  "^c#71ff00^",     "^d^" },
	                {  90,    "󰂂",  "^c#39ff00^",     "^d^" },
	                {  100,   "󰁹",  "^c#00ff00^",     "^d^" },
	        };
		/* charging battery dynamic icons */
		const struct dynico cbdis[] = {
			/* level  icon  begin color code  end color code */
			{  10,    "󰢜",  "^c#ff0000^",     "^d^" },
	                {  20,    "󰂆",  "^c#ff3900^",     "^d^" },
	                {  30,    "󰂇",  "^c#ff7100^",     "^d^" },
	                {  40,    "󰂈",  "^c#ffaa00^",     "^d^" },
	                {  50,    "󰢝",  "^c#ffe300^",     "^d^" },
	                {  60,    "󰂉",  "^c#e3ff00^",     "^d^" },
	                {  70,    "󰢞",  "^c#aaff00^",     "^d^" },
	                {  80,    "󰂊",  "^c#71ff00^",     "^d^" },
	                {  90,    "󰂋",  "^c#39ff00^",     "^d^" },
	                {  100,   "󰂅",  "^c#00ff00^",     "^d^" },
	        };
	#elif defined(CPU_CONFIG)
		/* cpu dynamic icons */
		const struct dynico cdis[] = {
			/* level  icon  begin color code  end color code */
			{  10,    "󰻠",  "^c#00ff00^",     "^d^" },
                        {  20,    "󰻠",  "^c#39ff00^",     "^d^" },
                        {  30,    "󰻠",  "^c#71ff00^",     "^d^" },
                        {  40,    "󰻠",  "^c#aaff00^",     "^d^" },
                        {  50,    "󰻠",  "^c#e3ff00^",     "^d^" },
                        {  60,    "󰻠",  "^c#ffe300^",     "^d^" },
                        {  70,    "󰻠",  "^c#ffaa00^",     "^d^" },
                        {  80,    "󰻠",  "^c#ff7100^",     "^d^" },
                        {  90,    "󰻠",  "^c#ff3900^",     "^d^" },
                        {  100,   "󰻠",  "^c#ff0000^",     "^d^" },
		};
	#elif defined(RAM_CONFIG)
		/* ram dynamic icons */
		const struct dynico rdis[] = {
			/* level  icon  begin color code  end color code */
			{  10,    "󰘚",  "^c#00ff00^",     "^d^" },
                        {  20,    "󰘚",  "^c#39ff00^",     "^d^" },
                        {  30,    "󰘚",  "^c#71ff00^",     "^d^" },
                        {  40,    "󰘚",  "^c#aaff00^",     "^d^" },
                        {  50,    "󰘚",  "^c#e3ff00^",     "^d^" },
                        {  60,    "󰘚",  "^c#ffe300^",     "^d^" },
                        {  70,    "󰘚",  "^c#ffaa00^",     "^d^" },
                        {  80,    "󰘚",  "^c#ff7100^",     "^d^" },
                        {  90,    "󰘚",  "^c#ff3900^",     "^d^" },
                        {  100,   "󰘚",  "^c#ff0000^",     "^d^" },
		};
	#elif defined(TEMP_CONFIG)
		/* temperature dynamic icons */
		/* would need a rework to support multiple uses
		 * but I personally don't need that so I'll leave it as is for now */
		const struct dynico tdis[] = {
			/* level  icon  begin color code  end color code */
			{  50,    "󱤋",  "^c#00ff00^",     "^d^" },
                        {  65,    "󱤋",  "^c#aaff00^",     "^d^" },
                        {  80,    "󱤋",  "^c#ffaa00^",     "^d^" },
                        {  90,    "󱤋",  "^c#ff0000^",     "^d^" },
		};
	#elif defined(WIFI_CONFIG)
		/* wifi dynamic icons */
		const struct dynico wdis[] = {
			/* level  icon  begin color code  end color code */
			{  25,    "󰤟",  "^c#ff0000^",     "^d^" },
			{  40,    "󰤢",  "^c#ffaa00^",     "^d^" },
			{  70,    "󰤥",  "^c#aaff00^",     "^d^" },
			{  100,   "󰤨",  "^c#00ff00^",     "^d^" },
		};
		const char *disconnected = "n/a";
		/* disconnected wifi dynamic icon */
		const struct dynico dwdi = { -1, "󰤮", "^c#ff0000^", "^d^" };
	#endif
#endif
