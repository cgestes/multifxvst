//Copyleft CTAF
#include "stdafx.h"

#include "multifxVST.h"
#include "stockeffet.h"
#include "vstHost/SmpEffect.h"
#include "CCVSThost.h"
#include "multifxvsteditor.h"

#include "ControleurLst.h"
#include "TAFadeInOut.h"

//##############################################################################
//stock des info sur un plug-ins
//class CEffectStk

CEffectStk::CEffectStk()
{
  effect_nb = 0;
  effect_name = _T("");
  effectbisnb = 1;
  bankname = _T("");
  tabval = NULL;
  tabcontroleur = NULL;
  chunk = NULL;
  ischunk = false;
  length = 0;
  
  bypass = false;
  //bypass_fade_value = 1.0; //0 -> 1
  //fade = false;

}
int CEffectStk::Get_Controleur(int nb){
  if((tabcontroleur))
  {
    if((nb<0) || (nb>tabcontroleur[0]))
      return -1;
    else
      return tabcontroleur[nb+1];
  }
  else
    return -1;

}

void CEffectStk::Set_Controleur(int nb,int val,CEffect *ceff)
{
  if(!tabcontroleur)
  {
    int i,j = ceff->pEffect->numParams;
    tabcontroleur = new char[j + 1];
    for(i = 0;i<j;i++)
      tabcontroleur[i+1] = -1; 
    tabcontroleur[0] = j;
  }
  tabcontroleur[nb + 1] = val;
}

CEffectStk::~CEffectStk()
{
  if(tabval)delete tabval;
  if(chunk)delete chunk;
  if(tabcontroleur)delete tabcontroleur;
  TRACE("DESTROY::CEffectStk\n");
}

CEffectStk::CEffectStk(CEffectStk & eff)
{
  tabval = NULL;
  tabcontroleur = NULL;
  chunk = NULL;
  effect_nb = eff.effect_nb;
  effect_name = eff.effect_name;
  effectbisnb = eff.effectbisnb;
  bankname = eff.bankname;
  ischunk = eff.ischunk;
  length = eff.length;

  bypass= eff.bypass;
  //bypass_fade_value= eff.bypass_fade_value; //0 -> 1
  //fade= eff.fade;
  fader = eff.fader;


  if(eff.tabcontroleur)
  {
    tabcontroleur = new char[(int)eff.tabcontroleur[0]+1];

    for(int i =0; i<=eff.tabcontroleur[0];i++)
      tabcontroleur[i] = eff.tabcontroleur[i];
  } 

  if(eff.ischunk)
  {
    if(eff.chunk)
    {
      chunk = new char[eff.length];
      memcpy(chunk,eff.chunk,eff.length);
    }
  }
  else
  {
    if(eff.tabval)
    {
      tabval = new float[(int)eff.tabval[0]+1];

      for(int i =0; i<=eff.tabval[0];i++)
        tabval[i] = eff.tabval[i];
    }  
  }
}
void CEffectStk::loadFromMem(CEffect * ceff)
{
  if(ceff->pEffect->flags & effFlagsProgramChunks)
  {
    if(chunk)
    {
      ceff->EffSetChunk(chunk,length,true);
    }
  }
  else
  {
    if(tabval)
    {
    int k,l = (int)tabval[0];
    for( k = 0; k < l;k++)
      ceff->EffSetParameter(k,tabval[k+1]);
    }
  }
}


void CEffectStk::saveToMem(CEffect * ceff)
{
  if(ceff->pEffect->flags & effFlagsProgramChunks)
  {
        if(tabval)
          delete tabval;

        tabval = NULL;

        void * chunke = NULL;
        long lg = ceff->EffGetChunk(&chunke,true);
        if(lg != length)
        {
          if( chunk )
            delete chunk;
          chunk = NULL;
        }

        if(!chunk)
          chunk = new char [lg];

        ASSERT(chunk);
        memcpy(chunk,chunke,lg);
        length = lg;
        ischunk = true;

  }
  else
  {
        int k,l = ceff->pEffect->numParams;

        //par sécurité on kill les chunk si yen a
        if(chunk)
          delete chunk;

        chunk = NULL;
        length = 0;
        ischunk = false;


        //si c'est pas le bon tableau on kill
        if(tabval && tabval[0] != l)
        {delete [] tabval;
        tabval = NULL;
        }

        //si pas de tableau
        if(!tabval)
        {tabval = new float[l+1];
        tabval[0] = (float)l;
        }

        //on sauvegarde l'effet
        for( k = 0; k < l;k++)
          tabval[k+1] = ceff->EffGetParameter(k);
  }
}


