#pragma once

class CTAFadeInOut
{
public:
  CTAFadeInOut(void);
  virtual ~CTAFadeInOut(void);
  
  //#### Configuration    ####
  void SetFadeIn(float start = 1.0,float stop = 0.0);
  void SetSampleRate(double samplerate);//futur use
  void SetFadeLenght(double lenght_ms) ;//futur use
  void SetFadeOut(float start = 0.0,float stop = 1.0);
  bool Fade(){return fade;}
  
  //#### Process function ####
  //return true si le fondu est atteint completement
  bool FonduBuffer(float ** dest,float ** source,float ** afondre,long size);
  
  //copy source dans dest
  static void CopyBuffer(float ** dest, float ** source,long size);
  //additionne dest et source met le resultat dans dest
  static void AddBuffer(float ** dest, float ** source,long size);

  //#### Internal members ####
protected:
  bool fade;    //fade actif
  bool fadein;  //fade in or fade out

  //value between 0.0f and 1.0f
  float current;  //current fade value 
  float start;    //start fade value
  float stop;
  float inc;     //incrément
  //float stop;
};
