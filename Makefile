all:
	mkdir build; cd build; cmake ..; make
	mv build/TextMiningCompiler .

clean:
	rm -rf build
	rm TextMiningCompiler
