#include "stdafx.h"
#include "multifxvst.h"
#include ".\controleurlst.h"
//-------#############################################-------//
//-------#############################################-------//
//-------#############################################-------//
CString long2Action(long action)
{
  CString retval;
  switch(action)
  {
    case ACTION_C_SET: retval = "Controleur Set";break;
    case ACTION_C_PLUS: retval = "Controleur Plus";break;
    case ACTION_C_MOINS: retval = "Controleur Moins";break;
    case ACTION_N_ON_OFF: retval = "Note On/Off";break;
    case ACTION_N_TOGGLE: retval = "Note Toggle";break;
    default: retval = "Ouhla!";break;
  }
  return retval;
}

CString long2Channel(long channel)
{
  CString retval;
  if(channel == 0)
    retval = "ALL";
  else if(channel < 17)
    retval.Format("%d",channel);
  else
    retval = "Valeur Impossible!";

  return retval;
}

//-------#############################################-------//
//-------#############################################-------//
//-------#############################################-------//
CControleurStk::CControleurStk()
{
  long midi_channel = 0;
  long midi_controleur = 0;
  long action = 0;
  long value1 = 0;
  bool invert = 0;
  long min = 0;
  long max = 0;
  long controleur_value = 0;
}

CControleurStk::CControleurStk(CControleurStk & controleur)
{
  midi_channel = controleur.midi_channel;
  midi_controleur = controleur.midi_controleur;
  action = controleur.action;
  value1 = controleur.value1;
  invert = controleur.invert;
  min = controleur.min;
  max = controleur.max;
  controleur_value = controleur.controleur_value;
}

//affiche l'ensemble des controleurs dans une liste
void CControleurStk::ViewControleur(CListCtrl & lst,int pos)
{

  CString buf;
  buf.Format("%d",pos);
  int nItem = lst.InsertItem(pos,buf);
  buf = long2Channel(midi_channel);
  lst.SetItemText(pos,1,buf);
  buf.Format("%d",midi_controleur);
  lst.SetItemText(pos,2,buf);
  buf = long2Action(action);
  lst.SetItemText(pos,3,buf);
  buf.Format("%d",value1);
  lst.SetItemText(pos,4,buf);
  buf.Format("%d",invert);
  lst.SetItemText(pos,5,buf);
  buf.Format("%d",min);
  lst.SetItemText(pos,6,buf);
  buf.Format("%d",max);
  lst.SetItemText(pos,7,buf);
  buf.Format("%d",controleur_value);
  lst.SetItemText(pos,8,buf);


}

void CControleurStk::operator=(CControleurStk & controleur)
{

}

//-------#############################################-------//
//-------#############################################-------//
//-------#############################################-------//
//vrai si le nb est un controeur valide
#define VCC(nb) ((nb>=0)&&(nb<nb_controleur))

CControleurLst::CControleurLst(void)
{
  nb_controleur=0;
}

CControleurLst::~CControleurLst(void)
{

}
void CControleurLst::Init(int nbcontroleur)
{
  m_lstcontroleur.SetSize(nbcontroleur);
  nb_controleur = nbcontroleur;
}

void CControleurLst::Load()
{
  return;
}

void CControleurLst::Save()
{
  return;
}

void CControleurLst::ProcessEvent(VstEvent * event)
{
  return;
}

//affiche l'ensemble des controleurs dans une liste
void CControleurLst::ViewControleur(CListCtrl & lst,int nbsel)
{
  lst.SetRedraw(FALSE);

  lst.DeleteAllItems();

  int i,j = m_lstcontroleur.GetCount(); 

  if(nbsel>(j-1))
    nbsel = j-1;

  if(nbsel<0)
    nbsel = 0;

  for (i=0;i<j;i++)
  {
    m_lstcontroleur[i].ViewControleur(lst,i);
  }

  lst.EnsureVisible(nbsel,TRUE);
  lst.SetItemState(nbsel, LVIS_SELECTED, LVIS_SELECTED);

  lst.SetRedraw(TRUE);

}

CControleurStk * CControleurLst::Get(int n)
{
  ASSERT(VCC(n));
  return &m_lstcontroleur[n];

}
void CControleurLst::Add(int chaine,int index)
{
  return;
}

void CControleurLst::Suppr(int chaine,int index)
{
  return;
}