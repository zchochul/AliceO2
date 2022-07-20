This repository is a collection of notes I collected while working with O2 software. Keep in mind that I am still working on this repository and it is not complete. If you have any comments feel free to email me: [zchochul@cern.ch](mailto:zchochul@cern.ch)
# Table of contents
1. [Alice O2 - introduction](#AliceO2)
2. [Femtoscopy](#Femtoscopy)
3. [Github help ;)](#github)

# AliceO2 <a name="AliceO2"></a>
In this repository you can find instructions on how to get started with O2 Framework. Keep in mind that this framework is constantly evolving and many files in this repository can be outdated.<br>
Here are some links you may find useful:<br>
- [ALICE O2 documentation](https://aliceo2group.github.io/analysis-framework/docs/installing/) - O2 documentation, <br>
- [Analysis train : Run3_Conversion](https://alimonitor.cern.ch/trains/train.jsp?train_id=132) - information about converted trains (data information),<br>
- [Testing the O2 Analysis Framework](https://twiki.cern.ch/twiki/bin/viewauth/ALICE/AliceO2WP14AF).

## How to install O2 Framework?
Keep in mind most of us use Ubuntu 18.04 / 20.04. Firstly, build O2 with alibuild as explained in the [ALICE O2 documentation](https://aliceo2group.github.io/analysis-framework/docs/installing/). So, you need to follow instructions from [Installation via alibuild](https://alice-doc.github.io/alice-analysis-tutorial/building/custom.html) page, but do not follow instructions on "build packages" there! You also need to acquire GRID certificate, it's explained in detail here -> [_Get a Grid certificate_](https://alice-doc.github.io/alice-analysis-tutorial/start/cert.html).<br>
Keep in mind that it may not be easy and can take quite a long time. The first time I built this system it took me a while.

## Updating ALICE system
ALICE O2 Framework is constantly evolving so remember to stay up-to-date with the official repo. To update it you need to do the following: <br>
1. in `alice/O2` type `git pull --rebase`, <br>
2. in `alice/O2Physics` type `git reset --hard` and then `git pull --rebase`, <br>
3. in `alice/alidist` type `git pull --rebase upstream master`<br>
4. then in `alice/` type `aliBuild -j6 build O2Physics --defaults o2` <br>

Unfortunately, it takes a while and you have to do it quite often due to the constantly evolving system.<br>

### Remembering the repo version
Sometimes it is crucial to remember version you are on. In all repositories (in  `alice/alidist/`, `alice/O2/`, `alice/O2Physics`) type `git log`. To remember version you are on write down hash number.<br>
<details><summary>Here is an example ;)</summary>
<p>

![image](https://user-images.githubusercontent.com/87480906/173186280-9b8004dd-8f3a-4e17-a120-7821924a37fc.png)<br>
hash number in this case is: **291457fce1bf6da3c0ac5cf1afe53a12624a3a6c**

</p>
</details>

## How to obtain data files?
To get smaller AOD file go to -> [_Analysis train : Run3_Conversion_](https://alimonitor.cern.ch/trains/train.jsp?train_id=132) and scroll down. Choose train number you're interested in and click on run number (for example 246) and then on Test Results (in the middle). Scroll down and find full train option and then click on output. Now you're supposed to see a lot of files, but you're only interested in `AO2D.root` and it's usually the first. Then you need to only click on it and it will start to download.<br>
When you want to run your analysis on a bigger file go -> [_alimonitor.cern.ch/catalogue_](https://alimonitor.cern.ch/catalogue/index.jsp?path=%2Falice%2Fdata%2F2018%2FLHC18b%2F000285064%2Fpass1%2FPWGZZ%2FRun3_Conversion%2F267_20220301-1202_child_1#/alice/data/2018/LHC18b/000285064/pass1/PWGZZ/Run3_Conversion/267_20220301-1202_child_1). This may take a while, as the files are quite large. <br>
Information about trains (job details) can be found here -> [_alimonitor.cern.ch/job_details_](https://alimonitor.cern.ch/job_details.jsp).

### Possible errors
If you can't even enter alimonitor it is possible that you haven't added your GRID certificate to your browser correctly. Follow instructions on the [_Get a Grid certificate_](https://alice-doc.github.io/alice-analysis-tutorial/start/cert.html) page.

## How to create a task?
You can find useful examples on how to write your code in `~/alice/O2Physics/Tutorials/src/`. 
Then place your code in C++ in for example: `~/alice/O2Physics/Tutorials/src/`. Remember to always change CMakeList in the parent folder (in this case `~/alice/O2Physics/Tutorials/`) when you add a new file to your analysis. Without that step, you won't be able to build it. 

## How to build a task?
Go to `~/alice/sw/BUILD/O2Physics-latest-master/O2Physics`.
<details><summary>As soon as you enter a lot of text should appear and it should look like this:</summary>
<p>

![image](https://user-images.githubusercontent.com/87480906/162129203-4a4b833b-fefc-48c6-9229-908354cf0620.png)

</p>
</details>

In `~/alice/sw/BUILD/O2Physics-latest-master/O2Physics` enter ninja and O2Physics environment (`alienv load ninja/latest O2Physics/latest `). Then build your task using `ninja stage/bin/<your-analysis-file>`. If you don't know what you should type in <`your-analysis-file`> place, open `CMakeList.txt` and see how your analysis is called there. <br>
Keep in mind that if you add a new file or modify CMakeList you need to use `cmake .` 
Then, after building part, copy this builded file to directory with AOD file (`cp stage/bin/<your-analysis-file>`). You can skip this step (copying) when you use `ninja install` instead of `ninja` or you can use alibuild.

### Possible errors
If upon entering the `~/alice/sw/BUILD/O2Physics-latest-master/O2Physics` you don't see much text you probably don't have `eval "$(direnv hook bash)"` in your ~/.bashrc file. Type it there and open a new terminal. <br>
In general, in ~/.bashrc you should have following lines: <br>
`export ALIBUILD_WORK_DIR="$HOME/alice/sw"` <br>
``eval "`alienv shell-helper`"`` <br>
`eval "$(direnv hook bash)"` <br>
This can be written anywhere and will work but try writing it in one place, for example at the end of ~/.bashrc file <br>

## How to run a code?
In your directory with an AOD file type: <br>
`alienv enter O2Physics/latest` and  `alien-token-init your-cern-nickname`<br>
Then to run your code you only need to type: <br>
`./o2-analysistutorial-simple-analysis --aod-file <aod_file_name> -b` <br>
But sometimes you need to add more for it to work. When you see `Exception caught: Couldn't get TTree [sth]` you need to add it to your analysis. <br>
<details><summary>Example</summary>
<p>

While performing PID analysis you need to type for example: <br>
`./o2-analysistutorial-simple-analysis --aod-file <aod_file_name> --aod-memory-rate-limit 100000000000000 -b | o2-analysis-pid-tpc -b | o2-analysis-pid-tof -b | o2-analysis-trackselection -b | o2-analysis-tracksextention -b ` <br>
 Those o2-analysis-pid-tpc tasks are called **helper tasks** and they are used to produce tables that will be later used in the analysis. But keep in mind not everything might be useful in your case and `-b` stops GUI from showing.

</p>
</details>

### Possible errors
<details><summary>Couldn't get TTree.</summary>
<p>

 Sometimes you may get an error, for example: `[ERROR] Exception caught: Couldn't get TTree "DF_2853960297589372650/O2v0dataext from <your AOD file>"`. It means that this `v0dataext` couldn't be found in your AOD file or it wasn't produced by any attached helper task, _helper tasks are attached to your analysis like in the **Example** toggle above_. <br>
 So now you know which table is missing but finding the right task to add is still very confusing. Don't worry, there are two paths you can choose ;)<br>
 1. Easier one <br>
 Here is tutorial from [ ALICE O2 documentation- Tree not found ](https://aliceo2group.github.io/analysis-framework/docs/troubleshooting/treenotfound.html) and it's useful if you want to find more common tasks. So basically you need to enter [ALICE O2 documentation - helper task tables](https://aliceo2group.github.io/analysis-framework/docs/datamodel/helperTaskTables.html) and then you just search the table you are missing and you will find the task you need to attach.<br>
 2. More difficult one <br>
 So now, you need to enter directory `alice/O2Physics`. Then imagine you are missing `pidtofka` table. You will need to use grep command (if you didn't know it already check [this page](https://www.cyberciti.biz/faq/howto-use-grep-command-in-linux-unix/) ;)). Here is an example to make it easier:
 ```c
grep -rnw . -e "PIDTOFKA"
grep -rnw . -e "PIDTOFKa"
grep -rnw . -e "pidTOFKa"
```
 Now you should have a list of files where this phrase occurs. Next you need to find out the name of the file which produces this table. And then you need to go to `CMakeLists.txt` file and there you will find the name of the task you are looking for. If you are not familiar with CMakeLists don't worry! You can see that it has many sections but they all look alike, and here is an example:
  ```c
o2physics_add_dpl_workflow(<task-name>
                    SOURCES <C++ file>
                    PUBLIC_LINK_LIBRARIES O2::Framework O2::DetectorsBase O2Physics::AnalysisCore
                    COMPONENT_NAME Analysis)
```
So you need to find the **task-name** which will correspond to the **C++ file** you've found using grep ;) <br>
 

 
</p>
</details>

<details><summary>Fatal errors with non-existing CCDB entries.</summary>
<p>

While trying to run various O2 tasks/tutorials you can run into fatal errors with non-existing CCDB entries for specific data. <br>
Usually adding `--isRun2MC` to executables (likely `o2-analysis-timestamp`) might help. We also added `--isMC` to o2-analysis-event-selection. <br>
For example: `./o2-analysis-mm-dndeta --aod-file AODmc2.root --aod-memory-rate-limit 100000000000000 -b | o2-analysis-pid-tpc -b | o2-analysis-pid-tof -b | o2-analysis-trackselection -b | o2-analysis-trackextension -b | o2-analysis-event-selection --isMC -b | o2-analysis-timestamp --isRun2MC -b `

</p>
</details>

## Results
Running command `./o2-analysistutorial-simple-analysis --aod-file <aod_file_name> -b` (and others of that type) creates .root file and .json file. <br>
To enter AnalysisResults.root file enter O2Physics environment (`enter O2Physics/latest`) and type: `root -l` and then `new TBrowser`. (`-l` is optional but it runs root without additional information about it).<br>
You can change histograms manually or you can write macros and run them on .root file. If you've never written any root macros consider [_this repository_](https://github.com/zchochul/KADDre). You can find more on root and how to use it here -> [_root.cern/manual/_](https://root.cern/manual/first_steps_with_root/). <br>

## Useful tutorials and commands 
There has been a series of tutorials led by Victor Luis Gonzalez. It's an amazing source of examples! Most of those sessions are recorded and all of them have slides attached to them, I've listed them below, feel free to check them ;) <br>
Tutorials by Victor Gonzalez:<br>
 1. [Getting started: Configuring tasks, selecting collisions, filling outputs](https://indico.cern.ch/event/1171307/) <br>
 2. [Basic error recognition, json config files](https://indico.cern.ch/event/1172366/) <br>
 3. [Templates and process switch](https://indico.cern.ch/event/1175793/)<br>
 4. [Templates continuation, how to not repeat code](https://indico.cern.ch/event/1178484/) <br>
 5. [CCDB: Correcting for NUA, NUE, ...](https://indico.cern.ch/event/1180289/)<br>

This step may seem obvious, but sometimes you might find `grep` command useful. Especially, when you want to find tutorials with functions you are interested in. 

# Femtoscopy (FemtoWorld)<a name="Femtoscopy"></a>
Example codes are available here: `O2Physics/PWGCF/`
For data I've used 267 train.

## Preparing AOD
First of all you need to _simplify_ your AOD. After typing `alienv enter O2Physics/latest` run a shell script called <details><summary>`prod.sh`</summary>
<p>

`o2-analysis-cf-femtodream-producer-reduced --configuration json://prod-config.json  --aod-writer-resfile FemtoAO2D  --aod-writer-keep AOD/FEMTODREAMPARTS/0,AOD/FEMTODREAMCOLS/0,AOD/FEMTODEBUGPARTS/0 -b | o2-analysis-timestamp  --configuration json://prod-config.json -b | o2-analysis-multiplicity-table  --configuration json://prod-config.json -b | o2-analysis-event-selection  --configuration json://prod-config.json -b | o2-analysis-trackextension  --configuration json://prod-config.json -b | o2-analysis-pid-tpc  --configuration json://prod-config.json -b |  o2-analysis-pid-tof  --configuration json://prod-config.json --aod-memory-rate-limit 600000000 -b` <br>
 
I'm changing it quite frequently so it's just a concept, but I'll try keep it up to date.

</p>
</details>  <br>
Make sure you have `prod-config.json` file, you can download it from this repo. You need to change the "aod-file" part to match with your .root file/files (keep in mind this file also evolves, so this file may not be the latest version).<br>

### Cutculator
Type: `alienv enter O2Physics/latest` and then `o2-analysis-cf-femtodream-cutculator prod-config.json`. The following should appear on the screen:<br>
![image](https://user-images.githubusercontent.com/87480906/163264059-1c7a87d2-ba8f-4903-8ba9-4a91f2ee9ad1.png)<br>
After answering the cutculator's questions you should see something like that:<br>

![image](https://user-images.githubusercontent.com/87480906/163264481-98f87b78-fb4a-440e-b3c1-403dcd3a4d93.png)<br>

Then you need to change number in lines: <br>
`Configurable<uint32_t> ConfCutPartOne{"ConfCutPartOne", <number>, "Particle 1 - Selection bit from cutCulator"};` and similar line but with `ConfCutPartTwo`(in femtoDreamPairTaskTrackTrack.cxx) to the highlighted number from cutCulator.
For more info check [Femto dream framework introduction](https://indico.cern.ch/event/1092615/contributions/4594901/attachments/2350467/4009563/tutorial.pdf).

### Possible errors
**Cutculator can't find variables** - [Maja Kabus](https://github.com/saganatt/) wrote a patch for that (string.patch). <br>
Error: *terminate called after throwing an instance of 'boost::wrapexcept<boost::property_tree::ptree_bad_path>'* <br>

  ![image](https://user-images.githubusercontent.com/87480906/163265426-b105502d-c74c-40e1-8140-66fde2363f1a.png)<br>

  <br>Note that we ran the code on task `o2-analysis-cf-femtodream-producer-reduced`, but for now in `prod-config.json` you need to change that for `o2-analysis-cf-femtodream-producer` to fix this problem. That problem will be fixed later on.

## Running a simple task
To run a simple task type something like that:<br>
`o2-analysis-cf-femtodream-hash -b | o2-analysis-cf-femtodream-pair-track-track --aod-file FemtoAO2D.root --aod-memory-rate-limit 600000000 --ConfCutPartTwo 5543046 --ConfCutPartOne 5543046 -b`<br>

### Possible errors
You need to **make sure your aod is properly produced for femto analysis**. Using typical AOD files will cause errors (for example: `Couldn't get TTree "DF_2853960030894995121/O2femtodreamcols" from <your-AOD-file>`). <br>
Another problem I've encountered was the problem of **leaves in several trees not filling up**, make sure that you've changed `ConfCutPartOne` and `ConfCutPartTwo` value to the one from cutculator in run command and in code itself. <br>

## Creating dEta dPhi task
To do that, I've used existing code from FemtoDream directory. We are creating pararell directory to FemtoDream, called FemtoWorld.

Version of Femtodream FemtoWorld is based on is:
1.  `alice/alidist/` commit hash: **8ebd919bb75e468cb1a994f1012abfa22235ff2f**
2.  `alice/O2Physics/` commit hash: **c3dcdbca87dc9535de85f9837ad777de475ceee3**
3.  `alice/O2/` commit hash: **291457fce1bf6da3c0ac5cf1afe53a12624a3a6c**


# Github help ;) [work in progress c:] <a name="github"></a>
## Forking official repo
According to [this tutorial](https://alisw.github.io/git-tutorial/) create fork of [O2Physics repo](https://github.com/AliceO2Group/O2Physics). Then setup user configuration on your local computer:<br>
```
git config --global user.name "<Firstname> <Lastname>"
git config --global user.email <your-email-address>
git config --global user.github <your-github-username>
```
Then point git to this fork with:<br>
```
git remote add origin https://github.com/<your-github-username>/O2Physics
```
When you use `git status` you will be able to display the state of the working directory and the staging area. It lets you see which changes have been staged, which haven't, and which files aren't being tracked by Git. If you want to add files to your branch use:
```
git status <-- everything should be colored red
git add <file> <-- adding file to commit
git push https://<token>@github.com/zchochul/O2Physics.git <--pushing
git commit -m <commit info> <-- submitting a commit
```
If you want to add your file not to your branch only but also to main repo you need to make pull request.<br>
![image](https://user-images.githubusercontent.com/87480906/170225172-7c12f4bf-ea47-45e2-b060-d664a9ef46aa.png)<br>
and then after clicking on _1 commits ahead_ this page will show up and on it you need to create pull request<br>
![image](https://user-images.githubusercontent.com/87480906/170225312-9e5479a9-b3a0-4ac5-8a10-7ad6653ce84b.png)

### Clang formatting 
In order to add changes to official repo, your code needs to pass clang formatting test. 
What you need to do is quite easy, just enter clang environment by using `alienv enter Clang/latest` and then in the directory you have files you want to add, type `find . -iname "*.h" -o -iname "*.cxx" | xargs clang-format -style=file -i`. Keep in mind that this command changes only .h and .cxx, but you can add more or change to fit your files.

### Adding changes to fork branch
1.  `git pull --rebase` if you are not up-to-date with [AliceO2Group/O2Physics](https://github.com/AliceO2Group/O2Physics) repo.
2.  `git add `
3.  `git commit -m`
4.  `git push -f origin master:alifemto` <- pushing to fork, branch: alifemto (but can be any other)

The `-f` is actually required because of the rebase. Whenever you do a rebase you would need to do a force push because the remote branch cannot be fast-forwarded to your commit. You'd always want to make sure that you do a pull before pushing, but if you don't like to force push to master or dev for that matter, you can create a new branch to push to and then merge or make a PR. This action forces your remote repo to ignore differences in commits between your local and remote versions.

### Possible errors
`git reset --hard upstream/master`


