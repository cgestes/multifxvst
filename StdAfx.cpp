// stdafx.cpp : source file that includes just the standard includes
//	PassManager.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


float NBChaine2float(int nbchaine)
{
  return  nbchaine / 125.5f;
}

int float2NBChaine(float flot)
{
  return int(flot * 126.0f);
}
