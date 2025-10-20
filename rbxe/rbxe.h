/*
 * RBXE | The pixel_info Engine by Roger Boesch
 *
 * Copyright (C) 2024 Roger Boesch
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef RB_PIXEL_ENGINE_H
#define RB_PIXEL_ENGINE_H

#include <stdarg.h>

#ifndef PIXEL_TYPE_DEFINED
#define PIXEL_TYPE_DEFINED

#define UNUSED(x) (void)(x)
#define ABS(n) ((n) > 0 ? (n) : -(n))

#ifndef BYTE
    typedef unsigned char BYTE;
#endif

#ifndef BOOL
    typedef int BOOL;
#endif

#define TRUE    1
#define FALSE   0

typedef struct pixel_info {
    unsigned char r, g, b, a;
} pixel_info;

#endif /* PIXEL_TYPE_DEFINED */

/* RBXE core */
int rbxeStep(void);
int rbxeRun(void);
int rbxeEnd(void);
int rbxeStart(const char* title, const int win_width, const int win_height, const int scr_width, const int scr_height);

pixel_info* rbxeGetBuffer(void);
void rbxeScreenSize(int* width, int* height);
void rbxeWindowSize(int* width, int* height);
void rbxeBackgroundColor(const pixel_info px);

/* Drawing */
void rbxeClear(const int value);
void rbxeSetPixel(const int x, const int y, const pixel_info color);
void rbxePlotLine(int x1, int y1, int x2, int y2, const pixel_info color);
void rbxePlotRectangle(int x1, int y1, int x2, int y2, const pixel_info color);
void rbxePlotCircle(int xc, int yc, int r, const pixel_info color);

/* time input */
double rbxeTime(void);
double rbxeDeltaTime(void);

/* keyboard input */
int rbxeKeyDown(const int key);
int rbxeKeyPressed(const int key);
int rbxeKeyReleased(const int key);
char rbxeKeyChar(void);

/* mouse input */
void rbxeMousePos(int* x, int* y);
int rbxeMouseDown(const int button);        
int rbxeMousePressed(const int button);
int rbxeMouseReleased(const int button);
void rbxeMouseVisible(const int visible);

/* Logging */
void rbxe_log(const char *fmt, ...);

