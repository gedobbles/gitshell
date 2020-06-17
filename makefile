all: gitsh

install: all
	sudo cp gitsh /usr/local/bin/gitsh
	sudo chmod +x /usr/local/bin/gitsh

uninstall:
	sudo rm /usr/local/bin/gitsh

% :: %.c
	gcc -o $@ $< -lreadline
