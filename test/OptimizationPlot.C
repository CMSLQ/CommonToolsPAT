
// root -l -q OptimizationPlot.C+\(100\)

//   Suppose a macro myscript.C with a function myscript(int i, float x)
//   you can execute this macro in batch, with eg:
//   root -b -q "myscript.C(5,3.14)"
//   If one of the arguments is a string, you must use the form with teh backslash
//   \"a string\"

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



// void printCuts(int cuts){
//   double var[6];
//   double varMax[6];
//   double varStep[6];
//   int NSteps=10;
//   string varName[6];

//   //Mee                             OPT           >         80              260             1
//   //sT                              OPT           >         520             700             1
//   //maxMEtaEles_IDISO_NoOvrl        OPT           <         0.75            3.0             1
//   //maxMEtaJets_noOvrlpEle          OPT           <         0.6             4.2             1
//   varName[0]="Mee   "; var[0]=80;   varMax[0]=260;
//   varName[1]="sT    "; var[1]=520;  varMax[1]=700;
//   varName[2]="etaEle"; var[2]=0.75; varMax[2]=3.0;
//   varName[3]="etaJet"; var[3]=0.6;  varMax[3]=4.2;

//   for(int j=0; j<6; j++){
//     varStep[j]=(varMax[j]-var[j])/(NSteps-1);
//   }
//   int count =0;
//   for(int j=0; j<4; j++){
//     for(int i=0; i<NSteps; i++){
//       count++;
//       var[j] = var[j]+varStep[j];
//       std::cout << varName[j] << " = " << var[j]<<"\t" << "count = "<< count << std::endl;
//     }  
//   }
// }


