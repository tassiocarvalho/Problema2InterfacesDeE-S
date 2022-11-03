main: main.c lib.a
        gcc -g -o main main.c lib.a

lib.a: mapear.o escreverLcd.o iniciarLcd.o clear.o
        ar -cvq lib.a mapear.o escreverLcd.o iniciarLcd.o clear.o

mapear.o: mapear.s
        as -g -o mapear.o mapear.s

escreverLcd.o: escreverLcd.s
        as -g -o escreverLcd.o escreverLcd.s

iniciarLcd.o: iniciarLcd.s
        as -g -o iniciarLcd.o iniciarLcd.s

clear.o: clear.s
        as -g -o clear.o clear.s

clean:
        rm *.o *.a main