void CEffectStk::save(CArchive &ar)
{
  //ar << effect_nb;
  ar << effect_name;
  ar << bankname;
  ar << ischunk;
  ar << length;
  char c = 0;
  //**7.0.3**//deb
  if(!tabcontroleur)
      ar << c;
  else
  {
    ar << tabcontroleur[0];
    for(int i = 1;i<= tabcontroleur[0]; i++)
      ar << tabcontroleur[i];
  }
  //**7.0.3**//end

  if(ischunk)
  {
    ar.Write(chunk,length);
  }
  else
  {
    if(!tabval)
      ar << 0;
    else
    {
    ar << tabval[0];
    for(int i = 1;i<= tabval[0]; i++)
      ar << tabval[i];
    }
  }




}
void CEffectStk::load(CArchive &ar,int version)
{
  //ar >> effect_nb;

  ar >> effect_name;
  ar >> bankname;
  ar >> ischunk;
  ar >> length;

   //**7.0.3**//deb
  //if(tabcontroleur)
  //{
    char nbval;
    ar >> nbval;
    if(tabcontroleur)
    {delete [] tabcontroleur;
    tabcontroleur = NULL;}
    if(nbval > 0)
    {
      tabcontroleur = new char[(int)nbval+1];
      tabcontroleur[0] = nbval;
      for(int i =1;i<=(int)nbval;i++)
      {
        ar >> tabcontroleur[i];
      }
    }
  //}
  //**7.0.3**//end

  if(ischunk)
  {
    chunk = new char[length];
    ar.Read(chunk,length);
    tabval = NULL;
  }
  else
  {
    chunk = NULL;
    float nbval;
    ar >> nbval;
    if(tabval)
    {delete [] tabval;
    tabval = NULL;}
    if(nbval > 0)
    {
      tabval = new float[(int)nbval+1];
      tabval[0] = nbval;
      for(int i =1;i<=(int)nbval;i++)
      {
        ar >> tabval[i];
      }
    }
  }
 
}



//worker thread pour le changement de chaine
UINT TDLoadAll(LPVOID Param) 
{

  CAppPointer * APP = (CAppPointer *)Param;
  //une seule thread de changement de chaine a la fois
  APP->chaine_eff->CS_ChangingChain.Lock();


  //########################### FD_CHTOFADEOUT ###################
  //on recup la chaine a traiter
  APP->chaine_eff->CS_newchaine.Lock();
  int futur_chaine = APP->chaine_eff->GetNextChaine();
  APP->chaine_eff->ResetNextChaine();
  APP->chaine_eff->CS_newchaine.Unlock();

  //rien a faire on se casse...
  if(futur_chaine == -1 || futur_chaine == APP->current_chaine)
  {
    APP->chaine_eff->CS_fade.Lock();
    APP->chaine_eff->fadestate = FD_NOTHING;
    APP->chaine_eff->CS_fade.Unlock();
    APP->chaine_eff->SetWorkingThread(FALSE);
    return FALSE;
  }


  //on reset l'evenement
  APP->m_waitfade->ResetEvent();



  //########################### FD_FADEOUT #######################
  //on lui di de faire un fadeout
  APP->chaine_eff->fader.SetFadeOut();
  APP->chaine_eff->CS_fade.Lock();
  APP->chaine_eff->fadestate = FD_FADEOUT;
  APP->chaine_eff->CS_fade.Unlock();

  //on attend 1sec max que process ait fini le fadeout
  APP->m_waitfade->Lock(1000);


  APP->chaine_eff->CS_newchaine.Lock();
  int futur_chaine2 = APP->chaine_eff->GetNextChaine();
  APP->chaine_eff->ResetNextChaine();
  APP->chaine_eff->CS_newchaine.Unlock();

  //rien a faire on se casse...
  if(futur_chaine2 != -1)
  {
    futur_chaine = futur_chaine2;
  }

  //########################### FD_CHTOFADEIN ####################
  if(futur_chaine != APP->current_chaine)
  {
      APP->chaine_eff->CS_fade.Lock();
      APP->chaine_eff->fadestate = FD_CHTOFADEIN;
      APP->chaine_eff->CS_fade.Unlock();

      BOOL b =APP->chaine_eff->m_processing;
      //on arrete de les plugins
      //APP->chaine_eff->SetChangingFlag(true);
      if(b)
        APP->chaine_eff->suspend(APP->current_chaine);

      //on sauvegarde les parametres de l'anciens chaine (de cq plug)
      APP->chaine_eff->SaveParamsToMem(APP->current_chaine);

      APP->chaine_eff->CS_fade.Lock();
      APP->current_chaine = futur_chaine;
      APP->chaine_eff->CS_fade.Unlock();

      //on met a jours l'affichage graphique
      if(APP->editor)
      {
        //fait le travail pendant l'idle
        APP->editor->setParameter(0,NBChaine2float(APP->current_chaine));
      }

      //on charge les params de l'autre chaine
      APP->chaine_eff->LoadParamsFromMem(APP->current_chaine);

      APP->chaine_eff->IoChanged(APP->current_chaine);
      //on lance les autres
      if(b)
        APP->chaine_eff->resume(APP->current_chaine);
  }

  //on signale a process que le changement est effectif
  APP->chaine_eff->SetChangingFlag(false);


  //########################### FD_FADEIN ########################
  APP->chaine_eff->CS_fade.Lock();
  APP->chaine_eff->fadestate = FD_FADEIN;
  APP->chaine_eff->CS_fade.Unlock();

  APP->chaine_eff->fader.SetFadeIn();


  // en dernier point on verifie si ya aps de demande de changement de chaine
  APP->chaine_eff->CS_newchaine.Lock();
  futur_chaine2 = APP->chaine_eff->GetNextChaine();
  APP->chaine_eff->ResetNextChaine();
  APP->chaine_eff->CS_newchaine.Unlock();

  //on signale que nous ne travaillons plus
  APP->chaine_eff->SetWorkingThread(FALSE);

  //rien a faire on se casse...
  if(futur_chaine2 != -1)
  {
    APP->chaine_eff->ChangeChaine(APP->current_chaine,futur_chaine2);
  }



  //une seule thread de changement de chaine a la fois
  APP->chaine_eff->CS_ChangingChain.Unlock();


  return false;//default
}

















