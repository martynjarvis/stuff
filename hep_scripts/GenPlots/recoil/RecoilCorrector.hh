#include <vector>
#include <sstream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom1.h"

//
// ** apply phil's recoil corrections **
// 
// usage: 
//    double met=rawMetValue;
//    double metphi=rawMetPhiValue;
//    RecoilCorrector corrector;
//    corrector->Correct(met,metphi,GenZPt,GenZPhi,leptonPt,leptonPhi);
//    printf("corrected met: %10.2f%10.2f\n",met,metphi);
//
// where leptonPt, leptonPhi are dilepton kinematics for z->ll and single lepton kinematics for w->lnu
//

using namespace std;

class RecoilCorrector
{
  
public:
  RecoilCorrector(string iNameZDat, int iSeed=0xDEADBEEF);
  RecoilCorrector(string iNameZDat1, string iPrefix, int iSeed=0xDEADBEEF);
  void CorrectAll(double &met, double &metphi, double iGenPt, double iGenPhi, double iLepPt, double iLepPhi,double &iU1,double &iU2,double iFluc,int njet=0);
  void Correct(double &pfmet, double &pfmetphi, double &trkmet, double &trkmetphi, 
	       double iGenPt, double iGenPhi, double iLepPt, double iLepPhi,double iFluc    ,int njet=0);
  void CorrectU1U2(double &pfu1, double &pfu2, double &trku1, double &trku2, 
		   double iGenPt, double iGenPhi, double iLepPt, double iLepPhi,double iFluc,int njet=0);
  void addDataFile(std::string iNameDat);
  void addMCFile  (std::string iNameMC);
protected:
  enum Recoil { 
    PFU1,
    PFU2,
    PFMSU1,
    PFMSU2,
    PFS1U1,
    PFS2U1,
    PFS1U2,
    PFS2U2,
    TKU1,
    TKU2,
    TKMSU1,
    TKMSU2,
    TKS1U1,
    TKS2U1,
    TKS1U2,
    TKS2U2
  };

  void readRecoil(std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,
		  std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,
		  std::string iFName,std::string iPrefix); 
  void readCorr(std::string iName,int iType=2);

  void metDistribution(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
		       double iLepPt,double iLepPhi,TRandom1 *iRand,
		       TF1 *iU1RZFit, 
		       TF1 *iU1MSZFit, 
		       TF1 *iU1S1ZFit,
		       TF1 *iU1S2ZFit,
		       TF1 *iU2MSZFit,		   
		       TF1 *iU2S1ZFit, 
		       TF1 *iU2S2ZFit, 
		       TF1 *iU1U2Corr, 
		       double &iU1,double &iU2,double iFluc=0);

  void metDistribution(double &iPFMet,double &iPFMPhi,double &iTKMet,double &iTKMPhi,
		       double iGenPt,double iGenPhi,
		       double iLepPt,double iLepPhi,TRandom1 *iRand,
		       TF1 *iU1RZPFFit,  TF1 *iU1RZTKFit, 
		       TF1 *iU1MSZPFFit, TF1 *iU1MSZTKFit, 
		       TF1 *iU1S1ZPFFit, TF1 *iU1S1ZTKFit,
		       TF1 *iU1S2ZPFFit, TF1 *iU1S2ZTKFit,
		       TF1 *iU2MSZPFFit, TF1 *iU2MSZTKFit,		   
		       TF1 *iU2S1ZPFFit, TF1 *iU2S1ZTKFit, 
		       TF1 *iU2S2ZPFFit, TF1 *iU2S2ZTKFit, 
		       TF1 *iPFU1U2Corr, TF1 *iTKU1U2Corr,
		       TF1 *iPFTKU1Corr, TF1 *iPFTKU2Corr,
		       TF1 *iPFTKU1MCorr,TF1 *iPFTKU2MCorr,
		       double &iU1,double &iU2,double iFluc=0);

  double calculate(int iMet,double iEPt,double iEPhi,double iWPhi,double iU1,double iU2);
  double getError(double iVal,TF1 *iZDatFit,Recoil iType);
  double getError2(double iVal,TF1 *iFit);
  double getCorError2(double iVal,TF1 *iFit);
  double mag(double iV0,double iV1,double iV2,double iV3);
  double correlatedSeed(double iVal, double iCorr1,double iCorr2,double iCorr3,double iSeed0,double iSeed1,double iSeed2,double iSeed3);
  TF1*   getFunc(bool iMC, Recoil iType);
  double CorrVal(double iPt,double iVal,Recoil iType);
  //void   Correct(double &met, double &metphi, double lGenPt, double lGenPhi, double lepPt, double lepPhi,double iFluc,int njet);

  TRandom1 *fRandom; 
  vector<TF1*> fF1U1Fit; vector<TF1*> fF1U1RMSSMFit; vector<TF1*> fF1U1RMS1Fit; vector<TF1*> fF1U1RMS2Fit; 
  vector<TF1*> fF1U2Fit; vector<TF1*> fF1U2RMSSMFit; vector<TF1*> fF1U2RMS1Fit; vector<TF1*> fF1U2RMS2Fit; 
  vector<TF1*> fF2U1Fit; vector<TF1*> fF2U1RMSSMFit; vector<TF1*> fF2U1RMS1Fit; vector<TF1*> fF2U1RMS2Fit; 
  vector<TF1*> fF2U2Fit; vector<TF1*> fF2U2RMSSMFit; vector<TF1*> fF2U2RMS1Fit; vector<TF1*> fF2U2RMS2Fit; 

  vector<TF1*> fD1U1Fit; vector<TF1*> fD1U1RMSSMFit; vector<TF1*> fD1U1RMS1Fit; vector<TF1*> fD1U1RMS2Fit; 
  vector<TF1*> fD1U2Fit; vector<TF1*> fD1U2RMSSMFit; vector<TF1*> fD1U2RMS1Fit; vector<TF1*> fD1U2RMS2Fit; 
  vector<TF1*> fD2U1Fit; vector<TF1*> fD2U1RMSSMFit; vector<TF1*> fD2U1RMS1Fit; vector<TF1*> fD2U1RMS2Fit; 
  vector<TF1*> fD2U2Fit; vector<TF1*> fD2U2RMSSMFit; vector<TF1*> fD2U2RMS1Fit; vector<TF1*> fD2U2RMS2Fit; 

