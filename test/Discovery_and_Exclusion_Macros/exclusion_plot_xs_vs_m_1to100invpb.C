#include "../CLA.C"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include "TPolyLine.h"
#include "TPad.h"
#include "TLatex.h"

using namespace std;

void style()
{
 gStyle->Reset("Default");
 gStyle->SetCanvasColor(0);
 gStyle->SetPadColor(0);
 gStyle->SetTitleFillColor(10);
 gStyle->SetCanvasBorderMode(0);
 gStyle->SetStatColor(0);
 gStyle->SetPadBorderMode(0);
 gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
 gStyle->SetPadTickY(1);
 gStyle->SetFrameBorderMode(0);
 gStyle->SetPalette(1);
   
   //gStyle->SetOptStat(kFALSE);
 gStyle->SetOptStat(111110);
 gStyle->SetOptFit(1);
 gStyle->SetStatFont(42);
 gStyle->SetPadLeftMargin(0.13);
 gStyle->SetPadRightMargin(0.07);
//    gStyle->SetTitleFont(42);
//    gStyle->SetTitleFont(42, "XYZ");
//    gStyle->SetLabelFont(42, "XYZ");
 gStyle->SetStatY(.9);
}

void makePlots()
{
 // **********************************************
 // *            Input parameters                *
 // **********************************************
 // switch to include/exclude sytematics uncertainties 

 // array of signal efficiencies
 Double_t S_eff[5] = {0.36, 0.43, 0.54, 0.59, 0.66};
 
 // array of N_background for 100 pb^-1
 Double_t N_bkg_100[5] = {10.35, 6.37, 2.83, 1.27, 1.27};
 
 // arrays of leptoquark masses
 Double_t m[12] = {100, 200, 250, 300, 400, 500, 600, 650, 700, 800, 900, 1000};

 // array of theoretical cross-sections for different leptoquark masses
 Double_t xsTh[12] = {446, 11.9, 3.48, 1.21, 0.206, 0.0463, 0.0124, 0.00676, 0.00377, 0.00124, 0.000437, 0.00016};
  
 // filename for the final plot (NB: changing the name extension changes the file format)
 string fileName = "xs95CL_vs_m.eps";
  
 // axes labels for the final plot  
 string title = ";m (GeV/c^{2});#beta^{2}#times#sigma (pb)";

 TH2F *bg = new TH2F("bg",title.c_str(), 100, 100., 650., 100, 0.01, 500.);
 bg->SetStats(kFALSE);
 bg->SetTitleOffset(1.,"X");
 bg->SetTitleOffset(1.5,"Y");

 // region excluded by Tevatron limits
 Double_t x_shaded[5] = {200,256,256,100,100};
 Double_t y_shaded[5] = {0.01,0.01,500,500,0.01};
 Double_t NEW_x_shaded[2] = {299,299};
 Double_t NEW_y_shaded[2] = {0.01,500};

 // PDF uncertainty band
 Double_t x_pdf[12] = {250,300,400,500,600,650,650,600,500,400,300,250};
 Double_t y_pdf[12] = {3.3321,1.142,0.187,0.04054,0.01046,0.00554,0.00792,0.01434,0.05206,0.225,1.278,3.6279};

 // **********************************************
 // *  Don't change anything below this point!   *
 // **********************************************

  // turn on/off batch mode
 gROOT->SetBatch(kTRUE);
 // set ROOT style
 style();
 
 TCanvas *c = new TCanvas("c","",800,800);
 c->cd();
 
 bg->Draw();

 TGraph *grshade = new TGraph(12,x_pdf,y_pdf);
 grshade->SetFillColor(kYellow);
 grshade->Draw("f");

 TPolyLine *pl = new TPolyLine(5,x_shaded,y_shaded,"F");
//  pl->SetFillStyle(3001);   
 pl->SetFillColor(kGray+1);   
 pl->Draw();
 TPolyLine *pl_NEW = new TPolyLine(2,NEW_x_shaded,NEW_y_shaded,"L");
 pl_NEW->SetLineStyle(6);   
 pl_NEW->SetLineWidth(3);   
 pl_NEW->Draw();

 gPad->RedrawAxis();
 
 Int_t size = sizeof(S_eff)/sizeof(*S_eff);

 // Upper Limits can be entered manually here when the call to CLA.C is commented below
 // Array of 95% CL upper limits on the cross section
 Double_t xsUp[5] = {0.23722, 0.166074, 0.10131, 0.0748393, 0.0669023};
 Double_t NEWx[5] = {250,300,400,500,600};
 Double_t STRLy[5]= {0.17612, 0.112823, 0.0709512, 0.051767, 0.0495704};//rescaled with Stirling plot
 Double_t CRSCy[5] = {0.198158,0.123589,0.080478,0.064229,0.0575465};//rescaled with cross section ratio

 // FastSim MLQ300 and Pythia Z+Jet rescaled to madgraph
 Double_t MCx[3] = {300,400,500};
 // Double_t MCy_Lumi1[3]  = {7.81, 5.74, 5.38}; 
 // Double_t MCy_Lumi10[3] = {0.995, 0.660, 0.584}; 
 // Double_t MCy_Lumi100[3] = {0.182, 0.095, 0.071};
 // FullSim MLQ300 and Alpgen Z+Jet
 Double_t MCy_Lumi03[2]  = {20.04, 17.92}; 
 Double_t MCy_Lumi1[2]  = {6.35, 5.54}; 
 //Double_t MCy_Lumi5[1]  = {1.46}; 
 Double_t MCy_Lumi10[2] = {0.809, 0.640};
 Double_t MCy_Lumi100[2] = {0.147, 0.092};

//  for(Int_t i = 0; i < size; i++){
//   
//   xsUp[i] = CLA(100, 0, S_eff[i], 0, N_bkg_100[i], 0);
//  }

 TGraph *xsTh_vs_m = new TGraph(size+3, m, xsTh);
 xsTh_vs_m->SetLineWidth(2);
 xsTh_vs_m->SetLineColor(kRed);
 xsTh_vs_m->SetFillColor(kYellow);
 xsTh_vs_m->SetMarkerSize(1.);
 xsTh_vs_m->SetMarkerStyle(22);
 xsTh_vs_m->SetMarkerColor(kRed);
 xsTh_vs_m->Draw("C");

 TGraph *STRLxsection = new TGraph(5, NEWx, STRLy);
 STRLxsection->SetMarkerStyle(23);
 STRLxsection->SetMarkerColor(kRed);
 STRLxsection->SetLineColor(kRed);
 STRLxsection->SetLineWidth(2);
 STRLxsection->SetLineStyle(9);
 STRLxsection->SetMarkerSize(.9);
 STRLxsection->Draw("CP");

 TGraph *CRSCxsection = new TGraph(5, NEWx, CRSCy);
 CRSCxsection->SetMarkerStyle(23);
 CRSCxsection->SetMarkerColor(kGreen);
 CRSCxsection->SetLineColor(kGreen);
 CRSCxsection->SetLineWidth(2);
 CRSCxsection->SetLineStyle(9);
 CRSCxsection->SetMarkerSize(.9);
 //CRSCxsection->Draw("CP");

 TGraph *MCxs_Lumi03 = new TGraph(2, MCx, MCy_Lumi03);
 MCxs_Lumi03->SetMarkerStyle(20);
 MCxs_Lumi03->SetMarkerColor(kYellow);
 MCxs_Lumi03->SetLineColor(kYellow);
 MCxs_Lumi03->SetLineWidth(2);
 MCxs_Lumi03->SetLineStyle(9);
 MCxs_Lumi03->SetMarkerSize(1.5);
 MCxs_Lumi03->Draw("P");

 TGraph *MCxs_Lumi1 = new TGraph(2, MCx, MCy_Lumi1);
 MCxs_Lumi1->SetMarkerStyle(20);
 MCxs_Lumi1->SetMarkerColor(kGreen);
 MCxs_Lumi1->SetLineColor(kGreen);
 MCxs_Lumi1->SetLineWidth(2);
 MCxs_Lumi1->SetLineStyle(9);
 MCxs_Lumi1->SetMarkerSize(1.5);
 MCxs_Lumi1->Draw("P");

//  TGraph *MCxs_Lumi5 = new TGraph(1, MCx, MCy_Lumi5);
//  MCxs_Lumi5->SetMarkerStyle(20);
//  MCxs_Lumi5->SetMarkerColor(kRed);
//  MCxs_Lumi5->SetLineColor(kGreen);
//  MCxs_Lumi5->SetLineWidth(2);
//  MCxs_Lumi5->SetLineStyle(9);
//  MCxs_Lumi5->SetMarkerSize(1.5);
//  MCxs_Lumi5->Draw("CP");

 TGraph *MCxs_Lumi10 = new TGraph(2, MCx, MCy_Lumi10);
 MCxs_Lumi10->SetMarkerStyle(20);
 MCxs_Lumi10->SetMarkerColor(kBlue);
 MCxs_Lumi10->SetLineColor(kBlue);
 MCxs_Lumi10->SetLineWidth(2);
 MCxs_Lumi10->SetLineStyle(9);
 MCxs_Lumi10->SetMarkerSize(1.5);
 MCxs_Lumi10->Draw("P");

 TGraph *MCxs_Lumi100 = new TGraph(2, MCx, MCy_Lumi100);
 MCxs_Lumi100->SetMarkerStyle(20);
 MCxs_Lumi100->SetMarkerColor(kRed);
 MCxs_Lumi100->SetLineColor(kRed);
 MCxs_Lumi100->SetLineWidth(2);
 MCxs_Lumi100->SetLineStyle(9);
 MCxs_Lumi100->SetMarkerSize(1.5);
 MCxs_Lumi100->Draw("P");

 TLegend *legend = new TLegend(.36,.73,.93,.90);
 legend->SetBorderSize(1);
 legend->SetFillColor(0);
 //legend->SetFillStyle(0);
 legend->SetMargin(0.15);
 legend->SetHeader("LQ #rightarrow eq");
 legend->AddEntry(pl,"Tevatron exclusion (250 pb^{-1})","f");
 legend->AddEntry(pl_NEW,"Tevatron exclusion (1 fb^{-1})","l");
 legend->AddEntry(xsTh_vs_m,"#beta^{2}#times #sigma_{theory} with PDF unc. band, #beta=1","lf");
 legend->AddEntry(STRLxsection,"Stirling Rescaling - 95% C.L. upper limit (no sys. unc.)","lp");
 //legend->AddEntry(CRSCxsection,"xsec ratio - 7TeV 95% C.L. upper limit (no sys. unc.)","lp");
 legend->AddEntry(MCxs_Lumi03, "MC @ 7TeV - 95% C.L. upper limit, L=300 nb-1","p");
 legend->AddEntry(MCxs_Lumi1,  "MC @ 7TeV - 95% C.L. upper limit, L=1 pb-1","p");
 legend->AddEntry(MCxs_Lumi10, "MC @ 7TeV - 95% C.L. upper limit, L=10 pb-1","p");
 legend->AddEntry(MCxs_Lumi100,"MC @ 7TeV - 95% C.L. upper limit, L=100 pb-1","p");
 legend->Draw();

 TLatex l1;
 l1.SetTextAlign(12);
 l1.SetTextSize(0.04);
 l1.SetTextFont(62);
 l1.SetNDC();
 l1.DrawLatex(0.6,0.64,"CMS Preliminary");

 TLatex l2;
 l2.SetTextAlign(12);
 l2.SetTextSize(0.04);
 l2.SetTextFont(62);
 l2.SetNDC();
 // l2.DrawLatex(0.6,0.55,"#intLdt=100 pb^{-1}");

 TLatex l3;
 l3.SetTextAlign(12);
 l3.SetTextSize(0.037);
 l3.SetTextFont(62);
 l3.SetNDC();
 // l3.DrawLatex(0.4,0.485,"EXO-08-010 scaled to #sqrt{s} = 7 TeV");

 c->SetGridx();
 c->SetGridy();

 string name, extension;
 size_t pos = fileName.find(".");

 name = fileName.substr(0,pos);
 extension = fileName.substr(pos);

 c->SetLogy();
 c->SaveAs((name + "_log" + extension).c_str());

 delete pl;
 delete xsTh_vs_m;
 delete bg;
 delete c;
}
