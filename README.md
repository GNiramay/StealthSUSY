# StealthSUSY
Code to process flat ntuples produced using TreeMaker

### Note: Please make folders named PDF, ROOT_IN manually. This repo ignores all the pdf and root files, hence these folders did not get added in the repo.
The SUSY samples were generated at Fermilab CMS LPC cluster, following the instructions given at [Dr. Kenichi's twiki page](https://twiki.cern.ch/twiki/bin/viewauth/CMS/SUSYPrivateMCNoteKH). The specific commands can be found by clicking on "Show" button in the section " Private MC production info "

The program files generated using TTree.MakeClass() were modified to do the analysis.

## File description
1. **driver.C** : This is the main file that will be executed by root. This will
   * Open flat ntuple
   * initialize `PreSelection` class object
   * Loop over all the events
   * Make PDF of histograms
2. **PreSelection.h**: This is the header file. Defines the `PreSelection` class. Also, defines `Cut()` function. This is also the location where all the histogram handles are initialized
3. **PreSelection.C**: This file defines the following functions of `PreSelection` class
   * `Loop()`: The function that loops over all the events. This is the place where the histograms are filled.
   * `BookHistogram(TString ofroot)`: Opens the output root file, defines the histogram handles
   * `SortPt(vector<TLorentzVector>* tv)`: The function arranges TLorentzVecots according to their Pt in descending order.
   * `MakePDF(TString outkey)`: The function to create PDFs of the histograms.
4. **CommonFunctions.h**: A library of commonly used functions. This defines
   * A string list of particles normally used.
   * Class `MyPDGParticles` which stores particle information like particle name, mass, PDG ID
   * An object of class `MyPDGParticles`, with information about the commonly used particles.
   * `SaveMe(TCanvas*,TString,TString)`: Function to save a canvas as a PDF.
   * `void GetMaxY(vector<TH1F*>)`: Function that determines the maximum range to be used in case of overlaying a bunch of histograms.
   * `void Decorate()`: Decorates a histogram. ie, rebins, sets the line width, color, axis names etc.

The program can be run using a single command:

    root -l -b -q driver.C
