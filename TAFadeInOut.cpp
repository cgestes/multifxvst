
#include "stdafx.h"
#include "tafadeinout.h"




CTAFadeInOut::CTAFadeInOut(void)
{
  fade = false;
  fadein = false;
  //inc =  0.00002f; //50000 block
  //inc =  0.001f; //50000 block 
  inc = 0.0002f;
  start = 0.0;
  stop = 0.0;
  current = 0.0;
}

CTAFadeInOut::~CTAFadeInOut(void)
{
}

void CTAFadeInOut::SetFadeIn(float start ,float stop)//start 1.0, stop = 0.0
{
  if(this->fade == true)//on fait deja un fade
  {
    if(current > stop)  //il faut continuer le fade
    {
      this->stop = stop;
      this->start = start;
      this->fadein = true;
    }else             //on fait un fade out
    {
      this->stop = stop;
      this->start = start;
      this->fadein = false;
    }
  }else ///on fait un nouveau fade in
  {
    this->start = start;
    this->stop = stop;
    this->fade = true;
    this->fadein = true;
    this->current = start;
  }
}

void CTAFadeInOut::SetSampleRate(double samplerate)//futur use
{
}

void CTAFadeInOut::SetFadeLenght(double lenght_ms) //futur use
{
}

void CTAFadeInOut::SetFadeOut(float start,float stop)//start 0.0, stop = 1.0
{  if(this->fade == true)//on fait deja un fade
  {
    if(current < stop)  //il faut continuer le dade
    {
      this->stop = stop;
      this->start = start;
      this->fadein = false;
    }else             //on fait un fade in
    {
      this->stop = stop;
      this->start = start;
      this->fadein = true;
    }
  }else ///on fait un nouveau fade in
  {
  this->start = start;
  this->stop = stop;
  this->fade = true;
  this->fadein = false;
  this->current = start;
  }

}



//addition à coef ! (CTAFMATHEMATIQUE!)
#define OPERATION_SCIENTIFIQUE(x,y,coef) ( (x*(1.0f - coef))+(y*coef) )

//return true si le fondu est atteint completeme
// dest  // process //clean
bool CTAFadeInOut::FonduBuffer(float ** dest,float ** source,float ** afondre,long size/*,float start,float inc*/)
{
  //float val = current/*start*/;
  //float inc = (end - start) / float(size);

  for (int i = 0; i< size; i++)
  {
    /*if(current < 0.0)current = 0.0;
    if(current > 1.0)current = 1.0;*/
    if(fadein)
    {
      if(current < stop)
      {
        current = stop;
      }
    }
    else
    {
      if(current > stop)
      {
        current = stop;
      }
    }

    dest[0][i] = OPERATION_SCIENTIFIQUE(source[0][i],afondre[0][i],current);
    dest[1][i] = OPERATION_SCIENTIFIQUE(source[1][i],afondre[1][i],current);
    
    if(current != stop)
      if(fadein)
       current -= inc;
      else
       current += inc;
  }
  fade = !(current == stop);
  return !fade;

}

//on voit l'algo direct! (vive l'assembleur!)
void CTAFadeInOut::AddBuffer(float ** dest, float ** source,long size)
{
  for (int i = 0; i< size; i++)
  {
    dest[0][i] += source[0][i];
    dest[1][i] += source[1][i];
  }
}

//on voit l'algo direct! (vive l'assembleur!)
void CTAFadeInOut::CopyBuffer(float ** dest, float ** source,long size)
{
  for (int i = 0; i< size; i++)
  {
    dest[0][i] = source[0][i];
    dest[1][i] = source[1][i];
  }
}
