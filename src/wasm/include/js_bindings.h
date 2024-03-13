#ifndef __JS_BINDINGS__H
#define __JS_BINDINGS__H

void print(char *str);

void create_canvas(int width, int height);

void fill_rect(int x, int y, int width, int height, int color);

void clear_rect(int x, int y, int width, int height);

void fill_text(int x, int y, char *text);

#endif // __JS_BINDINGS__H