//##############################################################################

//worker thread pour le changement de chaine
UINT TD_ChangeChain(LPVOID Param) 
{
  CAppPointer * APP = (CAppPointer *)Param;
  //une seule thread de changement de chaine a la fois
  APP->chaine_eff->CS_ChangingChain.Lock();


  //########################### FD_CHTOFADEOUT ###################
  //on recup la chaine a traiter
  APP->chaine_eff->CS_newchaine.Lock();
  int futur_chaine = APP->chaine_eff->GetNextChaine();
  APP->chaine_eff->ResetNextChaine();
  APP->chaine_eff->CS_newchaine.Unlock();

  //rien a faire on se casse...
  if(futur_chaine == -1 || futur_chaine == APP->current_chaine)
  {
    APP->chaine_eff->CS_fade.Lock();
    APP->chaine_eff->fadestate = FD_NOTHING;
    APP->chaine_eff->CS_fade.Unlock();
    APP->chaine_eff->SetWorkingThread(FALSE);
    return FALSE;
  }


  //on reset l'evenement
  APP->m_waitfade->ResetEvent();



  //########################### FD_FADEOUT #######################
  //on lui di de faire un fadeout
  APP->chaine_eff->fader.SetFadeOut();
  APP->chaine_eff->CS_fade.Lock();
  APP->chaine_eff->fadestate = FD_FADEOUT;
  APP->chaine_eff->CS_fade.Unlock();

  //on attend 1sec max que process ait fini le fadeout
  BOOL retval = APP->m_waitfade->Lock(500);


  APP->chaine_eff->CS_newchaine.Lock();
  int futur_chaine2 = APP->chaine_eff->GetNextChaine();
  APP->chaine_eff->ResetNextChaine();
  APP->chaine_eff->CS_newchaine.Unlock();

  //rien a faire on se casse...
  if(futur_chaine2 != -1)
  {
    futur_chaine = futur_chaine2;
  }

  //########################### FD_CHTOFADEIN ####################
  if(futur_chaine != APP->current_chaine)
  {
      APP->chaine_eff->CS_fade.Lock();
      APP->chaine_eff->fadestate = FD_CHTOFADEIN;
      APP->chaine_eff->CS_fade.Unlock();

      BOOL b =APP->chaine_eff->m_processing;
      //on arrete de les plugins
      //APP->chaine_eff->SetChangingFlag(true);
      if(b)
        APP->chaine_eff->suspend(APP->current_chaine);

      //on sauvegarde les parametres de l'anciens chaine (de cq plug)
      APP->chaine_eff->SaveParamsToMem(APP->current_chaine);

      APP->chaine_eff->CS_fade.Lock();
      APP->current_chaine = futur_chaine;
      APP->chaine_eff->CS_fade.Unlock();

      //on met a jours l'affichage graphique
      if(APP->editor)
      {
        //fait le travail pendant l'idle
        APP->editor->setParameter(0,NBChaine2float(APP->current_chaine));
      }

      //on charge les params de l'autre chaine
      APP->chaine_eff->LoadParamsFromMem(APP->current_chaine);

      APP->chaine_eff->IoChanged(APP->current_chaine);
      //on lance les autres
      if(b)
        APP->chaine_eff->resume(APP->current_chaine);
  }

  //on signale a process que le changement est effectif
  APP->chaine_eff->SetChangingFlag(false);


  //########################### FD_FADEIN ########################
  APP->chaine_eff->CS_fade.Lock();
  APP->chaine_eff->fadestate = FD_FADEIN;
  APP->chaine_eff->CS_fade.Unlock();

  APP->chaine_eff->fader.SetFadeIn();


  // en dernier point on verifie si ya aps de demande de changement de chaine
  APP->chaine_eff->CS_newchaine.Lock();
  futur_chaine2 = APP->chaine_eff->GetNextChaine();
  APP->chaine_eff->ResetNextChaine();
  APP->chaine_eff->CS_newchaine.Unlock();

  //on signale que nous ne travaillons plus
  APP->chaine_eff->SetWorkingThread(FALSE);

  //rien a faire on se casse...
  if(futur_chaine2 != -1)
  {
    APP->chaine_eff->ChangeChaine(APP->current_chaine,futur_chaine2);
  }



  //une seule thread de changement de chaine a la fois
  APP->chaine_eff->CS_ChangingChain.Unlock();


  return false;//default
}




//##############################################################################



//#include "vsthost/cvsthost.h"
CStockEffetLst::CStockEffetLst()
{
  processbuffer = NULL;
  processreplacebuffer = NULL;
  fadebuffer = NULL;
  m_processing = FALSE;
  host = NULL;
  APP = NULL;
  nb_effect_used = 0;
  newchaine = -1;
  fadechaineanc= 0;
  fadechainenext= 0;
  fadestate = FD_NOTHING;
  m_changing_chain = false;
//  m_processingcalled = TRUE;
  InitDelay = 0;
  m_workingthread = FALSE;
}//constructeur


void CStockEffetLst::SaveParamsToMem(int chaine)
{
  CEffectStk * eff;
  CEffect *ceff;
  int i,j = get_count(chaine);
  for(i=0;i<j;i++)
  {
    eff = get(chaine,i);
    if(eff)
    {
      ceff = host->GetAt(eff->effect_nb);
      eff->saveToMem(ceff);
    }
  }
}

