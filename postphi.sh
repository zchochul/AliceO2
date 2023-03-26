#!/bin/bash
# this script runs track-phi post production analysis
config_file=$1
o2-analysis-cf-femto-world-pair-track-phi --configuration json://$config_file --aod-memory-rate-limit 600000000 -b


