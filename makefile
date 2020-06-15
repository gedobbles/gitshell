all: gitsh

install: all
	sudo cp gitsh /usr/local/gitsh

% :: %.c
	gcc -o $@ $< -lreadline
