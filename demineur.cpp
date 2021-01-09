// demineur.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <SDL.h>
#include <iostream>
#include<cstdint>
#include<bitset>
#include <stdlib.h>



#define SIZEX 44
#define SIZEY 32



using namespace std;


void resetGame(SDL_Surface* screen, SDL_Surface* img);
SDL_Rect getCasePos(SDL_Rect pos);
void decouvrirCase(int x, int y);
void generationMines(int x, int y);
void compteMines(int x, int y);
int offsetX = 40;
int offsetY = 70;
int nombreMine = 400;
int nbDrapeau = nombreMine;

char champMine[SIZEY][SIZEX] = { 0 };
/*
0000 000X drapeau
0000 00X0 découvert
0000 0X00 presence de mine 
XXXX 0000 nombre de mines autour

*/


SDL_Surface* ecran = NULL;
SDL_Event event;
SDL_Surface* img_case = NULL, * img_flag = NULL, * img_vide = NULL;
SDL_Surface* img_1 = NULL;
SDL_Surface* img_2 = NULL;
SDL_Surface* img_3 = NULL;
SDL_Surface* img_4 = NULL;
SDL_Surface* img_5 = NULL;
SDL_Surface* img_6 = NULL;
SDL_Surface* img_7 = NULL;
SDL_Surface* img_8 = NULL;
SDL_Surface* img_mine = NULL;
SDL_Surface* img_reset = NULL;
SDL_Surface* img_defaite = NULL;

bool defaite = false; //indicateur defaite

int main(int argc, char* argv[])
{


   

    SDL_Init(SDL_INIT_VIDEO);

    ecran = SDL_SetVideoMode(960, 720, 32, SDL_HWSURFACE);
    SDL_WM_SetCaption("Démineur", NULL);

    img_case = SDL_LoadBMP("img/case.bmp");
    img_flag = SDL_LoadBMP("img/drapeau.bmp");
    img_vide = SDL_LoadBMP("img/vide.bmp");
    img_1 = SDL_LoadBMP("img/1.bmp");
    img_2 = SDL_LoadBMP("img/2.bmp");
    img_3 = SDL_LoadBMP("img/3.bmp");
    img_4 = SDL_LoadBMP("img/4.bmp");
    img_5 = SDL_LoadBMP("img/5.bmp");
    img_6 = SDL_LoadBMP("img/6.bmp");
    img_7 = SDL_LoadBMP("img/7.bmp");
    img_8 = SDL_LoadBMP("img/8.bmp");
    img_mine = SDL_LoadBMP("img/mine.bmp");
    img_defaite = SDL_LoadBMP("img/defaite.bmp");
    img_reset = SDL_LoadBMP("img/reset.bmp");

    // Coloration de la surface ecran en bleu-vert
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 195, 195, 195));

    resetGame(ecran, img_case);


    SDL_Flip(ecran); 



    bool continuer = true;
    bool started = false;
    SDL_Rect pos;
    int caseX;
    int caseY;
   

    SDL_Rect posReset;
    posReset.x = 410;
    posReset.y = 5;

    while (continuer) {

        if (!defaite) {
            SDL_BlitSurface(img_reset, NULL, ecran, &posReset);
        }
        else {
            SDL_BlitSurface(img_defaite, NULL, ecran, &posReset);
        }



        SDL_WaitEvent(&event);

        switch (event.type) {
        case SDL_QUIT:
            continuer = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            pos.x = event.button.x;
            pos.y = event.button.y;

            SDL_Rect casePos = getCasePos(pos);
            /*
            int caseX = casePos.x;
            int caseY = casePos.y;*/

            caseX = casePos.x;
            caseY = casePos.y;


            if (pos.x >= posReset.x && pos.x <= posReset.x + 50 && pos.y >= posReset.y && pos.y <= posReset.y + 50) {
                defaite = false;
                started = false;
                resetGame(ecran, img_case);
            }



            if (caseX != -1 && !defaite) {
                if (event.button.button == SDL_BUTTON_RIGHT) {

                    

                    if ((champMine[caseY][caseX] & (1 << 1)) == 0) {
                        cout << ((champMine[caseY][caseX] & 1) == 0) << endl;
                        if ((champMine[caseY][caseX] & (1<<0)) == 0) {
                            champMine[caseY][caseX] |= (1<<0);
                            pos.x = offsetX + caseX * 20;
                            pos.y = offsetY + caseY * 20;
                            SDL_BlitSurface(img_flag, NULL, ecran, &pos);
                        }
                        else {
                            champMine[caseY][caseX] &= ~(1<<0);
                            pos.x = offsetX + caseX * 20;
                            pos.y = offsetY + caseY * 20;
                            SDL_BlitSurface(img_case, NULL, ecran, &pos);
                        }

                        cout << bitset<8>(champMine[caseY][caseX]) << endl;
                    }

                }


                if (event.button.button == SDL_BUTTON_LEFT) {
                    
                    
                    cout << bitset<8>(champMine[caseY][caseX]) << endl;

                    if (!started) {
                        generationMines(caseX, caseY);
                        started = true;
                    }
                    
                    
                    if (!(champMine[caseY][caseX] & (1 << 1)) && !(champMine[caseY][caseX] & 1)) {
                        /*cout << "check" << endl;
                        champMine[caseY][caseX] |= (1 << 1);
                       
                        pos.x = 30 + caseX * 20;
                        pos.y = 30 + caseY * 20;
                        SDL_BlitSurface(img_vide, NULL, ecran, &pos);*/

                        decouvrirCase(caseX, caseY);
                            
                    }
                }

               
                
            }
            

            break;

        case SDL_KEYDOWN:


#ifdef _DEBUG




            if (event.key.keysym.sym == SDLK_u) {
                for (int i = 0; i < SIZEX; i++) {
                    for (int j = 0; j < SIZEY; j++) {
                        pos.x = offsetX + i * 20;
                        pos.y = offsetY + j * 20;
                        if (champMine[j][i] & 0x04) {
                            SDL_BlitSurface(img_mine, NULL, ecran, &pos);
                        }
                        else {
                            SDL_BlitSurface(img_vide, NULL, ecran, &pos);
                        }

                    }
                }
           }
#endif // DEBUG

            break;
        }

        SDL_Flip(ecran);
    }

    

    SDL_Quit();

    return EXIT_SUCCESS;
}



