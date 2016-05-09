modsniff : slave.h slave.cpp main.cpp
	@g++ -g -o modsniff slave.h slave.cpp main.cpp -L/usr/local/lib -lmodbus

install: modsniff
	cp -f modsniff /usr/local/bin
	cp -f modsniff /usr/local/include/modsniff

clean:
	@rm -f modsniff