void OptimizationPlot(int nbins = 100)
{

  //nbins=10000;
  cout << "nbins = " << nbins << endl;

  TFile file1("/home/prumerio/cms/phys/lq/2010/opt/rootNtupleAnalyzerPAT/data/output/analysisClass_eejjSample_plots.root");
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
  double ILum = 100; // pb-1
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
  for (int i=1; i<=nbins; i++){ // Bin = 0 is the underflow
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
    upperLimit = CLA(ILum, ILum*ILum_err_rel, eff, eff*eff_err_rel, Nb, Nb*Nb_err_rel);
    UpperLimit.SetBinContent(i,upperLimit);
    if (upperLimit<minUpperLimit) {
      minUpperLimit = upperLimit;
      bin_fromUpperLimit = i;  
    }
  }

  //char *ss = new char[1]; 
  //std::cout << "Press Enter to continue"<<std::endl;  gets(ss); 

  //  print output from significance method
  TCanvas * c2 = new TCanvas;
  Signif.Draw();
  std::cout << "maxSignif: " << maxSignif << "\t" << "Bin_FromSignif - 1 (as bin zero is overflow): " << bin_fromSignif - 1 << std::endl;
  std::cout << "Significance: " << Signif.GetBinContent(bin_fromSignif) << std::endl;
  std::cout << "Ns = " << Signal.GetBinContent(bin_fromSignif) << "\t" << "Nb = " << Backgnd.GetBinContent(bin_fromSignif) << std::endl;
  c2->Print("Signif.gif","gif");

  // print output from upper limit method
  TCanvas * c3 = new TCanvas;
  UpperLimit.Draw();
  std::cout << "UpperLimit: " << minUpperLimit << "\t" << "Bin_FromUpperLimit - 1 (as bin zero is overflow): " << bin_fromUpperLimit - 1 << std::endl;
  std::cout << "Ns = " << Signal.GetBinContent(bin_fromUpperLimit) << "\t" << "Nb = " << Backgnd.GetBinContent(bin_fromUpperLimit) << std::endl;
  c3->Print("UpperLimit.gif","gif");

  //printCuts(cuts);  

  //std::cout << "Press Enter to continue"<<std::endl;  gets(ss); 

  TFile outfile("OptimizationPlot.root","recreate");
  Signif.Write();
  UpperLimit.Write();
  outfile.Close();
 
//c1->Print("Signif.gif","gif");




// TCanvas c2;
// c2->cd();

// TH2D * ele1pt_ele2pt = new TH2D("","",10,0,10,10,0,10);

// int ele1ptBin = 0;
// int ele2ptBin = 0;
// double MaxSig = 0;
// for (int j=1;j<10000000;j++){
//   ele1ptBin = j / 100000 + 1;
//   ele2ptBin = (j / 10000)%10 + 1;
//   double z = Signif.GetBinContent(j);
//   if (z>MaxSig) MaxSig=z;
//   if (j%10000==0) MaxSig=0;
//   //ele1pt_ele2pt.SetBinContent(ele1ptBin,ele2ptBin,z);
//   ele1pt_ele2pt.SetBinContent(ele1ptBin,ele2ptBin,MaxSig);
// }
// gStyle->SetPalette(1);
// //ele1pt_ele2pt->Draw("LEGO2");
// ele1pt_ele2pt->Draw("SURF3");
// ele1pt_ele2pt->GetXaxis().SetTitle("ele1ptBin");
// ele1pt_ele2pt->GetYaxis().SetTitle("ele2ptBin");
// c2->Print("ele1ele2.gif","gif");

// TCanvas c3;
// c3->cd();

// TH2D * ele1pt_jet1pt = new TH2D("","",10,0,10,10,0,10);

// ele1ptBin = 0;
// int jet1ptBin = 0;
// double MaxSig_1 = 0;
// double MaxSig_2 = 0;
// double MaxSig_3 = 0;
// double MaxSig_4 = 0;
// double MaxSig_5 = 0;
// double MaxSig_6 = 0;
// double MaxSig_7 = 0;
// double MaxSig_8 = 0;
// double MaxSig_9 = 0;
// double MaxSig_10 = 0;
// for (int j=1;j<10000000;j++){
//   ele1ptBin = j / 1000000 + 1;
//   jet1ptBin = (j / 10000)%10 + 1;
//   double z = Signif.GetBinContent(j);
//   if ((jet1ptBin==1)&&(MaxSig_1<z)){
//     MaxSig_1=z;
//   ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_1);
//   }
//   if ((jet1ptBin==2)&&(MaxSig_2<z)){
//     MaxSig_2=z;
//   ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_2);
//   }
//   if ((jet1ptBin==3)&&(MaxSig_3<z)){
//     MaxSig_3=z;
//   ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_3);
//   }
//   if ((jet1ptBin==4)&&(MaxSig_4<z)){
//     MaxSig_4=z;
//   ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_4);
//   }
//   if ((jet1ptBin==5)&&(MaxSig_5<z)){
//     MaxSig_5=z;
//   ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_5);
//   }
//   if ((jet1ptBin==6)&&(MaxSig_6<z)){
//     MaxSig_6=z;
//   ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_6);
//   }
//   if ((jet1ptBin==7)&&(MaxSig_7<z)){
//     MaxSig_7=z;
//   ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_7);
//   }
//   if ((jet1ptBin==8)&&(MaxSig_8<z)){
//     MaxSig_8=z;
//   ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_8);
//   }
//   if ((jet1ptBin==9)&&(MaxSig_9<z)){
//     MaxSig_9=z;
//   ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_9);
//   }
//   if ((jet1ptBin==10)&&(MaxSig_10<z)){
//     MaxSig_10=z;
//   ele1pt_jet1pt.SetBinContent(ele1ptBin,jet1ptBin,MaxSig_10);
//   }
//   if (j%1000000==0){
//    MaxSig_1 = 0;
//    MaxSig_2 = 0;
//    MaxSig_3 = 0;
//    MaxSig_4 = 0;
//    MaxSig_5 = 0;
//    MaxSig_6 = 0;
//    MaxSig_7 = 0;
//    MaxSig_8 = 0;
//    MaxSig_9 = 0;
//    MaxSig_10 = 0;
//   }
// }
// gStyle->SetPalette(1);
// //ele1pt_jet1pt->Draw("LEGO2");
// ele1pt_jet1pt->Draw("SURF3");
// ele1pt_jet1pt->GetXaxis().SetTitle("ele1ptBin");
// ele1pt_jet1pt->GetYaxis().SetTitle("jet1ptBin");
// c3->Print("ele1jet1.gif","gif");

// TCanvas c4;
// c4->cd();

// TH2D * ele1pt_sT = new TH2D("","",10,0,10,10,0,10);
// TH1D * sT = new TH1D("","",10,0,10);

// ele1ptBin = 0;
// int StBin = 0;
// double MaxSig_1 = 0;
// double MaxSig_2 = 0;
// double MaxSig_3 = 0;
// double MaxSig_4 = 0;
// double MaxSig_5 = 0;
// double MaxSig_6 = 0;
// double MaxSig_7 = 0;
// double MaxSig_8 = 0;
// double MaxSig_9 = 0;
// double MaxSig_10 = 0;
// for (int j=1;j<10000000;j++){
//   ele1ptBin = j / 1000000 + 1;
//   StBin = (j / 100)%10 + 1;
//   //cout << StBin << "\t" << j << endl;
//   double z = Signif.GetBinContent(j);
//   if ((StBin==1)&&(MaxSig_1<z)){
//     MaxSig_1=z;
//   ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_1);
//   }
//   if ((StBin==2)&&(MaxSig_2<z)){
//     MaxSig_2=z;
//   ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_2);
//   }
//   if ((StBin==3)&&(MaxSig_3<z)){
//     MaxSig_3=z;
//   ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_3);
//   }
//   if ((StBin==4)&&(MaxSig_4<z)){
//     MaxSig_4=z;
//   ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_4);
//   }
//   if ((StBin==5)&&(MaxSig_5<z)){
//     MaxSig_5=z;
//   ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_5);
//   }
//   if ((StBin==6)&&(MaxSig_6<z)){
//     MaxSig_6=z;
//   ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_6);
//   }
//   if ((StBin==7)&&(MaxSig_7<z)){
//     MaxSig_7=z;
//   ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_7);
//   }
//   if ((StBin==8)&&(MaxSig_8<z)){
//     MaxSig_8=z;
//   ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_8);
//   }
//   if ((StBin==9)&&(MaxSig_9<z)){
//     MaxSig_9=z;
//   ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_9);
//   }
//   if ((StBin==10)&&(MaxSig_10<z)){
//     MaxSig_10=z;
//   ele1pt_sT.SetBinContent(ele1ptBin,StBin,MaxSig_10);
//   }
//   if (j%1000000==0){  //moving to next ele1ptBin, reset sT max values
//    MaxSig_1 = 0;
//    MaxSig_2 = 0;
//    MaxSig_3 = 0;
//    MaxSig_4 = 0;
//    MaxSig_5 = 0;
//    MaxSig_6 = 0;
//    MaxSig_7 = 0;
//    MaxSig_8 = 0;
//    MaxSig_9 = 0;
//    MaxSig_10 = 0;
//   }
// }
// sT->SetBinContent(1,MaxSig_1);
// sT->SetBinContent(2,MaxSig_2);
// sT->SetBinContent(3,MaxSig_3);
// sT->SetBinContent(4,MaxSig_4);
// sT->SetBinContent(5,MaxSig_5);
// sT->SetBinContent(6,MaxSig_6);
// sT->SetBinContent(7,MaxSig_7);
// sT->SetBinContent(8,MaxSig_8);
// sT->SetBinContent(9,MaxSig_9);
// sT->SetBinContent(10,MaxSig_10);

// gStyle->SetPalette(1);
// //ele1pt_sT->Draw("LEGO2");
// ele1pt_sT->Draw("SURF3");
// ele1pt_sT->GetXaxis().SetTitle("ele1ptBin");
// ele1pt_sT->GetYaxis().SetTitle("StBin");
// //sT->Draw("hist");
// //sT->GetXaxis().SetTitle("sT Bin");
// c4->Print("sT.gif","gif");

}





