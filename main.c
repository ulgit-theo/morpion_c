#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <SDL2/SDL.h>
#include <time.h>
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define GRID_SIZE 3
#define CELL_SIZE (WINDOW_WIDTH / GRID_SIZE)
/*#include <unistd.h>
#include <conio.h>
#include <fcntl.h>*/

typedef enum { EMPTY, PLAYER_X, PLAYER_O } Cell;
typedef enum { EASY, HARD } Difficulty;
typedef struct {
    Cell grid[GRID_SIZE][GRID_SIZE];
    Cell currentPlayer;
    bool gameOver;
} GameState;
typedef struct {
    char pseudo[30];
     int wins;
     int losses;
     int games_played; } Player;
Player player1;
 Player player2;
  Difficulty difficulty;
   bool vsComputer;
   char dif[3][100]={"EASY","HARD","RANDOM"};
char Menu_p[5][100]={"START VS                ","START NEW SOLO GAME ","PLAYER STATISTICS     ","GAME DATA              ","QUIT                 "};
char data[3][100]={"PRIVACY","DATA AND CONTACT","EXIT"};
void color(int t,int f)
{
        HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(H,f*16+t);
}

void gotoxy(int x, int y)
{
   HANDLE hConsoleOutput;
   COORD dwCursorPosition;
   //cout.flush();
   dwCursorPosition.X = x;
   dwCursorPosition.Y = y;
   hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleCursorPosition(hConsoleOutput,dwCursorPosition);
}
void cadre(int x1,int y1,int x2, int y2, int c,char t[])
{
    int i;
    color(c,1);
    for(i=x1;i<=x2;i++)
    {
        gotoxy(i,y1); printf("%c",196);
        gotoxy(i,y2); printf("%c",196);
        Sleep(10);
    }
    for(i=y1;i<=y2;i++)
    {
        gotoxy(x1,i);printf("%c",179);
        gotoxy(x2,i); printf("%c",179);
        Sleep(10);
    }
    gotoxy(x1,y1);printf("%c",218);
    gotoxy(x2,y1);printf("%c",191);
    gotoxy(x1,y2);printf("%c",192);
    gotoxy(x2,y2);printf("%c",217);
    gotoxy(x1+1,y1);printf("%s",t);
}
int Menu(int x,int y,char Tab[][100],int larg,int lon,char texte[50],int taille_parcours)
{
    cadre(4,1,larg,lon,15,texte);
    int k=0,t;
    do
    {
        for(int i=0;i<taille_parcours;i++)
        {
            if(i==k)
                color(0,15);
            else
                color(15,0);
            gotoxy(x+1,y+1+i);
            printf("%s",Tab[i]);
        }
        t=getch();
        if(t==80)
        {
            /*if(k==taille_parcours-1)
                k=0;
            else*/
            k++;
           k=k%taille_parcours;
        }
        if(t==72)
        {
            if(k==0)
                k=taille_parcours-1;
            else
                k--;
        }
    }while(t!=13);
    return(k);
}
void initGame(GameState *game) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            game->grid[i][j] = EMPTY;
        }
    }
    game->currentPlayer = PLAYER_X;
    game->gameOver = false;
}
FILE *file;
int present(Player pe)
{
    Player g;
    file=fopen("scores.txt","r");
    if(file==NULL)
    {
        //printf("probleme d'ouertur");
        return 0;
    }
    else
        {
            while(!feof(file)&&fread(&g,sizeof(g),1,file))
            {
                if(strcmp(g.pseudo,pe.pseudo)==0)
                {
                    fclose(file);
                    return(1);
                }
            }
        }
        fclose(file);
        return(0);
}
void saveGame()

{
   int v1=present(player1);
    int v2=present(player2);
     Player p;

    file = fopen("scores.txt", "r+");
    while(fread(&p,sizeof(p),1,file)&&(v1==1||v2==1))
    {
        if(strcmp(p.pseudo,player1.pseudo)==0&&(v1==1))
        {
            p.games_played++;
            if(player1.wins)
            {
                p.wins++;
            }
            else
            p.losses++;

            fseek(file,-sizeof(p),SEEK_CUR);
            fwrite(&p,sizeof(p),1,file);


        }
     if(strcmp(p.pseudo,player2.pseudo)==0&&(v2==1))
        {
            p.games_played++;
            if(player2.wins)
            {
                p.wins++;
            }
            else
            p.losses++;
            fseek(file,-sizeof(p),SEEK_CUR);
            fwrite(&p,sizeof(p),1,file);


    }
    }
    fclose(file);
    if(v1==0&&v2==0)
    {
        player1.games_played++;
        player2.games_played++;
        file=fopen("scores.txt","a");
        fwrite(&player1,sizeof(player1),1,file);
        fwrite(&player2,sizeof(player2),1,file);
        fclose(file);
    }
    if(v1==1&&v2==0)
    {
        player2.games_played++;
        file=fopen("scores.txt", "a");
        fwrite(&player2,sizeof(player2),1,file);

        fclose(file);
    }
    if(v1==0&&v2==1)
    {
        player1.games_played++;
         file=fopen("scores.txt", "a");
        fwrite(&player1,sizeof(player1),1,file);
        fclose(file);
    }
}

