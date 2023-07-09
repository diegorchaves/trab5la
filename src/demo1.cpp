//==================================================//
//    Multi-Platform Console (MPC) - v4.6.2 @ 2010  //
//     UFSM - Federal University of Santa Maria     //
//==================================================//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpc_c.h"
#include "demo1.h"


//variaveis globais para controle do demo.
bool changed = true;
int ImgID[4], id = 0;
float alpha = 0;
int imgX = 0;
int direction = 1;
int horizontal_line = 50;
int mouseY = 0;


//faz as inicializacoes basicas do MPC
void initMpc(void) {
   //mpc configuration
   mpcSetSize(APP_LINES, APP_COLUMNS);

   //mpc callbacks
   mpcSetMouseFunc(cbMouse);
   mpcSetUpdateFunc(cbUpdate);
   mpcSetKeyboardFunc(cbKeyboard);

   mpcSetCursorColor(RED_3);
   mpcSetCursorVisible(true);

   mpcAbout();

   criaImagens();
}

//faz o carregamento de um BMP, gera um grafico, copia uma imagem, faz o negativo e cria uma aleatoria, e define a area de recorte de figuras.
//todas as funcoes que criam/carregam figuras retornam um ID, que deve ser usado para se referenciar a essa figura em chamadas subsequentes.
void criaImagens(void) {
   int vet[] = {12, 4, 5, 1};
   const char* legenda[] = {"Idade 1", "Idade 2" ,"Idade 3","Idade 4"};

   //cria um grafico
   ImgID[0] = mpcBuildGraph(vet, 4, "Grafico", "Eixo Y", legenda, GRAPH_MEDIUM);

   //carrega uma imagem de arquivo
   ImgID[1] = mpcLoadBmp("./resources/nvidia.bmp", CYAN_5); //path e cor de transparencia

   //Faz uma copia da imagem com ID 0 para a imagem com ID 2
   ImgID[2] = mpcCreateImg(mpcGetImg(ImgID[0]), mpcGetImgWidth(ImgID[0]), mpcGetImgHeight(ImgID[0]));

   //cria uma imagem a partir de funcoes seno e cosseno
   ImgID[3] = criaImg(300);

   //Exemplo avancado - transforma uma imagem em seu negativo
   negativoImg(ImgID[0]);

   printf("\n\nDados da imagem com ID %d = %d %d %d", ImgID[0], mpcGetImgWidth(0), mpcGetImgHeight(0), mpcGetImgSize(0));
   printf("\n\nDados da imagem com ID %d = %d %d %d", ImgID[1], mpcGetImgWidth(1), mpcGetImgHeight(1), mpcGetImgSize(1));
   printf("\n\nDados da imagem com ID %d = %d %d %d", ImgID[2], mpcGetImgWidth(2), mpcGetImgHeight(2), mpcGetImgSize(2));
   printf("\n\nDados da imagem com ID %d = %d %d %d", ImgID[3], mpcGetImgWidth(3), mpcGetImgHeight(3), mpcGetImgSize(3));


   mpcSetClippingArea(0 , 0, APP_LINES-2, APP_COLUMNS-30);
   mpcSetImgIdVisible(true);

   //mpcDestroyImg(0);
}

//funcao que eh camada a cada frame pela callback cbUpdate() para atualizar a tela
void displayApp(void)
{
    preencheTela();
    animaImagens();

    showBorder();
    showMouse();
}

//muda a cor da linha onde esta o cursor do mouse.
void showMouse()
{
   for (int c = 0, i = 0; c < APP_COLUMNS; c++, i++)
      mpcSetChar(mouseY, c, i, F_STD, BLACK, RED_1, 1);
}

