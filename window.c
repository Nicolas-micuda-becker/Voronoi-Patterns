#include "tiny4D.h"
#include <GL4D/gl4dm.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

enum {
    S_EXPANDING = 0,
    S_COMPLETED = 2
};

typedef struct {
    int posX, posY;
    GLuint shade;
    int progress;
} region_t;

static region_t *_regions = NULL;
static int _totalRegions = 0;
static int _expandRadius = 0;
static const GLuint _freePixel = 0x0;

static SDL_Surface * _sourceImage = NULL;

static void terminate(void);
static void render(void);
static void growth_init(int count, SDL_Surface *image);
static int growth_expand(void);
static void growth_free(void);

static void growth_init(int count, SDL_Surface *image){
    if(_regions){
        free(_regions);
    }
    _totalRegions = count;
    _expandRadius = 0;
    _regions = malloc(_totalRegions * sizeof(region_t));
    assert(_regions);
    int width = image->w;
    int height = image->h;
    GLuint *pixels = gl4dpGetPixels();
    memset(pixels, _freePixel, width * height * sizeof(GLuint));
    for(int i = 0; i < _totalRegions; ++i){
        _regions[i].posX = rand() % width;
        _regions[i].posY = rand() % height;
        Uint32 pixelValue = ((Uint32*)image->pixels)[_regions[i].posY * width + _regions[i].posX];
        Uint8 r, g, b;
        SDL_GetRGB(pixelValue, image->format, &r, &g, &b);
        _regions[i].shade = _rgb(r, g, b);
        _regions[i].progress = S_EXPANDING;
    }

    gl4dpScreenHasChanged();
}

static int growth_expand(void){
    int activeRegions = 0;
    int width = gl4dpGetWidth();
    int height = gl4dpGetHeight();
    GLuint *pixels = gl4dpGetPixels();
    for(int i = 0; i < _totalRegions; ++i){
        if(_regions[i].progress == S_COMPLETED){
            continue;
        }
        int centerX = _regions[i].posX;
        int centerY = _regions[i].posY;
        int updated = 0;
        for(int y = -_expandRadius; y <= _expandRadius; ++y){
            for(int x = -_expandRadius; x <= _expandRadius; ++x){
                int newX = centerX + x, newY = centerY + y;
                if(newX >= 0 && newY >= 0 && newX < width && newY < height &&
                    (x * x + y * y <= _expandRadius * _expandRadius) && pixels[newY * width + newX] == _freePixel){
                    pixels[newY * width + newX] = _regions[i].shade;
                updated = 1;
                    }
            }
        }
        if(!updated){
            _regions[i].progress = S_COMPLETED;
        }else{
            activeRegions++;
        }
    }
    _expandRadius++;
    gl4dpScreenHasChanged();
    return activeRegions;
}

static void growth_free(void){
    if(_regions){
        free(_regions);
        _regions = NULL;
    }
    _totalRegions = 0;
    _expandRadius = 0;
}


static void render(void) {
    while (growth_expand() > 0);
    gl4dpUpdateScreen(NULL);
    SDL_Delay(2500);
    growth_init(1500, _sourceImage);
}

static void terminate(void){
    growth_free();
    gl4duClean(GL4DU_ALL);
    if(_sourceImage){
        SDL_FreeSurface(_sourceImage);
    }
}

int main(int argc, char **argv){
    if(argc != 2){
        fprintf(stderr, "Syntax: %s <input.bmp>\n", argv[0]);
        return 1;
    }
    SDL_Surface *tempImage = SDL_LoadBMP(argv[1]);
    if(tempImage == NULL){
        fprintf(stderr, "Unable to load image %s\n", argv[1]);
        return 2;
    }
    _sourceImage = SDL_CreateRGBSurface(0, tempImage->w, tempImage->h, 32, R_MASK, G_MASK, B_MASK, A_MASK);
    SDL_BlitSurface(tempImage, NULL, _sourceImage, NULL);
    SDL_FreeSurface(tempImage);
    if (!gl4duwCreateWindow(argc, argv, "Circle-Based Voronoi", 0, 0, _sourceImage->w, _sourceImage->h, GL4DW_SHOWN)){
        return 1;
    }
    SDL_GL_SetSwapInterval(1);
    gl4dpInitScreenWithDimensions(_sourceImage->w, _sourceImage->h);
    srand(time(NULL));
    growth_init(3000, _sourceImage);
    gl4dpUpdateScreen(NULL);
    atexit(terminate);
    gl4duwDisplayFunc(render);
    gl4duwMainLoop();
    return 0;
}
