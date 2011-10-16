
CFLAG=

triangle_prb: triangle_prb.c ../triangle.c
	gcc -Wall -o triangle_prb triangle_prb.c ../triangle.c -DREAL=double -lm -DTRILIBRARY $(CFLAG)

cubic_prb: cubic_prb.c ../triangle.c
	gcc -Wall -o cubic_prb cubic_prb.c ../triangle.c -DREAL=double -lm -DTRILIBRARY $(CFLAG)

clean:
	rm -rf triangle_prb cubic_prb *.o *.dat