//faz a movimentacao das imagens na tela.
void animaImagens()
{
   imgX += direction;
   if ((imgX + mpcGetImgWidth(id)/CHARACTER_WIDTH) >= APP_COLUMNS) {
      direction = -1;
      imgX = APP_COLUMNS - mpcGetImgWidth(id)/CHARACTER_WIDTH;
   }
   else if (imgX <= 0) {
      direction = 1;
      imgX = 0;
   }

   mpcShowImg(1, imgX, ImgID[id], 1.0);

   mpcShowImg(APP_LINES - mpcGetImgHeight(ImgID[id])/CHARACTER_HEIGHT, APP_COLUMNS -
         imgX - mpcGetImgWidth(ImgID[id])/CHARACTER_WIDTH, ImgID[id], alpha);

   alpha += 0.02f;
   if (alpha > 1.0)
   {
      id = (id+1)%4;
      alpha = 0.0;
   }
}

//preenche a tela com caracteres com transparencia em degrade.
void preencheTela()
{
   int i = 0;
   for (int l = 0; l < APP_LINES; l++)
   {
      for (int c = 0, i=0; c < APP_COLUMNS; c++)
         mpcSetChar(l, c, i++, F_STD, BLACK, YELLOW_5, (float)c / APP_COLUMNS );
   }

   char texto[] = "Texto Texto Texto Texto Texto Texto";
   mostraTexto(4, 2, texto);
   mostraTexto(6, 12, "Controles: setas direcionais, e teclas 1, 2, 3 e 4.");


   mpcHLine(horizontal_line,10,1000, RED_2, 1);
   horizontal_line++;
}

//desenha uma janela com moldura
void showBorder(void) {
   for (int x = 3; x < APP_COLUMNS-3; x++) {
      mpcSetChar(3, x, 9, F_STD, WHITE, BLUE_2, 1.0);
      mpcSetChar(APP_LINES - 3, APP_COLUMNS - x, 9, F_STD, WHITE, BLUE_2, 1.0);
   }
   for (int y = 3; y < APP_LINES-3; y++) {
      mpcSetChar(y, 3, 10, F_STD, WHITE, BLUE_2, 1.0);
      mpcSetChar(APP_LINES - y, APP_COLUMNS - 3, 10, F_STD, WHITE, BLUE_2, 1.0);
   }
   mpcSetChar(3, 3,                       11, F_STD, WHITE, BLUE_2, 1.0);
   mpcSetChar(3, APP_COLUMNS-3,           12, F_STD, WHITE, BLUE_2, 1.0);
   mpcSetChar(APP_LINES-3, 3,             13, F_STD, WHITE, BLUE_2, 1.0);
   mpcSetChar(APP_LINES-3, APP_COLUMNS-3, 14, F_STD, WHITE, BLUE_2, 1.0);

}


//funcao auxiliar para exibir strings na tela.
void mostraTexto(int l, int c, char *msg)
{
   for (int cont = 0; cont < strlen(msg); cont++)
      mpcSetChar(l, c+cont, msg[cont], F_IN, BLACK, GREEN_1, 1 );
}


//*************************************************************
//callbacks implementation
//*************************************************************
void cbMouse(int lin, int col, int button, int state) {
   //mpcSetCursorPos(l, c);
   //printf("\ncalback de mouse %d %d",lin, col);
   mouseY = lin;

   changed = true;
}

void cbKeyboard(int key, int modifier, bool special, bool up) {
   if (special) {
      if (up) {
         //printf("SpecialKeyUp: %d    modifier: %d\n",key,modifier);
      } else {
         //printf("SpecialKey    %d    modifier: %d\n",key,modifier);
      }
      switch (key) {
         case 100:
            direction = -1;
            break;
         case 103:
            direction = 0;
            break;
         case 102:
            direction = 1;
            break;
      }
   } else {
      if (up) {
         //printf("KeyUp: %d    modifier: %d\n",key,modifier);
      } else {
         //printf("Key    %d    modifier: %d\n",key,modifier);
      }
      switch (key) {
         case '1':
            id = 0;
            break;
         case '2':
            id = 1;
            break;
         case '3':
            id = 2;
            break;
         case '4':
            id = 3;
            break;
      }
   }
   //changed = true;
}

void cbUpdate(void) {
   displayApp();
   /*if (changed) {
      displayApp();
      changed = false;
   }*/
}
