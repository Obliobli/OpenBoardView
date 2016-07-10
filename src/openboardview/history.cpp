#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits.h>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"

History::~History() {
	// Destructor
}

int History::Set_filename(const char *f) {
	fname = strdup(f);
	return 0;
}

int History::Load(void) {
	if (fname) {
		FILE *f;

		f     = fopen(fname, "r");
		count = 0;
		if (!f) return 0;

		while (count < HISTORY_COUNT_MAX) {
			char *r;

			r = fgets(history[count], HISTORY_FNAME_LEN_MAX, f);
			if (r) {
				count++;

				/// strip off the trailing line break
				while (*r) {
					if ((*r == '\r') || (*r == '\n')) {
						*r = '\0';
						break;
					}
					r++;
				}

			} else {
				break;
			}
		}
		fclose(f);
	} else {
		return -1;
	}

	return count;
}

int History::Prepend_save(char *newfile) {
	if (fname) {
		FILE *f;
		f = fopen(fname, "w");
		if (f) {
			int i;

			fprintf(f, "%s\n", newfile);
			for (i = 0; i < count; i++) {
				// Don't create duplicate entries, so check each one against the newfile
				if (strcmp(newfile, history[i])) {
					fprintf(f, "%s\n", history[i]);
				}
			}
			fclose(f);

			Load();
		}
	}

	return 0;
}

/**
 * Only displays the tail end of the filename path, where
 * 'stops' indicates how many paths up to truncate at
 *
 * PLD20160618-1729
 */
char *History::Trim_filename(char *s, int stops) {

	int l   = strlen(s);
	char *p = s + l - 1;

	while ((stops) && (p > s)) {
		if ((*p == '/') || (*p == '\\')) stops--;
		p--;
	}
	if (!stops) p += 2;

	return p;
}
