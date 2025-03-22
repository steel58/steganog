all: test steganog

test: test.c img_ops.c read_write_ops.c
	cc -o test test.c img_ops.c read_write_ops.c -lpng -lz -ggdb

steganog: main.c img_ops.c read_write_ops.c
	cc -o steganog main.c img_ops.c read_write_ops.c -lpng -lz -ggdb
