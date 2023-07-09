#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpc_c.h"
#include "demo2.h"

// variaveis globais que guardam as coordenadas do cursor, que se move segundo o mouse ou setas
int cursorLin = 5, cursorCol = 5;
int estagio = 0;
int tamanhoPalavra = 0;
const int posicaoInicial = 5;
char num1 = ' ', num2 = ' ';

// mensagem estatica
char msg[] = "Use as setas para mover o cursor, aperte ENTER para fazer as operacoes";
char qtdCaracteres[] = "Qtd caract:";

// matriz global que guarda o texto digitado pelo usuario para ser exibido a cada render
// sem essa matriz, o texto digitado é perdido, pois o MPC nao tem buffer permanente de dados
char texto[APP_LINES][APP_COLUMNS];
char *texto1 = (char *)malloc(21 * sizeof(char));
// char *texto2 = (char*) malloc (21 * sizeof(char));
char *texto3 = (char *)malloc(21 * sizeof(char));

// faz as inicializacoes basicas do MPC
void initMpc(void)
{
    // mpc configuration
    mpcSetSize(APP_LINES, APP_COLUMNS);

    // mpc callbacks
    mpcSetMouseFunc(cbMouse);
    mpcSetUpdateFunc(cbUpdate);
    mpcSetKeyboardFunc(cbKeyboard);

    mpcSetCursorColor(RED_3);
    mpcSetCursorVisible(true);

    // inicializa a matriz com espacos, que eh o padrao de "vazio" no MPC
    memset(texto, ' ', APP_LINES * APP_COLUMNS);

    // posição e tamanho da 'janela' onde as IMAGENS poderão aparecer
    mpcSetClippingArea(0, 0, APP_LINES, APP_COLUMNS);

    // carrega a imagem e associa com um identificador numerico
    // idDaImagem = mpcLoadBmp("./resources/mini.bmp");
    for (int i = 0; i < 21; i++)
    {
        texto1[i] = ' ';
    }

    for (int i = 0; i < 21; i++)
    {
        texto3[i] = ' ';
    }

    mpcAbout();
}

void desenhaCaixas(void)
{
    for (int l = posicaoInicial; l < posicaoInicial + 1; l++)
        for (int c = posicaoInicial; c < posicaoInicial + 22; c++)
        {
            mpcSetChar(l, c, texto[l][c], F_STD, BLACK, YELLOW_1, 1);
        }

    for (int l = posicaoInicial + 3; l < posicaoInicial + 4; l++)
        for (int c = posicaoInicial; c < posicaoInicial + 22; c++)
        {
            mpcSetChar(l, c, texto[l][c], F_STD, BLACK, BLUE_6, 1);
        }

    for (int l = posicaoInicial + 6; l < posicaoInicial + 7; l++)
        for (int c = posicaoInicial; c < posicaoInicial + 22; c++)
        {
            mpcSetChar(l, c, texto[l][c], F_STD, BLACK, BLUE_6, 1);
        }

    for (int l = posicaoInicial + 9; l < posicaoInicial + 10; l++)
        for (int c = posicaoInicial; c < posicaoInicial + 15; c++)
        {
            mpcSetChar(l, c, texto[l][c], F_STD, BLACK, BLUE_6, 1);
        }
}

void escreveTexto()
{
    int OFFSET = 5;
    for (int c = 0; c < 20; c++)
    {
        mpcSetChar(5, c + OFFSET, texto1[c], F_STD, BLACK, YELLOW_1, 1);
    }
    if (estagio == 1)
    {
        for (int c = 0; c < 20; c++)
        {
            mpcSetChar(8, c + OFFSET, texto1[c], F_STD, BLACK, BLUE_6, 1);
        }
        for (int c = 0; c < 20; c++)
        {
            mpcSetChar(11, c + OFFSET, texto3[c], F_STD, BLACK, BLUE_6, 1);
        }

        for (int cont = 0; cont < strlen(qtdCaracteres); cont++)
        {
            mpcSetChar(14, 5 + cont, qtdCaracteres[cont], F_STD, BLACK, BLUE_6, 1);
        }
            mpcSetChar(14, 16, num1, F_STD, BLACK, BLUE_6, 1);
            mpcSetChar(14, 17, num2, F_STD, BLACK, BLUE_6, 1);

    }
}

void deletar(int c)
{
    int deslocando = 1;
    while (deslocando)
    {
        texto1[c] = texto1[c + 1];
        c++;
        if (texto1[c] == ' ')
        {
            deslocando = 0;
        }
    }
}

void fimTexto(int c)
{
    int varrendo = 1;
    while (varrendo)
    {
        if (texto1[c - 5] == ' ')
        {
            varrendo = 0;
        }
        c++;
    }
    cursorCol = c - 1;
}

void maiusculo()
{
    for (int i = 0; i <= 20; i++)
    {
        if (texto3[i] >= 'a' && texto3[i] <= 'z')
        {
            texto3[i] -= 32;
        }
    }
}