void CStockEffetLst::LoadParamsFromMem(int chaine)
{
  CEffectStk * eff;
  CEffect *ceff;
  APP->parameter->DeleteAllItems();
  int i,j = get_count(chaine);
  for(i=0;i<j;i++)
  {
    eff = get(chaine,i);
    if(eff)
    {
      ceff = host->GetAt(eff->effect_nb);
      eff->loadFromMem(ceff);
      if(eff->tabcontroleur)
       for(int k=0;k<ceff->pEffect->numParams;k++)
       {
         int controleur = eff->Get_Controleur(k);
         if(controleur != -1)
           APP->parameter->ParamAddParam(controleur-1,i,k);
       }
    }
  }
}

CString CStockEffetLst::Get_Name(int chaine,int nb)
{
    CEffect * eff ;
    CEffectStk  * eff2; 
    CString sFile = "";
    eff2 = get(chaine,nb);
    ASSERT(eff2);
    //recup le num de l'effet
    int nbc = eff2->effect_nb;
    if(nbc < 0)return "";

    eff = (CEffect *)host->GetAt(nbc);
    ASSERT(eff);
    if(eff->EffGetVstVersion() >= 2000)
    {
    if (!eff->EffGetProductString(sFile.GetBuffer(256)))/* if V2 plugin                      */
      { sFile.ReleaseBuffer();
        sFile = "";                         /* use plugin info                   */
      }else
        sFile.ReleaseBuffer();
    } 

      if(sFile.GetLength()==0)                                  /* if V1 plugin                      */
      {
        sFile = eff->sName;
        int nrbsl  = sFile.ReverseFind('\\');
        int nrbsl2 = sFile.ReverseFind('.');
        sFile = sFile.Mid(nrbsl + 1,(nrbsl2 - nrbsl -1>0?nrbsl2 - nrbsl-1:0));    /* use plugin file name              */
      }


    sFile.AppendFormat("[%d]",eff2->effectbisnb);

    return sFile;


}

void CStockEffetLst::ViewChaine(int chaine,CListCtrl & lst,int nbsel)
{
  CString nb;
  CEffectStk  * eff;
  if(!VCH(chaine))return ;

  lst.SetRedraw(FALSE);

  lst.DeleteAllItems();

  CString sFile;


	int i=0,j = get_count(chaine);
  

  
  if(nbsel>(j-1))
    nbsel = j-1;

  if(nbsel<0)
    nbsel = 0;

	for (i = 0;i<j;i++)
	{
    sFile = Get_Name(chaine,i);
    eff = get(chaine,i);
    ASSERT(eff);
    //lst.AddString(sFile);
    int nItem =0;
    nb = "";
	  nItem = lst.InsertItem(lst.GetItemCount(),sFile);
    nb.Format("%d", eff->effect_nb);
    lst.SetItemText(nItem,1,nb);
      nb.Format("%d", eff->effectbisnb);
    lst.SetItemText(nItem,2,nb);
      nb.Format("%d", i);
    lst.SetItemText(nItem,3,nb);
	}
  lst.EnsureVisible(nbsel,TRUE);
  lst.SetItemState(nbsel, LVIS_SELECTED, LVIS_SELECTED);


  lst.SetRedraw(TRUE);

}

//supprime tous les objects
CStockEffetLst::~CStockEffetLst()
{
  RemoveAll(true);
  //on supprime nos buffers
  if(processbuffer)
  {
    delete [] processbuffer[0];
    delete [] processbuffer[1];
    delete [] processbuffer;
  }
  if(processreplacebuffer)
  {
    delete [] processreplacebuffer[0];
    delete [] processreplacebuffer[1];
    delete [] processreplacebuffer;
  }
  if(fadebuffer)
  {
    delete [] fadebuffer[0];
    delete [] fadebuffer[1];
    delete [] fadebuffer;
  }
  CString buf;buf.Format("DESTROY :: CStockEffetLst(%d) \n", this);  TRACE(buf);

}

void CStockEffetLst::RemoveAll(bool killing_plug)
{
  for (int k = 0;k < MAX_CHAINE;k++)
  {
	  RemoveAt(k,killing_plug);
  }
}

void CStockEffetLst::RemoveAt(int chaine,bool killing_plug)
{
	  int i=0,j = get_count(chaine);
	  for (i = j-1;i>=0;i--)
	  {
		  suppr_eff(chaine,i,killing_plug);
	  }
}

//fait monter un effet dans la chaine (décrémente son nb)
void CStockEffetLst::Up_eff(int chaine,int index)
{
  int c = get_count(chaine);
  CEffectStk * eff1;
  CEffectStk * eff2;
  POSITION pos1,pos2;

  if((index < 1) || (index>c))return;

  pos1 = lst_stock[chaine].FindIndex(index);
  pos2 = lst_stock[chaine].FindIndex(index - 1);

  eff1 = lst_stock[chaine].GetAt(pos1);
  eff2 = lst_stock[chaine].GetAt(pos2);

  lst_stock[chaine].SetAt(pos1,eff2);
  lst_stock[chaine].SetAt(pos2,eff1);
}

