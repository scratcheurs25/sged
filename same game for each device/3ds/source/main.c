#include <citro2d.h>
#include <3ds.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

static int playerImg;
static int enemyImg;
static int rockImg;
static C2D_SpriteSheet gameImg;
static C2D_Sprite Player;
static C2D_Sprite Enemy;
static C2D_Sprite rocks[40];
static size_t rocknum = 0;

int checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    if (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2) {
        return 1;
    }
    return 0;
}

void initSprite(){
    srand(time(NULL));
    playerImg = 0;
    C2D_SpriteFromSheet(&Player,gameImg,playerImg);
    C2D_SpriteSetPos(&Player,180,200);

    enemyImg = 1;
    C2D_SpriteFromSheet(&Enemy,gameImg,enemyImg);
    C2D_SpriteSetPos(&Enemy,rand() % (int)(SCREEN_WIDTH - Enemy.params.pos.w),0);
}

void makeRock(){
    rockImg = 2;
    C2D_SpriteFromSheet(&rocks[rocknum],gameImg,rockImg);
    C2D_SpriteSetPos(&rocks[rocknum],Player.params.pos.x+15,Player.params.pos.y);
    rocknum += 1;
} 

void rockIdel(){
    for (size_t i = 0; i < rocknum;i++){
        C2D_Sprite* spr = &rocks[i];
        C2D_SpriteMove(spr,0,-5);
        C2D_DrawSprite(spr);

        int x = (int)spr->params.pos.x;
        int y = (int)spr->params.pos.y;
        int w = (int)spr->params.pos.w;
        int h = (int)spr->params.pos.h;

        int ex = (int)Enemy.params.pos.x;
        int ey = (int)Enemy.params.pos.y;
        int ew = (int)Enemy.params.pos.w;
        int eh = (int)Enemy.params.pos.h;

        if (checkCollision(x, y, w, h, ex, ey, ew, eh)){
            srand(time(NULL));
            C2D_SpriteSetPos(&Enemy,rand() % (int)(SCREEN_WIDTH - Enemy.params.pos.w),0);
        }

        if (y < 0){
            for (size_t j = i; j < rocknum - 1; j++) {
                rocks[j] = rocks[j + 1];
            }
            rocknum--;
            i--;
        }
    }
}


int main(){
    romfsInit();
    gfxInitDefault();
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Prepare();
    consoleInit(GFX_BOTTOM,NULL);

    touchPosition pos;

    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP,GFX_LEFT);

    u32 sky_blue = C2D_Color32(0x00,0xFF,0xFF,0xFF);

    gameImg = C2D_SpriteSheetLoad("romfs:/gfx/game.t3x");
    if (!gameImg) svcBreak(USERBREAK_PANIC);

    initSprite();

    int timer = 0;
    int oldTimer = timer+10;
    printf("<-                                ->");
    while (aptMainLoop()){
        hidScanInput();
        timer += 1;

        u32 KDOWN = hidKeysDown();

        hidTouchRead(&pos);

        if (KDOWN & KEY_START){
            break;
        }
        if(KDOWN & (KEY_R | KEY_L) && (oldTimer+10 < timer)){
            makeRock();
            oldTimer = timer;
        }
        if (pos.px > 128){
            C2D_SpriteMove(&Player,5,0);
        }else if ((pos.px < 128) && !(pos.px == 0)){
            C2D_SpriteMove(&Player,-5,0);
        }
        if (Enemy.params.pos.y > SCREEN_HEIGHT){
            break;
        }
        C2D_SpriteMove(&Enemy,0,1);
        C3D_FrameBegin(1);
        C2D_TargetClear(top,sky_blue);
        C2D_SceneBegin(top);

        C2D_DrawSprite(&Player);
        C2D_DrawSprite(&Enemy);
        rockIdel();

        C3D_FrameEnd(0);

    }
    C2D_SpriteSheetFree(gameImg);

    romfsExit();
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}

