// stdafx.cpp : source file that includes just the standard includes
//	PassManager.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


float NBChaine2float(int nbchaine)
{
  float nb = nbchaine / 126.5f;
  if(nb < 0.0f)nb = 0.0f;
  if(nb > 1.0f)nb = 1.0f;
  return  nb;
}

int float2NBChaine(float flot)
{
  int nb = int(flot * 127.0f);
  if(nb < 0)nb = 0;
  if(nb > 127)nb = 127;
  return nb;
}
