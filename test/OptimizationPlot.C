
// root -l -q OptimizationPlot.C+\(100\)
// where the argument is the number of bins in the optimization histograms
// See also UserCode/Leptoquarks/CommonToolsPAT/test/README


#include "CLA.C"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TF1.h"
#include "TFile.h"
#include "TLegend.h"
#include "TPolyLine.h"
#include "TPad.h"
#include "TLatex.h"



void OptimizationPlot(int nbins = 100)
{

  //nbins=10000;
  cout << "nbins = " << nbins << endl;

  //////////////////////////////
  // User inputs here
  ////////////////////////////// 
  TFile file1("../../rootNtupleAnalyzerPAT/data/output/analysisClass_eejjSample_plots.root");
  file1.cd();

  TH1D Signal;
  ((TH1D*)gDirectory->Get("histo1D__LQtoUE_M300__Optimizer"))->Copy(Signal); Signal.Sumw2();                 
  TH1D Backgnd;
  ((TH1D*)gDirectory->Get("histo1D__ALLBKG__Optimizer"))->Copy(Backgnd); Backgnd.Sumw2();                 

  double Ns_noCut( ((TH1D*)gDirectory->Get("histo1D__LQtoUE_M300__EventsPassingCuts"))->GetBinContent(1) ); // needed for CLA.C method only

  ////////////////////////////////////////////////
  // Parameters needed when using the CLA.C method 
  ////////////////////////////////////////////////
  double eff; // signal efficiency
  double eff_err_rel = 0.15; // Releative error on signal efficiency, dominated by ~10% from JES, ~10% from FastSim vs FullSim
  double Nb_err_rel = 0.30; // Releative error on background, dominated by JES
  double ILum = 10; // pb-1
  double ILum_err_rel = 0.10; // Releative error on Integrated Luminosity
  double upperLimit;
  
  TCanvas * c1 = new TCanvas;
  TPad * pad1 = new TPad("pad1","This is pad1",0.01,0.51,0.95,0.99);
  TPad * pad2 = new TPad("pad2","This is pad2",0.01,0.01,0.95,0.49);
  //pad1->SetFillColor(11);
  //pad2->SetFillColor(11);
  pad1->Draw();
  pad2->Draw();
  pad1->cd();
  Signal.Draw();
  pad2->cd();
  Backgnd.Draw();
  c1->Print("SignBkg.gif","gif");

  TH1D Signif ("Significance","Significance",nbins,0.0,nbins);
  TH1D UpperLimit ("Upper Limit","Upper Limit",nbins,0.0,nbins);

  double maxSignif = -1;
  int bin_fromSignif = -1;
  double minUpperLimit = 999999999;
  int bin_fromUpperLimit = -1;
  double Ns;
  double Nb;
  double NbErr;
  double NbErrAtMinUpLim;
  double effAtMinUpLim;
  for (int i=1; i<=nbins; i++){ // Bin = 0 is the underflow
    //if ( (i-1)%100 != 0 ) continue;
    //if ( (i-1) != 1200 ) continue;
    //if ( (i-1) < 2200 || (i-1) >= 2300 ) continue;
    //cout<<"i = "<<i<<endl;
    Ns = Signal.GetBinContent(i);
    Nb = Backgnd.GetBinContent(i);
    double signif = 0;

    // use significance method
    if ((Ns+Nb)>0) signif = Ns / sqrt(Ns+Nb);
    Signif.SetBinContent(i,signif);
    if (signif>maxSignif) {
      maxSignif = signif;
      bin_fromSignif = i;  
    }

    // use upper limit method
    eff = Ns/Ns_noCut;
    cout<< "Calling CLA for bin i = " << i << endl;
    //cout << ILum <<" "<< ILum*ILum_err_rel <<" "<<  eff <<" "<<  eff*eff_err_rel <<" "<<  Nb <<" "<<  Nb*Nb_err_rel << endl;
    NbErr = Nb*Nb_err_rel;
    upperLimit = CLA(ILum, ILum*ILum_err_rel, eff, eff*eff_err_rel, Nb, NbErr);
    UpperLimit.SetBinContent(i,upperLimit);
    if (upperLimit<minUpperLimit) {
      minUpperLimit = upperLimit;
      effAtMinUpLim = eff;
      NbErrAtMinUpLim=NbErr;
      bin_fromUpperLimit = i;  
    }
  }

  //char *ss = new char[1]; 
  //std::cout << "Press Enter to continue"<<std::endl;  gets(ss); 

  //  print output from significance method
  TCanvas * c2 = new TCanvas;
  Signif.Draw();
  std::cout << "maxSignif: " << maxSignif << "\t" << "Bin_FromSignif - 1 (as bin zero is overflow): " << bin_fromSignif - 1 << std::endl;
  //std::cout << "Significance: " << Signif.GetBinContent(bin_fromSignif) << std::endl;
  std::cout << "ILum = " << ILum << "\t Ns = " << Signal.GetBinContent(bin_fromSignif) << "\t Nb = " << Backgnd.GetBinContent(bin_fromSignif) << std::endl;
  c2->Print("Signif.gif","gif");

  // print output from upper limit method
  TCanvas * c3 = new TCanvas;
  UpperLimit.Draw();
  std::cout << "UpperLimit: " << minUpperLimit << "\t" << "Bin_FromUpperLimit - 1 (as bin zero is overflow): " << bin_fromUpperLimit - 1 << std::endl;
  std::cout << "ILum = " << ILum << "\t Ns = " << Signal.GetBinContent(bin_fromUpperLimit) << "\t Nb = " << Backgnd.GetBinContent(bin_fromUpperLimit) << "\t Ns/sqrt(Ns+Nb) = " << Signal.GetBinContent(bin_fromUpperLimit)/sqrt(Signal.GetBinContent(bin_fromUpperLimit)+Backgnd.GetBinContent(bin_fromUpperLimit))  << std::endl;
  std::cout << "effAtMinUpLim = " << effAtMinUpLim << "\t" << "NbErrAtMinUpLim = "<< NbErrAtMinUpLim <<std::endl;
  c3->Print("UpperLimit.gif","gif");

  //printCuts(cuts);  

  //std::cout << "Press Enter to continue"<<std::endl;  gets(ss); 

  TFile outfile("OptimizationPlot.root","recreate");
  Signif.Write();
  UpperLimit.Write();
  outfile.Close();

}





