
{

  #define NH 6 // Number of histograms to plot

  TFile * infile = TFile::Open("data/output/analysisClass_eejjSample_plots.root");

  TCanvas c1; 
  c1.SetLogy();       
  
  TH1F ah[10];
  TString as[10];
  TString ak[10];

  as[0] =  "histo1D__ZeeJet_Pt__sT_Mee80_100";
  as[1] =  "histo1D__ZeeJet_Pt__sT_Mee100_120";
  as[2] =  "histo1D__ZeeJet_Pt__sT_Mee120_150";
  as[3] =  "histo1D__ZeeJet_Pt__sT_Mee150_200";
  as[4] =  "histo1D__ZeeJet_Pt__sT_Mee200_300";
  as[5] =  "histo1D__ZeeJet_Pt__sT_Mee300_inf";
   
  ak[0] =  "sT with  80 < Mee < 100 GeV";
  ak[1] =  "sT with 100 < Mee < 120 GeV";
  ak[2] =  "sT with 120 < Mee < 150 GeV";
  ak[3] =  "sT with 150 < Mee < 200 GeV";
  ak[4] =  "sT with 200 < Mee < 300 GeV";
  ak[5] =  "sT with 300 < Mee GeV";

  TString xtitle = "Mee [GeV]";


  for(int i=0; i<NH; i++)
    { 
      ah[i] = (TH1F) infile->Get(as[i]);
      ah[i]->Rebin(2);
      ah[i]->SetMarkerSize(0.5);
      ah[i]->SetMarkerStyle(20+i);
      ah[i]->SetMarkerColor(i+1);
      ah[i]->SetLineColor(i+1);
      if(i == 0) 
	{ 
	  ah[i]->GetXaxis()->SetRangeUser(0,1500); 
	  ah[i]->GetYaxis()->SetRangeUser(0.0001,300); 
          ah[i]->GetXaxis().SetTitle(xtitle);
	  ah[i]->Draw("histep"); 
	}
      else       
	{ 
	  ah[i]->Draw("same,histep"); 
	}
    }

  TLegend * legend = new TLegend(0.40,0.60,0.77,0.90);
  legend->SetFillColor(kWhite);
  for(int i=0; i<NH; i++)
    {
      legend->AddEntry(as[i],ak[i],"lp");
    }
  legend->Draw();

  //  c1->Print("plotSeveralHistograms.gif");


}
