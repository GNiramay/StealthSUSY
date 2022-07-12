///////////////////////////////////////////////////
// ROOT is told to initialize with this file     //
// By Modifying the following file		 // 
// /etc/root/system.rootrc -> Line 285	         // 
// Instead of the default line		         // 
//  " Rint.Logon:rootlogon.C "		         //
// The modified line reads as:		         // 
// "Rint.Logon: /home/niramay/rootlogon.C"       // 
// The script below was copied from :            //
// https://home.fnal.gov/~apana/cms/.rootlogon.C //
///////////////////////////////////////////////////



{
  printf("Using /home/niramay/NIRAMAY/Work/Stealth_SUSY/NTuples_Analysis/ROOT_OUT/rootlogon.C\n");
  TStyle *myStyle  = new TStyle("MyStyle","My Root Styles");

  // from ROOT plain style
  myStyle->SetCanvasBorderMode(0);
  myStyle->SetPadBorderMode(0);
  myStyle->SetPadColor(0);
  myStyle->SetCanvasColor(0);
  myStyle->SetTitleColor(1);
  myStyle->SetStatColor(0);

  myStyle->SetOptStat(111111);
  myStyle->SetOptFit(1111);

  // myStyle->SetLabelSize(0.03,"xyz"); // size of axis values
  // // myStyle->SetLabelSize(0.,"xyz"); // size of axis values

  // // default canvas positioning
  // myStyle->SetCanvasDefX(900);
  // myStyle->SetCanvasDefY(20);
  // myStyle->SetCanvasDefH(550);
  // myStyle->SetCanvasDefW(540);

  // myStyle->SetPadBottomMargin(0.1);
  // myStyle->SetPadTopMargin(0.1);
  // myStyle->SetPadLeftMargin(0.1);
  // myStyle->SetPadRightMargin(0.1);


  // myStyle->SetPadTickX(1);
  // myStyle->SetPadTickY(1);

  // myStyle->SetFrameBorderMode(0);

  // // // US letter
  // // myStyle->SetPaperSize(20, 24);

  gROOT->SetStyle("MyStyle"); // comment to suppress this style
}
