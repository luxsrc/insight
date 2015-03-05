default: insight

tools: harm2raw

insight: src/*.cc Makefile
	@mkdir -p bin
	@echo -n 'Compiling $@... '
	@g++ src/*.cc -O3 -o bin/insight -ljpeg \
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

harm2raw: tools/*.cc Makefile
	@mkdir -p bin
	@echo -n 'Compiling $@... '
	@g++ tools/*.cc -O3 -o bin/harm2raw -lm
	@if [ -f bin/harm2raw ]; then \
		echo 'DONE.  Use `bin/harm2raw` to run harm2raw.'; \
	else \
		echo 'FAILED!!!'; \
	fi

clean:
	@echo -n 'Clean up... '
	@rm -f bin/insight bin/harm2raw
	@rm -f src/*~
	@if [ -z "`ls bin 2>&1`" ]; then rmdir bin; fi
	@echo 'DONE'
