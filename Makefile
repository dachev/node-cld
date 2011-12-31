all: test

build: clean configure compile

configure:
	node-waf configure

compile: configure
	node-waf build

test: build
	@./test/runner.js

clean:
	rm -Rf .lock-wscript
	rm -Rf build


.PHONY: clean test build