
{

  #define NH 5 // Number of histograms to plot

  TFile * infile = TFile::Open("../../rootNtupleAnalyzerPAT/data/output/analysisClass_eejjSample_plots.root");

  TCanvas c1; 
  c1.SetLogy();       
  c1->Divide(Int_t nx = 2, Int_t ny = 3, Float_t xmargin = 0.01, Float_t ymargin = 0.01, Int_t color = 0);
  
  TH1F ah[10];
  TString as[10];
  TString ak[10];

  as[0] =  "histo1D__ZJetAlpgen__cutHisto_skim___________________sT";
  as[1] =  "histo1D__Pt0to100__cutHisto_skim___________________sT";
  as[2] =  "histo1D__Pt100to300__cutHisto_skim___________________sT";
  as[3] =  "histo1D__Pt300to800__cutHisto_skim___________________sT";
  as[4] =  "histo1D__Pt800to1600__cutHisto_skim___________________sT";
   
  ak[0] =  "sT";
  ak[1] =  "sT, Pt0-100";
  ak[2] =  "sT, Pt100-300";
  ak[3] =  "sT, Pt300-800";
  ak[4] =  "sT, Pt800-1600";

  TString xtitle = "sT [GeV]";

  //  c1->Print("tmp.pdf[","pdf");
  //c1->Print("tmp.ps[");
   
  for(int i=0; i<NH; i++)
    { 
      //      ac[0].SetLogy();
      (c1->cd(i+1))->SetLogy();
      ah[i] = (TH1F) infile->Get(as[i]);
      ah[i]->Rebin(2);
      ah[i]->SetMarkerSize(0.5);
      //ah[i]->SetMarkerStyle(20+i);
      //ah[i]->SetMarkerColor(i+1);
      //ah[i]->SetLineColor(i+1);
      //ah[i]->GetXaxis()->SetRangeUser(0,1500); 
      //ah[i]->GetYaxis()->SetRangeUser(0.0001,300); 
      ah[i]->GetXaxis().SetTitle(xtitle);
      ah[i]->Draw("histep"); 

      TLegend * legend = new TLegend(0.35,0.70,0.55,0.90);
      legend->SetTextSize(0.07);
      legend->SetFillColor(kWhite);
      legend->AddEntry(as[i],ak[i],"l");
      legend->Draw();
      //      c1->Print("tmp.pdf","pdf");
      //c1->Print("tmp.ps");
      //delete legend;
    }

  //    c1->Print("tmp.pdf]","pdf");
  //c1->Print("tmp.ps]");

  c1->Print("sT.gif");

}