/* macro keyboard and mouse input values */
#define KEY_SPACE           32
#define KEY_APOSTROPHE      39
#define KEY_COMMA           44
#define KEY_MINUS           45
#define KEY_PERIOD          46
#define KEY_SLASH           47
#define KEY_0               48
#define KEY_1               49
#define KEY_2               50
#define KEY_3               51
#define KEY_4               52
#define KEY_5               53
#define KEY_6               54
#define KEY_7               55
#define KEY_8               56
#define KEY_9               57
#define KEY_SEMICOLON       59
#define KEY_EQUAL           61
#define KEY_A               65
#define KEY_B               66
#define KEY_C               67
#define KEY_D               68
#define KEY_E               69
#define KEY_F               70
#define KEY_G               71
#define KEY_H               72
#define KEY_I               73
#define KEY_J               74
#define KEY_K               75
#define KEY_L               76
#define KEY_M               77
#define KEY_N               78
#define KEY_O               79
#define KEY_P               80
#define KEY_Q               81
#define KEY_R               82
#define KEY_S               83
#define KEY_T               84
#define KEY_U               85
#define KEY_V               86
#define KEY_W               87
#define KEY_X               88
#define KEY_Y               89
#define KEY_Z               90
#define KEY_LEFT_BRACKET    91
#define KEY_BACKSLASH       92
#define KEY_RIGHT_BRACKET   93
#define KEY_GRAVE_ACCENT    96
#define KEY_WORLD_1         161
#define KEY_WORLD_2         162
#define KEY_ESCAPE          256
#define KEY_ENTER           257
#define KEY_TAB             258
#define KEY_BACKSPACE       259
#define KEY_INSERT          260
#define KEY_DELETE          261
#define KEY_RIGHT           262
#define KEY_LEFT            263
#define KEY_DOWN            264
#define KEY_UP              265
#define KEY_PAGE_UP         266
#define KEY_PAGE_DOWN       267
#define KEY_HOME            268
#define KEY_END             269
#define KEY_CAPS_LOCK       280
#define KEY_SCROLL_LOCK     281
#define KEY_NUM_LOCK        282
#define KEY_PRINT_SCREEN    283
#define KEY_PAUSE           284
#define KEY_F1              290
#define KEY_F2              291
#define KEY_F3              292
#define KEY_F4              293
#define KEY_F5              294
#define KEY_F6              295
#define KEY_F7              296
#define KEY_F8              297
#define KEY_F9              298
#define KEY_F10             299
#define KEY_F11             300
#define KEY_F12             301
#define KEY_F13             302
#define KEY_F14             303
#define KEY_F15             304
#define KEY_F16             305
#define KEY_F17             306
#define KEY_F18             307
#define KEY_F19             308
#define KEY_F20             309
#define KEY_F21             310
#define KEY_F22             311
#define KEY_F23             312
#define KEY_F24             313
#define KEY_F25             314
#define KEY_KP_0            320
#define KEY_KP_1            321
#define KEY_KP_2            322
#define KEY_KP_3            323
#define KEY_KP_4            324
#define KEY_KP_5            325
#define KEY_KP_6            326
#define KEY_KP_7            327
#define KEY_KP_8            328
#define KEY_KP_9            329
#define KEY_KP_DECIMAL      330
#define KEY_KP_DIVIDE       331
#define KEY_KP_MULTIPLY     332
#define KEY_KP_SUBTRACT     333
#define KEY_KP_ADD          334
#define KEY_KP_ENTER        335
#define KEY_KP_EQUAL        336
#define KEY_LEFT_SHIFT      340
#define KEY_LEFT_CONTROL    341
#define KEY_LEFT_ALT        342
#define KEY_LEFT_SUPER      343
#define KEY_RIGHT_SHIFT     344
#define KEY_RIGHT_CONTROL   345
#define KEY_RIGHT_ALT       346
#define KEY_RIGHT_SUPER     347
#define KEY_MENU            348
#define KEY_LAST            KEY_MENU
#define MOUSE_1             0
#define MOUSE_2             1
#define MOUSE_3             2
#define MOUSE_4             3
#define MOUSE_5             4
#define MOUSE_6             5
#define MOUSE_7             6
#define MOUSE_8             7
#define MOUSE_LAST          MOUSE_8
#define MOUSE_LEFT          MOUSE_1
#define MOUSE_RIGHT         MOUSE_2
#define MOUSE_MIDDLE        MOUSE_3

/* pixel_info Engine implementation -------------------------------- */

#ifdef RBXE_ENGINE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef __APPLE__
    #include <GL/glew.h>
    #define GLFW_MOD_CAPS_LOCK 0x0010
#else
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
#endif

#include <GLFW/glfw3.h>

#define _RBXE_TOK2STR(s) #s
#define RBXE_TOK2STR(s) _RBXE_TOK2STR(s)

#ifndef RBXE_SHADER_LAYOUT_LOCATION
    #define RBXE_SHADER_LAYOUT_LOCATION 0
#endif

#ifndef RBXE_SHADER_HEADER
    #ifdef __APPLE__ 
        #define RBXE_SHADER_HEADER "#version 330 core\n"
    #else 
        #define RBXE_SHADER_HEADER "#version 300 es\nprecision mediump float;\n"
    #endif
#endif

#define RBXE_SHADER_LAYOUT_STR RBXE_TOK2STR(RBXE_SHADER_LAYOUT_LOCATION)

#define RBXE_SHADER_VERTEX "layout (location = " RBXE_SHADER_LAYOUT_STR ")" \
"in vec4 vertCoord;\n"                          \
"out vec2 TexCoords;\n"                         \
"void main(void) {\n"                           \
"    TexCoords = vertCoord.zw;\n"               \
"    gl_Position = vec4(vertCoord.xy,0.,1.); \n"\
"}\n"

#define RBXE_SHADER_FRAGMENT                    \
"in vec2 TexCoords;\n"                          \
"out vec4 FragColor;\n"                         \
"uniform sampler2D tex;\n"                      \
"void main(void) {\n"                           \
"    FragColor = texture(tex, TexCoords);\n"    \
"}\n"

