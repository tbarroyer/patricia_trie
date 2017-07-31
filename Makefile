all:
	mkdir build; cd build; cmake ..; make
	mv build/TextMiningCompiler .
	mv build/TextMiningApp .

clean:
	rm -rf build
	rm TextMiningCompiler
	rm TextMiningApp
