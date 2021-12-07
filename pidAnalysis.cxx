#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"
#include "Framework/HistogramRegistry.h"
#include "Common/DataModel/TrackSelectionTables.h"
#include "Common/Core/PID/PIDResponse.h"
#include "Common/Core/MC.h"
#include <TParameter.h>


using namespace o2;
using namespace o2::framework;
using namespace o2::framework::expressions;

struct mcParticlesH {
    HistogramRegistry registry{
    "registry",
    {
      {"pions", "Primary pions;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}},
      {"protons", "Primary protons;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}},
      {"kaons", "Primary kaons;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}},
      {"el", "Primary electrons;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}},
      {"muons", "Primary muons;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}},
      {"deuter", "Primary deuteron;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}},
      {"hel", "Primary helium;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}}
    }
  };

  using mcParticleTracks = aod::McParticles;
  void process(mcParticleTracks const& mcParticles) {
    for (auto& particle: mcParticles) {
        if (MC::isPhysicalPrimary(particle))
        {
            if (particle.pdgCode() == 211)
            {
                registry.fill(HIST("pions"), particle.pt());
            }
            if (particle.pdgCode() == 11)
            {
                registry.fill(HIST("el"), particle.pt());
            }
            if (particle.pdgCode() == 321)
            {
                registry.fill(HIST("kaons"), particle.pt());
            }
            if (particle.pdgCode() == 2212)
            {
                registry.fill(HIST("protons"), particle.pt());
            }
            if (particle.pdgCode() == 13)
            {
                registry.fill(HIST("muons"), particle.pt());
            }
            if (particle.pdgCode() == 1000010020)
            {
                registry.fill(HIST("deuter"), particle.pt());
            }
        }
      }
  }

};

struct pidParticleH {

    int indexofSmallestElement(const float array[], int size)
    {
        int index = 0;
        for(int i = 1; i < size; i++)
        {
            if(array[i] < array[index])
                index = i;              
        }
        return index;
    }

    double combindeSignal(float val1, float val2)
    {
        return sqrt(pow(val1, 2) + pow(val2, 2));
    }

    int indexofSmallestElementCombined(const float array1[], const float array2[], int size)
    {
        int index = 0;
        for(int i = 1; i < size; i++)
        {
            double combined_i = combindeSignal(array1[i], array2[i]);
            double combined_index = combindeSignal(array1[index], array2[index]);
            if(combined_i < combined_index)
                index = i;              
        }
        return index;
    }

    bool countableTPCOnly(const float tpcNSigmas[], int particleIndex, int arraysLen) {
        return indexofSmallestElement(tpcNSigmas, arraysLen) == particleIndex;
    }

    bool countableCombined(const float tpcNSigmas[], const float tofNSigmas[], int particleIndex, int arraysLen) {
        return indexofSmallestElementCombined(tpcNSigmas, tofNSigmas, arraysLen) == particleIndex;
    }

    HistogramRegistry registry{
        "registry",
        {
            {"pions", "Real pions;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}},
            {"el", "Real electrons;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}},
            {"kaons", "Real kaons;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}},
            {"protons", "Real protons;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}},
            {"muons", "Real muons;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}},
            {"deuter", "Real deuteron;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}},
            {"hel", "Real helium;pT (GeV/c);Counts", {HistType::kTH1F, {{200, 0, 5}}}}
        }
    };

    HistogramRegistry dregistry{
        "double",
        {
            {"doubleCounts", "DoubleCounts;trackCounts;Counts", {HistType::kTH1F, {{10, 0, 10}}}}
        }
    };

//   Filter trackFilter = aod::track::isGlobalTrack == (uint8_t) true;
// mam błąd "Filter cannot be null"
    using pidParticleTracks = soa::Join<aod::Tracks, aod::pidTPCPr, aod::pidTPCKa, aod::pidTPCEl, aod::pidTPCPi, aod::pidTOFPr, aod::pidTOFKa, aod::pidTOFPi, aod::pidTOFEl>;
  void process(pidParticleTracks const& tracks)
  {

    Configurable<float> nsigmacut{"nsigmacut", 3.0, "Value of the Nsigma cut"};

    for (auto& track : tracks) 
    {
        // LOGF(info, "TPCPi: %f, TOFPi: %f, TPCPr: %f, TOFPr: %f, TPCKa: %f, TOFKa: %f", track.tpcNSigmaPi(), track.tofNSigmaPi(), track.tpcNSigmaPr(), track.tofNSigmaPr(), track.tpcNSigmaKa(), track.tofNSigmaKa());
        
        // indexes for particles: 0-Pi, 1-Pr, 2-Ka, 3-El
        const float tpcNSigmas[4] = {track.tpcNSigmaPi(), track.tpcNSigmaPr(), track.tpcNSigmaKa(), track.tpcNSigmaEl()};
        const float tofNSigmas[4] = {track.tofNSigmaPi(), track.tofNSigmaPr(), track.tofNSigmaKa(), track.tofNSigmaEl()};

        int trackCount = 0;

        // pions PID
        if (track.pt() < 0.5)
        {
            // for pions
            if (countableTPCOnly(tpcNSigmas, 0, 4) && (tpcNSigmas[0] < nsigmacut)) {
                trackCount++;
                registry.fill(HIST("pions"), track.pt());
            } // for protons
            if (countableTPCOnly(tpcNSigmas, 1, 4) && (tpcNSigmas[1] < nsigmacut)) {
                trackCount++;
                registry.fill(HIST("protons"), track.pt());
            }
            // for kaons
            if (countableTPCOnly(tpcNSigmas, 2, 4) && (tpcNSigmas[2] < nsigmacut)) {
                trackCount++;
                registry.fill(HIST("kaons"), track.pt());
            } // for electrons
            if (countableTPCOnly(tpcNSigmas, 3, 4) && (tpcNSigmas[3] < nsigmacut)) {
                trackCount++;
                registry.fill(HIST("el"), track.pt());
            } 
            
            // // for pions
            // if ((tpcNSigmas[0] < tpcNSigmas[1]) && (tpcNSigmas[0] < tpcNSigmas[2]) && (tpcNSigmas[0] < nsigmacut)) {
            //     trackCount++;
            //     registry.fill(HIST("pions"), track.pt());
            // } // for kaons
            // if ((tpcNSigmas[2] < tpcNSigmas[1]) && (tpcNSigmas[2] < tpcNSigmas[0]) && (tpcNSigmas[2] < nsigmacut)) {
            //     trackCount++;
            //     registry.fill(HIST("kaons"), track.pt());
            // } // for protons
            // if ((tpcNSigmas[1] < tpcNSigmas[0]) && (tpcNSigmas[1] < tpcNSigmas[2]) && (tpcNSigmas[1] < nsigmacut)) {
            //     trackCount++;
            //     registry.fill(HIST("protons"), track.pt());
            // }

        }
        if (track.pt() >= 0.5)
        {
            // for protons change condition to pt >=0.8
            if ((track.pt() < 0.8) && (countableTPCOnly(tpcNSigmas, 1, 4)) && (tpcNSigmas[1] < nsigmacut)) {
                trackCount++;
                registry.fill(HIST("protons"), track.pt());
            }
                
            float cmdPi = combindeSignal(tpcNSigmas[0], tofNSigmas[0]);
            float cmdPr = combindeSignal(tpcNSigmas[1], tofNSigmas[1]);
            float cmdKa = combindeSignal(tpcNSigmas[2], tofNSigmas[2]);

            if ((countableCombined(tpcNSigmas, tofNSigmas, 0, 4)) && (tpcNSigmas[0] < nsigmacut)) {
                trackCount++;
                registry.fill(HIST("pions"), track.pt());
            }
            if ((track.pt() >= 0.8) && (countableCombined(tpcNSigmas, tofNSigmas, 1, 4)) && (tpcNSigmas[1] < nsigmacut)) {
                trackCount++;
                registry.fill(HIST("protons"), track.pt());
            }
            if ((countableCombined(tpcNSigmas, tofNSigmas, 2, 4)) && (tpcNSigmas[2] < nsigmacut)) {
                trackCount++;
                registry.fill(HIST("kaons"), track.pt());
            }
            if ((countableCombined(tpcNSigmas, tofNSigmas, 3, 4)) && (tpcNSigmas[3] < nsigmacut)) {
                trackCount++;
                registry.fill(HIST("el"), track.pt());
            }

            // if ((cmdPi < cmdPr) && (cmdPi < cmdKa) && (cmdPi < nsigmacut)) {
            //     trackCount++;
            //     registry.fill(HIST("pions"), track.pt());
            // }
            // if ((cmdKa < cmdPi) && (cmdKa < cmdPi) && (cmdKa < nsigmacut)){
            //     trackCount++;
            //     registry.fill(HIST("kaons"), track.pt());
            // }
            // if ((track.pt() >= 0.8) && (cmdPr < cmdPi) && (cmdPr < cmdKa) && (cmdPr < nsigmacut)){
            //     trackCount++;
            //     registry.fill(HIST("protons"), track.pt());
            // }
        }
        // czasami są podwójne zliczenia ~500 takich przypadków na jeden plik
        if (trackCount > 1){
            // LOGF(info, "counts: %i, pT = %f", trackCount, track.pt());
            dregistry.fill(HIST("doubleCounts"), trackCount);
        }       
    }
  }
};

// struct checkSignal {
//     HistogramRegistry registry {
//     "registry",
//         {
//             {"tofNSigmaPi", "tofNSigmaPi", {HistType::kTH1F, {{500, -6.5, 6.5}}}},
//             {"tpcNSigmaPi", "tpcNSigmaPi", {HistType::kTH1F, {{500, -6.5, 6.5}}}},
//             {"tofNSigmaKa", "tofNSigmaKa", {HistType::kTH1F, {{500, -6.5, 6.5}}}},
//             {"tpcNSigmaKa", "tpcNSigmaKa", {HistType::kTH1F, {{500, -6.5, 6.5}}}},
//             {"tofNSigmaPr", "tofNSigmaPr", {HistType::kTH1F, {{500, -6.5, 6.5}}}},
//             {"tpcNSigmaPr", "tpcNSigmaPr", {HistType::kTH1F, {{500, -6.5, 6.5}}}}
//         }
//     };

//     HistogramRegistry detectorRegistry {
//         "registry2",
//         {
//             {"tpcSignalvsP", "tpcP;pT(GeV/c);TPCSignal", {HistType::kTH2F, {{200, 0, 8}, {10000, 0, 100000}}}},
//             {"tofSignalvsP", "tofP;pt(GeV/c);TOFSignal", {HistType::kTH2F, {{200, 0, 8}, {10000, 0, 100000}}}}
//         }
//     };

//     using myTracks = soa::Join<aod::Tracks, aod::TracksExtra, aod::pidTPCPr, aod::pidTPCPi, aod::pidTPCKa, aod::pidTOFPr, aod::pidTOFPi, aod::pidTOFKa>;
//     using myBigTracks = soa::Join<aod::Tracks, aod::TracksExtra, aod::McTrackLabels, aod::pidTPCPr, aod::pidTPCKa, aod::pidTPCPi, aod::pidTOFPr, aod::pidTOFKa, aod::pidTOFPi>;
//     void process(myTracks const& tracks, aod::McParticles const& mcParticles)
//     {
//         for (auto& track : tracks)
//         {       
//             // registry.fill(HIST("tofNSigmaPi"), track.tofNSigmaPi());
//             // registry.fill(HIST("tpcNSigmaPi"), track.tpcNSigmaPi());
//             // registry.fill(HIST("tofNSigmaKa"), track.tofNSigmaKa());
//             // registry.fill(HIST("tpcNSigmaKa"), track.tpcNSigmaKa());
//             // registry.fill(HIST("tofNSigmaPr"), track.tofNSigmaPr());
//             // registry.fill(HIST("tpcNSigmaPr"), track.tpcNSigmaPr());

//             // tpc/tof Signal vs P
//             // registry.fill(HIST("tofSignalvsP"), track.p(), track.tofSignal());
//             registry.fill(HIST("tpcSignalvsP"), track.p(), track.tpcSignal());

//         }
//     }
// };

// struct checkSignal2D {
//     HistogramRegistry registry {
//     "registry",
//         {
//             {"NSigmaPi", "NSigmaPi;tofNSigmaPi;tpcNSigmaPi", {HistType::kTH2F, {{400, -8, 8}, {400, -8, 8}}}},
//             {"NSigmaKa", "NSigmaKa;tofNSigmaKa;tpcNSigmaKa", {HistType::kTH2F, {{400, -8, 8}, {400, -8, 8}}}},
//             {"tpcNSigmaPivsKa", "NSigma;tpcNSigmaPi;tpcNSigmaKa", {HistType::kTH2F, {{400, -8, 8}, {400, -8, 8}}}}
//         }
//     };

//     void process(soa::Join<aod::Tracks, aod::McTrackLabels, aod::pidTPCPr, aod::pidTPCKa,
//         aod::pidTPCPi, aod::pidTOFPr, aod::pidTOFKa, aod::pidTOFPi> const& tracks, aod::McParticles const& mcParticles)
//     {
//         for (auto& track : tracks)
//         {       
//             registry.fill(HIST("NSigmaPi"), track.tofNSigmaPi(), track.tpcNSigmaPi());
//             registry.fill(HIST("NSigmaKa"), track.tpcNSigmaPi(), track.tpcNSigmaKa());
//             registry.fill(HIST("tpcNSigmaPivsKa"), track.tpcNSigmaPi(), track.tpcNSigmaKa());
//         }
//     }

// };

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
    return WorkflowSpec {
        adaptAnalysisTask<mcParticlesH>(cfgc),
        adaptAnalysisTask<pidParticleH>(cfgc),
        // adaptAnalysisTask<checkSignal>(cfgc),
        // adaptAnalysisTask<checkSignal2D>(cfgc),
    };
}