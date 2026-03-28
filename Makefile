.PHONY: test

test:
	bake example.c
	./example.out
	bake test.c
	./test.out

clean:
	${RM} *.out