  vector<TF1*> fM1U1Fit; vector<TF1*> fM1U1RMSSMFit; vector<TF1*> fM1U1RMS1Fit; vector<TF1*> fM1U1RMS2Fit; 
  vector<TF1*> fM1U2Fit; vector<TF1*> fM1U2RMSSMFit; vector<TF1*> fM1U2RMS1Fit; vector<TF1*> fM1U2RMS2Fit; 
  vector<TF1*> fM2U1Fit; vector<TF1*> fM2U1RMSSMFit; vector<TF1*> fM2U1RMS1Fit; vector<TF1*> fM2U1RMS2Fit; 
  vector<TF1*> fM2U2Fit; vector<TF1*> fM2U2RMSSMFit; vector<TF1*> fM2U2RMS1Fit; vector<TF1*> fM2U2RMS2Fit; 

  vector<TF1*> fF1U1U2Corr;     vector<TF1*> fF2U1U2Corr;
  vector<TF1*> fF1F2U1Corr;     vector<TF1*> fF1F2U2Corr;
  vector<TF1*> fF1F2U1U2Corr;   vector<TF1*> fF1F2U2U1Corr;
  int fId; int fJet;
};

//-----------------------------------------------------------------------------------------------------------------------------------------
  RecoilCorrector::RecoilCorrector(string iNameZDat,std::string iPrefix, int iSeed) {

  fRandom = new TRandom1(iSeed);

  // get fits for Z data
  readRecoil(fF1U1Fit,fF1U1RMSSMFit,fF1U1RMS1Fit,fF1U1RMS2Fit,fF1U2Fit,fF1U2RMSSMFit,fF1U2RMS1Fit,fF1U2RMS2Fit,iNameZDat,iPrefix);
  if(iPrefix == "PF") readCorr  (iNameZDat,0);
  if(iPrefix == "TK") readCorr  (iNameZDat,1);  
  fId = 0; fJet = 0;
}

