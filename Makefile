all: test steganog

test: test.c PNG_ops.c
	cc -o test test.c PNG_ops.c

steganog: main.c PNG_ops.c
	cc -o steganog main.c PNG_ops.c