static const char* vertexShader = RBXE_SHADER_HEADER RBXE_SHADER_VERTEX;
static const char* fragmentShader = RBXE_SHADER_HEADER RBXE_SHADER_FRAGMENT;

/* rbxe core handler */

static struct rbxe_info {
    GLFWwindow* window;
    pixel_info* data;

    double last_time;
    double delta_time;

    struct rbxe_res {
        int width;
        int height;
    } scr_res, win_res;

    struct rbxe_ratio {
        float width;
        float height;
    } ratio;

    struct rbxe_input {
        int mouse_state;
        int queued_char;
        int mem_char;
        unsigned char keys[KEY_LAST];
        unsigned char pressed_keys[KEY_LAST];
    } input;
} rbxe = {NULL, NULL, 0.0, 0.0, {400, 300}, {800, 600}, {1.0, 1.0}, {GLFW_RELEASE, 1, 0, {0}, {0}}};

/* implementation only static functions */

static void rbxeFrame(void) {
    int i;

    float vertices[16] = {
        1.0f,   1.0f,   1.0f,   1.0f,
        1.0f,  -1.0f,   1.0f,   0.0f,
        -1.0f, -1.0f,   0.0f,   0.0f,
        -1.0f,  1.0f,   0.0f,   1.0f
    };

    const float w = (float)rbxe.win_res.width / (float)rbxe.scr_res.width;
    const float h = (float)rbxe.win_res.height / (float)rbxe.scr_res.height;
    
    rbxe.ratio.width = (h < w) ? (h / w) : 1.0f;
    rbxe.ratio.height = (w < h) ? (w / h) : 1.0f;

    for (i = 0; i < 16; i += 4) {
        vertices[i] *= rbxe.ratio.width;
        vertices[i + 1] *= rbxe.ratio.height;
    }
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void rbxeKeyboard(GLFWwindow* win, int key, int code, int action, int mod) {
    (void)win;
    (void)code;

    if (key < 128 && !rbxe.input.keys[key]) {
        if (mod == GLFW_MOD_CAPS_LOCK || mod == GLFW_MOD_SHIFT || key < 65) {
            rbxe.input.mem_char = key;
        }
        else {
            rbxe.input.mem_char = key + 32;
        }

        rbxe.input.queued_char = 1;
    }

    rbxe.input.keys[key] = action;
    rbxe.input.pressed_keys[key] = rbxe.input.pressed_keys[key] * (action != 0);
}

static void rbxeWindow(GLFWwindow* window, int width, int height) {
    (void)window;
#ifndef __APPLE__
    glViewport(0, 0, width, height);
#endif
    rbxe.win_res.width = width;
    rbxe.win_res.height = height;
    rbxeFrame();
}

/* Get pixel buffer */

pixel_info* rbxeGetBuffer(void) {
    return rbxe.data;
}

/* window and screen size getters */

void rbxeWindowSize(int* width, int* height) {
    *width = rbxe.win_res.width;
    *height = rbxe.win_res.height;
}

void rbxeScreenSize(int* width, int* height) {
    *width = rbxe.scr_res.width;
    *height = rbxe.scr_res.height;
}

/* time input */

double rbxeTime(void) {
    return glfwGetTime();
}

double rbxeDeltaTime(void) {
    return rbxe.delta_time;
}

/* keyboard input */

int rbxeKeyDown(const int key) {
    return !!rbxe.input.keys[key];
}

int rbxeKeyPressed(const int key) {
    if (!rbxe.input.pressed_keys[key] && rbxe.input.keys[key]) {
        rbxe.input.pressed_keys[key] = 1;
        return 1;
    }

    return FALSE;
}

int rbxeKeyReleased(const int key) {
    if (!rbxe.input.keys[key] && rbxe.input.pressed_keys[key]) {
        rbxe.input.pressed_keys[key] = 0;
        return 1;
    }

    return FALSE;
}

char rbxeKeyChar(void) {
    char ch = 0;

    if (rbxe.input.queued_char) {
        ch = rbxe.input.mem_char;
        rbxe.input.queued_char = 0;
    }

    return ch;
}

/* mouse input */

void rbxeMousePos(int* x, int* y) {
    double dx, dy;
    float width, height, hwidth, hheight;
    
    glfwGetCursorPos(rbxe.window, &dx, &dy);
    width = (float)rbxe.scr_res.width;
    height = (float)rbxe.scr_res.height;
    hwidth = width * 0.5;
    hheight = height * 0.5;

    dx = dx * (width / (float)rbxe.win_res.width);
    dy = height - dy * (height / (float)rbxe.win_res.height);
    *x = (int)((dx - hwidth) / rbxe.ratio.width + hwidth);
    *y = (int)((dy - hheight) / rbxe.ratio.height + hheight);
}

int rbxeMouseDown(const int button) {
    return glfwGetMouseButton(rbxe.window, button);
}

int rbxeMousePressed(const int button) {
    const int mouseButton = glfwGetMouseButton(rbxe.window, button);
    const int pressed = (mouseButton == GLFW_PRESS) &&  (rbxe.input.mouse_state == GLFW_RELEASE);
    rbxe.input.mouse_state = mouseButton;

    return pressed;
}

int rbxeMouseReleased(const int button) {
    return !glfwGetMouseButton(rbxe.window, button);
}

void rbxeMouseVisible(const int visible) {
    glfwSetInputMode(rbxe.window, GLFW_CURSOR, !visible ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

/* rbxe core */

int rbxeStart(const char* title,  const int win_width, const int win_height, int scaling, int fullscreen) {
    pixel_info* pixbuf;
    GLFWwindow* window;
    unsigned int id, vao, ebo, texture;
    unsigned int shader, vshader, fshader;
    const int scr_width = win_width / scaling;
    const int scr_height = win_height / scaling;
    const size_t scrsize = scr_width * scr_height;
    const unsigned int indices[] = {
        0,  1,  3,
        1,  2,  3 
    };

    /* check scaling */
    if (scaling <= 0 || scr_width < 16 || scr_height < 16) {
        fprintf(stderr, "RBXE failed to initiate: scaling is wrong: %d\n", scaling);
        return FALSE;
    }

    /* init glfw */
    if (!glfwInit()) {
        fprintf(stderr, "RBXE failed to initiate glfw.\n");
        return FALSE;
    }

    /* open and setup window */
#ifdef __APPLE__
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#endif

    window = glfwCreateWindow(win_width, win_height, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

    if (!window) {
        fprintf(stderr, "RBXE failed to open glfw window.\n");
        glfwTerminate();

        return FALSE;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetWindowSizeLimits(window, scr_width, scr_height, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowSizeCallback(window, rbxeWindow);
    glfwSetKeyCallback(window, rbxeKeyboard);
    glfwSetInputMode(window, GLFW_MOD_CAPS_LOCK, GLFW_TRUE);

    /* OpenGL context and settings */
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "RBXE failed to initiate glew.\n");
        return FALSE;
    }
#endif

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    
    /* allocate pixel framebuffer */
    pixbuf = (pixel_info*)calloc(scrsize, sizeof(pixel_info));
    if (!pixbuf) {
        fprintf(stderr, "RBXE failed to allocate pixel framebuffer.\n");
        return FALSE;
    }

    /* set global information */
    rbxe.window = window;
    rbxe.win_res.width = win_width;
    rbxe.win_res.height = win_height;
    rbxe.scr_res.width = scr_width;
    rbxe.scr_res.height = scr_height;
    rbxe.data = pixbuf;

    /* compile and link shaders */
    shader = glCreateProgram();
    
    vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vertexShader, NULL);
    glCompileShader(vshader);

    fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fragmentShader, NULL);
    glCompileShader(fshader);

    glAttachShader(shader, vshader);
    glAttachShader(shader, fshader);
    glLinkProgram(shader);
    
    glDeleteShader(vshader);
    glDeleteShader(fshader);
    
    glUseProgram(shader);

    /* create vertex buffers */
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);

    glGenBuffers(1, &vao);
    glBindBuffer(GL_ARRAY_BUFFER, vao);
    rbxeFrame();
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(RBXE_SHADER_LAYOUT_LOCATION);
    glVertexAttribPointer(RBXE_SHADER_LAYOUT_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, vao);

    /* create render texture (framebuffer) */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rbxe.scr_res.width, rbxe.scr_res.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixbuf);

    rbxe.last_time = rbxeTime();
    rbxe.delta_time = 0.0;

    return 1;
}

