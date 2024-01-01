all:
	git submodule update --init vgszero
	cd vgszero/tools && make
	cd src && make


