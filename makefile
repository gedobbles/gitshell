all: gitsh

% :: %.c
	gcc -o $@ $< -lreadline
