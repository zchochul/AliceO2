#!/bin/bash

config_file=$1

#o2-analysis-cf-femtodream-debug-track -b --configuration json://$config_file -b |
#o2-analysis-cf-femtodream-pair-track-track --configuration json://$config_file -b |
#o2-analysis-cf-femtodream-debug-v0 -b --configuration json://$config_file -b |
#o2-analysis-cf-femtodream-pair-track-v0 --configuration json://$config_file --aod-memory-rate-limit 600000000 -b

./o2-analysis-cf-femto-world-pair-track-track --configuration json://$config_file --aod-memory-rate-limit 600000000 -b


