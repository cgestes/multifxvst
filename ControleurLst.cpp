#include "stdafx.h"
#include "multifxvst.h"
#include ".\controleurlst.h"


void CControleurParam::AddParam(int nbeffstk,int nparam)
{
  DWORD val = ((nparam<<16)>16) + (nbeffstk << 16);
  m_param.Add(val);
}

void CControleurParam::SupprParam(int nbeffstk,int nparam)
{
  DWORD val = (nparam<<16 >>16) + (nbeffstk << 16);
  int i,j = m_param.GetCount();
  for(i = 0;i<j;i++)
  {
    if(m_param[i] == val)
      m_param.RemoveAt(i);
  }

}


void CParameterLst::Init(int nbcontroleur)
{
  nb_controleur = nbcontroleur;
  controleur_value.SetSize(nbcontroleur);
}

void CParameterLst::setParameter(long index, float value)
{
  int val = int( value *127);
  controleur_value[index]->Set(val);
}

//-------#############################################-------//
//-------#############################################-------//
//-------#############################################-------//
CString long2Action(long action)
{
  CString retval;
  switch(action)
  {
    case ACTION_C_SET: retval = "Controler Set";break;
    case ACTION_C_PM: retval = "Controler +/-";break;
    case ACTION_C_TOGGLE: retval = "Controler Button";break;
    case ACTION_N_SET: retval = "Note Set";break;
    case ACTION_N_PM: retval = "Note +/-";break;
    case ACTION_N_TOGGLE: retval = "Note Toggle";break;
    case ACTION_PGCH: retval = "Program Change";break;
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
   midi_channel    =0;   //cannal midi
   midi_controleur =0;//controleur ou note
   action          =0;         
   value1          =0;         //depend de action
   invert          =0;         //inverse les valeurs en entrée
   controleurnb    =0;     //numero du controleur
}



//affiche l'ensemble des controleurs dans une liste
void CControleurStk::ViewControleur(CListCtrl & lst,int pos)
{
  CString buf;
  buf.Format("%d",controleurnb);
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
  //m_lstcontroleur.SetSize(nbcontroleur);
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
int CControleurLst::Add(CControleurStk & CParam)
{
  return m_lstcontroleur.Add(CParam);

}

void CControleurLst::Suppr(int index)
{
  m_lstcontroleur.RemoveAt(index);
  return;
}