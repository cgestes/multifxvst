//------------------------------------------------------//
//- CopyLeft : CTAF
//-
//- CEffectStk     : stock des données sur un plug
//- CStockEffetLst : stock 32 chaines de plug-ins et les gere (utilise CVSThost)
//- TODO :            
//-                                                   
//------------------------------------------------------//

//gestion d'une chaine de plugins (plusieurs chaines peuveut utiliser le mm plug-ins)
#ifndef __stockeffect__
#define __stockeffect__

class CAppPointer;
#include "Tafadeinout.h"

#pragma once
#define MAX_CHAINE 128

enum fadetype{FD_NOTHING = 0,FD_CHTOFADEOUT ,FD_FADEOUT,FD_CHTOFADEIN,FD_FADEIN};


class CEffect;
class CTAFadeInOut;
//stock des info sur un plug-ins
class CEffectStk
{
public :

	CEffectStk();
  ~CEffectStk();
	CEffectStk(CEffectStk & eff);

  //on sevegarde toujours dans la derniere version..
	void save(CArchive &ar);

	void load(CArchive &ar,int version);

  void loadFromMem(CEffect *ceff);
  void saveToMem(CEffect *ceff);

  int Get_Controleur(int nb);
  
  void Set_Controleur(int nb,int val,CEffect *ceff);


	int effect_nb;       //numero de l'effet dans CVSTHost
  int effectbisnb;     //numero d'instance de l'effet (si plusieurs fois le mm effet dans une chaine)
	CString effect_name; //path et nom de la dll
	CString bankname;    //path et nom de la bank d'effet utilis?
  

  float  * tabval;          //tableau de valeur des parametres
  bool   ischunk;          //sioui
  char   * chunk;         //chunk du plug-ins
  char   * tabcontroleur;//tableau contenant pour chaque parametre le controleur associé

  long   length; //taille du chunk


  CTAFadeInOut fader;//classe gérant le fade
  
  bool   bypass;
  //float  bypass_fade_value; //0 -> 1
  //bool   fade;
  //int    nbcontroleur;
};


struct TD_Arg
{
  CAppPointer * APP;
  CString     * Param;
};

//changement de chaine
UINT TD_ChangeChain(LPVOID Param) ;

UINT TDLoadAll(LPVOID Param) ;






//#include
class CCVSTHost;
class CAppPointer;
#define VCH(i) ((i>=0) && (i<MAX_CHAINE))
//gere une liste de plug-ins (manage un CVSTHost)
class CStockEffetLst
{
public:

  CStockEffetLst();
	~CStockEffetLst();
  void Set(CAppPointer * m_app);

  //ajoute un effet si l'effet existe,il le réutilise
	int add_eff(int chaine,LPCSTR dll_name);

  //suprime un effet 
  //killing_plug = true => le pointeur sur APP devient faut (le plug est fermé)
  //ne faire que si c'est pour détruire MultifxVST (enfin une instance)
	void suppr_eff(int chaine,int nb,bool killing_plug = false);

  //return effect nb
  //erreur = -1
  int find_eff(int chaine,LPCSTR dllname,int effectbisnb = 1);

  //recherche a partir de nbeffet
  int find_eff(int chaine,int nbeffect);

	//sauve  une liste d'effet
	void save(CArchive &ar);
	
  //retourne la chaine à activer
  int load(CArchive &ar);

  	//sauve  une liste d'effet
	void save_chaine(int chaine,CArchive &ar);
	void load_chaine(int chaine,CArchive &ar);

  //recuppere le nom d'un effet
  CString Get_Name(int chaine,int nb);

  //supprime 
  void suppr_chaine(int chaine,bool kiling_effect = false);

  //copie deux chaine
  void copie_chaine(int ch_from,int ch_to,bool kiling_effect = false);

  //affiche le contenu d'une chaine dans un listbox
  //selectionne nbsel
  void ViewChaine(int chaine,CListCtrl & lst,int nbsel);

  //change la position d'un effet
  void Up_eff(int chaine,int index);
  void Down_eff(int chaine,int index);


	CEffectStk * get(int chaine,int i);

