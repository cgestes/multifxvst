//Copyleft CTAF
#include "stdafx.h"


#include "stockeffet.h"
#include "vstHost/SmpEffect.h"
#include "CCVSThost.h"
#include "multifxVST.h"
#include "ControleurLst.h"
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
  if(tabcontroleur)
  {

  }
}


void CEffectStk::saveToMem(CEffect * ceff)
{
  if(ceff->pEffect->flags & effFlagsProgramChunks)
  {
        if(tabval)
          delete tabval;

        tabval = NULL;

        void * chunk;
        long lg = ceff->EffGetChunk(&chunk,true);
        if(lg != length)
        {
          if( chunk )
            delete chunk;
          chunk = NULL;
        }

        if(!chunk)
          chunk = new char [lg];

        ASSERT(chunk);
        memcpy(chunk,chunk,lg);
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
void CEffectStk::load(CArchive &ar)
{
  //ar >> effect_nb;

  ar >> effect_name;
  ar >> bankname;
  ar >> ischunk;
  ar >> length;
  if(ischunk)
  {
    chunk = new char[length];
    ar.Read(chunk,length);
  }
  else
  {
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




//##############################################################################



//#include "vsthost/cvsthost.h"
CStockEffetLst::CStockEffetLst()
{
  processbuffer = NULL;
  processreplacebuffer = NULL;
  m_processing = FALSE;
  host = NULL;
  APP = NULL;
  nb_effect_used = 0;
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
    if (!eff->EffGetProductString(sFile.GetBuffer(256)))/* if V2 plugin                      */
      { sFile.ReleaseBuffer();
        sFile = "";                         /* use plugin info                   */
      }else
        sFile.ReleaseBuffer();
      

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
        host->OnIoChanged(nb); //pourkoi réécrire ce qui est deja fait!
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
        return ((CEffectStk *)eff)->effect_nb;
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

void CStockEffetLst::suppr_eff(int chaine,int nb,bool killing_plug)
{
	CEffectStk * eff;

  ASSERT(VCH(chaine));

  //POSITION p;
	eff = get(chaine,nb);

  if(eff)
	{
    lst_stock[chaine].RemoveAt(lst_stock[chaine].FindIndex(nb));
    //decrémente le nb d'utilisation de l'effet (si 0 il le delete ts seul)
    if(host->DecreaseUse(eff->effect_nb,killing_plug,APP))
      nb_effect_used--;


    delete eff;
	}else 	ASSERT(eff);
}

	//void mod_eff();

	//sauve  une liste d'effet
void CStockEffetLst::save(CArchive &ar)
{
  //SaveParamsToMem(APP->current_chaine);
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

  eff.effectbisnb = find_effbisnb(chaine,eff.effect_name);

  int nb = find_eff_not_in(chaine,eff.effect_name,eff.effectbisnb);
  if(nb == -1)
  {
    nb = host->LoadPlugin(eff.effect_name,APP);
    if(nb<0)
      return -1;
    nb_effect_used++;
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
    ar >> j;
	  for (i = 0; i < j;i++)
	  {
      CEffectStk eff;
		  eff.load(ar);
      add_effload(chaine,eff);
	  }
    //LoadParamsFromMem(chaine);
}

void CStockEffetLst::Set(CAppPointer * m_app)
{
  host = m_app->host;
  APP=m_app;
}

//charge une liste d'effet
void CStockEffetLst::load(CArchive &ar)
{

  for (int k = 0; k < MAX_CHAINE; k ++)
  {
    int i =0,j =0;
    ar >> j;
	  for (i = 0; i < j;i++)
	  {
      CEffectStk eff;
		  eff.load(ar);
      add_effload(k,eff);
	  }
  }
  //LoadParamsFromMem(k);
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
}
void CStockEffetLst::resume(int chaine)
{
    if(!VCH(chaine))return ;
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


void CStockEffetLst::process(int chaine,float **inputs, float **outputs, long sampleFrames)
{
    if(!VCH(chaine))return ;
    int i =0,j = get_count(chaine);
    CEffectStk * effst;
    float ** out1,** out2;

    if(!j)return; //on se casse ya rien a faire;

    if(j % 2) //nombre d'effet impair
    {
      out1 = processbuffer;
      out2 = processreplacebuffer;
    }
    else     //nombre d'effet pair
    {
      out1 = processreplacebuffer;
      out2 = processbuffer;
    }

    if(j == 1)  //obligé a cause de processreplace qui nous utilise et qui as pas forcément 
      CopyBuffer(processreplacebuffer,inputs,sampleFrames); //2 buffers différents

    for (i = 0; i < j-1;i++)
    {
      if((effst = get(chaine,i))){
        if(!i)
        {
         host->EffProcessReplacing(effst->effect_nb ,inputs,out1,sampleFrames);
        }
        else
        if(i%2)  //IMPAIR on inverse l'entrée et la sortie un coups sur deux (com en vrai koi!)
         host->EffProcessReplacing(effst->effect_nb ,out1,out2,sampleFrames);
        else
         host->EffProcessReplacing(effst->effect_nb ,out2,out1,sampleFrames);
      }
    }

    //dernier effect de la liste on applique le process
    if((effst = get(chaine,i))){
      host->EffProcess(effst->effect_nb ,processreplacebuffer,outputs,sampleFrames);
    }

}



//remplace outputs
//CTAFFLEMARDISE suppose que ts le monde a un processreplace

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
  process(chaine,inputs, outputs, sampleFrames);

 /*   if(!VCH(chaine))return ;
    int i =0,j = get_count(chaine);
    CEffectStk * effst;
    float ** out1,** out2;

    if(!j)return; //on se casse ya rien a faire;

    if(j % 2) //nombre d'effet impair
    {
      out1 = outputs;
      out2 = processreplacebuffer;
    }
    else     //nombre d'effet pair
    {
      out1 = processreplacebuffer;
      out2 = outputs;
    }

    for (i = 0; i < j;i++)
    {
      if((effst = get(chaine,i))){
        if(!i)
        {
         host->EffProcessReplacing(effst->effect_nb ,inputs,out1,sampleFrames);
        }
        else
        if(i%2)  //IMPAIR on inverse l'entrée et la sortie un coups sur deux (com en vrai koi!)
         host->EffProcessReplacing(effst->effect_nb ,out1,out2,sampleFrames);
        else
         host->EffProcessReplacing(effst->effect_nb ,out2,out1,sampleFrames);
      }
    }*/
}

//on voit l'algo direct! (vive l'assembleur!)
void CStockEffetLst::CopyBuffer(float ** dest,float ** source,long size)
{
  for (int i = 0; i< size; i++)
  {
    dest[0][i] = source[0][i];
    dest[1][i] = source[1][i];
  }
}

//on voit l'algo direct! (vive l'assembleur!)
void CStockEffetLst::AddBuffer(float ** dest,float ** source,long size)
{
  for (int i = 0; i< size; i++)
  {
    dest[0][i] += source[0][i];
    dest[1][i] += source[1][i];
  }
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

  processreplacebuffer = new float *[2]; //stereo only     //CTAFLEMARDISE
  //ouais ben le 5.1 on y est pas encore hein!! on va faire marcher le reste d'abord
  processreplacebuffer[0] = new float[size];
  processreplacebuffer[1] = new float[size];

  processbuffer = new float *[2]; //stereo only     //CTAFLEMARDISE
  //ouais ben le 5.1 on y est pas encore hein!! on va faire marcher le reste d'abord
  processbuffer[0] = new float[size];
  processbuffer[1] = new float[size];

    //host->SetBlockSize(size);
}

