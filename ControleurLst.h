#pragma once

class CControleurParam
{
public:
  CControleurParam(){};
  CControleurParam(CControleurParam & Param){};
  int nbeffectstk;//nb effect dans la chaine 
  int paramnb;    //parametre associé
};

//####################################################################
//####################################################################
//####################################################################
  enum{ACTION_C_SET =0,ACTION_C_PLUS,ACTION_C_MOINS,ACTION_N_ON_OFF,ACTION_N_TOGGLE};
class CControleurStk
{
public:
  CControleurStk();
  CControleurStk(CControleurStk & controleur);
  void operator=(CControleurStk & controleur);
  void ViewControleur(CListCtrl & lst,int pos = 0);
  long midi_channel;     //cannal midi
  long midi_controleur;  //controleur ou note
  long action;           //0 = controleur set 
                         //1 = controleur + 
                         //2 = controleur -
                         //3 = note on/off
                         //4 = note toggle
  long value1;           //depend de action
  bool invert;           //inverse les valeurs en entrée
  long min;              //minimum value
  long max;              //maximum value
  long controleur_value; //valeur du controleur
  CArray <CControleurParam,CControleurParam &> m_lstparam[127];
};


//####################################################################
//####################################################################
//####################################################################
struct VstEvent;
class CAppPointer;

class CControleurLst
{
public:
  CControleurLst(void);
  ~CControleurLst(void);
  void Init(int nbcontroleur);
  void Load();
  void Set_APP(CAppPointer * m_app){APP = m_app;}
  void Save();
  CControleurStk * Get(int n);
  void ProcessEvent(VstEvent * event);
  void ViewControleur(CListCtrl & lst);
  void Add(int chaine,int index);
  void Suppr(int chaine,int index);
  CArray <CControleurStk, CControleurStk&  > m_lstcontroleur;
protected:
  CAppPointer * APP;
  int nb_controleur;
};