void rbxeBackgroundColor(const pixel_info c) {
    const float n = 1.0f / 255.0f;
    glClearColor((float)c.r * n, (float)c.g * n, (float)c.b * n, (float)c.a * n);
}

static void _rbxeRender(const pixel_info* pixbuf) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rbxe.scr_res.width, rbxe.scr_res.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixbuf);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int rbxeStep(void) { 
    glfwPollEvents();
    glfwSwapBuffers(rbxe.window);
    glClear(GL_COLOR_BUFFER_BIT);

    rbxe.delta_time = rbxeTime() - rbxe.last_time;
    rbxe.last_time = rbxeTime();

    return !glfwWindowShouldClose(rbxe.window);
}

int rbxeRun(void) {
    _rbxeRender(rbxe.data);
    return rbxeStep();
}

int rbxeEnd(void) {
    glfwTerminate();

    if (rbxe.data) {
        free(rbxe.data);
        rbxe.data = NULL;

        return TRUE;
    }

    return FALSE;
}

/* Drawing */

void rbxeClear(const int value) {
    int buflen = rbxe.scr_res.width*rbxe.scr_res.height*sizeof(pixel_info);
    memset(rbxe.data, value, buflen);
}

void rbxeSetPixel(const int x, const int y, const pixel_info color) {
    if (x < 0 || x >= rbxe.scr_res.width || y < 0 || y > rbxe.scr_res.height) return;
    
    rbxe.data[y * rbxe.scr_res.width + x] = color;
}

