PROJECT = bmarine-zero
SRC_FILES = 00_main.rel
SRC_FILES += 01_tables.rel
SRC_FILES += 02_title.rel
SRC_FILES += 03_score.rel
SRC_FILES += 04_game.rel
SRC_FILES += 05_game_player.rel
SRC_FILES += 06_game_pshot.rel
SRC_FILES += 07_game_effect.rel
SRC_FILES += 08_enemy.rel
SRC_FILES += 08-00_bomber.rel
SRC_FILES += 08-01_marine.rel
CHR_FILES = bg.chr
CHR_FILES += fg.chr
CHR_FILES += sprite.chr
WAV_FILES = se000_start.wav
WAV_FILES += se001_shot.wav
WAV_FILES += se002_move.wav
WAV_FILES += se003_missshot.wav
WAV_FILES += se004_jump.wav
WAV_FILES += se005_landing.wav
WAV_FILES += se006_landing2.wav
WAV_FILES += se007_explosion.wav
BGM_FILES = song1.bgm
BGM_FILES += song2.bgm
VGSHOME = ../vgszero
BMP2CHR = ${VGSHOME}/tools/bmp2chr/bmp2chr
VGSMML = ${VGSHOME}/tools/vgsmml/vgsmml
MAKEROM = ${VGSHOME}/tools/makerom/makerom
MAKEBGM = ${VGSHOME}/tools/makebgm/makebgm
MAKESE = ${VGSHOME}/tools/makese/makese
MAKEPKG = ${VGSHOME}/tools/makepkg/makepkg
COMMON_FLAGS = -mz80
COMMON_FLAGS += --opt-code-size
COMMON_FLAGS += --code-loc 0x100
COMMON_FLAGS += --xram-loc 0xC000 
COMMON_FLAGS += --xram-size 0x4000 
COMMON_FLAGS += --no-std-crt0
COMPILE = sdcc
COMPILE += ${COMMON_FLAGS}
COMPILE += -c
LINK = sdcc
LINK += ${COMMON_FLAGS}
LINK += -Wl${VGSHOME}/lib/sdcc/vgs0.lib
LINK += -o program.ihx
LINK += ${SRC_FILES}

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

program.bin: ${SRC_FILES} ${VGSHOME}/lib/sdcc/vgs0.lib ${VGSHOME}/lib/sdcc/vgs0lib.h
	${LINK}
	makebin -s 32768 program.ihx $@

se.dat: ${WAV_FILES}
	${MAKESE} $@ ${WAV_FILES}

bgm.dat: ${BGM_FILES}
	${MAKEBGM} $@ ${BGM_FILES}

.SUFFIXES: .rel .src

%.rel: %.c header.h
	make execute-format FILENAME=$<
	${COMPILE} $<

.SUFFIXES: .chr .bmp

%.chr: %.bmp
	${BMP2CHR} $< $@ palette.h

.SUFFIXES: .bgm .mml

%.bgm: %.mml
	${VGSMML} $< $@

execute-format:
	clang-format -style=file < ${FILENAME} > ${FILENAME}.bak
	cat ${FILENAME}.bak > ${FILENAME}
	rm ${FILENAME}.bak
