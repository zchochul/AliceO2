#!/bin/bash

#export alien_CLOSE_SE=ALICE::UPB::EOS
# export alien_CLOSE_SE=ALICE::Vienna::EOS
# export alien_CLOSE_SE=ALICE::ORNL::EOS
# export alien_CLOSE_SE=ALICE::CCIN2P3::SE
export alien_CLOSE_SE=ALICE::GSI::SE2
# export alien_CLOSE_SE=ALICE::FZK::SE


ConfigFile="./prod-MCReco.json"

Prefix="feature-bininng-23zzh-"

Output="AnalysisResults.root"
OutputQA="QAResults.root"
Derived="FemtoAO2D"

echo "----------Start Job----------"
date
Start=$(date '+%s')

    o2-analysis-timestamp -b --configuration json://$ConfigFile |
    o2-analysis-centrality-table -b --configuration json://$ConfigFile |
    #o2-analysis-mc-converter -b --configuration json://$ConfigFile |
    o2-analysis-event-selection -b --configuration json://$ConfigFile |
    o2-analysis-trackselection -b --configuration json://$ConfigFile |
    o2-analysis-multiplicity-table -b --configuration json://$ConfigFile |
    #o2-analysis-fdd-converter -b --configuration json://$ConfigFile |
    o2-analysis-track-propagation -b --configuration json://$ConfigFile |
    o2-analysis-bc-converter -b --configuration json://$ConfigFile |
    #o2-analysis-trackextension -b --configuration json://$ConfigFile |
    o2-analysis-pid-tpc-base -b --configuration json://$ConfigFile |
    o2-analysis-pid-tpc -b --configuration json://$ConfigFile |
    o2-analysis-pid-tof-base -b --configuration json://$ConfigFile |
    o2-analysis-pid-tof -b --configuration json://$ConfigFile |
    #o2-analysis-zdc-converter -b --configuration json://$ConfigFile |
    o2-analysis-tracks-extra-converter -b --configuration json://$ConfigFile |
    #o2-analysis-collision-converter -b --configuration json://$ConfigFile |
    o2-analysis-v0converter -b --configuration json://$ConfigFile |
    o2-analysis-ft0-corrected-table -b --configuration json://$ConfigFile |
    o2-analysis-lf-lambdakzerobuilder -b --configuration json://$ConfigFile |
    o2-analysis-lf-lambdakzeromcbuilder -b --configuration json://$ConfigFile |
    #o2-analysis-lf-lambdakzerofinder -b --configuration json://$ConfigFile |
    o2-analysis-cf-femtouniverse-producer -b --configuration json://$ConfigFile --aod-writer-resfile FemtoAO2D --aod-writer-keep AOD/FDPARTICLE/0,AOD/FDMCLabel/0,AOD/FDCOLLISION/0,AOD/FDEXTPARTICLE/0,AOD/FDMCPARTICLE/0
    #|
    #o2-analysis-cf-femtouniverse-pair-track-track -b --configuration json://$ConfigFile

     #--aod-writer-resfile FemtoAO2D --aod-writer-keep AOD/FEMTODREAMPARTS/0,AOD/FEMTODREAMMCPS/0,AOD/FEMTOMCLABELS/0,AOD/FEMTODREAMCOLS/0

 #--aod-writer-resfile FemtoAO2D --aod-writer-keep AOD/FEMTODREAMPARTS/0,AOD/FEMTODREAMCOLS/0

if [ -f "$Output" ]; then
    mv "$Output" "${Prefix}${Output}"
fi

if [ -f "$OutputQA" ]; then
    mv "$OutputQA" "${Prefix}${OutputQA}"
fi

# if [ -f "$Derived" ]; then
#     mv "$Derived" "${Prefix}${Derived}"
# fi

date
echo "----------End Job----------"
End=$(date '+%s')
echo "Duration $(date -d@$((End - Start)) -u '+%H:%M:%S')"

#exit 0
