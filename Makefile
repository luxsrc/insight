all: insight

insight: *.cc Makefile
	g++ *.cc -o insight \
		-I/usr/local/OculusSDK/LibOVR/Src \
		-L/usr/local/OculusSDK/LibOVR/Lib/Mac/Release -lovr \
		-I/opt/local/include \
		-L/opt/local/lib -lsdl2 \
		-framework OpenGL \
		-framework IOKit  \
		-framework Cocoa

clean:
	rm insight *~
