//==================================================//
//    Multi-Platform Console (MPC) - v4.6.2 @ 2010  //
//     UFSM - Federal University of Santa Maria     //
//==================================================//
#include "mpc_c.h"
#include "demo1.h"

#include <math.h>


//********************************************************************************
//aquivo com alguns exemplos de funcoes avancadas para manipulacao de iamgens no MPC
//21/12/2010
//Cesar Tadeu Pozzer
//********************************************************************************


//aletera o buffer da imagem fornecida para seu negativo.
//Nao eh alocado nenhum espaco novo
void negativoImg(int id)
{
   int LARG = mpcGetImgWidth(id);
   uchar *tmp = mpcGetImg(id);
   for(int y=0; y<mpcGetImgHeight(id); y++)
   {
      for(int x=0; x<LARG; x++)
      {
          tmp[y*LARG*3 + x*3] = 255 - tmp[y*LARG*3 + x*3];
          tmp[y*LARG*3 + x*3+1] = 255 - tmp[y*LARG*3 + x*3+1];
          tmp[y*LARG*3 + x*3+2] = 255 - tmp[y*LARG*3 + x*3+2];
      }
   }
}

//Criacao de uma nova imagem qualquer. Eh usado um algoritmo procedural que define a cor de cada pixel da imagem.
//com este recurso, pode-se fazer a exibição de qualquer tipo de grafico ou figura.
int criaImg(int TAM)
{
   unsigned char v[TAM*TAM*3];  //o vetor pode ser estatico ou dinamico
   //uchar *v = new uchar[TAM*TAM*3];

   for(int y=0; y<TAM; y++)
   {
      for(int x=0; x<TAM; x++)
      {
          v[y*TAM*3 + x*3]   = ( cos(x/5.0) + sin(y/5.0) )*125;
          v[y*TAM*3 + x*3+1] = ( cos(x/15.0) + sin(y/5.0) )*125;
          v[y*TAM*3 + x*3+2] = ( cos(x/5.0) + sin(y/95.0) )*125;
      }
   }
   return mpcCreateImg(v, TAM, TAM);
}


