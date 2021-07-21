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
#include <TH1F.h>
#include <TParameter.h>

#include <cmath>

using namespace o2;
using namespace o2::framework;

struct SimpleHistogramTaskWithRuntimeDefinition {
  HistogramRegistry registry{"registry", {}};

  void init(InitContext&)
  {
    std::vector<double> ptBinning = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0,
                                     1.1, 1.2, 1.3, 1.4, 1.5, 2.0, 5.0, 10.0, 20.0, 50.0};
    AxisSpec ptAxis = {ptBinning, "#it{p}_{T} (GeV/c)"};
    registry.add("pt", "pt", kTH1F, {ptAxis});
  }

  // aod::Track is an iterator over tracks table
  // HIST() is a macro to parse the string and find appropriate histogram at compile time
  void process(aod::Track const& track)
  {
    registry.get<TH1>(HIST("pt"))->Fill(track.pt());
  }
};

struct SimpleHistogramTask {
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
                                                       "#it{p}_{T} (GeV/c)"}}}} //
    }                                                                           //
  };

  // aod::Track is an iterator over tracks table
  void process(aod::Track const& track)
  {

    registry.get<TH1>(HIST("eta"))->Fill(track.eta());
    registry.get<TH1>(HIST("phi"))->Fill(track.phi());
    registry.get<TH1>(HIST("pt"))->Fill(track.pt());
    registry.get<TH1>(HIST("ptVariableBinning"))->Fill(track.pt());
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
struct HistogramsWithTPC {
  HistogramRegistry registry{
    "registry",
    {
     // {"eta", "#eta", {HistType::kTH1F, {{102, -2.01, 2.01}}}},                     //  
	//{"ptToPt", "ptToPt", {HistType::kTH2F, {{100, -0.01, 10.01}, {100, -0.01, 10.01}}}}, //
	{"TPCSignal", "TPC signal", {HistType::kTH2F, {{100, 0, 10, "pt"}, {100, 0, 600, "tpc"}}}}//
    }                                                                                      //
  };

  // If we fill histograms with filters, we need to provide full tables
  // aod::Tracks instead of aod::Track
  void process(aod::FullTrack const& track)
  {
   // registry.fill<aod::track::Eta>(HIST("eta"), tracks, aod::track::eta > 0.0f);
    //registry.fill<aod::track::Pt, aod::track::Pt>(HIST("ptToPt"), tracks, aod::track::pt < 5.0f);
    registry.fill(HIST("TPCSignal"), track.pt(), track.tpcSignal());
  }
};
//end TPCvsPT ZCH

//start TOFvsPT ZCH
struct HistogramsWithTof {
  HistogramRegistry registry{
    "registry",
    {
	{"TOFSignal", "TOF signal", {HistType::kTH2F, {{100, 0, 10, "pt"}, {100, 0, 10, "tof"}}}}//
    }                                                                                      //
  };

  // If we fill histograms with filters, we need to provide full tables
  // aod::Tracks instead of aod::Track
  void process(aod::FullTrack const& track)
  {
    registry.fill(HIST("TOFSignal"), track.pt(), track.tofSignal());
  }
};
//end TOFvsPT ZCH

//start ETAvsPHI
struct HistogramEtaVsPhi{
  HistogramRegistry registry{
    "registry",
    {
	{"EtaVsPhi", "EtaVsPhi", {HistType::kTH2F, {{100, 0, 2*TMath::Pi(), "phi"}, {100, -1.2, 1.2, "eta"}}}}//
    }                                                                                      //
  };

  // If we fill histograms with filters, we need to provide full tables
  // aod::Tracks instead of aod::Track
  void process(aod::FullTrack const& track)
  {
    registry.fill(HIST("EtaVsPhi"), track.phi(), track.eta());
  }
};
//end EtavsPhi ZCH

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    adaptAnalysisTask<SimpleHistogramTaskWithRuntimeDefinition>(cfgc),
    adaptAnalysisTask<SimpleHistogramTask>(cfgc),
    adaptAnalysisTask<HistogramsWithTPC>(cfgc),
    adaptAnalysisTask<HistogramsWithTof>(cfgc),
    adaptAnalysisTask<HistogramEtaVsPhi>(cfgc),
    adaptAnalysisTask<HistogramsWithFilters>(cfgc)};	
}
