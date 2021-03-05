gcc ./src/main.c -lm -g -Wl,-rpath='${ORIGIN}' -Wall -o demo

chmod +x ./demo

echo done compiling