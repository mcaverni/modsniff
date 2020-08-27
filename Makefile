modsniff : slave.h slave.cpp main.cpp
	@clang++ -g slave.h slave.cpp main.cpp -L/Users/massimo/WorkDir/sysroot/lib -lmodbus

install: modsniff
	cp -f modsniff /Users/massimo/WorkDir/sysroot/bin
	cp -f modsniff /Users/massimo/WorkDir/sysroot/include/modsniff

clean:
	@rm -f modsniff
