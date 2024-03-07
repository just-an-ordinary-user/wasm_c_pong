CC=clang

CFLAS_WEB=--target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined

web_app:
	${CC} ${CFLAS_WEB} -o ../../public/web_app.wasm main.c

native_app:
	${CC} main.c -o app