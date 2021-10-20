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
    TChain* fChain=new TChain("events");
    fChain->Add(Form("%s/dcmqgsm_%d.root", inFile, 1+(Event-1)/nEvents));
    vector<TLorentzVector> Positions;
    TLorentzVector NucleonPosition;
    UEvent* fEvent = new UEvent;
    EventInitialState* fIniState = new EventInitialState;
    fChain->SetBranchAddress("event", &fEvent); 
    fChain->SetBranchAddress("iniState", &fIniState);
    fChain->GetEntry(Event);
    
    if(ifImpactParameter)
    {
        TLorentzVector ImpactParameter(fEvent->GetBx(),fEvent->GetBy(),0.,0.);
        Positions.push_back(ImpactParameter);
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
        return Positions;
    }  
}
