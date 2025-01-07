#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define LOCK_FILE "/tmp/gayclicker.lock"
#define CLICK(display) do {									\
    XTestFakeButtonEvent(display, 1, True, CurrentTime);	\
    XTestFakeButtonEvent(display, 1, False, CurrentTime);	\
    XFlush(display);										\
} while (0)


volatile bool running = true;

void handle_signal(int signum) {
	running = false;
}

int main(int argc, char *argv[]) {
	FILE *lockfile = fopen(LOCK_FILE, "r");
	if (lockfile) {
		pid_t pid;
		fscanf(lockfile, "%d", &pid);
		fclose(lockfile);

		if (kill(pid, SIGTERM) != 0) {
			perror("failed to stop existing clicker");
		}

		return 0;
	}

	bool useCps;
	int cps;

	if (argc >= 2) {
		cps = atoi(argv[1]);
		if (cps <= 0) {
			perror("invalid cps value");
			return 1;
		}
		useCps = true;
	}


	lockfile = fopen(LOCK_FILE, "w");
	if (!lockfile) {
		perror("failed to create lock file");
		return 1;
	}
	fprintf(lockfile, "%d", getpid());
	fclose(lockfile);

	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);

	Display *display = XOpenDisplay(NULL);
	if (display == NULL) {
		perror("unable to open display");
		unlink(LOCK_FILE);
		return 1;
	}
	
	if (useCps) {
		struct timespec delay;
		delay.tv_sec = cps >= 1 ? 1 / cps : 1;
		delay.tv_nsec = cps >= 1 ? (1000000000L / cps) % 1000000000L : 0;

		while (running) {
			CLICK(display);
			nanosleep(&delay, NULL);
		}
	} else {
		while (running) {
			CLICK(display);
		}
	}

	XCloseDisplay(display);
	unlink(LOCK_FILE);
	return 0;
}
