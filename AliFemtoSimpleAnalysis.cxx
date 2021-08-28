// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"
#include "Framework/AnalysisDataModel.h"
#include "Framework/HistogramRegistry.h"
#include "AnalysisDataModel/PID/PIDResponse.h"
#include "AnalysisDataModel/PID/PIDTOF.h"
#include "AnalysisDataModel/TrackSelectionTables.h"
#include "AnalysisCore/MC.h"
#include "TPDGCode.h"
#include <TH1F.h>
#include <TParameter.h>

#include <cmath>

using namespace o2;
using namespace o2::framework;
using namespace o2::aod;
using namespace o2::framework::expressions;
using namespace MC;



struct BasicHistogramsNoPID {
  HistogramRegistry registry{
    "registry",
    {
      // name, title, hist type, vector of axes
      // here each axis is defined as: {nBins, min, max, optional title, optional name}
      // alternatively, for variable binning: {{binEdge1, binEdge2, ...}, optional title, optional name}
      {"eta", "#eta", {HistType::kTH1F, {{102, -2.01, 2.01}}}},                     //
      {"phi", "#varphi", {HistType::kTH1F, {{100, 0., 2. * M_PI}}}},                //
      {"pt", "pt", {HistType::kTH1F, {{100, -0.01, 10.01, "#it{p}_{T} (GeV/c)"}}}}, //
      {"ptVariableBinning", "pt", {HistType::kTH1F, {{                                                        //
                                                       {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, //
                                                        1.1, 1.2, 1.3, 1.4, 1.5, 2.0, 5.0, 10.0, 20.0, 50.0},
                                                       "#it{p}_{T} (GeV/c)"}}}}, //
      {"EtaVsPhi", "#eta vs #varphi", {HistType::kTH2F, {{100, 0, 2*TMath::Pi(), "#varphi (rad)"}, {100, -1.2, 1.2, "#eta"}}}}////
    }
  };

  // aod::Track is an iterator over tracks table
  Filter trackFilter = aod::track::isGlobalTrack == (uint8_t)true;
  void process(soa::Filtered<soa::Join<aod::Tracks,aod::TrackSelection>>::iterator const& track)
   //void process(aod::FullTrack const& track)
  {

    registry.get<TH1>(HIST("eta"))->Fill(track.eta());
    registry.get<TH1>(HIST("phi"))->Fill(track.phi());
    registry.get<TH1>(HIST("pt"))->Fill(track.pt());
    registry.get<TH1>(HIST("ptVariableBinning"))->Fill(track.pt());
    registry.fill(HIST("EtaVsPhi"), track.phi(), track.eta());
  }
};

struct HistogramsWithFilters {
  HistogramRegistry registry{
    "registry",
    {
      {"eta", "#eta", {HistType::kTH1F, {{102, -2.01, 2.01}}}},                            //
      {"ptToPt", "ptToPt", {HistType::kTH2F, {{100, -0.01, 10.01}, {100, -0.01, 10.01}}}} //
    }                                                                                      //
  };

  // If we fill histograms with filters, we need to provide full tables
  // aod::Tracks instead of aod::Track
  void process(aod::Tracks const& tracks)
  {
    registry.fill<aod::track::Eta>(HIST("eta"), tracks, aod::track::eta > 0.0f);
    registry.fill<aod::track::Pt, aod::track::Pt>(HIST("ptToPt"), tracks, aod::track::pt < 5.0f);
  }
};
//start TPCvsPT ZCH
struct PidSignalsMonitorAllPart {
  HistogramRegistry registry{
    "registry",
    {
     // {"eta", "#eta", {HistType::kTH1F, {{102, -2.01, 2.01}}}},                     //  
	//{"ptToPt", "ptToPt", {HistType::kTH2F, {{100, -0.01, 10.01}, {100, -0.01, 10.01}}}}, //
     {"TPCSignal", "TPC signal", {HistType::kTH2F, {{100, 0, 10, "pt"}, {100, 0, 600, "tpc"}}}},//
	{"TOFSignal", "TOF signal", {HistType::kTH2F, {{100, 0, 10, "pt"}, {10000, 0, 50000, "tof"}}}}//
    }                                                                                      //
  };