void statti() {
    Player p;
    printf("NAME\tPLAYED PARTY\tWIN\tLOSE\n");
    file=fopen("scores.txt", "r");
     while(!feof(file)&&fread(&p,sizeof(p),1,file))
     {
         printf("%s\t%d           \t%d\t%d\n",p.pseudo,p.games_played,p.wins,p.losses);
     }
     fclose(file);
}
bool checkWin(GameState *game, Cell players) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        if (game->grid[i][0] == players && game->grid[i][1] == players && game->grid[i][2] == players) return true;
        if (game->grid[0][i] == players && game->grid[1][i] == players && game->grid[2][i] == players) return true;
    }
    if (game->grid[0][0] == players && game->grid[1][1] == players && game->grid[2][2] == players) return true;
    if (game->grid[0][2] == players && game->grid[1][1] == players && game->grid[2][0] == players) return true;
    return false;
}
void makeAIMove(GameState *game) {
    if (difficulty == EASY) {
        // Choix facile : premier emplacement vide
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                if (game->grid[i][j] == EMPTY) {
                    game->grid[i][j] = game->currentPlayer;
                    return;
                }
            }
        }
    } else {
        // Choix difficile : IA Minimax ou autre algorithme avancé
        // Pour simplification, on va faire un mouvement pseudo-intelligent :
        int bestMoveScore = -1;
        int moveRow = -1;
        int moveCol = -1;

        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                if (game->grid[i][j] == EMPTY) {
                    game->grid[i][j] = game->currentPlayer;
                    int score = (checkWin(game, game->currentPlayer)) ? true: false;
                    game->grid[i][j] = EMPTY;
                    if (score > bestMoveScore) {
                        bestMoveScore = score;
                        moveRow = i;
                        moveCol = j;
                    }
                }
            }
        }

        if (moveRow != -1 && moveCol != -1) {
            game->grid[moveRow][moveCol] = game->currentPlayer;
        } else {
            // Fallback to easy move if no best move is found
            makeAIMove(game);
        }
    }
}

void drawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 1; i < GRID_SIZE; ++i) {
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, WINDOW_HEIGHT);
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, WINDOW_WIDTH, i * CELL_SIZE);
    }
}
void SDL_RenderDrawCircle(SDL_Renderer *renderer, int x, int y, int radius) {
    int offsetX, offsetY, d;
    int status;

    offsetX = 0;
    offsetY = radius;
    d = radius -1;
    status = 0;

    while (offsetY >= offsetX) {
        status += SDL_RenderDrawPoint(renderer, x + offsetX, y + offsetY);
        status += SDL_RenderDrawPoint(renderer, x + offsetY, y + offsetX);
        status += SDL_RenderDrawPoint(renderer, x - offsetX, y + offsetY);
        status += SDL_RenderDrawPoint(renderer, x - offsetY, y + offsetX);
        status += SDL_RenderDrawPoint(renderer, x + offsetX, y - offsetY);
        status += SDL_RenderDrawPoint(renderer, x + offsetY, y - offsetX);
        status += SDL_RenderDrawPoint(renderer, x - offsetX, y - offsetY);
        status += SDL_RenderDrawPoint(renderer, x - offsetY, y - offsetX);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetX) {
            d -= 2*offsetX + 1;
            offsetX += 1;
        } else if (d < 2 * (radius - offsetY)) {
            d += 2 * offsetY - 1;
            offsetY -= 1;
        } else {
            d += 2 * (offsetY - offsetX - 1);
            offsetY -= 1;
            offsetX += 1;
        }
    }
}

void drawCell(SDL_Renderer *renderer, int row, int col, Cell cell) {
    int x = col * CELL_SIZE;
    int y = row * CELL_SIZE;
    if (cell == PLAYER_X) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, x, y, x + CELL_SIZE, y + CELL_SIZE);
        SDL_RenderDrawLine(renderer, x + CELL_SIZE, y, x, y + CELL_SIZE);
    } else if (cell == PLAYER_O) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderDrawCircle(renderer, x + CELL_SIZE / 2, y + CELL_SIZE / 2, CELL_SIZE / 2);
    }
}