  CEffectStk * get(int chaine,LPCSTR fname); //NULL si erreur

	//return le numero réel de l'effet
  //-1 si erreur
	int get_effect(int chaine,int i);

	//nombre d'eefet dans cette chaine d'effet
	int get_count(int chaine);

  //###VST Implementation###
	//fait un process de la chaine
	void process(int chaine,float **inputs, float **outputs, long sampleFrames,bool replace=false);
  //return false si il n'a rien fait
  bool ProcessEffect(CEffectStk * effst,float **inputs, float **outputs, long sampleFrames);
  //fait un fondu de afondre avec source dans dest 
  //void FonduBuffer(float ** dest,float ** source,float ** afondre,long size,float start,float end);
	

  //demande un changement de chaine
  //=> fadeout 
  //=> demande affichage changement chaine + changement de chaine effectif
  //=> dry tant que pa fini changé
  //=> fadein
  void ChangeChaine(int from,int to);
  
  void processReplace(int chaine,float **inputs, float **outputs, long sampleFrames);
  void suspend(int chaine);
  void resume(int chaine);
/*
  void startProcess(){m_processingcalled = TRUE;};	// Called one time before the start of process call
  void stopProcess(){m_processingcalled = FALSE;};	// Called after the stop of process call
*/
  void SetBlockSize(long size);
  void SetSampleRate(float size);
  long CalculDelay(int chaine);

  int InitDelay; //correspond a initialdelay
  void IoChanged(int chaine);


  

  void SetByPass(int chaine,int nbeffstk,bool bypass);
  bool GetByPass(int chaine,int nbeffstk);

  //###VST Implementation###

  //pour changer de chaine et se souvenir des param des plugs
  void SaveParamsToMem(int chaine);
  void LoadParamsFromMem(int chaine);


  //supprime une chaine
  //si killing_plug = true le decrease_use du plug-ins recherche une autre APP
  //si il est associer a celle associer ac nous (les effets ne perdent jamais
  //leur pointeur sur APP comme ca)
  //ne faire oui que si notre instance du plug va être killé
  void RemoveAll(bool killing_plug = false);

  //supprime tout!
  void RemoveAt(int chaine,bool killing_plug = false);

  int  GetNextChaine(){return newchaine;}//fadechainenext;}
  void ResetNextChaine(){CS_newchaine.Lock(); newchaine = -1;CS_newchaine.Unlock(); }
  void SetChangingFlag(bool changing){m_changing_chain = changing;};
  volatile BOOL m_processing; //en mode resume ou suspend??? 
  int  nb_effect_used;

protected:
	int add_eff(int chaine,CEffectStk & effstk);//ajoute un effet
  int add_effload(int chaine,CEffectStk & effstk);//ajoute un effet
  
  //cherche si un effet ce trouve dans une chaine autre que <chaine>
  int find_eff_not_in(int chaine,LPCSTR dll_name,int effectbisnb = 1);

  //cherche le nombrer de fois que le plugins existe dans une chaine et ajoute 1...
  int find_effbisnb(int chaine,LPCSTR dllname);



  CCVSTHost * host;
  CList <CEffectStk *,CEffectStk *> lst_stock[MAX_CHAINE];
  CAppPointer * APP;
  float ** processbuffer; 
  float ** processreplacebuffer;
  float ** fadebuffer;


  volatile int newchaine,fadechaineanc,fadechainenext;
  float increment; // increment pour le fade (calculer a partir du samplerate)
  
  volatile bool m_changing_chain;


  CEvent m_fadeoutevent;  //synchro entre process et la thread de changment de chaine


  BOOL m_workingthread;   //est ce qu'il y a une thread qui change de chaine

  //int            current_chaine;
public:
  CTAFadeInOut fader;
  void SetWorkingThread(BOOL working){m_workingthread = working;}
  CCriticalSection CS_Processing; //protege resume/suspend/process
  CCriticalSection CS_fade;  //protege fadestate

  CCriticalSection CS_ChangingChain; //protege les threads de changement de chaines

  CCriticalSection CS_newchaine;//protege la variable newchaine
  volatile fadetype  fadestate;


};


#endif