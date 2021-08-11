#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"
#include <CCDB/BasicCCDBManager.h>
#include "AnalysisDataModel/PID/PIDResponse.h"
#include "AnalysisDataModel/PID/PIDTOF.h"
#include "AnalysisDataModel/TrackSelectionTables.h"

#include <iostream>

using namespace o2;
using namespace o2::framework;
using namespace o2::framework::expressions;

struct TPCvsMomentum {
    static constexpr std::string_view sigvsp[6] = {"TpcVsMomentumPr", "TpcVsMomentumKa", "TpcVsMomentumPi", "TofVsMomentumPr", "TofVsMomentumKa", "TofVsMomentumPi"};
    HistogramRegistry registry {
        "registry",
        {
            {"TpcVsMomentumPr", "TpcVsMomentumPr;p GeV/c^2; TPC signal", {HistType::kTH2F, {{500, 0, 10}, {500, 0, 6000}}}},
            {"TpcVsMomentumKa", "TpcVsMomentumKa;p GeV/c^2; TPC signal", {HistType::kTH2F, {{500, 0, 10}, {500, 0, 4000}}}},
            {"TpcVsMomentumPi", "TpcVsMomentumPi;p GeV/c^2; TPC signal", {HistType::kTH2F, {{500, 0, 10}, {500, 0, 1000}}}},
            {"TofVsMomentumPr", "TofVsMomentumPr;p GeV/c^2; TOF signal", {HistType::kTH2F, {{500, 0, 10}, {500, 0, 100000}}}},
            {"TofVsMomentumKa", "TofVsMomentumKa;p GeV/c^2; TOF signal", {HistType::kTH2F, {{500, 0, 10}, {500, 0, 100000}}}},
            {"TofVsMomentumPi", "TofVsMomentumPi;p GeV/c^2; TOF signal", {HistType::kTH2F, {{500, 0, 10}, {500, 0, 100000}}}}
        }
    };

    Configurable<float> nsigmacut{"nsigmacut", 3, "Value of the Nsigma cut"};

