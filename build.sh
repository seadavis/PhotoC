#g++  -I/usr/local/include/opencv -I/usr/local/include/opencv2 -Wunused-function -Wunused-variable -o photoc main.cpp context.c -L/usr/local/lib -Wl,-rpath /usr/local/lib/  -lgphoto2 -lgphoto2_port `pkg-config --libs opencv`
cmake .
make VERBOSE=1