void CStockEffetLst::Down_eff(int chaine,int index)
{
  ASSERT(VCH(chaine));
  int c = get_count(chaine);
  CEffectStk * eff1;
  CEffectStk * eff2;
  POSITION pos1,pos2;


  if((index < 0) || (index>=(c-1)))return;
  pos1 = lst_stock[chaine].FindIndex(index);
  pos2 = lst_stock[chaine].FindIndex(index + 1);

  eff1 = lst_stock[chaine].GetAt(pos1);
  eff2 = lst_stock[chaine].GetAt(pos2);

  lst_stock[chaine].SetAt(pos1,eff2);
  lst_stock[chaine].SetAt(pos2,eff1);
}

//cherche un effet dans les autre chaines que <chaine>
//si trouvé retourne nb sinon -1
//effectbisnb = num d'instance de l'effet (si ya plusieur eff)
int CStockEffetLst::find_eff_not_in(int chaine,LPCSTR dll_name,int effectbisnb)
{
  ASSERT(VCH(chaine));

  int retval = -1;
  for (int i = 0;i < MAX_CHAINE;i ++)
  {
    if(i == chaine)continue;
    
    retval = find_eff(i,dll_name,effectbisnb);

    if(retval != -1)return retval;


  }

  return -1;
}
//(si l'effet existe deja dans une autre liste,il réutilise,si c'est la mm liste => il crée un nve
//ajoute un effet  (l'effet peut deja exister)
int CStockEffetLst::add_eff(int chaine,LPCSTR dll_name)
{
 // if(find_eff_not_in(int chaine,LPCSTR dll_name)
  ASSERT(VCH(chaine));
  bool Neweff = false;
  CEffectStk eff;
  eff.effectbisnb = find_effbisnb(chaine,dll_name);
  int nb = find_eff_not_in(chaine,dll_name,eff.effectbisnb);
  if(nb == -1)
  {
    nb = host->LoadPlugin(dll_name,APP);
    if(nb<0)
      return -1;

    nb_effect_used++;
    Neweff = true;
  }else
  {
    CSmpEffect * ceff = (CSmpEffect *)host->GetAt(nb);
    ceff->DefParam.loadFromMem(ceff);
  }
  if(nb<0)
    return -1;

  host->IncreaseUse(nb);

  eff.effect_nb = nb;
  eff.effect_name = dll_name;
  eff.tabval = NULL;
  int retval = add_eff(chaine,eff);


  if(Neweff && (retval != -1))
  {
    CSmpEffect * eff2 = (CSmpEffect *)host->GetAt(nb);
    if(eff2)
    {
      //verification de plug (ici?)
      //if(eff2->

      long delay = eff2->pEffect->initialDelay;
      if(delay) //si pas de delay on change rien!
        APP->chaine_eff->IoChanged(APP->current_chaine); //pourkoi réécrire ce qui est deja fait!
    }
  }


  return retval;

}
//ajoute un effet et c'est tout!
int CStockEffetLst::add_eff(int chaine,CEffectStk & effstk)
{
  ASSERT(VCH(chaine));

	CEffectStk * eff = new CEffectStk(effstk);
  if(!eff)return -1;

	lst_stock[chaine].AddTail(eff);
  return lst_stock[chaine].GetCount() - 1;

}


//determine l'instance d'un mm plug a utilisé  WA!
int CStockEffetLst::find_effbisnb(int chaine,LPCSTR dllname)
{
  int nbbis = 1;
  int retval = 1;
  int j = get_count(chaine);

  if( j <= 0)return 1;// pas d'effet donc pas de pb!

  //on est sure de faire de boucle infini comm ca
  for(int i = 0;i<j ; i++)
  {
    //pas trouvé donc on le prend!
    if((retval =find_eff(chaine,dllname,nbbis))== -1)
      return nbbis;

    nbbis++;
  }
  //tous les effect sont pris
  //return nbbis (max)+1 !

  return nbbis;//si on a trouvé ttes les instances prise

 /* CEffectStk * eff;
  int count = 1;
  int j = get_count(chaine);
  for (int i = 0; i< j;i ++)
  {
    eff = get(chaine,i);
		if(eff)
      if(eff->effect_name == dllname)
        count++;
        
  }

  return count;*/

}

int CStockEffetLst::find_eff(int chaine,int nbeffect)
{
  ASSERT(VCH(chaine));
	int i,j = get_count(chaine);
	CEffectStk * eff;

	for (i = 0; i < j;i++)
	{
    eff = get(chaine,i);
		if(eff)
      if( (eff->effect_nb == nbeffect))
        return i/*((CEffectStk *)eff)->effect_nb*/;
	}

  return -1;
}

int CStockEffetLst::find_eff(int chaine,LPCSTR dllname,int effectbisnb)
{
  ASSERT(VCH(chaine));
	int i,j = get_count(chaine);
	CEffectStk * eff;

	for (i = 0; i < j;i++)
	{
    eff = get(chaine,i);
		if(eff)
      if( (eff->effect_name == dllname) && (eff->effectbisnb == effectbisnb))
        return ((CEffectStk *)eff)->effect_nb;
	}

  return -1;
}

//supprime 
void CStockEffetLst::suppr_chaine(int chaine,bool kiling_effect)
{
  RemoveAt(chaine,kiling_effect);
}

