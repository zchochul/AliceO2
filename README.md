# AliceO2
In this file crucial commands for my analysis are  listed:<br>
## How to create?
In for example: `~/alice/O2Physics/Tutorials/src/` place your code in C++, then change CMakeList in `~/alice/O2Physics/Tutorials/` (add this file).
## How to build?
In `~/alice/sw/BUILD/O2Physics-latest-master/O2Physics` type `alienv load ninja/latest O2Physics/latest ` and then `ninja stage/bin/your-analysis-file` to build your code. 
Keep in mind that if you add a new file or modify CMakeList you need to use `cmake .` 
Then `cp` it to directory with AOD file. (You can skip this step when you use `ninja install` instead of `ninja`) 
## How to run a code?
In your dir with AOD file type: <br>
`alienv enter O2Physics/latest` and  `alien-token-init your-cern-nickname`<br>
Then to run your code you only need to type: <br>
`./o2-analysistutorial-simple-analysis --aod-file <aod_file_name> -b` <br>
But sometimes you need to add more for it to work. When you see `Exception caught: Couldn't get TTree [sth]` you need to add it to your analysis. <br>
For example while performing PID analysis. In this case you need to type for example: <br>
`./o2-analysistutorial-simple-analysis --aod-file <aod_file_name> --aod-memory-rate-limit 100000000000000 -b | o2-analysis-pid-tpc -b | o2-analysis-pid-tof -b | o2-analysis-trackselection -b | o2-analysis-tracksextention -b ` <br>
But keep in mind not everything might be useful in your case and `-b` stops GUI from showing.
### Possible errors
While trying to run various O2 tasks/tutorials you can run into fatal errors with non-existing CCDB entries for specific data. <br>
Usually adding `--isRun2MC` to executables (likely `o2-analysis-timestamp`) might help. We also added `isMC` to o2-analysis-event-selection. <br>
## Results
Running command `./o2-analysistutorial-simple-analysis --aod-file <aod_file_name> -b` (and others of that type) creates .root file and .json file. <br>
To enter AnalysisResults.root file type: `root -l` and then `TBrowser`. (`-l` is optional but it runs root without additional information about it).<br>
You can change histograms manually or you can write macros and run them on .root file. <br>
More on root and how to use it [here](https://root.cern/manual/first_steps_with_root/). <br>
## Helpful commands
Sometimes you might find `grep` command useful. It's especially useful, when you want to find tutorials with functions you are interested in. <br>
You can read more [here](https://www.cyberciti.biz/faq/howto-use-grep-command-in-linux-unix/). <br>