  // If we fill histograms with filters, we need to provide full tables
  // aod::Tracks instead of aod::Track
  Filter trackFilter = aod::track::isGlobalTrack == (uint8_t)true;
  void process(soa::Filtered<soa::Join<aod::Tracks,aod::TracksExtra,aod::TrackSelection>>::iterator const& track)
  //void process(aod::FullTrack const& track)
  {
   // registry.fill<aod::track::Eta>(HIST("eta"), tracks, aod::track::eta > 0.0f);
    //registry.fill<aod::track::Pt, aod::track::Pt>(HIST("ptToPt"), tracks, aod::track::pt < 5.0f);
    registry.fill(HIST("TPCSignal"), track.pt(), track.tpcSignal());
    registry.fill(HIST("TOFSignal"), track.pt(), track.tofSignal());
  }
};
//end TPCvsPT ZCH



struct PidMonitors {
  static constexpr std::string_view sigvsp[6] = {"TpcVsMomentumPr", "TpcVsMomentumKa", "TpcVsMomentumPi", "TofVsMomentumPr", "TofVsMomentumKa", "TofVsMomentumPi"};

static constexpr std::string_view nsigm[6] = {"NSigmaTpcVsMomentumPr", "NSigmaTpcVsMomentumKa", "NSigmaTpcVsMomentumPi", "NSigmaTofVsMomentumPr", "NSigmaTofVsMomentumKa", "NSigmaTofVsMomentumPi"};
  HistogramRegistry registry {
    "registry",
      {
	{"TpcVsMomentumPr", "TpcVsMomentumPr;p GeV/c^2; TPC signal", {HistType::kTH2F, {{500, 0, 10}, {500, 0, 6000}}}},
	{"TpcVsMomentumKa", "TpcVsMomentumKa;p GeV/c^2; TPC signal", {HistType::kTH2F, {{500, 0, 10}, {500, 0, 4000}}}},
	{"TpcVsMomentumPi", "TpcVsMomentumPi;p GeV/c^2; TPC signal", {HistType::kTH2F, {{500, 0, 10}, {500, 0, 1000}}}},
	{"TofVsMomentumPr", "TofVsMomentumPr;p GeV/c^2; TOF signal", {HistType::kTH2F, {{500, 0, 10}, {500, 0, 100000}}}},
	{"TofVsMomentumKa", "TofVsMomentumKa;p GeV/c^2; TOF signal", {HistType::kTH2F, {{500, 0, 10}, {500, 0, 100000}}}},
	{"TofVsMomentumPi", "TofVsMomentumPi;p GeV/c^2; TOF signal", {HistType::kTH2F, {{500, 0, 10}, {500, 0, 100000}}}},
	{"ptPi", "pt pion", {HistType::kTH1F, {{100, -0.01, 10.01, "#it{p}_{T} (GeV/c)"}}}},
	{"ptKa", "pt kaon", {HistType::kTH1F, {{100, -0.01, 10.01, "#it{p}_{T} (GeV/c)"}}}},
	{"ptPr", "pt proton", {HistType::kTH1F, {{100, -0.01, 10.01, "#it{p}_{T} (GeV/c)"}}}},
      	{"NSigmaTpcVsMomentumPr", "NSigmaTpcVsMomentumPr;#it{p}_{T} (GeV/c); NSigma TPC", {HistType::kTH2F, {{500, 0, 10}, {500, -4, 4}}}},
	{"NSigmaTpcVsMomentumKa", "NSigmaTpcVsMomentumKa;#it{p}_{T} (GeV/c); NSigma TPC", {HistType::kTH2F, {{500, 0, 10}, {500, -4, 4}}}},
	{"NSigmaTpcVsMomentumPi", "NSigmaTpcVsMomentumPi;#it{p}_{T} (GeV/c); NSigma TPC", {HistType::kTH2F, {{500, 0, 10}, {500, -4, 4}}}},
	{"NSigmaTofVsMomentumPr", "NSigmaTofVsMomentumPr;#it{p}_{T} (GeV/c); NSigma TOF", {HistType::kTH2F, {{500, 0, 10}, {500, -4, 4}}}},
	{"NSigmaTofVsMomentumKa", "NSigmaTofVsMomentumKa;#it{p}_{T} (GeV/c); NSigma TOF", {HistType::kTH2F, {{500, 0, 10}, {500, -4, 4}}}},
	{"NSigmaTofVsMomentumPi", "NSigmaTofVsMomentumPi;#it{p}_{T} (GeV/c); NSigma TOF", {HistType::kTH2F, {{500, 0, 10}, {500, -4, 4}}}},
	{"NSigmaTpcVsMomentum", "#it{n}^{#sigma} TPS vs #it{p}_{T};#it{p}_{T} (GeV/c); #it{n}^{#sigma} TPC", {HistType::kTH2F, {{500, 0, 10}, {30, -4, 4}}}},
	{"NSigmaTofVsMomentum", "#it{n}^{#sigma} TOF vs #it{p}_{T};#it{p}_{T} (GeV/c); #it{n}^{#sigma} TOF", {HistType::kTH2F, {{500, 0, 10}, {30, -4, 4}}}}
	}
  };

