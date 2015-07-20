#include <string.h>
#include <stdio.h>

char *message = "OK+DISISOK+DISC:00000000:00000000000000000000000000000000:0000000000:6C94F8E6C188:-068OK+DISC:00000000:00000000000000000000000000000000:0000000000:2CB43A299015:-074OK+DISC:00000000:00000000000000000000000000000000:0000000000:217BA084F7B5:-061OK+DISC:4C000215:00000000000000000000000000000000:00010001C6:20CD39A14D89:-044OK+DISC:00000000:00000000000000000000000000000000:0000000000:08E17EAA9AD9:-063OK+DISC:00000000:00000000000000000000000000000000:0000000000:07F54DE86477:-072OK+DISCE";

void main() {
	int i;
	printf("Message: %s\n\n", message);

	char device[80];
	char *idx_start;
	char *idx_end;
	idx_start = strstr(message, "OK+DISC");

	while(strcmp(idx_start, "OK+DISCE")) {
		idx_end = strstr(idx_start + 7, "OK+DISC");
		memcpy(device, idx_start + 7, idx_end - idx_start - 7);
		puts(device); // This is what shall be sent through internet
		idx_start = idx_end;
	}

	return;
}