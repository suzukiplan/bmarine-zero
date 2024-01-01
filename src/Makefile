PROJECT = bmarine-zero
CHR_FILES = image1.chr
WAV_FILES = se1.wav se2.wav
BGM_FILES = song1.bgm
VGSHOME = ../vgszero
BMP2CHR = ${VGSHOME}/tools/bmp2chr/bmp2chr
VGSMML = ${VGSHOME}/tools/vgsmml/vgsmml
MAKEROM = ${VGSHOME}/tools/makerom/makerom
MAKEBGM = ${VGSHOME}/tools/makebgm/makebgm
MAKESE = ${VGSHOME}/tools/makese/makese
MAKEPKG = ${VGSHOME}/tools/makepkg/makepkg
SDCC_FLAGS = -mz80
SDCC_FLAGS += --opt-code-size
SDCC_FLAGS += --code-loc 0x100
SDCC_FLAGS += --xram-loc 0xC000 
SDCC_FLAGS += --xram-size 0x4000 
SDCC_FLAGS += --no-std-crt0
SDCC_FLAGS += -Wl${VGSHOME}/lib/sdcc/vgs0.lib

all: build
	cd ${VGSHOME}/src/sdl2 && make
	${VGSHOME}/src/sdl2/vgs0 game.pkg

build: game.pkg

clean:
	rm -f *.rel
	rm -f *.chr
	rm -f *.lst
	rm -f *.sym
	rm -f *.asm
	rm -f *.ihx
	rm -f *.map
	rm -f *.bin
	rm -f *.lk
	rm -f *.noi
	rm -f *.bgm
	rm -f ${PROJECT}.rom
	rm -f bgm.dat
	rm -f se.dat
	rm -f game.pkg

game.pkg: ${PROJECT}.rom bgm.dat se.dat
	${MAKEPKG} -o game.pkg -r ${PROJECT}.rom -b bgm.dat -s se.dat

${PROJECT}.rom: ${CHR_FILES} program.bin
	${MAKEROM} ${PROJECT}.rom program.bin ${CHR_FILES}

program.bin: program.c ${VGSHOME}/lib/sdcc/vgs0.lib ${VGSHOME}/lib/sdcc/vgs0lib.h
	sdcc ${SDCC_FLAGS} $<
	makebin -s 32768 program.ihx $@

se.dat: ${WAV_FILES}
	${MAKESE} $@ ${WAV_FILES}

bgm.dat: ${BGM_FILES}
	${MAKEBGM} $@ ${BGM_FILES}

.SUFFIXES: .chr .bmp

%.chr: %.bmp
	${BMP2CHR} $< $@ palette.h

.SUFFIXES: .bgm .mml

%.bgm: %.mml
	${VGSMML} $< $@
