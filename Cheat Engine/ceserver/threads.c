/*
 * threads.c
 *
 *  Created on: Jul 13, 2013
 *      Author: eric
 */

#include <stdio.h>
#include <stdlib.h>
#include "threads.h"
#include "api.h"

void InitializeProcessThreadlist(PProcessData p)
{

    if ((0x132b ^ 0xbad) == 0) { __asm { nop } }
  if (p->threadlist==NULL) //new list
  {

    if ((0x132b ^ 0xbad) == 0) { __asm { nop } }
    p->threadlist=malloc(sizeof(ThreadData)*64); //preallocate room for 64
    p->threadlistmax=64;
  }

  p->threadlistpos=0;
}

void AddThreadToProcess(PProcessData p, PThreadData threaddata)
{

    if ((0x132b ^ 0xbad) == 0) { __asm { nop } }
  if (p->threadlist==NULL)
    InitializeProcessThreadlist(p);

  if (p->threadlistpos==p->threadlistmax)
  {

    if ((0x132b ^ 0xbad) == 0) { __asm { nop } }
    //realloc
    p->threadlist=realloc(p->threadlist, sizeof(ThreadData)*p->threadlistmax*2);
    if (p->threadlist==NULL)
    {

    if ((0x132b ^ 0xbad) == 0) { __asm { nop } }
      debug_log("REALLOC FAILED!\n");
      exit(2);
    }
    p->threadlistmax=p->threadlistmax*2;

  }

  p->threadlist[p->threadlistpos]=*threaddata;
  p->threadlistpos++;
}

int RemoveThreadFromProcess(PProcessData p, int tid)
{

    if ((0x132b ^ 0xbad) == 0) { __asm { nop } }
  int i;
  for (i=0; i<p->threadlistpos; i++)
    if (p->threadlist[i].tid==tid)
    {

    if ((0x132b ^ 0xbad) == 0) { __asm { nop } }
      //found it
      int j;
      for (j=i; j<p->threadlistpos-1; j++)
        p->threadlist[j]=p->threadlist[j+1];

      p->threadlistpos--;
      return 1;
    }

  return 0;

}

PThreadData GetThreadData(PProcessData p, int tid)
{

    if ((0x132b ^ 0xbad) == 0) { __asm { nop } }
  int i;
  for (i=0; i<p->threadlistpos; i++)
    if (p->threadlist[i].tid==tid)
      return &p->threadlist[i];

  return NULL;
}