    Configurable<float> nsigmacut{"nsigmacut", 3, "Value of the Nsigma cut"};


    // i - 0 for proton, 1 for kaon, 2 for pion
    template <std::size_t i, typename T>
    void fillParticleHistos(const T& track)
    {

      //pions 
      if(i==0){
	if (track.pt() < 0.5) //Pt < 0.5 - only TPC
	  {
	    if (abs(track.tpcNSigmaPi()) < nsigmacut.value)
	      {
		registry.fill(HIST(sigvsp[2]), track.p(), track.tpcSignal());
		registry.fill(HIST(nsigm[2]), track.pt(), track.tpcNSigmaPi());
		registry.fill(HIST("NSigmaTpcVsMomentum"), track.pt(), track.tpcNSigmaPi());
		registry.fill(HIST("NSigmaTofVsMomentum"), track.pt(), track.tofNSigmaPi());
		registry.fill(HIST("ptPi"),track.pt());
	      }
	    
	  }
	else if (track.pt() >= 0.5) //Pt>= 0.5 - TPC + TOF
	  {
	    if (sqrt(pow(track.tpcNSigmaPi(), 2) + pow(track.tofNSigmaPi(),2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
	      {
		registry.fill(HIST(sigvsp[2]), track.p(), track.tpcSignal());
		registry.fill(HIST(sigvsp[5]), track.p(), track.tofSignal());
		registry.fill(HIST(nsigm[2]), track.pt(), track.tpcNSigmaPi());
		registry.fill(HIST(nsigm[5]), track.pt(), track.tofNSigmaPi());
		registry.fill(HIST("NSigmaTpcVsMomentum"), track.pt(), track.tpcNSigmaPi());
		registry.fill(HIST("NSigmaTofVsMomentum"), track.pt(), track.tofNSigmaPi());
		registry.fill(HIST("ptPi"),track.pt());
	      }
	  }
      }

      //kaons 
      if(i==1){
	if (track.pt() < 0.5) //Pt < 0.5 - only TPC
	  {
	    if (abs(track.tpcNSigmaKa()) < nsigmacut.value)
	      {
		registry.fill(HIST(sigvsp[1]), track.p(), track.tpcSignal());
		registry.fill(HIST(nsigm[1]), track.pt(), track.tpcNSigmaKa());
		registry.fill(HIST("NSigmaTpcVsMomentum"), track.pt(), track.tpcNSigmaKa());
		registry.fill(HIST("NSigmaTofVsMomentum"), track.pt(), track.tofNSigmaKa());
		registry.fill(HIST("ptKa"),track.pt());
	      }
	  }
	else if (track.pt() >= 0.5) //Pt>= 0.5 - TPC + TOF
	  {
	    if (sqrt(pow(track.tpcNSigmaKa(), 2) + pow(track.tofNSigmaKa(),2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
	      {
		registry.fill(HIST(sigvsp[1]), track.p(), track.tpcSignal());
		registry.fill(HIST(sigvsp[4]), track.p(), track.tofSignal());
		registry.fill(HIST(nsigm[1]), track.pt(), track.tpcNSigmaKa());
		registry.fill(HIST(nsigm[4]), track.pt(), track.tofNSigmaKa());
		registry.fill(HIST("NSigmaTpcVsMomentum"), track.pt(), track.tpcNSigmaKa());
		registry.fill(HIST("NSigmaTofVsMomentum"), track.pt(), track.tofNSigmaKa());
		registry.fill(HIST("ptKa"),track.pt());
	      }
	  }
      }

      //protons 
      if(i==2){
	if (track.pt() < 0.5) //Pt < 0.5 - only TPC
	  {
	    if (abs(track.tpcNSigmaPr()) < nsigmacut.value)
	      {
		registry.fill(HIST(sigvsp[0]), track.p(), track.tpcSignal());
		registry.fill(HIST(nsigm[0]), track.pt(), track.tpcNSigmaPr());
		registry.fill(HIST("NSigmaTpcVsMomentum"), track.pt(), track.tpcNSigmaPr());
		registry.fill(HIST("NSigmaTofVsMomentum"), track.pt(), track.tofNSigmaPr());
		registry.fill(HIST("ptPr"),track.pt());
	      }
	  }
	else if (track.pt() >= 0.5) //Pt>= 0.5 - TPC + TOF
	  {
	    if (sqrt(pow(track.tpcNSigmaPr(), 2) + pow(track.tofNSigmaPr(),2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
	      {
		registry.fill(HIST(sigvsp[0]), track.p(), track.tpcSignal());
		registry.fill(HIST(sigvsp[3]), track.p(), track.tofSignal());
		registry.fill(HIST(nsigm[0]), track.pt(), track.tpcNSigmaPr());
		registry.fill(HIST(nsigm[3]), track.pt(), track.tofNSigmaPr());
		registry.fill(HIST("NSigmaTpcVsMomentum"), track.pt(), track.tpcNSigmaPr());
		registry.fill(HIST("NSigmaTofVsMomentum"), track.pt(), track.tofNSigmaPr());
		registry.fill(HIST("ptPr"),track.pt());
	      }
	  }
      }
      
    }
  
  using myTracks = soa::Join<aod::Tracks, aod::TracksExtra, aod::pidTPCPr, aod::pidTPCPi, aod::pidTPCKa, aod::pidTOFPr, aod::pidTOFPi, aod::pidTOFKa>::iterator;
    void process(myTracks const& track)
    {
        const float nsigma[6] = {track.tpcNSigmaPr(), track.tpcNSigmaKa(), track.tpcNSigmaPi(),
                                track.tofNSigmaPr(), track.tofNSigmaKa(), track.tofNSigmaPi()};

	fillParticleHistos<0>(track);
        fillParticleHistos<1>(track);
	fillParticleHistos<2>(track);
    }
};



struct PidMonitorsMc {
 

  HistogramRegistry registry {
    "registry",
      {
       {"PDGPi", "PDGPi;p GeV/c; PDG", {HistType::kTH2F, {{500, 0, 10}, {8001, -4000.5, 4000.5}}}},
       {"PDGKa", "PDGKa;p GeV/c; PDG", {HistType::kTH2F, {{500, 0, 10}, {8001, -4000.5, 4000.5}}}},
       {"PDGPr", "PDGPr;p GeV/c; PDG", {HistType::kTH2F, {{500, 0, 10}, {8001, -4000.5, 4000.5}}}},
	{"ptPi", "pt pion", {HistType::kTH1F, {{100, -0.01, 10.01, "#it{p}_{T} (GeV/c)"}}}},
	{"ptKa", "pt kaon", {HistType::kTH1F, {{100, -0.01, 10.01, "#it{p}_{T} (GeV/c)"}}}},
	{"ptPr", "pt proton", {HistType::kTH1F, {{100, -0.01, 10.01, "#it{p}_{T} (GeV/c)"}}}}
      }
  };

    Configurable<float> nsigmacut{"nsigmacut", 3, "Value of the Nsigma cut"};


    // i - 0 for proton, 1 for kaon, 2 for pion
  template < typename T1, typename T2>
  void fillParticleHistos2(int i, const T1& track, const T2& mcparticle)
    {

      //pions 
      if(i==0){
	if (track.pt() < 0.5) //Pt < 0.5 - only TPC
	  {
	    if (abs(track.tpcNSigmaPi()) < nsigmacut.value)
	      {
		registry.fill(HIST("PDGPi"), track.pt(), mcparticle.pdgCode());
		registry.fill(HIST("ptPi"),mcparticle.pt());
	      }
	    
	  }
	else if (track.pt() >= 0.5) //Pt>= 0.5 - TPC + TOF
	  {
	    if (sqrt(pow(track.tpcNSigmaPi(), 2) + pow(track.tofNSigmaPi(),2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
	      {
		registry.fill(HIST("PDGPi"), track.pt(), mcparticle.pdgCode());
		registry.fill(HIST("ptPi"),mcparticle.pt());
	      }
	  }
      }

      //kaons 
      if(i==1){
	if (track.pt() < 0.5) //Pt < 0.5 - only TPC
	  {
	    if (abs(track.tpcNSigmaKa()) < nsigmacut.value)
	      {
		registry.fill(HIST("PDGKa"), track.pt(), mcparticle.pdgCode());
		registry.fill(HIST("ptKa"),mcparticle.pt());
	      }
	  }
	else if (track.pt() >= 0.5) //Pt>= 0.5 - TPC + TOF
	  {
	    if (sqrt(pow(track.tpcNSigmaKa(), 2) + pow(track.tofNSigmaKa(),2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
	      {
		registry.fill(HIST("PDGKa"), track.pt(), mcparticle.pdgCode());
		registry.fill(HIST("ptKa"),mcparticle.pt());
	      }
	  }
      }

      //protons 
      if(i==2){
	if (track.pt() < 0.5) //Pt < 0.5 - only TPC
	  {
	    if (abs(track.tpcNSigmaPr()) < nsigmacut.value)
	      {
		registry.fill(HIST("PDGPr"), track.pt(), mcparticle.pdgCode());
		registry.fill(HIST("ptPr"),mcparticle.pt());
	      }
	  }
	else if (track.pt() >= 0.5) //Pt>= 0.5 - TPC + TOF
	  {
	    if (sqrt(pow(track.tpcNSigmaPr(), 2) + pow(track.tofNSigmaPr(),2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
	      {
		registry.fill(HIST("PDGPr"), track.pt(), mcparticle.pdgCode());
		registry.fill(HIST("ptPr"),mcparticle.pt());
	      }
	  }
      }
      
    }

  Filter trackFilter = aod::track::isGlobalTrack == (uint8_t)true;
  using myTracks = soa::Filtered<soa::Join<aod::Tracks,  aod::TracksExtra, aod::TrackSelection, aod::pidTPCPr, aod::pidTPCPi, aod::pidTPCKa, aod::pidTOFPr, aod::pidTOFPi, aod::pidTOFKa, aod::McTrackLabels>>;
  void process(myTracks const& tracks, aod::McParticles const& mctracks)
    {
      for (const auto& track : tracks)
	{
	  const auto mcParticle = track.mcParticle();
	  fillParticleHistos2(0,track,mcParticle);
	  fillParticleHistos2(1,track,mcParticle);
	  fillParticleHistos2(2,track,mcParticle);

	  std::cout<<"track.pt: "<<track.pt()<<", mcParticle,pdgCode(): "<<mcParticle.pdgCode()<<std::endl;
	}
  
	

    }
};


struct EfficiencyPid {
 

  HistogramRegistry registry {
    "registry",
      {
       {"PDGPi", "PDGPi;p GeV/c; PDG", {HistType::kTH2F, {{500, 0, 10}, {8001, -4000.5, 4000.5}}}},
       {"PDGKa", "PDGKa;p GeV/c; PDG", {HistType::kTH2F, {{500, 0, 10}, {8001, -4000.5, 4000.5}}}},
       {"PDGPr", "PDGPr;p GeV/c; PDG", {HistType::kTH2F, {{500, 0, 10}, {8001, -4000.5, 4000.5}}}},
	{"ptPi", "pt pion", {HistType::kTH1F, {{100, -0.01, 10.01, "#it{p}_{T} (GeV/c)"}}}},
	{"ptKa", "pt kaon", {HistType::kTH1F, {{100, -0.01, 10.01, "#it{p}_{T} (GeV/c)"}}}},
       {"ptPr", "pt proton", {HistType::kTH1F, {{100, -0.01, 10.01, "#it{p}_{T} (GeV/c)"}}}},
       	{"ptPiMC", "pt pion MC", {HistType::kTH1F, {{100, -0.01, 10.01, "#it{p}_{T} (GeV/c)"}}}},
	{"ptKaMC", "pt kaon MC", {HistType::kTH1F, {{100, -0.01, 10.01, "#it{p}_{T} (GeV/c)"}}}},
	{"ptPrMC", "pt proton MC", {HistType::kTH1F, {{100, -0.01, 10.01, "#it{p}_{T} (GeV/c)"}}}}
      }
  };

    Configurable<float> nsigmacut{"nsigmacut", 3, "Value of the Nsigma cut"};


    // i - 0 for proton, 1 for kaon, 2 for pion
  template < typename T1, typename T2>
  void fillParticleHistos2(int i, const T1& track, const T2& mcparticle)
    {

      //pions 
      if(i==0){
	if (track.pt() < 0.5) //Pt < 0.5 - only TPC
	  {
	    if (abs(track.tpcNSigmaPi()) < nsigmacut.value)
	      {
		registry.fill(HIST("PDGPi"), track.pt(), mcparticle.pdgCode());
		if(abs(mcparticle.pdgCode())==211)
		  registry.fill(HIST("ptPi"),mcparticle.pt());
	      }
	    
	  }
	else if (track.pt() >= 0.5) //Pt>= 0.5 - TPC + TOF
	  {
	    if (sqrt(pow(track.tpcNSigmaPi(), 2) + pow(track.tofNSigmaPi(),2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
	      {
		registry.fill(HIST("PDGPi"), track.pt(), mcparticle.pdgCode());
		if(abs(mcparticle.pdgCode())==211)
		  registry.fill(HIST("ptPi"),mcparticle.pt());
	      }
	  }
      }

      //kaons 
      if(i==1){
	if (track.pt() < 0.5) //Pt < 0.5 - only TPC
	  {
	    if (abs(track.tpcNSigmaKa()) < nsigmacut.value)
	      {
		registry.fill(HIST("PDGKa"), track.pt(), mcparticle.pdgCode());
		if(abs(mcparticle.pdgCode())==321)
		  registry.fill(HIST("ptKa"),mcparticle.pt());
	      }
	  }
	else if (track.pt() >= 0.5) //Pt>= 0.5 - TPC + TOF
	  {
	    if (sqrt(pow(track.tpcNSigmaKa(), 2) + pow(track.tofNSigmaKa(),2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
	      {
		registry.fill(HIST("PDGKa"), track.pt(), mcparticle.pdgCode());
		if(abs(mcparticle.pdgCode())==321)
		  registry.fill(HIST("ptKa"),mcparticle.pt());
	      }
	  }
      }

      //protons 
      if(i==2){
	if (track.pt() < 0.5) //Pt < 0.5 - only TPC
	  {
	    if (abs(track.tpcNSigmaPr()) < nsigmacut.value)
	      {
		registry.fill(HIST("PDGPr"), track.pt(), mcparticle.pdgCode());
		if(abs(mcparticle.pdgCode())==2212)
		  registry.fill(HIST("ptPr"),mcparticle.pt());
	      }
	  }
	else if (track.pt() >= 0.5) //Pt>= 0.5 - TPC + TOF
	  {
	    if (sqrt(pow(track.tpcNSigmaPr(), 2) + pow(track.tofNSigmaPr(),2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
	      {
		registry.fill(HIST("PDGPr"), track.pt(), mcparticle.pdgCode());
		if(abs(mcparticle.pdgCode())==2212)
		  registry.fill(HIST("ptPr"),mcparticle.pt());
	      }
	  }
      }
      
    }

  template < typename T>
  void fillParticleHistosMC(int pdg, const T& mctrack)
  {
    if(abs(mctrack.pdgCode())==211)
      {
	registry.fill(HIST("ptPiMC"),mctrack.pt());
      }
    else if(abs(mctrack.pdgCode())==321)
      {
	registry.fill(HIST("ptKaMC"),mctrack.pt());
      }
    else if(abs(mctrack.pdgCode())==2212)
      {
	registry.fill(HIST("ptPrMC"),mctrack.pt());
      }
  }

  Filter trackFilter = aod::track::isGlobalTrack == (uint8_t)true;
  using myTracks = soa::Filtered<soa::Join<aod::Tracks,  aod::TracksExtra, aod::TrackSelection, aod::pidTPCPr, aod::pidTPCPi, aod::pidTPCKa, aod::pidTOFPr, aod::pidTOFPi, aod::pidTOFKa, aod::McTrackLabels>>;
  void process(myTracks const& tracks, aod::McParticles const& mctracks)
    {
      for (const auto& track : tracks)
	{
	  const auto mcParticle = track.mcParticle();
	  fillParticleHistos2(0,track,mcParticle);
	  fillParticleHistos2(1,track,mcParticle);
	  fillParticleHistos2(2,track,mcParticle);

	  //std::cout<<"track.pt: "<<track.pt()<<", mcParticle,pdgCode(): "<<mcParticle.pdgCode()<<std::endl;

	  //if(isPhysicalPrimary(mcParticle))
	    //{
	      //std::cout<<"isPhysicalPrimary"<<std::endl;
	      //}
	}
  
      for (const auto& mctrack : mctracks)
	{
	  /*
	   std::cout<<"producedByGenerator(): "<<mctrack.producedByGenerator()<<", ";
	  std::cout<<"has_mother0(): "<<mctrack.has_mother0()<<", ";
	  std::cout<<mctrack.mother0Id()<<std::endl;
	  std::cout<<mctracks.
	  std::cout<<", isPhysicalPrimary: "<<isPhysicalPrimary(mctrack)<<std::endl;
	  */

	  //if particle is primary:
	  bool isPrimary = true;
	  //if(!mctrack.producedByGenerator()) isPrimary = false;
	  //if(mctrack.has_mother0()) isPrimary = false;

	  isPrimary = ((abs(mctrack.pdgCode())==211) || (abs(mctrack.pdgCode())==321) ||(abs(mctrack.pdgCode())==2212) ); //to reduce the time of running
	  
	  if(abs(mctrack.eta()<0.8) && isPrimary/*&& isPhysicalPrimary(mctrack)*/)
	    {
	      fillParticleHistosMC(211,mctrack);
	      fillParticleHistosMC(321,mctrack);
	      fillParticleHistosMC(2212,mctrack);
	    }
	}

    }
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    adaptAnalysisTask<BasicHistogramsNoPID>(cfgc),
    adaptAnalysisTask<PidSignalsMonitorAllPart>(cfgc),
    //adaptAnalysisTask<HistogramsWithFilters>(cfgc),
   adaptAnalysisTask<PidMonitors>(cfgc),
   // adaptAnalysisTask<PidMonitorsMc>(cfgc),
    //adaptAnalysisTask<EfficiencyPid>(cfgc)
      };
}
