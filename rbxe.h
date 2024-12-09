#ifndef RB_PIXEL_ENGINE_H
#define RB_PIXEL_ENGINE_H

#ifndef PIXEL_TYPE_DEFINED
#define PIXEL_TYPE_DEFINED

typedef struct Pixel {
    unsigned char r, g, b, a;
} Pixel;

#endif /* PIXEL_TYPE_DEFINED */

/* RBXE core */
int rbxeStep(void);
int rbxeRun(const Pixel* pixbuf);
void rbxeRender(const Pixel* pixbuf);
int rbxeEnd(Pixel* pixbuf);
Pixel* rbxeStart(const char* title, const int winwidth, const int winheight, const int scrwidth, const int scrheight);

void rbxeScreenSize(int* widthptr, int* heightptr);
void rbxeWindowSize(int* widthptr, int* heightptr);
void rbxeBackgroundColor(const Pixel px);

/* time input */
double rbxeTime(void);

/* keyboard input */
int rbxeKeyDown(const int key);
int rbxeKeyPressed(const int key);
int rbxeKeyReleased(const int key);
char rbxeKeyChar(void);

/* mouse input */
void rbxeMousePos(int* xptr, int* yptr);
int rbxeMouseDown(const int button);        
int rbxeMousePressed(const int button);
int rbxeMouseReleased(const int button);
void rbxeMouseVisible(const int visible);

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

/* Roger Boesch's PiXel Engine implementation -------------------------------- */

#ifdef RBXE_APPLICATION

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

static struct rbxeInfo {
    GLFWwindow* window;
    struct rbxeRes {
        int width;
        int height;
    } scrres, winres;
    struct rbxeRatio {
        float width;
        float height;
    } ratio;
    struct rbxeInput {
        int mouseState;
        int queuedChar;
        int memChar;
        unsigned char keys[KEY_LAST];
        unsigned char pressedKeys[KEY_LAST];
    } input;
} rbxe = {NULL, {400, 300}, {800, 600}, {1.0, 1.0}, {GLFW_RELEASE, 1, 0, {0}, {0}}};

/* implementation only static functions */

static void rbxeFrame(void) {
    int i;

    float vertices[16] = {
        1.0f,   1.0f,   1.0f,   1.0f,
        1.0f,  -1.0f,   1.0f,   0.0f,
        -1.0f, -1.0f,   0.0f,   0.0f,
        -1.0f,  1.0f,   0.0f,   1.0f
    };

    const float w = (float)rbxe.winres.width / (float)rbxe.scrres.width;
    const float h = (float)rbxe.winres.height / (float)rbxe.scrres.height;
    
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
            rbxe.input.memChar = key;
        }
        else {
            rbxe.input.memChar = key + 32;
        }

        rbxe.input.queuedChar = 1;
    }

    rbxe.input.keys[key] = action;
    rbxe.input.pressedKeys[key] = rbxe.input.pressedKeys[key] * (action != 0);
}

static void rbxeWindow(GLFWwindow* window, int width, int height) {
    (void)window;
#ifndef __APPLE__
    glViewport(0, 0, width, height);
#endif
    rbxe.winres.width = width;
    rbxe.winres.height = height;
    rbxeFrame();
}

/* window and screen size getters */

void rbxeWindowSize(int* width, int* height) {
    *width = rbxe.winres.width;
    *height = rbxe.winres.height;
}

void rbxeScreenSize(int* width, int* height) {
    *width = rbxe.scrres.width;
    *height = rbxe.scrres.height;
}

/* time input */

double rbxeTime(void) {
    return glfwGetTime();
}

/* keyboard input */

int rbxeKeyDown(const int key) {
    return !!rbxe.input.keys[key];
}

int rbxeKeyPressed(const int key) {
    if (!rbxe.input.pressedKeys[key] && rbxe.input.keys[key]) {
        rbxe.input.pressedKeys[key] = 1;
        return 1;
    }

    return 0;
}

int rbxeKeyReleased(const int key) {
    if (!rbxe.input.keys[key] && rbxe.input.pressedKeys[key]) {
        rbxe.input.pressedKeys[key] = 0;
        return 1;
    }

    return 0;
}