//copie deux chaine
void CStockEffetLst::copie_chaine(int ch_from,int ch_to,bool kiling_effect)
{
  CEffectStk * eff;
  int nbeff;
  ASSERT(VCH(ch_from));
  ASSERT(VCH(ch_to));

  //supprime la chaine ch_to
  RemoveAt(ch_to,kiling_effect);
  
  int i,j = get_count(ch_from);
  for(i = 0;i<j;i++)
  {
    eff = get(ch_from,i);
    if(eff)
    {
      nbeff = get_effect(ch_from,i);
      ASSERT(nbeff>=0);
   
      host->IncreaseUse(nbeff);
      add_eff(ch_to,*eff);
    }
  }
  
}

void CStockEffetLst::IoChanged(int chaine)
{
  int NewInitDelay = APP->chaine_eff->CalculDelay(APP->current_chaine);
  if(NewInitDelay != InitDelay) 
  {
    APP->effect->setInitialDelay(InitDelay); //on fix le delay
    APP->effect->ioChanged(); // on dit a l'host de se mettre a jour
    InitDelay = NewInitDelay;
  }
}


void CStockEffetLst::suppr_eff(int chaine,int nbstk,bool killing_plug)
{
	CEffectStk * eff;
  CSmpEffect * eff2;

  ASSERT(VCH(chaine));

  //POSITION p;
	eff = get(chaine,nbstk);
  eff2 = (CSmpEffect *)APP->host->GetAt(eff->effect_nb);
  long delay = eff2->pEffect->initialDelay;

  if(eff)
	{
    lst_stock[chaine].RemoveAt(lst_stock[chaine].FindIndex(nbstk));
    //decrémente le nb d'utilisation de l'effet (si 0 il le delete ts seul)
    if(host->DecreaseUse(eff->effect_nb,killing_plug,APP))
      nb_effect_used--;

    
    if(delay) //si pas de delay on change rien!
        APP->chaine_eff->IoChanged(APP->current_chaine); //pourkoi réécrire ce qui est deja fait!


    delete eff;
	}else 	ASSERT(eff);
}

	//void mod_eff();

	//sauve  une liste d'effet
void CStockEffetLst::save(CArchive &ar)
{
  //SaveParamsToMem(APP->current_chaine);
  ar << CURRENT_VERSION;
  ar << APP->current_chaine;
  for (int k = 0; k < MAX_CHAINE; k++)
  {
	  int i,j = get_count(k);
	  CEffectStk * eff;
    ar << j;
	  for (i = 0; i < j;i++)
	  {
      eff = get(k,i);
		  /*if(eff)*/
      ASSERT(eff);
      eff->save(ar);
	  }
  }
}

//ajoute un effet et le charge
int CStockEffetLst::add_effload(int chaine,CEffectStk & eff)
{
 // if(find_eff_not_in(int chaine,LPCSTR dll_name)
  //CEffectStk eff;
  //CSmpEffect * eff2;

  eff.effectbisnb = find_effbisnb(chaine,eff.effect_name);

  int nb = find_eff_not_in(chaine,eff.effect_name,eff.effectbisnb);
  if(nb == -1)
  {
    nb = host->LoadPlugin(eff.effect_name,APP);
    if(nb<0)
      return -1;
    nb_effect_used++;


  //compensation du delay  on fait la compensation directement dans le load
  /*eff2 = (CSmpEffect *)APP->host->GetAt(nb);
  if(eff2)
	{
    long delay = eff2->pEffect->initialDelay;
    if(delay) //si pas de delay on change rien!
        APP->chaine_eff->IoChanged(APP->current_chaine); //pourkoi réécrire ce qui est deja fait!
  }*/


  }else
  {
    CSmpEffect * ceff = (CSmpEffect *)host->GetAt(nb);
    ceff->DefParam.loadFromMem(ceff);
  }
  if(nb<0)return -1;
  host->IncreaseUse(nb);




  eff.effect_nb = nb;
  int ret = add_eff(chaine,eff);

  //charge les parametres du plugins
  //LoadParams(ret);
  return ret;
}
void CStockEffetLst::save_chaine(int chaine,CArchive &ar)
{
    //SaveParamsToMem(APP->current_chaine);  
    ar << CURRENT_VERSION;

    
	  int i,j = get_count(chaine);
	  CEffectStk * eff;
    ar << j;
	  for (i = 0; i < j;i++)
	  {
      eff = get(chaine,i);
		  /*if(eff)*/
      ASSERT(eff);
      eff->save(ar);
	  }
}

void CStockEffetLst::load_chaine(int chaine,CArchive &ar)
{
    int i =0,j =0;
    int version =0;
    ar >> version;
    ar >> j;
	  for (i = 0; i < j;i++)
	  {
      CEffectStk eff;
		  eff.load(ar,version);
      add_effload(chaine,eff);
	  }
    //compensation du delay
    IoChanged(chaine);
    //LoadParamsFromMem(chaine);
}

void CStockEffetLst::Set(CAppPointer * m_app)
{
  host = m_app->host;
  APP=m_app;
}

//charge une liste d'effet
int CStockEffetLst::load(CArchive &ar)
{
  int version = 0,chainnb = 0;
  ar >> version;
  if(version >= 0x0002)
    ar >> chainnb;

  for (int k = 0; k < MAX_CHAINE; k ++)
  {
    int i =0,j =0;
    ar >> j;
	  for (i = 0; i < j;i++)
	  {
      CEffectStk eff;
		  eff.load(ar,version);
      add_effload(k,eff);
	  }
  }
  //non nécéssaire car ya le changement de chaine qui va le faire...
  IoChanged(chainnb);
  return chainnb;

}

