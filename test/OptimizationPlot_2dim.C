
// Makes 2d plot of the optimization variables by running on the root file
// produced by OptimizationPlot.C when run with 2 variables, 10 steps each


//void OptimizationPlot_2dim(int nbins = 100)
{
  // User inputs here
  int nbins = 100;
  TFile file1("OptimizationPlot.root");
  TH2D * Mee_St = new TH2D("","",10,80,125,10,500,680);

  file1.cd();  
  TH1D UpperLimit;
  ((TH1D*)gDirectory->Get("Upper Limit"))->Copy(UpperLimit); UpperLimit.Sumw2();
  
  TCanvas * c4 = new TCanvas;
  int xbin,ybin;
  for (int j=0;j<nbins;j++){
    xbin = j/10 + 1;
    ybin = j%10 + 1; 
    double z = UpperLimit.GetBinContent(j+1); // bin 0 is underflow in root
    cout<< xbin <<"\t"<<ybin<<"\t"<<z<<endl;
    Mee_St->SetBinContent(xbin,ybin,z);
  }
  gStyle->SetPalette(1);
  Mee_St->Draw("SURF3");
  //Mee_St->Draw("COLOR");
  Mee_St->GetXaxis().SetTitle("Mee");
  Mee_St->GetYaxis().SetTitle("S_T");
  c4->Print("Mee_St.gif","gif");
 
}