void calculaTamanho (int cont)
{
    int numA = 0, numB = 0;
    if (cont > 9)
    {
        numA = cont / 10;
        numB = cont % 10;
        num1 = numA + 48;
        num2 = numB + 48;
    }
    else
    {
        num1 = 48;
        num2 = cont + 48;
    }
}

void inverteString()
{
    int cont = 0, contando = 1;
    while (contando)
    {
        if (texto1[cont] != ' ')
        {
            cont++;
        }
        else
        {
            contando = 0;
        }
    }
    tamanhoPalavra = cont - 1;

    for (int i = 0; i <= 20; i++)
    {
        texto3[i] = texto1[i];
    }

    char temp;
    int i = 0, j = cont - 1;

    while (i < j)
    {
        temp = texto3[i];
        texto3[i] = texto3[j];
        texto3[j] = temp;
        i++;
        j--;
    }
    maiusculo();
    calculaTamanho(cont);
}
// funcao que eh camada a cada frame pela callback cbUpdate() para atualizar a tela
void displayApp(void)
{
    // desenha o texto da matriz.
    for (int l = 0; l < APP_LINES; l++)
        for (int c = 0; c < APP_COLUMNS; c++)
        {
            // desenha cada caractere da matriz com uma formacao especifica de cor de texto, fundo, transparencia e fonte.
            mpcSetChar(l, c, texto[l][c], F_STD, BLACK, GREY_1, 1);
        }

    desenhaCaixas();
    escreveTexto();
    mpcSetCursorPos(cursorLin, cursorCol);

    mostraTexto(2, 3, msg);

    // desenha a imagem na posição do mouse
    // mpcShowImg(5, 7, idDaImagem, 1);
    // mpcShowImg(cursorLin-7, cursorCol-17, idDaImagem, 1);
}

// funcao auxiliar para exibir strings na tela.
void mostraTexto(int l, int c, char *msg)
{
    for (int cont = 0; cont < strlen(msg); cont++)
    {
        mpcSetChar(l, c + cont, msg[cont], F_SN, BLACK, GREEN_1, 1);
    }
}

//*************************************************************
// callbacks implementation
//*************************************************************
void cbMouse(int lin, int col, int button, int state)
{
    printf("\nO Mouse foi movido: %d %d %d %d ", lin, col, button, state);

    // quando o mouse eh movido, faz-se o cursor acompanha-lo
    // mpcSetCursorPos(lin, col);
    // guarda-se as coordenadas do cursor para fazer a insercao de texto na callback cbKeyboard()
    // cursorLin = lin;
    // cursorCol = col;
}

void cbKeyboard(int key, int modifier, bool special, bool up)
{
    printf("\nAlguma tecla foi pressionada: %d %d %d %d", key, modifier, special, up);

    if (special) // se for uma tecla especial cai aqui
    {
        if (up == false) // pega o evento quando uma seta direcional eh pessionada e nao quando eh solta
        {
            switch (key)
            {
            case 100: // seta para esquerda
                if (cursorCol > 6)
                {
                    cursorCol--;
                }
                break;
            // case 101: // seta para cima
            // cursorLin--;
            // break;
            case 102: // seta para direita
                if (cursorCol < 25 && texto1 [cursorCol - 5] != ' ')
                {
                    cursorCol++;
                }
                break;
            // case 103: // seta para baixo
            // cursorLin++;
            // break;
            case 106:
                cursorCol = 5;
                break;
            }
        }
    }
    else
    {
        if (up == false) // soh pega o caractere quando ele for pressionado, ou seja, quando nao eh up.
        {
            // usa-se as coordenadas do cursor para inserir o texto.
            // mpcSetChar(cursorLin, cursorCol, key, F_STD, BLACK, YELLOW_5, 1 );
            // a cada caractere digitado, move-se o cursor para frente.
            // guarda-se a informacao em uma matriz para reexibicao a cada frame. Senao, nao aparece nada na tela.
            if (cursorCol >= 5 && cursorCol <= 25 && cursorLin == 5)
            {
                // texto[cursorLin][cursorCol] = key;
                // texto[cursorLin + 3][cursorCol] = key;
                if (key >= 'A' && key <= 'Z' && cursorCol < 25)
                {
                    texto1[cursorCol - 5] = key;
                    cursorCol++;
                }

                if (key >= 'a' && key <= 'z' && cursorCol < 25)
                {
                    texto1[cursorCol - 5] = key;
                    cursorCol++;
                }
                switch (key)
                {
                case 127:
                    deletar(cursorCol - 5);
                    break;

                case 8:
                    if (cursorCol < 25 && cursorCol > 5)
                    {
                        texto1[cursorCol - 6] = ' ';
                        cursorCol--;
                    }
                    break;

                case 50: // end
                    fimTexto(cursorCol);
                    break;
                case 13:
                    inverteString();
                    estagio = 1;
                    break;
                }
            }
        }
    }
}

void cbUpdate(void)
{
    displayApp();
}
