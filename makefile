build:
	g++ \
	main.cpp \
	-o \
	main \
	src/core/*.h \
	src/core/*.cpp \
	src/widgets/*.h \
	src/widgets/*.cpp \
	src/math/*.h \
	src/helpers/*.h \
	-I. \
	-I./src \
	-I/usr/include/x86_64-linux-gnu/qt5/QtWidgets \
	-I/usr/include/x86_64-linux-gnu/qt5/QtGui \
	-I/usr/include/x86_64-linux-gnu/qt5/QtCore \
	-I/usr/include/x86_64-linux-gnu/qt5 \
	-lQt5Gui \
	-lQt5Core \
	-lQt5Widgets \
	-fPIC