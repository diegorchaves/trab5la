#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpc_c.h"
#include "demo2.h"

// variaveis globais que guardam as coordenadas do cursor, que se move segundo o mouse ou setas
const int cursorLin = 5;
const int posicaoInicial = 5;
const int maxCaracteres = 20;

int cursorCol = 5;
int estagio = 0;
char dezenaQtdCaract = ' ', unidadeQtdCaract = ' ';

// mensagem estatica
char msg[] = "Setas movem o cursor, ENTER para fazer as operacoes, a tecla 2 funciona como END";
char qtdCaracteres[] = "Qtd caract:";

// matriz global que guarda o texto digitado pelo usuario para ser exibido a cada render
// sem essa matriz, o texto digitado � perdido, pois o MPC nao tem buffer permanente de dados
char texto[APP_LINES][APP_COLUMNS];

char *texto1 = (char *)malloc(21 * sizeof(char));
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

    // posi��o e tamanho da 'janela' onde as IMAGENS poder�o aparecer
    mpcSetClippingArea(0, 0, APP_LINES, APP_COLUMNS);

    for (int i = 0; i < maxCaracteres + 1; i++)
    {
        texto1[i] = ' ';
    }

    for (int i = 0; i < maxCaracteres + 1; i++)
    {
        texto3[i] = ' ';
    }

    mpcAbout();
}

void desenhaCaixas(void)
{
    for (int l = posicaoInicial; l < posicaoInicial + 1; l++)
        for (int c = posicaoInicial; c < posicaoInicial + maxCaracteres + 2; c++)
        {
            mpcSetChar(l, c, texto[l][c], F_STD, BLACK, YELLOW_1, 1);
        }

    for (int l = posicaoInicial + 3; l < posicaoInicial + 4; l++)
        for (int c = posicaoInicial; c < posicaoInicial + maxCaracteres + 2; c++)
        {
            mpcSetChar(l, c, texto[l][c], F_STD, BLACK, BLUE_6, 1);
        }

    for (int l = posicaoInicial + 6; l < posicaoInicial + 7; l++)
        for (int c = posicaoInicial; c < posicaoInicial + maxCaracteres + 2; c++)
        {
            mpcSetChar(l, c, texto[l][c], F_STD, BLACK, BLUE_6, 1);
        }

    for (int l = posicaoInicial + 9; l < posicaoInicial + 10; l++)
        for (int c = posicaoInicial; c < posicaoInicial + maxCaracteres - 5; c++)
        {
            mpcSetChar(l, c, texto[l][c], F_STD, BLACK, BLUE_6, 1);
        }
}

void escreveTexto()
{
    int offsetCol = 5, offsetLin = 3;
    for (int c = 0; c < maxCaracteres; c++)
    {
        mpcSetChar(posicaoInicial, c + offsetCol, texto1[c], F_STD, BLACK, YELLOW_1, 1);
    }
    if (estagio == 1)
    {
        for (int c = 0; c < maxCaracteres; c++)
        {
            mpcSetChar(posicaoInicial + offsetLin, c + offsetCol, texto1[c], F_STD, BLACK, BLUE_6, 1);
        }
        for (int c = 0; c < maxCaracteres; c++)
        {
            mpcSetChar(posicaoInicial + offsetLin * 2, c + offsetCol, texto3[c], F_STD, BLACK, BLUE_6, 1);
        }

        for (int cont = 0; cont < strlen(qtdCaracteres); cont++)
        {
            mpcSetChar(posicaoInicial + offsetLin * 3, posicaoInicial + cont, qtdCaracteres[cont], F_STD, BLACK, BLUE_6, 1);
        }
            mpcSetChar(posicaoInicial + offsetLin * 3, 16, dezenaQtdCaract, F_STD, BLACK, BLUE_6, 1);
            mpcSetChar(posicaoInicial + offsetLin * 3, 17, unidadeQtdCaract, F_STD, BLACK, BLUE_6, 1);
    }
}

void deletar(int c)
{
    while (texto1[c] != ' ')
    {
        texto1[c] = texto1[c + 1];
        c++;
    }
}

