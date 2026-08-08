
/* Build project with:
    cmake -S . -B build
    cmake --build build
*/
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
struct Point
{
    float x;
    float y;
    float z;
};

struct Camera{
    float x;
    float y;
    float z;
    float rx;
    float ry;
    float rz;
};


struct Camera* cam;


struct Point* newPoint(float x, float y, float z){
    struct Point* p = malloc(sizeof(struct Point));
    p->x = x;
    p->y = y;
    p->z = z;

    return p;
}


void project(struct Point* a, struct Camera* c, float* resx, float* resy){
    if (a->z < cam->z)
    {
        float x = 0;
        float y = 0;

        struct Point* A = malloc(sizeof(struct Point));
        

        A->x = cos(c->ry)*(sin(c->rz)*(a->y-c->y) + cos(c->rz)*(a->x-c->x)) - sin(c->ry) * (a->z-c->z);
        
        A->y = (sin(c->rx) * (cos(c->ry)*(a->z-c->z) + sin(c->ry)*(sin(c->rz)*(a->y-c->y) + cos(c->rz)*(a->x-c->x))) + cos(c->rx) * (cos(c->rz)*(a->y-c->y) - sin(c->rz)*(a->x-c->x)));
        A->z = (cos(c->rx) * (cos(c->ry)*(a->z-c->z) + sin(c->ry)*(sin(c->rz)*(a->y-c->y) + cos(c->rz)*(a->x-c->x))) - sin(c->rx) * (cos(c->rz)*(a->y-c->y) - sin(c->rz)*(a->x-c->x)));

        x = (10/A->z)*A->x + 1920 / 2;
        y = (10/A->z)*A->y + 1080 / 2;
        if (resx != NULL && resy != NULL)
        {
            *resx = x;
            *resy = y;
        }
        
        SDL_RenderPoint(renderer, x, y);



        free(A);
    }
    
}



/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    cam = malloc(sizeof(struct Camera));
    cam->x = 0;
    cam->y = 0;
    cam->z = 100;
    cam->rx = 0;
    cam->ry = 0;
    cam->rz = 0;

    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("Hello World", 1920, 1080, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    if (event->type == SDL_EVENT_KEY_DOWN){

    }

    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    struct Point* p1 = newPoint(0, 0, -100);
    struct Point* p2 = newPoint(300, 0, -100);
    struct Point* p3 = newPoint(0, -300, -100);
    struct Point* p4 = newPoint(300, -300, -100);
    struct Point* p5 = newPoint(0, 0, 0);
    struct Point* p6 = newPoint(300, 0, 0);
    struct Point* p7 = newPoint(0, -300, 0);
    struct Point* p8 = newPoint(300, -300, 0);

    float x1, x2, y1, y2, x3, x4, y3, y4;
    
    project(p1, cam, &x1, &y1);
    project(p2, cam, &x2, &y2);
    project(p3, cam, &x3, &y3);
    project(p4, cam, &x4, &y4);
    project(p5, cam, &x4, &y4);
    project(p6, cam, &x4, &y4);
    project(p7, cam, &x4, &y4);
    project(p8, cam, &x4, &y4);

    SDL_RenderLine(renderer, x1, y1, x2, y2);
    SDL_RenderLine(renderer, x1, y1, x3, y3);
    SDL_RenderLine(renderer, x1, y1, x4, y4);
    SDL_RenderLine(renderer, x3, y3, x4, y4);
    
    free(p1);
    free(p2);
    free(p3);
    free(p4);
    free(p5);
    free(p6);
    free(p7);
    free(p8);

    const bool* keyboard = SDL_GetKeyboardState(NULL);
    if (keyboard[SDL_SCANCODE_UP] == true){
        cam->rx += 0.0003;
    }
    if (keyboard[SDL_SCANCODE_DOWN] == true){
        cam->rx -= 0.0003;
    }
    if (keyboard[SDL_SCANCODE_LEFT] == true){
        cam->ry -= 0.0003;
    }
    if (keyboard[SDL_SCANCODE_RIGHT] == true){
        cam->ry += 0.0003;
    }
    if (keyboard[SDL_SCANCODE_W] == true)
    {
        cam->z -= cosf(cam->rx) + sinf(cam->ry);
    }
    if (keyboard[SDL_SCANCODE_S] == true)
    {
        cam->z += cosf(cam->rx) + sinf(cam->ry);
    }
    if (keyboard[SDL_SCANCODE_A] == true)
    {
        cam->x += cosf(cam->rx + 1.55) + sinf(cam->ry + 1.55);
    }
    if (keyboard[SDL_SCANCODE_D] == true)
    {
        cam->x -= cosf(cam->rx + 1.55) + sinf(cam->ry + 1.55);
    }

    
    



    SDL_RenderPresent(renderer);    

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    free(cam);
}