.PHONY: test clean

main: hl test

test:
	bake example.c
	./example.out
	bake test.c
	./test.out

hl: hl.c syntax.h
	bake hl.c

clean:
	-${RM} *.out
	-${RM} *.so
	-${RM} hl