void resetGame(SDL_Surface* screen, SDL_Surface* img) {
    SDL_Rect pos;
    pos.y = offsetY;

    for (int i = 0; i < SIZEY; i++) {
        pos.x = offsetX;
        for (int j = 0; j < SIZEX; j++) {
            SDL_BlitSurface(img, NULL, screen, &pos);
            champMine[i][j] = 0;
            pos.x += 20;
        }
        pos.y += 20;
    }

}


SDL_Rect getCasePos(SDL_Rect pos) {
    
    SDL_Rect casePos;

    if (pos.x - offsetX < 0 || pos.y - offsetY < 0 || pos.x - offsetX > 20 * SIZEX || pos.y - offsetY > 20 * SIZEY) {
        casePos.x = -1;
        
    }
    else {
        casePos.x = (pos.x - offsetX) / 20;
        casePos.y = (pos.y - offsetY) / 20;
    }
    return casePos;
}

void decouvrirCase(int x, int y) {

    if (x < 0 || x >= SIZEX || y < 0 || y >= SIZEY|| (champMine[y][x] & 0x02))
        return;

    SDL_Rect pos;
    pos.x = offsetX + x * 20;
    pos.y = offsetY + y * 20;

    if ((champMine[y][x] & (0xF5)) == 0) { // la case est vide et pas de mine autour et pas de drapeau
        champMine[y][x] |= (1 << 1);

        

        
        SDL_BlitSurface(img_vide, NULL, ecran, &pos);

        decouvrirCase(x - 1, y - 1);
        decouvrirCase(x - 1, y);
        decouvrirCase(x - 1, y + 1);
        decouvrirCase(x, y - 1);
        decouvrirCase(x, y + 1);
        decouvrirCase(x + 1, y - 1);
        decouvrirCase(x + 1, y);
        decouvrirCase(x + 1, y + 1);

        
    }
    else if (champMine[y][x] & 0x04) {
        champMine[y][x] |= (1 << 1);
        defaite = true;
        SDL_BlitSurface(img_mine, NULL, ecran, &pos);

    }
    else if ((champMine[y][x] & 0x01) == 0){
        champMine[y][x] |= (1 << 1);
        switch (champMine[y][x] >> 4) {
        case 1:
            SDL_BlitSurface(img_1, NULL, ecran, &pos);
            break;
        case 2:
            SDL_BlitSurface(img_2, NULL, ecran, &pos);
            break;
        case 3:
            SDL_BlitSurface(img_3, NULL, ecran, &pos);
            break;
        case 4:
            SDL_BlitSurface(img_4, NULL, ecran, &pos);
            break;
        case 5:
            SDL_BlitSurface(img_5, NULL, ecran, &pos);
            break;
        case 6:
            SDL_BlitSurface(img_6, NULL, ecran, &pos);
            break;
        case 7:
            SDL_BlitSurface(img_7, NULL, ecran, &pos);
            break;
        case 8:
            SDL_BlitSurface(img_8, NULL, ecran, &pos);
            break;
        default:
            cout << "error" << endl;
        }
        
    }
}

void generationMines(int x, int y) {
    
    int i = 0;
    while (i < nombreMine) { //génération des mines
        int rx = rand() % SIZEX;
        int ry = rand() % SIZEY;

        if ((rx == x - 1 && ry == y - 1) || (rx == x && ry == y - 1) || (rx == x + 1 && ry == y - 1) ||
            (rx == x - 1 && ry == y) || (rx == x && ry == y) || (rx == x + 1 && ry == y) ||
            (rx == x - 1 && ry == y + 1) || (rx == x && ry == y + 1) || (rx == x + 1 && ry == y + 1)) {
            
        }
        else if (champMine[ry][rx] & 0x04) {

        }
        else {
            champMine[ry][rx] |= 0x04;
            i++;
        }
    }

    for (int i = 0; i < SIZEX; i++) {
        for (int j = 0; j < SIZEY; j++) {
            compteMines(i, j);
        }
    }

}

// la fonction qui va etre un poil casse couille
void compteMines(int x, int y) {
    int compte = 0;

    //verif des out of bounds
    bool haut = !(y - 1 < 0);
    bool gauche = !(x - 1 < 0);
    bool droite = !(x > SIZEX - 2);
    bool bas = !(y > SIZEY - 2);

    if (haut)
        champMine[y - 1][x] & 0x04 ? compte++ : false;
    if (haut && gauche)
        champMine[y - 1][x - 1] & 0x04 ? compte++ : false;
    if (haut && droite)
        champMine[y - 1][x + 1] & 0x04 ? compte++ : false;
    if (gauche)
        champMine[y][x - 1] & 0x04 ? compte++ : false;
    if (droite)
        champMine[y][x + 1] & 0x04 ? compte++ : false;
    if (bas)
        champMine[y + 1][x] & 0x04 ? compte++ : false;
    if (bas && gauche)
        champMine[y + 1][x - 1] & 0x04 ? compte++ : false;
    if (bas && droite)
        champMine[y + 1][x + 1] & 0x04 ? compte++ : false;

    champMine[y][x] |= compte << 4;


}