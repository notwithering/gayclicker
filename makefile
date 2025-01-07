build: main.c
	install -d build
	gcc main.c -o build/gayclicker -lX11 -lXtst

clean:
	rm -rf build