void rbxePlotLine(int x1, int y1, int x2, int y2, const pixel_info color) {
    const int dx = ABS(x2 - x1);
    const int dy = -ABS(y2 - y1);
    const int sx = x1 < x2 ? 1 : -1;
    const int sy = y1 < y2 ? 1 : -1;
    int e2, error = dx + dy;
    
    while (1) {
        rbxeSetPixel(x1, y1, color);

        if (x1 == x2 && y1 == y2) {
            break;
        }

        e2 = error * 2;
        if (e2 >= dy) {
            if (x1 == x2) {
                break;
            }

            error = error + dy;
            x1 = x1 + sx;
        }

        if (e2 <= dx) {
            if (y1 == y2) {
                break;
            }

            error = error + dx;
            y1 = y1 + sy;
        }
    }
}

void rbxePlotRectangle(int x1, int y1, int x2, int y2, const pixel_info color) {
    for (int x = x1; x <= x2; x++) {
        for (int y = y1; y <= y2; y++) {
            rbxeSetPixel(x, y, color);
        }
    }
}

void rbxePlotCircle(int xc, int yc, int r, const pixel_info color) {
	int x = 0;
	int y = r;
	int p = 3 - 2 * r;
	
	if (!r)
		return;
	
	while (y >= x) {
		/* only formulate 1/8 of circle */
		rbxeSetPixel(xc - x, yc - y, color); /* upper left left */
		rbxeSetPixel(xc - y, yc - x, color); /* upper upper left */
		rbxeSetPixel(xc + y, yc - x, color); /* upper upper right */
		rbxeSetPixel(xc + x, yc - y, color); /* upper right right */
		rbxeSetPixel(xc - x, yc + y, color); /* lower left left */
		rbxeSetPixel(xc - y, yc + x, color); /* lower lower left */
		rbxeSetPixel(xc + y, yc + x, color); /* lower lower right */
		rbxeSetPixel(xc + x, yc + y, color); /* lower right right */
		
		if (p < 0)
			p += 4 * x++ + 6;
		else
			p += 4 * (x++ - y--) + 10;
	}
}

/* Logging */
static unsigned long counter = 0;

void rbxe_log(const char *fmt, ...) {
    char buf[1024];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, 1024, fmt, ap);
    va_end(ap);
    buf[1023] = 0;

    counter++;
    printf("%lu> %s\n", counter, buf);
}

#endif /* RBXE_ENGINE */
#endif /* RB_PIXEL_ENGINE_H */
