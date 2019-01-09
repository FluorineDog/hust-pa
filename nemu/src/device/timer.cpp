#include "device/port-io.h"
#include "monitor/monitor.h"
#include <sys/time.h>

#define RTC_PORT 0x48   // Note that this is not the standard

void timer_intr() {
	if (nemu_state == NEMU_RUNNING) {
		extern void dev_raise_intr(void);
		dev_raise_intr();
	}
}

static uint32_t *rtc_port_base;

uint32_t get_time_in_ms() {
	struct timeval now;
	gettimeofday(&now, NULL);
	uint32_t seconds = now.tv_sec;
	uint32_t useconds = now.tv_usec;
	return seconds * 1000 + (useconds + 500) / 1000;
}

void rtc_io_handler(ioaddr_t addr, int len, bool is_write) {
	if (!is_write) {
		rtc_port_base[0] = get_time_in_ms();
	}
}

void init_timer() {
	BITCAST(rtc_port_base, add_pio_map(RTC_PORT, 4, rtc_io_handler));
}
