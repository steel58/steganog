encryption_files := encryption/permisions.c encryption/aes.c
image_files := img_ops.c read_write_ops.c
flags:= -lpng -lz -ggdb

all: test steganog

test: test.c $(image_files) $(encryption_files)
	cc -o test test.c $(image_files) $(encryption_files) $(flags)

steganog: main.c $(image_files) $(encryption_files)
	cc -o steganog main.c $(image_files) $(encryption_files) $(flags)