void renderGame(SDL_Renderer *renderer, GameState *game) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    drawGrid(renderer);
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            drawCell(renderer, i, j, game->grid[i][j]);
        }
    }
    SDL_RenderPresent(renderer);
}



bool checkDraw(GameState *game) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (game->grid[i][j] == EMPTY) return false;
        }
    }
    return true;
}

void handleMouseClick(GameState *game, int x, int y) {
    int row = y / CELL_SIZE;
    int col = x / CELL_SIZE;
    if (game->grid[row][col] == EMPTY && !game->gameOver) {
        game->grid[row][col] = game->currentPlayer;
        if (checkWin(game, game->currentPlayer)) {
            game->gameOver = true;
            if (game->currentPlayer == PLAYER_X) {player1.wins++; player2.losses++;}
             else {player2.wins++; player1.losses++;}
        } else if (checkDraw(game)) {
            game->gameOver = true;
        } else {
            game->currentPlayer = (game->currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
            if (vsComputer && game->currentPlayer == PLAYER_O) {
                makeAIMove(game);
                if (checkWin(game, game->currentPlayer)) {
                    game->gameOver = true;
                    player1.losses++;
                } else if (checkDraw(game)) {
                    game->gameOver = true;
                } else {
                    game->currentPlayer = PLAYER_X;
                }
            }
        }
    }
}


int main(int argc, char *argv[])
{
    int ch,t,d,f;
    int blinking = 1;
     gotoxy(13,0);
    while (!_kbhit()) {
        if (blinking) {
                color(12,0);
            printf("\r                                            [ PRESS START ]");
            fflush(stdout);
        } else {

            printf("\r                                                           ");
            fflush(stdout);
        }
        blinking = !blinking;
        Sleep(1000);
        }

    // Clear the input buffer
    while (_getch() != '\r');
    system("cls");

 printf("Enter votre pseudo de joueur: ");
         scanf("%s", player1.pseudo);
    do
    {
         system("cls");
         player1.wins = 0;
         player1.losses = 0;
         player1.games_played = 0;
        ch=Menu(6,2,Menu_p,38,9,"Menu principal",5);
        system("cls");
switch(ch)
{
case 0:
    vsComputer = false;
    printf("Enter Player 2 pseudo: ");
    scanf("%s", player2.pseudo);
    player2.wins = 0;
    player2.losses = 0;
    player2.games_played = 0;
    system("cls");
     printf("\n\n\tchargement en cour...");
    SDL_Delay(500);
    system("cls");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Jeu du Morpion", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    GameState game;
    initGame(&game);

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                handleMouseClick(&game, event.button.x, event.button.y);
            }
        }
        renderGame(renderer, &game);
    }
    saveGame();


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
     break;
case 1:
vsComputer = true;
    d=Menu(6,2,dif,15,10,"difficulty",3);
    difficulty=HARD;
    if(d==0)
        difficulty=EASY;
    //difficulty = (d == 0) ? EASY : HARD;
     player2.wins = 0;
    player2.losses = 0;
    player2.games_played = 0;
     printf("\n\n\tchargement en cour...");
    SDL_Delay(500);
    system("cls");
    SDL_Init(SDL_INIT_VIDEO);
   SDL_Window *window1 = SDL_CreateWindow("Jeu du Morpion", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer1 = SDL_CreateRenderer(window1, -1, SDL_RENDERER_ACCELERATED);

    GameState game1;
    initGame(&game1);

    bool running1 = true;
    SDL_Event event1;
    while (running1) {
        while (SDL_PollEvent(&event1)) {
            if (event1.type == SDL_QUIT) {
                running1 = false;
            } else if (event1.type == SDL_MOUSEBUTTONDOWN) {
                handleMouseClick(&game1, event1.button.x, event1.button.y);
            }
        }
        renderGame(renderer1, &game1);
    }

    SDL_SetRenderDrawColor(renderer1,0,0,0,255);
    SDL_RenderClear(renderer1);
    SDL_DestroyRenderer(renderer1);
    SDL_DestroyWindow(window1);
    SDL_Quit();
    break;
case 2:
    statti();
    system("pause");
    break;
case 3:
    do
    {
         system("cls");
         f=Menu(6,2,data,38,9,"DATA",3);
        system("cls");
        switch(f)
        {
        case 0:
            printf("  ");
            system("pause");
            break;
        case 1:
            printf(" ");
            system("pause");
            break;
        }

}while(f!=2);
    break;
}

printf("\n");
//system("pause");
system("cls");
} while(ch!=4);
    return 0;
}
