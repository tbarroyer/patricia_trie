all:
	mkdir build; cd build; cmake ..; make
	mv build/TextMiningCompiler .
	mv build/TextMiningApp .

doc:
	doxygen
	xdg-open doc/html/index.html

clean:
	rm -rf doc
	rm -rf build
	rm TextMiningCompiler
	rm TextMiningApp

