let wasm = null;

let cnv: HTMLCanvasElement;
let ctx: CanvasRenderingContext2D | null;

type TModuleExports = {
    main(): void;
    frame(): number;
    on_key_down(key: number): void;
    on_key_up(key: number): void;
    memory: {
        buffer: ArrayBufferLike;
    }
}

function color_hex(color: number) {
    const a = ((color>>(0*8))&0xFF).toString(16).padStart(2, '0');
    const b = ((color>>(1*8))&0xFF).toString(16).padStart(2, '0');
    const g = ((color>>(2*8))&0xFF).toString(16).padStart(2, '0');
    const r = ((color>>(3*8))&0xFF).toString(16).padStart(2, '0');
    return "#"+r+g+b+a;
}

function cstrlen(mem: Uint8Array, ptr: number) {
    let len = 0;
    while (mem[ptr] != 0) {
        len++;
        ptr++;
    }
    return len;
}

function cstr_by_ptr(mem_buffer: ArrayBufferLike, ptr: number) {
    const mem = new Uint8Array(mem_buffer);
    const len = cstrlen(mem, ptr);
    const bytes = new Uint8Array(mem_buffer, ptr, len);
    return new TextDecoder().decode(bytes);
}

function loop(fn: CallableFunction) {
    fn();
    window.requestAnimationFrame(() => loop(fn));
}

WebAssembly.instantiateStreaming(fetch('web_app.wasm'), {
    env: {
        print(str_ptr: number) {
            const buffer = wasm!.instance.exports.memory.buffer;
            console.log(cstr_by_ptr(buffer, str_ptr));
        },
        create_canvas(width: number, height: number) {
            cnv = document.createElement("canvas");
            cnv.width = width;
            cnv.height = height;
            ctx = cnv.getContext("2d");
            document.body.append(cnv);
        },
        fill_rect(x: number, y: number, width: number, height: number, color: number) {
            ctx!.fillStyle = color_hex(color);
            ctx!.fillRect(x, y, width, height);
        },
        stroke_rect(x: number, y: number, width: number, height: number, color: number, thickness: number) {
            ctx!.strokeStyle = color_hex(color);
            ctx!.lineWidth = thickness < 0 ? 1 : thickness;
            ctx!.strokeRect(x, y, width, height);
        },
        clear_rect(x: number, y: number, width: number, height: number) {
            ctx!.clearRect(x, y, width, height);
        },

        fill_text(x: number, y: number, str_ptr: number) {
            const buffer = wasm!.instance.exports.memory.buffer;
            ctx!.fillStyle = "#ffffff";
            ctx!.font = "24px monospace";
            ctx!.fillText(cstr_by_ptr(buffer, str_ptr), x, y);
        }
    }
}).then((w) => {
    wasm = w;

    console.log(wasm);

    const exports: TModuleExports = wasm.instance.exports as unknown as TModuleExports;

    document.addEventListener("keydown", (ev) => {
        exports.on_key_down(ev.keyCode);
    });

    document.addEventListener("keyup", (ev) => {
        exports.on_key_up(ev.keyCode);
    });

    exports.main();
    window.requestAnimationFrame(() => loop(exports.frame));

    // const buffer = exports.memory.buffer;
}).catch((err) => console.error("[ERROR]: ", err));
 