CEffectStk * CStockEffetLst::get(int chaine,int i)
{
  if(!VCH(chaine))return NULL;
	if((i>=0) && (i<get_count(chaine)) /* && lst_stock.FindIndex(i) ??*/)
		return lst_stock[chaine].GetAt(lst_stock[chaine].FindIndex(i));
	else
		return NULL;
}

//CEffectStk * get(LPCSTR fname) //return effect_name = "" si aucun effet trouv�

//nombre d'eefet dans cette chaine d'effet
//-1 si erreur
int CStockEffetLst::get_count(int chaine)
{
  if(!VCH(chaine))return -1;
  return lst_stock[chaine].GetCount();
}

int CStockEffetLst::get_effect(int chaine,int i)
{
  if(!VCH(chaine))return -1;
  CEffectStk * eff = get(chaine,i);
  if(eff)
    return eff->effect_nb;
  else
    return -1;
}

void CStockEffetLst::suspend(int chaine)
{
    if(!VCH(chaine))return ;
    CS_Processing.Lock();
    int i =0,j = get_count(chaine);
    CEffectStk * effst;
    CEffect * eff;
    for (i = 0; i < j;i++)
    {
      if((effst = get(chaine,i))){
        eff = host->GetAt(effst->effect_nb);
        if(eff)
          eff->EffStopProcess();
      }
    }
    m_processing = FALSE;
    CS_Processing.Unlock();
}
void CStockEffetLst::resume(int chaine)
{
    if(!VCH(chaine))return ;
    CS_Processing.Lock();
    int i =0,j = get_count(chaine);
    CEffectStk * effst;
    CEffect * eff;
    for (i = 0; i < j;i++)
    {
      if((effst = get(chaine,i))){
        eff = host->GetAt(effst->effect_nb);
        if(eff)
          eff->EffStartProcess();
      }
    }
    m_processing = TRUE;
    /*APP->chaine_eff->CS_fade.Lock();
    if(!m_workingthread)  // pas de thread en cours
    {
      fadestate = FD_FADEIN;
    }
    APP->chaine_eff->CS_fade.Unlock();

    fadechainenext = chaine;
    fader.SetFadeIn();*/
    CS_Processing.Unlock();
}



//suivant la parité du nombre d'effet on affecte out1 et out2
//de maniere a avoir processbuffer en sortie de chaine a chaque fois et pas processbufferreplace!
//CTAFTACTIQUE!
//on fait des processreplace sur toutes la chaines (comme une chaine en insert)
//si premier effet      input >eff1> out1
//suivant i%2 = 1        out1 >eff2> out2
//suivant i%2 = 0        out2 >eff3> out1
//suivant i%2 = 1        out1 >eff4> out2

//ensuite pour le dernier effet on applique juste un process
//de processbuffer dans outputs!

//CQFD!


void CStockEffetLst::process(int chaine,float **inputs, float **outputs, long sampleFrames,bool replace)
{
    if(!VCH(chaine))return;

    CS_Processing.Lock();
    int i =0,k = 0,j = get_count(chaine);
    CEffectStk * effst;
    float ** out1,** out2;

    out1 = processbuffer;
    out2 = processreplacebuffer;

    //par sécurité on copy notre buffer d'entrées dans out1
    CTAFadeInOut::CopyBuffer(out1,inputs,sampleFrames);

    //changement de chaine on ne traite pas le signal
    if(fadestate == FD_CHTOFADEIN)//signal dry
    {
      j = -1; //on empeche le process
    }
    
    bool processeed = false;
    while(i < j)
    {
      if((effst = get(chaine,i)))
      {

        if(k%2)//impair
        {
          processeed = ProcessEffect(effst,out2,out1,sampleFrames);
        }
        else   //pair
        {
          processeed = ProcessEffect(effst,out1,out2,sampleFrames);
        }
        if(processeed)
          k++;
      }
      i++;
    }

    if(k%2) //on inverse les buffers si nécessaire
    {
      float **buf = out1;
      out1 = out2;
      out2 = buf;
    }


 


    if(fadestate == FD_FADEIN || fadestate == FD_FADEOUT)
    {
      bool finish = fader.FonduBuffer(out2,out1,inputs,sampleFrames);
      //on inverse les buffers
      {
        float **buf = out1;
        out1 = out2;
        out2 = buf;
      }

      if(finish)
      {
        if(fadestate == FD_FADEIN)
        {
          fadestate = FD_NOTHING;
        }
        else if(fadestate == FD_FADEOUT)
        {
          SetChangingFlag(true);
      
          //on precise a la thread de changemnet de chaine que notre fadein est fini
          APP->m_waitfade->SetEvent();
        }

      }
    }

    //on gere le fade des chaines
    if(replace)
    {
      CTAFadeInOut::CopyBuffer(outputs,out1,sampleFrames);
    }
    else
    {
      CTAFadeInOut::AddBuffer(outputs,out1,sampleFrames);
    }
        

    CS_Processing.Unlock();
}


