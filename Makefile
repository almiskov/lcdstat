outdir=./bin
outname=lcdstat

build:
	mkdir -p $(outdir)
	gcc -pthread -o $(outdir)/$(outname) lcdstat.c -lwiringPi -lwiringPiDev -lcrypt -lm -lrt

run:
	$(outdir)/$(outname)