all: insight

insight: *.cc Makefile
	g++ *.cc -o insight \
		-I/usr/local/OculusSDK/LibOVR/Src \
		-L/usr/local/OculusSDK/LibOVR/Lib/Mac/Release -lovr \
		-framework OpenGL \
		-framework IOKit  \
		-framework Cocoa

clean:
	rm insight *~