char rbxeKeyChar(void) {
    char ch = 0;

    if (rbxe.input.queuedChar) {
        ch = rbxe.input.memChar;
        rbxe.input.queuedChar = 0;
    }

    return ch;
}

/* mouse input */

void rbxeMousePos(int* x, int* y) {
    double dx, dy;
    float width, height, hwidth, hheight;
    
    glfwGetCursorPos(rbxe.window, &dx, &dy);
    width = (float)rbxe.scrres.width;
    height = (float)rbxe.scrres.height;
    hwidth = width * 0.5;
    hheight = height * 0.5;

    dx = dx * (width / (float)rbxe.winres.width);
    dy = height - dy * (height / (float)rbxe.winres.height);
    *x = (int)((dx - hwidth) / rbxe.ratio.width + hwidth);
    *y = (int)((dy - hheight) / rbxe.ratio.height + hheight);
}

int rbxeMouseDown(const int button) {
    return glfwGetMouseButton(rbxe.window, button);
}

int rbxeMousePressed(const int button) {
    const int mouseButton = glfwGetMouseButton(rbxe.window, button);
    const int pressed = (mouseButton == GLFW_PRESS) &&  (rbxe.input.mouseState == GLFW_RELEASE);
    rbxe.input.mouseState = mouseButton;

    return pressed;
}

int rbxeMouseReleased(const int button) {
    return !glfwGetMouseButton(rbxe.window, button);
}

void rbxeMouseVisible(const int visible) {
    glfwSetInputMode(rbxe.window, GLFW_CURSOR, !visible ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

/* rbxe core */

Pixel* rbxeStart(const char* title,  const int winwidth, const int winheight, const int scrwidth, const int scrheight) {
    Pixel* pixbuf;
    GLFWwindow* window;
    unsigned int id, vao, ebo, texture;
    unsigned int shader, vshader, fshader;

    const size_t scrsize = scrwidth * scrheight;
    const unsigned int indices[] = {
        0,  1,  3,
        1,  2,  3 
    };

    /* init glfw */
    if (!glfwInit()) {
        fprintf(stderr, "RBXE failed to initiate glfw.\n");
        return NULL;
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

    window = glfwCreateWindow(winwidth, winheight, title, glfwGetPrimaryMonitor(), NULL);
    if (!window) {
        fprintf(stderr, "RBXE failed to open glfw window.\n");
        glfwTerminate();
        return NULL;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetWindowSizeLimits(window, scrwidth, scrheight, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowSizeCallback(window, rbxeWindow);
    glfwSetKeyCallback(window, rbxeKeyboard);
    glfwSetInputMode(window, GLFW_MOD_CAPS_LOCK, GLFW_TRUE);

    /* OpenGL context and settings */
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "RBXE failed to initiate glew.\n");
        return NULL;
    }
#endif

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    
    /* allocate pixel framebuffer */
    pixbuf = (Pixel*)calloc(scrsize, sizeof(Pixel));
    if (!pixbuf) {
        fprintf(stderr, "RBXE failed to allocate pixel framebuffer.\n");
        return NULL;
    }

    /* set global information */
    rbxe.window = window;
    rbxe.winres.width = winwidth;
    rbxe.winres.height = winheight;
    rbxe.scrres.width = scrwidth;
    rbxe.scrres.height = scrheight;
    
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
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rbxe.scrres.width, rbxe.scrres.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixbuf);

    return pixbuf;
}

void rbxeBackgroundColor(const Pixel c) {
    const float n = 1.0F / 255.0F;
    glClearColor((float)c.r * n, (float)c.g * n, (float)c.b * n, (float)c.a * n);
}

void rbxeRender(const Pixel* pixbuf) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rbxe.scrres.width, rbxe.scrres.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixbuf);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int rbxeStep(void) { 
    glfwPollEvents();
    glfwSwapBuffers(rbxe.window);
    glClear(GL_COLOR_BUFFER_BIT);

    return !glfwWindowShouldClose(rbxe.window);
}

int rbxeRun(const Pixel* pixbuf) {
    rbxeRender(pixbuf);
    return rbxeStep();
}

int rbxeEnd(Pixel* pixbuf) {
    glfwTerminate();

    if (pixbuf) {
        free(pixbuf);
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

#endif /* RBXE_APPLICATION */
#endif /* RB_PIXEL_ENGINE_H */
