PROJECT = bmarine-zero

# source files
COMMON_SRC = 00_main.rel
COMMON_SRC += 01_tables.rel
COMMON_SRC += 02_score.rel

PROGRAM0 = ${COMMON_SRC}
PROGRAM0 += 03-01_submain0.rel
PROGRAM0 += 03-02_logo.rel
PROGRAM0 += 03-03_title1.rel
PROGRAM0 += 03-04_title2.rel
PROGRAM0 += 03-05_ranking.rel

PROGRAM1 = ${COMMON_SRC}
PROGRAM1 += 04_submain1.rel
PROGRAM1 += 05_game_player.rel
PROGRAM1 += 06_game_pshot.rel
PROGRAM1 += 07_game_effect.rel
PROGRAM1 += 08_enemy.rel
PROGRAM1 += 08-00_bomber.rel
PROGRAM1 += 08-01_marineLR.rel
PROGRAM1 += 08-02_marineRL.rel
PROGRAM1 += 08-03_thunder.rel
PROGRAM1 += 08-04_bird.rel
PROGRAM1 += 08-05_unk.rel
PROGRAM1 += 08-06_fish.rel
PROGRAM1 += 08-07_kani.rel
PROGRAM1 += 08-08_kanitama.rel
PROGRAM1 += 09_level.rel

# image files
CHR_FILES = bg.chr
CHR_FILES += fg.chr
CHR_FILES += sprite.chr
CHR_FILES += sprite_bomb.chr
CHR_FILES += sprite_hit.chr
CHR_FILES += sprite_medal.chr
CHR_FILES += sprite_laser.chr
CHR_FILES += sprite_laser2.chr
CHR_FILES += sprite_nabura.chr
CHR_FILES += sprite_catk.chr
CHR_FILES += sprite_bomb00.chr
CHR_FILES += sprite_bomb01.chr
CHR_FILES += sprite_bomb02.chr
CHR_FILES += sprite_bomb03.chr
CHR_FILES += sprite_bomb04.chr
CHR_FILES += sprite_bomb05.chr
CHR_FILES += sprite_bomb06.chr
CHR_FILES += sprite_bomb07.chr
CHR_FILES += sprite_bomb08.chr
CHR_FILES += sprite_bomb09.chr
CHR_FILES += sprite_bomb10.chr
CHR_FILES += sprite_bomb11.chr
CHR_FILES += sprite_bomb12.chr
CHR_FILES += sprite_bomb13.chr
CHR_FILES += sprite_bomb14.chr
CHR_FILES += fg_title.chr
CHR_FILES += sprite_suzukiplan1.chr
CHR_FILES += sprite_suzukiplan2.chr

# sound effect
WAV_FILES = se000_start.wav
WAV_FILES += se001_shot.wav
WAV_FILES += se002_move.wav
WAV_FILES += se003_missshot.wav
WAV_FILES += se004_jump.wav
WAV_FILES += se005_landing.wav
WAV_FILES += se006_landing2.wav
WAV_FILES += se007_explosion.wav
WAV_FILES += se008_thunder.wav
WAV_FILES += se009_thunder2.wav
WAV_FILES += se010_medal.wav
WAV_FILES += se011_charge.wav
WAV_FILES += se012_charge-full.wav
WAV_FILES += se013_laser.wav
WAV_FILES += se014_birdvoice.wav
WAV_FILES += se015_birddrop.wav
WAV_FILES += se016_birdfall.wav
WAV_FILES += se017_kanijump.wav
WAV_FILES += se018_kanitama.wav
WAV_FILES += se019_bigbomb.wav
WAV_FILES += se020_suzukiplan.wav
WAV_FILES += se021_cursor.wav
WAV_FILES += se022_coin.wav
WAV_FILES += se023_enter.wav
WAV_FILES += se024_cancel.wav
WAV_FILES += se025_register.wav

# background music
BGM_FILES = song1.bgm
BGM_FILES += song2.bgm
BGM_FILES += song3.bgm
BGM_FILES += song4.bgm

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
LINK = sdcc ${COMMON_FLAGS} -Wl${VGSHOME}/lib/sdcc/vgs0.lib

all: build
	cd ${VGSHOME}/src/sdl2 && make
	cp -p game.pkg ../image/game.pkg
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

${PROJECT}.rom: program0.bin program1.bin palette.bin ${CHR_FILES} replay.dat
	${MAKEROM} ${PROJECT}.rom program0.bin program1.bin palette.bin ${CHR_FILES} replay.dat

program0.bin: ${PROGRAM0} ${VGSHOME}/lib/sdcc/vgs0.lib ${VGSHOME}/lib/sdcc/vgs0lib.h
	${LINK} -o program0.ihx ${PROGRAM0}
	makebin -s 32768 program0.ihx $@

program1.bin: ${PROGRAM1} ${VGSHOME}/lib/sdcc/vgs0.lib ${VGSHOME}/lib/sdcc/vgs0lib.h
	${LINK} -o program1.ihx ${PROGRAM1}
	makebin -s 32768 program1.ihx $@

palette.bin: sprite.bmp
	${BMP2CHR} $< sprite.chr palette.bin

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
	${BMP2CHR} $< $@

.SUFFIXES: .bgm .mml

%.bgm: %.mml
	${VGSMML} $< $@

execute-format:
	clang-format -style=file < ${FILENAME} > ${FILENAME}.bak
	cat ${FILENAME}.bak > ${FILENAME}
	rm ${FILENAME}.bak