void fimTexto()
{
    int c = maxCaracteres;
    while (texto1[c] == ' ')
    {
        c--;
    }
    cursorCol = c + posicaoInicial + 1;
}

void maiusculo()
{
    for (int i = 0; i <= maxCaracteres; i++)
    {
        if (texto3[i] >= 'a' && texto3[i] <= 'z')
        {
            texto3[i] -= 32;
        }
    }
}

void converteTamanhoAscii (int cont)
{
    int dezena = 0, unidade = 0;
    if (cont > 9)
    {
        dezena = cont / 10;
        unidade = cont % 10;
        dezenaQtdCaract = dezena + 48;
        unidadeQtdCaract = unidade + 48;
    }
    else
    {
        dezenaQtdCaract = 48;
        unidadeQtdCaract = cont + 48;
    }
}

void inverteString()
{
    int cont = 0, index = 0;
    while (index < maxCaracteres)
    {
        if (texto1[index] != ' ')
        {
            cont++;
        }
        index++;
    }

    for (int i = 0; i <= maxCaracteres; i++)
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
    converteTamanhoAscii(cont);
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
    mostraTexto(posicaoInicial - 3, posicaoInicial - 2, msg);
}

// funcao auxiliar para exibir strings na tela.
void mostraTexto(int l, int c, char *msg)
{
    for (int cont = 0; cont < strlen(msg); cont++)
    {
        mpcSetChar(l, c + cont, msg[cont], F_SN, BLACK, GREEN_3, 1);
    }
}

//*************************************************************
// callbacks implementation
//*************************************************************
void cbMouse(int lin, int col, int button, int state)
{
    //printf("\nO Mouse foi movido: %d %d %d %d ", lin, col, button, state);
}

void cbKeyboard(int key, int modifier, bool special, bool up)
{
    //printf("\nAlguma tecla foi pressionada: %d %d %d %d", key, modifier, special, up);

    if (special) // se for uma tecla especial cai aqui
    {
        if (up == false) // pega o evento quando uma seta direcional eh pessionada e nao quando eh solta
        {
            switch (key)
            {
            case 100: // seta para esquerda
                if (cursorCol > posicaoInicial + 1)
                {
                    cursorCol--;
                }
                break;

            case 102: // seta para direita
                if (cursorCol < maxCaracteres + 5 && texto1[cursorCol - posicaoInicial] != ' ')
                {
                    cursorCol++;
                }
                break;

            case 106: //home
                cursorCol = posicaoInicial;
                break;
            }
        }
    }
    else
    {
        if (up == false) // soh pega o caractere quando ele for pressionado, ou seja, quando nao eh up.
        {
            // usa-se as coordenadas do cursor para inserir o texto.
            // a cada caractere digitado, move-se o cursor para frente.
            // guarda-se a informacao em uma matriz para reexibicao a cada frame. Senao, nao aparece nada na tela.
            if (cursorCol >= posicaoInicial && cursorCol <= posicaoInicial + maxCaracteres && cursorLin == posicaoInicial)
            {
                if (key >= 'A' && key <= 'Z' && cursorCol < posicaoInicial + maxCaracteres) //digitacao
                {
                    texto1[cursorCol - posicaoInicial] = key;
                    cursorCol++;
                }

                if (key >= 'a' && key <= 'z' && cursorCol < posicaoInicial + maxCaracteres) //digitacao
                {
                    texto1[cursorCol - posicaoInicial] = key;
                    cursorCol++;
                }
                switch (key)
                {

                case 8: //backspace
                    if (cursorCol <= posicaoInicial + maxCaracteres && cursorCol > posicaoInicial)
                    {
                        texto1[cursorCol - (posicaoInicial+1)] = ' ';
                        cursorCol--;
                    }
                    break;

                case 13: //enter
                    inverteString();
                    estagio = 1;
                    break;

                case 50: // end (2 funciona como END)
                    fimTexto();
                    break;

                case 127: //delete
                    deletar(cursorCol - posicaoInicial);
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
