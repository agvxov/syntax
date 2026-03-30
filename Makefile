.PHONY: test

test:
	bake example.c
	./example.out
	bake test.c
	./test.out

hl:
	bake hl.c

clean:
	-${RM} *.out
	-${RM} *.so
	-${RM} hl