RecoilCorrector::RecoilCorrector(string iNameZ, int iSeed) {

  fRandom = new TRandom1(iSeed);
  // get fits for Z data
  readRecoil(fF1U1Fit,fF1U1RMSSMFit,fF1U1RMS1Fit,fF1U1RMS2Fit,fF1U2Fit,fF1U2RMSSMFit,fF1U2RMS1Fit,fF1U2RMS2Fit,iNameZ,"PF");
  readRecoil(fF2U1Fit,fF2U1RMSSMFit,fF2U1RMS1Fit,fF2U1RMS2Fit,fF2U2Fit,fF2U2RMSSMFit,fF2U2RMS1Fit,fF2U2RMS2Fit,iNameZ,"TK");
  readCorr  (iNameZ);
  fId = 0; fJet = 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::addDataFile(std::string iNameData) { 
  readRecoil(fD1U1Fit,fD1U1RMSSMFit,fD1U1RMS1Fit,fD1U1RMS2Fit,fD1U2Fit,fD1U2RMSSMFit,fD1U2RMS1Fit,fD1U2RMS2Fit,iNameData,"PF");
  readRecoil(fD2U1Fit,fD2U1RMSSMFit,fD2U1RMS1Fit,fD2U1RMS2Fit,fD2U2Fit,fD2U2RMSSMFit,fD2U2RMS1Fit,fD2U2RMS2Fit,iNameData,"TK");
  readCorr(iNameData);
  fId++;   
}
void RecoilCorrector::addMCFile  (std::string iNameMC) { 
  fId++;
  readRecoil(fM1U1Fit,fM1U1RMSSMFit,fM1U1RMS1Fit,fM1U1RMS2Fit,fM1U2Fit,fM1U2RMSSMFit,fM1U2RMS1Fit,fM1U2RMS2Fit,iNameMC,"PF");
  readRecoil(fM2U1Fit,fM2U1RMSSMFit,fM2U1RMS1Fit,fM2U1RMS2Fit,fM2U2Fit,fM2U2RMSSMFit,fM2U2RMS1Fit,fM2U2RMS2Fit,iNameMC,"TK");
}
void RecoilCorrector::CorrectAll(double &met, double &metphi, double lGenPt, double lGenPhi, double lepPt, double lepPhi,double &iU1,double &iU2,double iFluc,int njet) {
  fJet = njet; if(njet > 2) fJet = 2;  
  if(fJet > int(fF1U1Fit.size())) fJet = 0; 
  metDistribution(met,metphi,lGenPt,lGenPhi,lepPt,lepPhi,fRandom,
		  fF1U1Fit     [fJet],
		  fF1U1RMSSMFit[fJet],
		  fF1U1RMS1Fit [fJet],
		  fF1U1RMS2Fit [fJet],
		  fF1U2RMSSMFit[fJet],
		  fF1U2RMS1Fit [fJet],
		  fF1U2RMS2Fit [fJet],
		  fF1U1U2Corr  [fJet],
		  iU1, iU2,iFluc
		  );
}
void RecoilCorrector::Correct(double &pfmet, double &pfmetphi, double &trkmet, double &trkmetphi, 
                              double lGenPt, double lGenPhi, double lepPt, double lepPhi,double iFluc,int njet) {
  double lU1 = 0; double lU2 = 0;
  fJet = njet; if(njet > 2) fJet = 2;  
  if(fJet > int(fF1U1Fit.size())) fJet = 0; 
  metDistribution(pfmet,pfmetphi,trkmet,trkmetphi,lGenPt,lGenPhi,lepPt,lepPhi,fRandom,
		  fF1U1Fit     [fJet],fF2U1Fit     [fJet],
		  fF1U1RMSSMFit[fJet],fF2U1RMSSMFit[fJet],
		  fF1U1RMS1Fit [fJet],fF2U1RMS1Fit [fJet],
		  fF1U1RMS2Fit [fJet],fF2U1RMS2Fit [fJet],
		  fF1U2RMSSMFit[fJet],fF2U2RMSSMFit[fJet],
		  fF1U2RMS1Fit [fJet],fF2U2RMS1Fit [fJet],
		  fF1U2RMS2Fit [fJet],fF2U2RMS2Fit [fJet],
		  fF1U1U2Corr  [fJet],fF2U1U2Corr  [fJet],
		  fF1F2U1Corr  [fJet],fF1F2U2Corr  [fJet],
		  fF1F2U1U2Corr[fJet],fF1F2U2U1Corr[fJet],
		  lU1                , lU2,
		  iFluc
		  );
}
void RecoilCorrector::CorrectU1U2(double &iPFU1, double &iPFU2, double &iTKU1, double &iTKU2, 
				  double lGenPt, double lGenPhi, double lepPt, double lepPhi,double iFluc,int njet) {
  double pfmet = 0; double pfmetphi = 0; //double trkmet = 0; double trkmetphi = 0;
  fJet = njet; if(njet > 2) fJet = 2;  
  if(fJet > int(fF1U1Fit.size())) fJet = 0; 
  metDistribution(pfmet,pfmetphi,iTKU1,iTKU2,lGenPt,lGenPhi,lepPt,lepPhi,fRandom,
		  fF1U1Fit     [fJet],fF2U1Fit     [fJet],
		  fF1U1RMSSMFit[fJet],fF2U1RMSSMFit[fJet],
		  fF1U1RMS1Fit [fJet],fF2U1RMS1Fit [fJet],
		  fF1U1RMS2Fit [fJet],fF2U1RMS2Fit [fJet],
		  fF1U2RMSSMFit[fJet],fF2U2RMSSMFit[fJet],
		  fF1U2RMS1Fit [fJet],fF2U2RMS1Fit [fJet],
		  fF1U2RMS2Fit [fJet],fF2U2RMS2Fit [fJet],
		  fF1U1U2Corr  [fJet],fF2U1U2Corr  [fJet],
		  fF1F2U1Corr  [fJet],fF1F2U2Corr  [fJet],
		  fF1F2U1U2Corr[fJet],fF1F2U2U1Corr[fJet],
		  iPFU1              ,iPFU2,
		  iFluc
		  );
  //iTKU1 = 0; iTKU2 = 0;
}
double RecoilCorrector::CorrVal(double iPt, double iVal, Recoil iType) { 
// this is the funciton that applies the Zdata/Zmc correction 
  if(fId == 0) return iVal;
  switch(iType) {
  case PFU1   : return iVal*(fD1U1Fit     [fJet]->Eval(iPt)/fM1U1Fit     [fJet]->Eval(iPt));
  case PFMSU1 : return iVal*(fD1U1RMSSMFit[fJet]->Eval(iPt)/fM1U1RMSSMFit[fJet]->Eval(iPt));
  case PFS1U1 : return iVal*(fD1U1RMS1Fit [fJet]->Eval(iPt)/fM1U1RMS1Fit [fJet]->Eval(iPt));
  case PFS2U1 : return iVal*(fD1U1RMS2Fit [fJet]->Eval(iPt)/fM1U1RMS2Fit [fJet]->Eval(iPt));
  case PFU2   : return 0;
  case PFMSU2 : return iVal*(fD1U2RMSSMFit[fJet]->Eval(iPt)/fM1U2RMSSMFit[fJet]->Eval(iPt));
  case PFS1U2 : return iVal*(fD1U2RMS1Fit [fJet]->Eval(iPt) /fM1U2RMS1Fit[fJet]->Eval(iPt));
  case PFS2U2 : return iVal*(fD1U2RMS2Fit [fJet]->Eval(iPt) /fM1U2RMS2Fit[fJet]->Eval(iPt));
  case TKU1   : return iVal*(fD2U1Fit     [fJet]->Eval(iPt)/fM2U1Fit     [fJet]->Eval(iPt));
  case TKMSU1 : return iVal*(fD2U1RMSSMFit[fJet]->Eval(iPt)/fM2U1RMSSMFit[fJet]->Eval(iPt));
  case TKS1U1 : return iVal*(fD2U1RMS1Fit [fJet]->Eval(iPt) /fM2U1RMS1Fit[fJet]->Eval(iPt));
  case TKS2U1 : return iVal*(fD2U1RMS2Fit [fJet]->Eval(iPt) /fM2U1RMS2Fit[fJet]->Eval(iPt));
  case TKU2   : return 0;
  case TKMSU2 : return iVal*(fD2U2RMSSMFit[fJet]->Eval(iPt)/fM2U2RMSSMFit[fJet]->Eval(iPt));
  case TKS1U2 : return iVal*(fD2U2RMS1Fit [fJet]->Eval(iPt) /fM2U2RMS1Fit[fJet]->Eval(iPt));
  case TKS2U2 : return iVal*(fD2U2RMS2Fit [fJet]->Eval(iPt) /fM2U2RMS2Fit[fJet]->Eval(iPt));
  }
  return iVal;
}
TF1* RecoilCorrector::getFunc(bool iMC, Recoil iType) { 
  if(fId == 0) return 0;
  switch(iType) {
  case PFU1   : if(iMC) {return fD1U1Fit     [fJet];} return fM1U1Fit     [fJet];
  case PFMSU1 : if(iMC) {return fD1U1RMSSMFit[fJet];} return fM1U1RMSSMFit[fJet];
  case PFS1U1 : if(iMC) {return fD1U1RMS1Fit [fJet];} return fM1U1RMS1Fit [fJet];
  case PFS2U1 : if(iMC) {return fD1U1RMS2Fit [fJet];} return fM1U1RMS2Fit [fJet];
  case PFU2   : return 0;
  case PFMSU2 : if(iMC) {return fD1U2RMSSMFit[fJet];} return fM1U2RMSSMFit[fJet];
  case PFS1U2 : if(iMC) {return fD1U2RMS1Fit [fJet];} return fM1U2RMS1Fit [fJet];
  case PFS2U2 : if(iMC) {return fD1U2RMS2Fit [fJet];} return fM1U2RMS2Fit [fJet];
  case TKU1   : if(iMC) {return fD2U1Fit     [fJet];} return fM2U1Fit     [fJet];
  case TKMSU1 : if(iMC) {return fD2U1RMSSMFit[fJet];} return fM2U1RMSSMFit[fJet];
  case TKS1U1 : if(iMC) {return fD2U1RMS1Fit [fJet];} return fM2U1RMS1Fit [fJet];
  case TKS2U1 : if(iMC) {return fD2U1RMS2Fit [fJet];} return fM2U1RMS2Fit [fJet];
  case TKU2   : return 0;
  case TKMSU2 : if(iMC) {return fD2U2RMSSMFit[fJet];} return fM2U2RMSSMFit[fJet];
  case TKS1U2 : if(iMC) {return fD2U2RMS1Fit [fJet];} return fM2U2RMS1Fit [fJet];
  case TKS2U2 : if(iMC) {return fD2U2RMS2Fit [fJet];} return fM2U2RMS2Fit [fJet];
  }
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::readRecoil(std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,
		                 std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,
		                 std::string iFName,std::string iPrefix) {
//  if(!getenv("CMSSW_BASE")) {
//    printf("error! RecoilCorrector called without input files. Define CMSSW_BASE or add by hand.\n");
//    assert(0);
//  }
  TFile *lFile  = new TFile(iFName.c_str());
  int lNJet = 0;
  std::stringstream lSS; lSS << iPrefix << "u1Mean_" << lNJet;
  while(lFile->FindObjectAny(lSS.str().c_str()) != 0) { lSS.clear();
    lSS << iPrefix << "u1Mean_"    << lNJet; iU1Fit.push_back    ( (TF1*) lFile->FindObjectAny((iPrefix+"u1Mean_0").c_str()));    lSS.clear();
    lSS << iPrefix << "u1MeanRMS_" << lNJet; iU1MRMSFit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u1MeanRMS_0").c_str())); lSS.clear(); 
    lSS << iPrefix << "u1RMS1_"    << lNJet; iU1RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u1RMS1_0").c_str()));    lSS.clear(); 
    lSS << iPrefix << "u1RMS2_"    << lNJet; iU1RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u1RMS2_0").c_str()));    lSS.clear(); 
    lSS << iPrefix << "u2Mean_"    << lNJet; iU2Fit    .push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u2Mean_0").c_str()));    lSS.clear();
    lSS << iPrefix << "u2MeanRMS_" << lNJet; iU2MRMSFit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u2MeanRMS_0").c_str())); lSS.clear();
    lSS << iPrefix << "u2RMS1_"    << lNJet; iU2RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u2RMS1_0").c_str()));    lSS.clear();
    lSS << iPrefix << "u2RMS2_"    << lNJet; iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u2RMS2_0").c_str()));    lSS.clear();
    lSS << iPrefix << "u2RMS2_"    << lNJet; iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u2RMS2_0").c_str()));    lSS.clear();
    lNJet++; lSS << iPrefix << "u1Mean_" << lNJet;
  }
  lFile->Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::readCorr(std::string iName,int iType) {
  TFile *lFile = new TFile(iName.c_str());
  std::stringstream pSS1,pSS2,pSS3,pSS4,pSS5,pSS6;
  int lNJet = 0;
  std::stringstream lSS; lSS << "PFu1Mean_" << lNJet;
  while(lFile->FindObjectAny(lSS.str().c_str()) != 0) { 
    lSS.clear(); pSS1.clear(); pSS2.clear(); pSS3.clear(); pSS4.clear(); pSS5.clear(); pSS6.clear();
    if(iType != 1) {pSS1  << "u1u2pfCorr_" << lNJet;   fF1U1U2Corr.push_back((TF1*) lFile->FindObjectAny(pSS1.str().c_str())); }
    if(iType != 0) {pSS2  << "u1u2tkCorr_" << lNJet;   fF2U1U2Corr.push_back((TF1*) lFile->FindObjectAny(pSS2.str().c_str())); }
    if(iType <  2) { lNJet++; lSS <<  "PFu1Mean_" << lNJet;  continue;}
    pSS3  << "pftku1Corr_"  << lNJet;   fF1F2U1Corr   .push_back((TF1*) lFile->FindObjectAny(pSS3.str().c_str()));
    pSS4  << "pftku2Corr_"  << lNJet;   fF1F2U2Corr   .push_back((TF1*) lFile->FindObjectAny(pSS4.str().c_str()));
    pSS5  << "pftkum1Corr_" << lNJet;   fF1F2U1U2Corr .push_back((TF1*) lFile->FindObjectAny(pSS5.str().c_str()));
    pSS6  << "pftkum2Corr_" << lNJet;   fF1F2U2U1Corr .push_back((TF1*) lFile->FindObjectAny(pSS6.str().c_str()));
    lNJet++; lSS   << "PFu1Mean_" << lNJet;
  }
  lFile->Close();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::metDistribution(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
		                      double iLepPt,double iLepPhi,TRandom1 *iRand,
		                      TF1 *iU1RZDatFit,
		                      TF1 *iU1MSZDatFit, 
		                      TF1 *iU1S1ZDatFit,
		                      TF1 *iU1S2ZDatFit,
		                      TF1 *iU2MSZDatFit, 		   
		                      TF1 *iU2S1ZDatFit, 
		                      TF1 *iU2S2ZDatFit, 		                      
		                      TF1 *iU1U2Corr,
				      double &iU1, double &iU2,
		                      double iFluc) {
  double lRescale  = sqrt((TMath::Pi())/2.);		     
  double pU1       = CorrVal(iGenPt,iU1RZDatFit->Eval(iGenPt),PFU1);
  double pU2       = 0; //Right guys are for cumulants => code deleted
  //double pFrac1    = CorrVal(iGenPt,iU1MSZDatFit->Eval(iGenPt),PFMSU1)*lRescale;
  //double pFrac2    = CorrVal(iGenPt,iU2MSZDatFit->Eval(iGenPt),PFMSU2)*lRescale;
  //double pSigma1_1 = CorrVal(iGenPt,iU1S1ZDatFit->Eval(iGenPt),PFS1U1)*lRescale*CorrVal(iGenPt,iU1MSZDatFit->Eval(iGenPt),PFMSU1);
  //double pSigma1_2 = CorrVal(iGenPt,iU1S2ZDatFit->Eval(iGenPt),PFS2U1)*lRescale*CorrVal(iGenPt,iU1MSZDatFit->Eval(iGenPt),PFMSU1);
  //double pSigma2_1 = CorrVal(iGenPt,iU2S1ZDatFit->Eval(iGenPt),PFS1U2)*lRescale*CorrVal(iGenPt,iU2MSZDatFit->Eval(iGenPt),PFS1U2);
  //double pSigma2_2 = CorrVal(iGenPt,iU2S2ZDatFit->Eval(iGenPt),PFS2U2)*lRescale*CorrVal(iGenPt,iU2MSZDatFit->Eval(iGenPt),PFS2U2);
  double pFrac1    = CorrVal(iGenPt,iU1MSZDatFit->Eval(iGenPt),PFMSU1)*lRescale;
  double pFrac2    = CorrVal(iGenPt,iU2MSZDatFit->Eval(iGenPt),PFMSU2)*lRescale;
  double pSigma1_1 = CorrVal(iGenPt,iU1S1ZDatFit->Eval(iGenPt),PFS1U1)*lRescale*CorrVal(iGenPt,iU1MSZDatFit->Eval(iGenPt),PFMSU1);
  double pSigma1_2 = CorrVal(iGenPt,iU1S2ZDatFit->Eval(iGenPt),PFS2U1)*lRescale*CorrVal(iGenPt,iU1MSZDatFit->Eval(iGenPt),PFMSU1);
  double pSigma2_1 = CorrVal(iGenPt,iU2S1ZDatFit->Eval(iGenPt),PFS1U2)*lRescale*CorrVal(iGenPt,iU2MSZDatFit->Eval(iGenPt),PFMSU2);
  double pSigma2_2 = CorrVal(iGenPt,iU2S2ZDatFit->Eval(iGenPt),PFS2U2)*lRescale*CorrVal(iGenPt,iU2MSZDatFit->Eval(iGenPt),PFMSU2);
  
  //Uncertainty propagation
  if(iFluc != 0) { 
    double lEUR1    = getError(iGenPt,iU1RZDatFit ,PFU1);
    double lEUS1_1  = getError(iGenPt,iU1S1ZDatFit,PFS1U1);
    double lEUS1_2  = getError(iGenPt,iU1S2ZDatFit,PFS2U1);
    double lEU1Frac = getError(iGenPt,iU1MSZDatFit,PFMSU1);
    double lEUS2_1  = getError(iGenPt,iU2S1ZDatFit,PFS1U2);
    double lEUS2_2  = getError(iGenPt,iU2S2ZDatFit,PFS2U2);
    double lEU2Frac = getError(iGenPt,iU2MSZDatFit,PFMSU2);
    
    //Modify all the different parameters the choice of signs makes it maximal
    pU1       = pU1       - iFluc*lEUR1;             //Recoil
    pFrac1    = pFrac1    + iFluc*(lEU1Frac);        //Mean RMS 
    pSigma1_1 = pSigma1_1 - iFluc*lEUS1_1*pFrac1;    //Sigma 1 smalles sigma
    pSigma1_2 = pSigma1_2 + iFluc*lEUS1_2*pFrac1;    //Sigma 2 (Maximal when oppsite sigma 1)
    pFrac2    = pFrac2    + iFluc*(lEU2Frac);        //Mean RMS for U2
    pSigma2_1 = pSigma2_1 - iFluc*lEUS2_1*pFrac2;    //Sigma 1 U2
    pSigma2_2 = pSigma2_2 + iFluc*(lEUS2_2)*pFrac2;
  }
  //Caculat the proper fraction
  pFrac1 = (pFrac1-pSigma1_2)/(pSigma1_1-pSigma1_2);
  pFrac2 = (pFrac2-pSigma2_2)/(pSigma2_1-pSigma2_2);

  //Now sample for the MET distribution
  double pVal0 = iRand->Uniform(0,1);
  double pVal1 = iRand->Uniform(0,1);
  double pCorr1     = iRand->Gaus(0,1);     
  double pCorr2     = iRand->Gaus(0,1);  

  //double pCorrT1    = iRand->Gaus(0,1);     double pCorrT2    = iRand->Gaus(0,1);  
  pSigma1_1 = ((pVal0 < pFrac1)*(pSigma1_1)+(pVal0 > pFrac1)*(pSigma1_2)); 
  pSigma2_1 = ((pVal1 < pFrac2)*(pSigma2_1)+(pVal1 > pFrac2)*(pSigma2_2)); 

  double lU1U2   = 0.;//iU1U2Corr->Eval(iGenPt);TODO 
  double pVal1_1 = correlatedSeed(pSigma1_1,lU1U2,0.,0.,pCorr1,pCorr2,0.,0.);
  double pVal2_1 = correlatedSeed(pSigma2_1,lU1U2,0.,0.,pCorr2,pCorr1,0.,0.);
  //double pVal2_1 = correlatedSeed(pSigma2_1,lU1U2,0.,0.,pCorr2,pCorr1,0.,0.);
  //double pVal2_2 = correlatedSeed(pSigma2_2,lU1U2,0.,0.,pCorrT2,pCorrT1,0.,0.);


  pU1   = pVal1_1+pU1;//(pVal0 < pFrac1)*(pVal1_1+pU1)+(pVal0 > pFrac1)*(pVal1_2+pU1);
  pU2   = pVal2_1+pU2;//(pVal1 < pFrac2)*(pVal2_1+pU2)+(pVal1 > pFrac2)*(pVal2_2+pU2);

  //pU1   = (lVal0 < pFrac1)*iRand->Gaus(pU1,pSigma1_1)+(lVal0 > pFrac1)*iRand->Gaus(pU1,pSigma1_2);
  //pU2   = (lVal1 < pFrac2)*iRand->Gaus(pU2,pSigma2_1)+(lVal1 > pFrac2)*iRand->Gaus(pU2,pSigma2_2);
  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iU1   = pU1; 
  iU2   = pU2;
  return;
}

void RecoilCorrector::metDistribution(double &iPFMet,double &iPFMPhi,double &iTKMet,double &iTKMPhi,
				      double iGenPt,double iGenPhi,
		                      double iLepPt,double iLepPhi,TRandom1 *iRand,
		                      TF1 *iU1RPFFit,   TF1 *iU1RTKFit,
		                      TF1 *iU1MSPFFit,  TF1 *iU1MSTKFit, 
		                      TF1 *iU1S1PFFit,  TF1 *iU1S1TKFit,
		                      TF1 *iU1S2PFFit,  TF1 *iU1S2TKFit,
		                      TF1 *iU2MSPFFit,  TF1 *iU2MSTKFit, 		      
		                      TF1 *iU2S1PFFit,  TF1 *iU2S1TKFit, 
		                      TF1 *iU2S2PFFit,  TF1 *iU2S2TKFit, 
		                      TF1 *iPFU1U2Corr, TF1 *iTKU1U2Corr,
		                      TF1 *iPFTKU1Corr, TF1 *iPFTKU2Corr,
		                      TF1 *iPFTKU1MCorr,TF1 *iPFTKU2MCorr,
				      double       &iU1, double &iU2,
				      double iFluc) {
  //Important constants re-scaling of sigma on left and mean wpt of W resbos on right
  double lRescale  = sqrt((TMath::Pi())/2.); //double lPtMean = 16.3; //==> tuned for W bosons
  ///
  double pPFU1       = CorrVal(iGenPt,iU1RPFFit->Eval(iGenPt),PFU1);
  double pPFU2       = 0;
  double pPFSigma1_1 = CorrVal(iGenPt,iU1S1PFFit->Eval(iGenPt),PFS1U1)*CorrVal(iGenPt,iU1MSPFFit->Eval(iGenPt),PFMSU1)*lRescale;
  double pPFSigma1_2 = CorrVal(iGenPt,iU1S2PFFit->Eval(iGenPt),PFS2U1)*CorrVal(iGenPt,iU1MSPFFit->Eval(iGenPt),PFMSU1)*lRescale;
  double pPFFrac1    = CorrVal(iGenPt,iU1MSPFFit->Eval(iGenPt),PFMSU1)                         *lRescale;
  double pPFSigma2_1 = CorrVal(iGenPt,iU2S1PFFit->Eval(iGenPt),PFS1U2)*CorrVal(iGenPt,iU2MSPFFit->Eval(iGenPt),PFMSU2)*lRescale;
  double pPFSigma2_2 = CorrVal(iGenPt,iU2S2PFFit->Eval(iGenPt),PFS2U2)*CorrVal(iGenPt,iU2MSPFFit->Eval(iGenPt),PFMSU2)*lRescale;
  double pPFFrac2    = CorrVal(iGenPt,iU2MSPFFit->Eval(iGenPt),PFMSU2)                         *lRescale;
  if(pPFSigma1_1 > pPFSigma1_2) {double pT = pPFSigma1_2; pPFSigma1_2 = pPFSigma1_1; pPFSigma1_1 = pT;}
  if(pPFSigma2_1 > pPFSigma2_2) {double pT = pPFSigma2_2; pPFSigma2_2 = pPFSigma2_1; pPFSigma2_2 = pT;}
  
  double pTKU1       = CorrVal(iGenPt,iU1RTKFit->Eval(iGenPt),TKU1);
  double pTKU2       = 0;
  double pTKSigma1_1 = CorrVal(iGenPt,iU1S1TKFit->Eval(iGenPt),TKS1U1)*CorrVal(iGenPt,iU1MSTKFit->Eval(iGenPt),TKMSU1)*lRescale;
  double pTKSigma1_2 = CorrVal(iGenPt,iU1S2TKFit->Eval(iGenPt),TKS2U1)*CorrVal(iGenPt,iU1MSTKFit->Eval(iGenPt),TKMSU1)*lRescale;
  double pTKFrac1    = CorrVal(iGenPt,iU1MSTKFit->Eval(iGenPt),TKMSU1)                         *lRescale;
  double pTKSigma2_1 = CorrVal(iGenPt,iU2S1TKFit->Eval(iGenPt),TKS1U2)*CorrVal(iGenPt,iU2MSTKFit->Eval(iGenPt),TKMSU2)*lRescale;
  double pTKSigma2_2 = CorrVal(iGenPt,iU2S2TKFit->Eval(iGenPt),TKS2U2)*CorrVal(iGenPt,iU2MSTKFit->Eval(iGenPt),TKMSU2)*lRescale;
  double pTKFrac2    = CorrVal(iGenPt,iU2MSTKFit->Eval(iGenPt),TKMSU2)                         *lRescale;
  if(pTKSigma1_1 > pTKSigma1_2) {double pT = pTKSigma1_2; pTKSigma1_2 = pTKSigma1_1; pTKSigma1_1 = pT;}
  if(pTKSigma2_1 > pTKSigma2_2) {double pT = pTKSigma2_2; pTKSigma2_2 = pTKSigma2_1; pTKSigma2_2 = pT;}

  //Uncertainty propagation
  if(iFluc != 0) { 
    double lEUR1    = getError(iGenPt ,iU1RPFFit , PFU1);
    double lEUS1_1  = getError(iGenPt ,iU1S1PFFit, PFS1U1);
    double lEUS1_2  = getError(iGenPt ,iU1S2PFFit, PFS2U1);
    double lEU1Frac = getError(iGenPt ,iU1MSPFFit, PFMSU1)*lRescale;
    double lEUS2_1  = getError(iGenPt ,iU2S1PFFit, PFS1U2);
    double lEUS2_2  = getError(iGenPt ,iU2S2PFFit, PFS2U2);
    double lEU2Frac = getError(iGenPt ,iU2MSPFFit, PFMSU2)*lRescale;
    //cout << "===> " << pPFSigma1_1 << " -- " << iU1S2PFFit->GetParError(0) << " -- " << lEUS1_1 << endl;
    //Modify all the different parameters the choice of signs makes it maximal
    if(iU1S1PFFit->Eval(iGenPt) > 1) {double pPF = lEUS1_1; lEUS1_1 = lEUS1_2; lEUS1_1 = pPF;}
    if(iU2S1PFFit->Eval(iGenPt) > 1) {double pPF = lEUS2_1; lEUS2_1 = lEUS2_2; lEUS2_1 = pPF;}

    pPFU1       = pPFU1       ;// iFluc*lEUR1;              //Recoil
    pPFFrac1    = pPFFrac1    ;//+ iFluc*(lEU1Frac);        //Mean RMS 
    pPFSigma1_1 = pPFSigma1_1 + iFluc*lEUS1_1*pPFFrac1;    //Sigma 1 smalles sigma
    pPFSigma1_2 = pPFSigma1_2 - iFluc*lEUS1_2*pPFFrac1;    //Sigma 2 (Maximal when oppsite sigma 1)
    pPFFrac2    = pPFFrac2    ;//+ iFluc*(lEU2Frac);        //Mean RMS for U2
    pPFSigma2_1 = pPFSigma2_1 ;//+ iFluc*lEUS2_1*pPFFrac2  ;    //Sigma 1 U2
    pPFSigma2_2 = pPFSigma2_2 ;//- iFluc*(lEUS2_2)*pPFFrac2;
    
    lEUR1    = getError(iGenPt,iU1RTKFit ,TKU1);
    lEUS1_1  = getError(iGenPt,iU1S1TKFit,TKS1U1);
    lEUS1_2  = getError(iGenPt,iU1S2TKFit,TKS2U1);
    lEU1Frac = getError(iGenPt,iU1MSTKFit,TKMSU1)*lRescale;
    lEUS2_1  = getError(iGenPt,iU2S1TKFit,TKS1U2);
    lEUS2_2  = getError(iGenPt,iU2S2TKFit,TKS2U2);
    lEU2Frac = getError(iGenPt,iU2MSTKFit,TKMSU2)*lRescale;
    if(iU1S1TKFit->Eval(iGenPt) > 1) {double pPF = lEUS1_1; lEUS1_1 = lEUS1_2; lEUS1_1 = pPF;}
    if(iU2S1TKFit->Eval(iGenPt) > 1) {double pPF = lEUS2_1; lEUS2_1 = lEUS2_2; lEUS2_1 = pPF;}
    //Modify all the different parameters the choice of signs makes it maximal
    pTKU1       = pTKU1        ;//+ iFluc*lEUR1;              //Recoil
    pTKFrac1    = pTKFrac1     ;//+ iFluc*(lEU1Frac);        //Mean RMS 
    pTKSigma1_1 = pTKSigma1_1  +  iFluc*lEUS1_1 *pTKFrac1;    //Sigma 1 smalles sigma
    pTKSigma1_2 = pTKSigma1_2  -  iFluc*lEUS1_2 *pTKFrac1;    //Sigma 2 (Maximal when oppsite sigma 1)
    pTKFrac2    = pTKFrac2     ;//+ iFluc*(lEU2Frac);        //Mean RMS for U2
    pTKSigma2_1 = pTKSigma2_1  ;//+ iFluc*lEUS2_1  *pTKFrac2;    //Sigma 1 U2
    pTKSigma2_2 = pTKSigma2_2  ;//+ iFluc*(lEUS2_2)*pTKFrac2;
  }
  //double lMeanU1 = pPFFrac1;
  //double lMeanU2 = pPFFrac2;
    
  //Caculat the proper fraction
  pPFFrac1 = (pPFFrac1-pPFSigma1_2)/(pPFSigma1_1-pPFSigma1_2);
  pPFFrac2 = (pPFFrac2-pPFSigma2_2)/(pPFSigma2_1-pPFSigma2_2);

  pTKFrac1 = (pTKFrac1-pTKSigma1_2)/(pTKSigma1_1-pTKSigma1_2);
  pTKFrac2 = (pTKFrac2-pTKSigma2_2)/(pTKSigma2_1-pTKSigma2_2);
  //if(iGenPt > 60) cout << "===> " << pFrac1 << " -- " <<pFrac2 << endl;
  double pPFVal0 = iRand->Uniform(0,1);
  double pPFVal1 = iRand->Uniform(0,1);

  double pTKVal0 = iRand->Uniform(0,1);
  double pTKVal1 = iRand->Uniform(0,1);

  double pPFCorr1     = iRand->Gaus(0,1);     double pPFCorr2     = iRand->Gaus(0,1);  
  //double pPFCorrT1    = iRand->Gaus(0,1);     double pPFCorrT2    = iRand->Gaus(0,1);  

  double pTKCorr1     = iRand->Gaus(0,1);     double pTKCorr2     = iRand->Gaus(0,1);  
  //double pTKCorrT1    = iRand->Gaus(0,1);     double pTKCorrT2    = iRand->Gaus(0,1);  

  double lPFU1U2  = TMath::Max(iPFU1U2Corr->Eval(iGenPt),0.);//iPFU1U2Corr->Eval(iGenPt) ,0.);
  double lTKU1U2  = TMath::Max(iTKU1U2Corr->Eval(iGenPt),0.);//iTKU1U2Corr->Eval(iGenPt) ,0.);
  double lPFTKU1  = iPFTKU1Corr ->Eval(iGenPt); //TMath::Max(iPFTKU1Corr->Eval(iGenPt) ,0.);
  double lPFTKU2  = iPFTKU2Corr ->Eval(iGenPt);//TMath::Max(iPFTKU2Corr->Eval(iGenPt) ,0.);
  double lPFTKU1M = iPFTKU1MCorr->Eval(iGenPt);
  double lPFTKU2M = iPFTKU2MCorr->Eval(iGenPt);

  //Needs some more work
  double pScale = 1.; //pPFFrac1 = (pPFFrac1+pPFFrac2)/2.; //pPFFrac2 = pPFFrac1;//+pPFFrac2)/2.;
  pPFSigma1_1 = ((pPFVal0 < pPFFrac1)*(pPFSigma1_1)+(pPFVal0 > pPFFrac1)*(pPFSigma1_2)); pPFVal0 = -1;
  pPFSigma2_1 = ((pPFVal1 < pPFFrac2)*(pPFSigma2_1)+(pPFVal1 > pPFFrac2)*(pPFSigma2_2)); pPFVal1 = -1;
  pTKSigma1_1 = (pTKVal0 < pTKFrac1)*(pTKSigma1_1)+(pTKVal0 > pTKFrac1)*(pTKSigma1_2);   pTKVal0 = -1;
  pTKSigma2_1 = (pTKVal1 < pTKFrac2)*(pTKSigma2_1)+(pTKVal1 > pTKFrac2)*(pTKSigma2_2);   pTKVal1 = -1;
  
  //double pPFSigma1_1_old = pPFSigma1_1*(lMeanU2/lMeanU1);
  //double pPFVal1_1 = correlatedSeed(pPFSigma1_1*pScale,lPFU1U2,lPFTKU1,lPFTKU1M,pPFSigma1_1,pPFSigma2_1*(lMeanU1/lMeanU2),pTKCorr1,pTKCorr2);
  //double pPFVal2_1 = correlatedSeed(pPFSigma2_1*pScale,lPFU1U2,lPFTKU2,lPFTKU2M,pPFSigma2_1,pPFSigma1_1_old,pTKCorr2,pTKCorr1);

  double pPFVal1_1 = correlatedSeed(pPFSigma1_1*pScale,lPFU1U2,lPFTKU1,lPFTKU1M,pPFCorr1,pPFCorr2,pTKCorr1,pTKCorr2);
  double pPFVal2_1 = correlatedSeed(pPFSigma2_1*pScale,lPFU1U2,lPFTKU2,lPFTKU2M,pPFCorr2,pPFCorr1,pTKCorr2,pTKCorr1);
  double pTKVal1_1 = correlatedSeed(pTKSigma1_1*pScale,lTKU1U2,lPFTKU1,lPFTKU2M,pTKCorr1,pTKCorr2,pPFCorr1,pPFCorr2);
  double pTKVal2_1 = correlatedSeed(pTKSigma2_1*pScale,lTKU1U2,lPFTKU2,lPFTKU1M,pTKCorr2,pTKCorr1,pPFCorr2,pPFCorr1);
   
  //double pPFVal1_2 = correlatedSeed(pPFSigma1_2*pScale,lPFU1U2,lPFTKU1,lPFTKU1M,pPFCorrT1,pPFCorrT2,pTKCorrT1,pTKCorrT2);
  //double pPFVal2_2 = correlatedSeed(pPFSigma2_2*pScale,lPFU1U2,lPFTKU2,lPFTKU2M,pPFCorrT2,pPFCorrT1,pTKCorrT2,pTKCorrT1);
  //double pTKVal1_2 = correlatedSeed(pTKSigma1_2*pScale,lTKU1U2,lPFTKU1,lPFTKU2M,pTKCorrT1,pTKCorrT2,pPFCorrT1,pPFCorrT2);
  //double pTKVal2_2 = correlatedSeed(pTKSigma2_2*pScale,lTKU1U2,lPFTKU2,lPFTKU1M,pTKCorrT2,pTKCorrT1,pPFCorrT2,pPFCorrT1);

  pPFU1   = (pPFVal0 < pPFFrac1)*(pPFVal1_1+pPFU1);//+(pPFVal0 > pPFFrac1)*(pPFVal1_2+pPFU1);
  pPFU2   = (pPFVal1 < pPFFrac2)*(pPFVal2_1+pPFU2);//+(pPFVal1 > pPFFrac2)*(pPFVal2_2+pPFU2);
  pTKU1   = (pTKVal0 < pTKFrac1)*(pTKVal1_1+pTKU1);//+(pTKVal0 > pTKFrac1)*(pTKVal1_2+pTKU1);
  pTKU2   = (pTKVal1 < pTKFrac2)*(pTKVal2_1+pTKU2);//+(pTKVal1 > pTKFrac2)*(pTKVal2_2+pTKU2);

  iPFMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pPFU1,pPFU2);
  iPFMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pPFU1,pPFU2);
  iTKMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pTKU1,pTKU2);
  iTKMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pTKU1,pTKU2);
  iU1 = pTKU1;
  iU2 = pTKU2;
  return;
  //Not used right now
  //iPFTKU2Corr->Eval(0);
  //iPFTKU1MCorr.Eval(0);
  //iPFTKU2MCorr.Eval(0);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::calculate(int iMet,double iEPt,double iEPhi,double iWPhi,double iU1,double iU2) { 
  double lMX = -iEPt*cos(iEPhi) - iU1*cos(iWPhi) + iU2*sin(iWPhi);
  double lMY = -iEPt*sin(iEPhi) - iU1*sin(iWPhi) - iU2*cos(iWPhi);
  if(iMet == 0) return sqrt(lMX*lMX + lMY*lMY);
  if(iMet == 1) {if(lMX > 0) {return atan(lMY/lMX);} return (fabs(lMY)/lMY)*3.14159265 + atan(lMY/lMX); } 
  if(iMet == 2) return lMX;
  if(iMet == 3) return lMY;
  return lMY;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::getCorError2(double iVal,TF1 *iFit) { 
  double lE = sqrt(iFit->GetParError(0))  + iVal*sqrt(iFit->GetParError(2));
  if(fabs(iFit->GetParError(4)) > 0) lE += iVal*iVal*sqrt(iFit->GetParError(4));
  return lE*lE;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::getError2(double iVal,TF1 *iFit) { 
  double lE2 = iFit->GetParError(0) + iVal*iFit->GetParError(1) + iVal*iVal*iFit->GetParError(2);
  if(fabs(iFit->GetParError(3)) > 0) lE2 += iVal*iVal*iVal*     iFit->GetParError(3);
  if(fabs(iFit->GetParError(4)) > 0) lE2 += iVal*iVal*iVal*iVal*iFit->GetParError(4);
  if(fabs(iFit->GetParError(5)) > 0 && iFit->GetParameter(3) == 0) lE2 += iVal*iVal*               iFit->GetParError(5);
  if(fabs(iFit->GetParError(5)) > 0 && iFit->GetParameter(3) != 0) lE2 += iVal*iVal*iVal*iVal*iVal*iFit->GetParError(5);
  if(fabs(iFit->GetParError(6)) > 0) lE2 += iVal*iVal*iVal*iVal*iVal*iVal*iFit->GetParError(6);
  return lE2;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::getError(double iVal,TF1 *iFit,Recoil iType) {
  if(fId == 0) return sqrt(getError2(iVal,iFit));
  if(fId != 2) return sqrt(getError2(iVal,iFit)); 
  double lEW2  = getError2(iVal,iFit);
  double lEZD2 = getError2(iVal,getFunc(true ,iType));
  double lEZM2 = getError2(iVal,getFunc(false,iType));
  double lZDat = getFunc(true ,iType)->Eval(iVal);
  double lZMC  = getFunc(false,iType)->Eval(iVal);
  double lWMC  = iFit                ->Eval(iVal);
  double lR    = lZDat/lZMC;
  double lER   = lR*lR/lZDat/lZDat*lEZD2 + lR*lR/lZMC/lZMC*lEZM2;
  double lVal  = lR*lR*lEW2 + lWMC*lWMC*lER;
  return lVal;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::mag(double iV0,double iV1,double iV2,double iV3) { 
  return sqrt(iV0+iV1*iV1+2*iV1*0.88 + iV2*iV2+2.*iV2*0.88+ iV3*iV3+2.*iV3*0.88);//
  //return sqrt(iV0*iV0 + iV1*iV1 + iV2*iV2 + iV3*iV3);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::correlatedSeed(double iVal, double iCorr1,double iCorr2,double iCorr3,double iSeed0,double iSeed1,double iSeed2,double iSeed3) { 
  double lMag = mag(1.,iCorr1,iCorr2,iCorr3); 
  //double lVal = ((1./lMag)*iSeed0 + (iCorr1/lMag)*iSeed1 + (iCorr2/lMag)*iSeed2 + (iCorr3/lMag)*iSeed3)*iVal;
  double lVal = ((1./lMag) + (iCorr1/lMag)*fabs(iSeed1) + (iCorr2/lMag)*fabs(iSeed2) + (iCorr3/lMag)*fabs(iSeed3))*iVal;
  lVal*=iSeed0;
  return lVal;
}