//lance le changement d'une chaine
void CStockEffetLst::ChangeChaine(int from,int to)
{
  //on annonce le changement de chaine
  CS_newchaine.Lock();
  newchaine = to;
  CS_newchaine.Unlock();

  if(!m_workingthread) //pas de changement en cours
  {
    CS_fade.Lock();
    fadestate = FD_CHTOFADEOUT;
    SetWorkingThread(TRUE);
    CS_fade.Unlock();
    AfxBeginThread(TD_ChangeChain,(LPVOID)APP,THREAD_PRIORITY_NORMAL,0,0,NULL);  
  }
}

//process l'effet gere le softbypass 
bool CStockEffetLst::ProcessEffect(CEffectStk * effst,float **inputs, float **outputs, long sampleFrames)
{
  if((effst->bypass == true) && (effst->fader.Fade() == false))  return false;

  int nbeff = effst->effect_nb;
  CSmpEffect * eff = (CSmpEffect *)host->GetAt(nbeff);
  ASSERT(eff);

  if(effst->fader.Fade())//on process au fade
  {
    eff->EffProcessReplacing(inputs,fadebuffer,sampleFrames);
    
    //return true si fini le fade
    effst->fader.FonduBuffer(outputs,fadebuffer,inputs,sampleFrames);
  }
  else
    eff->EffProcessReplacing(inputs,outputs,sampleFrames);
  return true;

}

//remplace outputs

//suivant la parité du nombre d'effet on affecte out1 et out2
//de maniere a avoir outputs en sortie de chaine a chaque fois et pas processbuffer
//CTAFTACTIQUE!
//si premier effet      input >eff1> out1
//suivant i%2 = 1        out1 >eff2> out2
//suivant i%2 = 0        out2 >eff3> out1
//suivant i%2 = 1        out1 >eff4> out2
void CStockEffetLst::processReplace(int chaine,float **inputs, float **outputs, long sampleFrames)
{
  //pq peu yavoir des hosts qui nous donne le mm buffer en entrée et en sortie
  //et si on fait un process (dans le CEffect::processreplace) on efface input
  //et ca,C pas bon!
  process(chaine,inputs, outputs, sampleFrames,true);
}


void CStockEffetLst::SetByPass(int chaine,int nbeffstk,bool bypass)
{
  CEffectStk * eff;
  eff = get(chaine,nbeffstk);
  if(eff == NULL)return;

  eff->bypass = bypass;

  /*if(!fader.Fade())//pa de fade avant on initialise
  {*/
  if(m_processing) //pas en process donc on s'en fou
    if(bypass)
      eff->fader.SetFadeOut();
      //eff->bypass_fade_value = 0.0f;
    else
      eff->fader.SetFadeIn();
      //eff->bypass_fade_value = 1.0f;
  //}
}

bool CStockEffetLst::GetByPass(int chaine,int nbeffstk)
{
  CEffectStk * eff;
  eff = get(chaine,nbeffstk);
  if(eff == NULL)return false;

  return eff->bypass;
}

void CStockEffetLst::SetSampleRate(float sample)
{
  int nbeff ;
  for (int k = 0; k < MAX_CHAINE; k ++)
  {
	  int i =0,j =get_count(k);
	  for (i = 0; i < j;i++)
	  {
      nbeff = get_effect(k,i);
      if(nbeff >= 0)
      {
        host->EffSetSampleRate(nbeff,sample);
      }
	  }
  }
}

long CStockEffetLst::CalculDelay(int chaine)
{
  int i,j = get_count(chaine);
  long retval = 0;
  int nbeff;
  CEffect * eff = NULL;
  for(i=0;i < j;i++)
  {
    nbeff = get_effect(chaine,i);
    if(nbeff < 0)continue;
    
    eff = APP->host->GetAt(nbeff);
    if(!eff)continue;

    retval += eff->pEffect->initialDelay;
  }

  return retval;
}

void CStockEffetLst::SetBlockSize(long size)
{
  int nbeff ;
  for (int k = 0; k < MAX_CHAINE; k ++)
  {
	  int i =0,j =get_count(k);
	  for (i = 0; i < j;i++)
	  {
      nbeff = get_effect(k,i);
      if(nbeff >= 0)
      {
        host->EffSetBlockSize(nbeff,size);
      }
	  }
  }
  if(processbuffer)
  {
    delete processbuffer[0];
    delete processbuffer[1];
    delete []processbuffer;
  }
  if(processreplacebuffer)
  {
    delete processreplacebuffer[0];
    delete processreplacebuffer[1];
    delete []processreplacebuffer;
  }

  if(fadebuffer)
  {
    delete fadebuffer[0];
    delete fadebuffer[1];
    delete []fadebuffer;
  }
  processreplacebuffer = new float *[2]; //stereo only     //CTAFLEMARDISE
  //ouais ben le 5.1 on y est pas encore hein!! on va faire marcher le reste d'abord
  processreplacebuffer[0] = new float[size];
  processreplacebuffer[1] = new float[size];

  processbuffer = new float *[2]; //stereo only     //CTAFLEMARDISE
  //ouais ben le 5.1 on y est pas encore hein!! on va faire marcher le reste d'abord
  processbuffer[0] = new float[size];
  processbuffer[1] = new float[size];

  fadebuffer = new float *[2]; //stereo only     //CTAFLEMARDISE
  //ouais ben le 5.1 on y est pas encore hein!! on va faire marcher le reste d'abord
  fadebuffer[0] = new float[size];
  fadebuffer[1] = new float[size];
    //host->SetBlockSize(size);
}

