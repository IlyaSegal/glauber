#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TMath.h"
#include <TLorentzVector.h>
#include <TDatabasePDG.h>
#include <Nucleon.h>
#include <UEvent.h>
#include <EventInitialState.h>
#include <UParticle.h>
#include <URun.h>

#include <fstream>
#include <iostream>
using namespace std;

vector<TLorentzVector> readNucleonsPositions(char* inFile, Int_t nEvents, Int_t Event, Int_t fA, Bool_t ifImpactParameter = false)
{
    TFile *fIn = new TFile(inFile, "read");
    TTree *fTree = (TTree*) fIn->Get("events");
    vector<TLorentzVector> Positions;
    TLorentzVector NucleonPosition;
    UEvent* fEvent = new UEvent;
    EventInitialState* fIniState = new EventInitialState;
    fTree->SetBranchAddress("event", &fEvent); 
    fTree->SetBranchAddress("iniState", &fIniState);
    fTree->GetEntry(Event-1);
    
    if(ifImpactParameter)
    {
        TLorentzVector ImpactParameter(fEvent->GetBx(),fEvent->GetBy(),0.,0.);
        Positions.push_back(ImpactParameter);
        fIn->Close();
        return Positions;
    }
    else 
    {
        
        for (int nucleonIndex = 0; nucleonIndex < fA; nucleonIndex++)
        {
            NucleonPosition.SetXYZT((fIniState->getNucleon(nucleonIndex+1)).getPosition().Px()-fEvent->GetBx()/2, (fIniState->getNucleon(nucleonIndex+1)).getPosition().Py()-fEvent->GetBy()/2, (fIniState->getNucleon(nucleonIndex+1)).getPosition().Pz(), 0.);
            Positions.push_back(NucleonPosition);
        }
        for (int nucleonIndex = fA; nucleonIndex < 2*fA; nucleonIndex++)
        {
            NucleonPosition.SetXYZT((fIniState->getNucleon(nucleonIndex+1)).getPosition().Px()+fEvent->GetBx()/2, (fIniState->getNucleon(nucleonIndex+1)).getPosition().Py()+fEvent->GetBy()/2, (fIniState->getNucleon(nucleonIndex+1)).getPosition().Pz(), 0.);
            Positions.push_back(NucleonPosition);
        }
        fIn->Close();
        return Positions;
    }  
}
