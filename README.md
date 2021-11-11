# AliceO2
In this file crucial commands for my analysis are  listed:<br>
## How to create?
In `~/alice/O2/Analysis/Tutorials/src/` place your code in C++, then change CMake (add this file).
## How to build?
In `~/alice/sw/BUILD/O2-latest/O2` type `ninja stage/bin/your-analysis-file` to build your code. <br>
Keep in mind that if you add a new file or modify CMakeList you need to use `cmake .` <br>
Then `cp` it to directory with AOD file. (You can skip this step when you use `ninja install` instead of `ninja`) <br>
## How to run a code?
In your dir with AOD file type: <br>
`alienv enter O2/latest` <br>
Then to run your code you only need to type: <br>
`./o2-analysistutorial-simple-analysis --aod-file <aod_file_name> -b` <br>
But sometimes you need to add more for it to work, for example while performing PID analysis. In this case you need to type for example: <br>
`./o2-analysistutorial-simple-analysis --aod-file <aod_file_name> --aod-memory-rate-limit 100000000000000 -b | o2-analysis-pid-tpc -b | o2-analysis-pid-tof -b | o2-analysis-trackselection -b | o2-analysis-tracksextention -b ` <br>
But keep in mind not everything might be useful in your case. 
## Results
Running command `./o2-analysistutorial-simple-analysis --aod-file <aod_file_name> -b` (and others of that type) creates .root file and .json file. <br>
To enter .root file type: `root -l` and then `TBrowser`. <br>
You can change histograms manually or you can write macros and run them on .root file. <br>
