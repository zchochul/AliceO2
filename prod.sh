#!/bin/bash

# alien-token-init desharma

config_file=$1
writer_file=$2

o2-analysis-collision-converter --configuration json://$config_file -b | 
o2-analysis-trackselection --configuration json://$config_file -b | 
o2-analysis-timestamp  --configuration json://$config_file -b | 
o2-analysis-multiplicity-table  --configuration json://$config_file -b |
o2-analysis-centrality-table  --configuration json://$config_file -b |
# o2-analysis-fdd-converter --configuration json://$config_file -b |
o2-analysis-event-selection  --configuration json://$config_file -b |
o2-analysis-trackextension  --configuration json://$config_file -b |
o2-analysis-pid-tpc  --configuration json://$config_file -b |
o2-analysis-pid-tof-base  --configuration json://$config_file -b |
o2-analysis-pid-tpc-base  --configuration json://$config_file -b |
o2-analysis-pid-tof  --configuration json://$config_file --aod-memory-rate-limit 600000000 -b |
o2-analysis-pid-tof-beta --configuration json://$config_file --aod-memory-rate-limit 600000000 -b |
o2-analysis-lf-lambdakzerobuilder --configuration json://$config_file -b |
o2-analysis-cf-femto-world-producer --configuration json://$config_file --aod-writer-json $writer_file -b #&>output.log &
# o2-analysis-cf-femto-world-producer --configuration json://$config_file --aod-writer-resfile FemtoAO2D --aod-writer-keep AOD/FEMTOWORLDPARTS/0,AOD/FEMTOWORLDCOLS/0 -b &>output.log &