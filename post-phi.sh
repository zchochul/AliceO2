#!/bin/bash
# this script runs track-track (post production) analysis
#config_file="./post-Run3.json"
#config_file="./148267_config_post.json"
config_file="post-phi.json"

#FemtoUniverseTaskTrackTrack.cxx (based on the FemtoDream task)
#o2-analysis-cf-femto-universe-pair-track-track --configuration json://$config_file --aod-memory-rate-limit 600000000 -b

#FemtoWorldTaskTrackPhi.cxx (based on the FemtoDream task with features from FemtoWorld task)
o2-analysis-cf-femtouniverse-pair-track-phi --configuration json://$config_file --aod-memory-rate-limit 600000000 -b