    // i - 0 for proton, 1 for kaon, 2 for pion
    // j - 0 for TPC, 1 for TOF
    template <std::size_t i, std::size_t j, typename T>
    void fillParticleHistos(const T& track, const float nsigma[])
    {
        if (track.pt() < 0.5)
        {
            if (abs(nsigma[i]) < nsigmacut.value)
            {
                if (j == 0)
                    registry.fill(HIST(sigvsp[i]), track.p(), track.tpcSignal());
                else if (j == 1)
                    registry.fill(HIST(sigvsp[i+3]), track.p(), track.tofSignal());  // i+3 to fill TofVsMomentum hists
            }
        } else if ((track.pt() >= 0.5) && ((i == 1) || (i == 2)))
        {
            if (sqrt(pow(nsigma[i], 2) + pow(nsigma[i+3],2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
            {
                if (j == 0)
                    registry.fill(HIST(sigvsp[i]), track.p(), track.tpcSignal());
                else if (j == 1)
                    registry.fill(HIST(sigvsp[i+3]), track.p(), track.tofSignal());
            }
        } else if ((track.pt() >= 0.8) || (i == 0))
        {
            if (sqrt(pow(nsigma[i], 2) + pow(nsigma[i+3],2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
            {
                if (j == 0)
                    registry.fill(HIST(sigvsp[i]), track.p(), track.tpcSignal());
                else if (j == 1)
                    registry.fill(HIST(sigvsp[i+3]), track.p(), track.tofSignal());
            }
        }
    }

    using myTracks = soa::Join<aod::Tracks, aod::TracksExtra, aod::pidTPCPr, aod::pidTPCPi, aod::pidTPCKa, aod::pidTOFPr, aod::pidTOFPi, aod::pidTOFKa>::iterator;
    void process(myTracks const& track)
    {
        const float nsigma[6] = {track.tpcNSigmaPr(), track.tpcNSigmaKa(), track.tpcNSigmaPi(),
                                track.tofNSigmaPr(), track.tofNSigmaKa(), track.tofNSigmaPi()};

        // first number in < > - 0 for
        // second - 0 for TPC, 1 for TOF
        fillParticleHistos<0, 0>(track, nsigma);
        fillParticleHistos<1, 0>(track, nsigma);
        fillParticleHistos<2, 0>(track, nsigma);
        fillParticleHistos<0, 1>(track, nsigma);
        fillParticleHistos<1, 1>(track, nsigma);
        fillParticleHistos<2, 1>(track, nsigma);  
    }
};

struct PHist{
    static constexpr std::string_view hname[3] = {"PrPt", "PiPt", "KaPt"};

    HistogramRegistry registry{
        "registry",
        {{"PrPt", "ProtonPt; pT GeV/c", {HistType::kTH1F, {{500, 0, 5}}}},
        {"PiPt", "PionPt; pT GeV/c", {HistType::kTH1F, {{500, 0, 5}}}},
        {"KaPt", "KaonPt; pT GeV/c", {HistType::kTH1F, {{500, 0, 5}}}}}
    };

    Configurable<float> nsigmacut{"nsigmacut", 3, "Value of the Nsigma cut"};

    template <std::size_t i, typename T>
    void fillParticleHistos(const T& track, const float nsigma[])
    {
        if (track.pt() < 0.5)
        {
            if (abs(nsigma[i]) < nsigmacut.value)
            {
                registry.fill(HIST(hname[i]),track.pt());
            }
        } else if ((track.pt() >= 0.5) && ((i == 1) || (i == 2)))
        { // for pions and kaons
            if (sqrt(pow(nsigma[i], 2) + pow(nsigma[i+3],2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
            {
                registry.fill(HIST(hname[i]),track.pt());
            }
        } else if ((track.pt() >= 0.5) && (i == 0))
        { // for protons with pt 0.5 <= 0.8 
            if (abs(nsigma[i]) < nsigmacut.value)
            {
                registry.fill(HIST(hname[i]),track.pt());
            }
        } else if ((track.pt() >= 0.8) && (i == 0))
        { // only for protons
            if (sqrt(pow(nsigma[i], 2) + pow(nsigma[i+3],2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
            {
                registry.fill(HIST(hname[i]), track.pt());
            }
        }
    }

    using myTracks = soa::Join<aod::Tracks, aod::TracksExtra, aod::pidTPCPr, aod::pidTPCPi, aod::pidTPCKa, aod::pidTOFPr, aod::pidTOFPi, aod::pidTOFKa>::iterator;
    void process(myTracks const& track)
    {
        const float nsigma[6] = {track.tpcNSigmaPr(), track.tpcNSigmaKa(), track.tpcNSigmaPi(),
                                track.tofNSigmaPr(), track.tofNSigmaKa(), track.tofNSigmaPi()};

        // i == 0 for Protons, 1 for Kaons, 2 for Pions
        fillParticleHistos<0,myTracks>(track, nsigma);
        fillParticleHistos<1, myTracks>(track, nsigma);
        fillParticleHistos<2, myTracks>(track, nsigma);  
    }

};

struct showNSigma {
    using myTracks = soa::Join<aod::Tracks, aod::TracksExtra, aod::pidTPCPr, aod::pidTPCPi, aod::pidTPCKa, aod::pidTOFPr, aod::pidTOFPi, aod::pidTOFKa>;
    void process(myTracks const& tracks)
    {
        std::ofstream f;
        f.open("nSigma.txt");

        for (auto& track : tracks) {
            f << track.tpcNSigmaPi() << "\t" << track.tpcNSigmaKa() << "\t" << track.tpcNSigmaPr() << "\t" << track.tofNSigmaPi() << "\t" << track.tofNSigmaKa() << "\t" <<track.tofNSigmaPr() << std::endl;
        }

        f.close();
    }
};

struct showPDG {
    using myTracks = soa::Join<aod::Tracks, aod::TracksExtra, aod::McParticles>;
    
    void process(myTracks const& tracks)
    {
        for (auto& track : tracks)
        {
            std::cout << track.pdgCode() << std::endl;
        }
    }
};

struct showPDGHist {
    using myTracks = soa::Join<aod::Tracks, aod::TracksExtra, aod::McParticles>;
    
    HistogramRegistry registry{
        "registry",
        {{"PDG", "PDG; pT", {HistType::kTH1F, {{5000, -4000, 4000}}}}}
    };

   void process(myTracks const& tracks)
    {
        for (auto& track : tracks)
        {
            std::cout << track.pdgCode() << std::endl;
       	registry.get<TH1>(HIST("PDG"))-> Fill(track.pdgCode());

	 }
    }
};


struct PDGHad {
    HistogramRegistry registry {
        "registry",
        {
		{"PDGPi", "PDGPi;p GeV/c; PDG", {HistType::kTH2F, {{500, 0, 10}, {500, -4000, 4000}}}}
        }
    };

    Configurable<float> nsigmacut{"nsigmacut", 3, "Value of the Nsigma cut"};
    using BigTracksMC = soa::Join<aod::FullTracks, aod::pidTPCPi, aod::pidTOFPi, aod::McTrackLabels>;

    void process(BigTracksMC const& tracks, aod::McParticles const& mctracks)
    {
  	for (const auto& track : tracks) {
 	  	const float nsigma[2] = {track.tpcNSigmaPi(), track.tofNSigmaPi()};
   		 const auto mcParticle = track.mcParticle();
      			if (track.pt() < 0.5) {
				if( (abs(nsigma[1]) < nsigmacut.value) || (abs(nsigma[0])<nsigmacut.value ) )
					registry.fill(HIST("PDGPi"), track.p(), mcParticle.pdgCode());
			} else if (track.pt() >= 0.5) 
        		{
           			 if (sqrt(pow(nsigma[0], 2) + pow(nsigma[1],2)) < nsigmacut.value) // square root of tpcSignal^2 + tofSignal^2
                    			registry.fill(HIST("PDGPi"), track.p(), mcParticle.pdgCode());
       			 }     
    	}
    }
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
//	 adaptAnalysisTask<TPCvsMomentum>(cfgc),
//	 adaptAnalysisTask<PHist>(cfgc),
//	 adaptAnalysisTask<showPDGHist>(cfgc),
//	 adaptAnalysisTask<PDGHadrons>(cfgc),
	  adaptAnalysisTask<PDGHad>(cfgc),
	// adaptAnalysisTask<showNSigma>(cfgc),
    // adaptAnalysisTask<showPDG>(cfgc),
    // adaptAnalysisTask<TofPt>(cfgc),
    // adaptAnalysisTask<TpcPt>(cfgc),
    // adaptAnalysisTask<CombinedPt>(cfgc),
  };
}
