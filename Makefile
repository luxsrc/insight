all: insight

insight: src/*.cc Makefile
	@mkdir -p bin
	@echo -n 'Compiling $@... '
	@g++ src/*.cc -o bin/insight \
		-I/usr/local/OculusSDK/LibOVR/Src \
		-L/usr/local/OculusSDK/LibOVR/Lib/Mac/Release -lovr \
		-I/opt/local/include \
		-L/opt/local/lib -lsdl2 -lglew \
		-framework OpenGL \
		-framework IOKit  \
		-framework Cocoa
	@if [ -f bin/insight ]; then \
		echo 'DONE.  Use `bin/insight` to run insight.'; \
	else \
		echo 'FAILED!!!'; \
	fi

clean:
	@echo -n 'Clean up... '
	@rm -f bin/insight
	@rm -f src/*~
	@if [ -z "`ls bin 2>&1`" ]; then rmdir bin; fi
	@echo 'DONE'
