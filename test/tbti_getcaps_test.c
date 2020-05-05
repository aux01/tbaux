#include "../tbti.c"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

int main(void) {
	int err;

	// load terminfo data from our test directory only
	setenv("TERMINFO", "./terminfo", 1);

	// load the terminfo data into the global tb_term struct and associate
	// with standard output:
	tb_term *t = tb_setupterm("xterm-color", 1, &err);
	assert(t != NULL);

	// read boolean capabilities with the tb_getflag() function
	// returns 1 if the terminal has the capability or 0 if not
	int has_meta_key     = tb_getflag(t, tb_km),
	    back_color_erase = tb_getflag(t, tb_bce),
	    hard_copy        = tb_getflag(t, tb_hc);
	assert(has_meta_key == 1);
	assert(back_color_erase == 0);
	assert(hard_copy == 0);

	// read numeric capabilities with the tb_getnum() function:
	int colors = tb_getnum(t, tb_colors);
	assert(colors == 8);

	// numeric capabilities not supported by the terminal return -1:
	int width_status_line = tb_getnum(t, tb_wsl);
	assert(width_status_line == -1);

	// read string capabilities with the tb_getstr() function:
	char *clr_eol = tb_getstr(t, tb_el);
	assert(strcmp("\x1b[K", clr_eol) == 0);

	// string capabilties not specified in the terminfo file return NULL:
	char *insert_padding = tb_getstr(t, tb_ip);
	assert(insert_padding == NULL);

	// when you're done, remember to free terminal info memory:
	tb_freeterm(t);

	return 0;
}

// vim: noexpandtab