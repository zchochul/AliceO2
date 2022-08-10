This repository is a collection of notes I collected while working with O2 software. Keep in mind that I am still working on this repository and it is not complete. If you have any comments feel free to email me: [zchochul@cern.ch](mailto:zchochul@cern.ch)
# Table of contents
1. [Alice O2 - introduction](#AliceO2)<br>
2. [Hyperloop](#Hyperloop)<br>
2. [Femtoscopy](#Femtoscopy)<br>
3. [Github help ;)](#github)<br>

# AliceO2 <a name="AliceO2"></a>
In this repository you can find instructions on how to get started with ALICE O2 Framework. Keep in mind that this framework is constantly evolving and many files in this repository can be outdated.<br>
Here are some links you may find useful:<br>
- [ALICE O2 documentation](https://aliceo2group.github.io/analysis-framework/docs/installing/) - O2 documentation, <br>
- [@saganatt's Learning O2 repo ;)](https://github.com/saganatt/Learning-O2),<br>
- [@lbariogl, @saganatt & @zchochul O2Physics Tutorial for CF](https://github.com/CF-tutorials/O2Physics/tree/tutorial/PWGCF/Tutorial).<br>

## How to install O2 Framework?
Keep in mind most of us use Ubuntu 18.04 / 20.04. Firstly, build O2 with alibuild as explained in the [ALICE O2 documentation](https://aliceo2group.github.io/analysis-framework/docs/installing/). So, you need to follow instructions from [Installation via alibuild](https://alice-doc.github.io/alice-analysis-tutorial/building/custom.html) page, but **DO NOT** follow instructions on "build packages" there! You also need to acquire GRID certificate, it's explained in detail here -> [_Get a Grid certificate_](https://alice-doc.github.io/alice-analysis-tutorial/start/cert.html).
Keep in mind that it may not be easy and can take quite a long time. 

## Updating ALICE system
ALICE O2 Framework is constantly evolving so remember to stay up-to-date with the official repo. To update it you need to do the following: <br>
1. in `alice/O2` type `git pull --rebase`, <br>
2. in `alice/O2Physics` type `git reset --hard` and then `git pull --rebase`, <br>
3. in `alice/alidist` type `git pull --rebase upstream master`<br>
4. then in `alice/` type `aliBuild build O2Physics --defaults o2 --debug` <br>

Unfortunately, it takes a while and you have to do it quite often due to the constantly evolving system.<br>

### Tip: Remembering the repo version
Sometimes it is crucial to remember version you are on. In all repositories (in  `alice/alidist/`, `alice/O2/`, `alice/O2Physics`) type `git log`. To remember version you are on write down hash number.<br>
<details><summary>Here is an example ;)</summary>
<p>

![image](https://user-images.githubusercontent.com/87480906/173186280-9b8004dd-8f3a-4e17-a120-7821924a37fc.png)<br>
hash number in this case is: **291457fce1bf6da3c0ac5cf1afe53a12624a3a6c**

</p>
</details>

## How to obtain data files?
To get AOD file go to -> [_Analysis train : Run3_Conversion_](https://alimonitor.cern.ch/trains/train.jsp?train_id=132) and scroll down. Choose train number you're interested in and click on run number (for example 246). Now you have two options, you can download:<br>
**Smaller file:**<br>
Then you need to click on the **Test Results** (in the middle). Scroll down and find full train option and then click on output. Now you're supposed to see a lot of files, but you're only interested in `AO2D.root`. Then you need to only click on it and it will start to download.<br>
**Bigger file:**<br>
You need to find the table with {Status, Files, Processing, Merging, Statistics} rows. In the **Processing** row click on the _processing progress_. It will redirect you to the _alimonitor.cern.ch/catalogue_. Click on the output directory you are interested in. Then scroll down and find _AOD_ directory, then enter another directory (I think it doesn't really matter which one) and then download the _AO2D.root_ file. This may take a while, as the files are quite large. Please save the path your AOD is from (I have a .txt file with [names : paths]).<br>
Information about trains (job details) can be found here -> [_alimonitor.cern.ch/job_details_](https://alimonitor.cern.ch/job_details.jsp).

### Possible errors
If you can't even enter alimonitor it is possible that you haven't added your GRID certificate to your browser correctly. Follow instructions on the [_Get a Grid certificate_](https://alice-doc.github.io/alice-analysis-tutorial/start/cert.html) page.

## How to create a task?
You can find useful examples on how to write your code in `~/alice/O2Physics/Tutorials/src/`. 
Then place your code in C++ somewhere, for example: `~/alice/O2Physics/Tutorials/src/`. Remember to always change CMakeList in the parent folder (in this case `~/alice/O2Physics/Tutorials/`) when you add a new file to your analysis. Without that step, you won't be able to build it. When you add a new file remember to use `cmake .` command before building (it will be explained in the [How to build a task section](#How_to_build)).

## How to build a task? <a name="How_to_build"></a>
Go to `~/alice/sw/BUILD/O2Physics-latest-master/O2Physics`.
<details><summary>As soon as you enter a lot of text should appear and it should look like this:</summary>
<p>

![image](https://user-images.githubusercontent.com/87480906/162129203-4a4b833b-fefc-48c6-9229-908354cf0620.png)

</p>
</details>

In `~/alice/sw/BUILD/O2Physics-latest-master/O2Physics` enter ninja and O2Physics environment (`alienv load ninja/latest O2Physics/latest `). Then build your task using `ninja stage/bin/<your-analysis-file>`. If you don't know what you should type in <`your-analysis-file`> place, open `CMakeList.txt` and see how your analysis is called there. <br>
Keep in mind that if you add a new file or modify CMakeList you need to use `cmake .` 
Then, after building part, copy this builded file to directory with AOD file (`cp stage/bin/<your-analysis-file>`). You can skip this step (copying) when you use `ninja install` instead of `ninja` or you can use alibuild (`aliBuild build O2Physics --defaults o2 --debug` in the `~/alice/`).

### Possible errors
<details><summary>You don't see a lot of text after entering the directory</summary>
<p>

If upon entering the `~/alice/sw/BUILD/O2Physics-latest-master/O2Physics` you don't see much text you probably don't have `eval "$(direnv hook bash)"` in your ~/.bashrc file. Type it there and open a new terminal. <br>
In general, in ~/.bashrc you should have following lines: <br>
`export ALIBUILD_WORK_DIR="$HOME/alice/sw"` <br>
``eval "`alienv shell-helper`"`` <br>
`eval "$(direnv hook bash)"` <br>
This can be written anywhere and it will work, but try writing it in one place, for example at the end of ~/.bashrc file <br>

</p>
</details>
<details><summary>Coudln't find O2Physics/latest</summary>
<p>

So there are two options:<br>
1. When you builded the O2 there was an error and it didn't compiled completly. You have to correct the mistakes and try to build again. <br>
2. If it did in fact compiled completly there is an other error (I had it only once but it is possible xD). You need to change the `/latest` part for something you have in the `~/alice/sw/ubuntu2004_x86-64/O2` directory. But when I fully built again, the error disappeared.

</p>
</details>


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

<details><summary>Segmentation fault and returning to the command line</summary>
<p>

Make sure you use `-b` after every helper task. For example: <br>
 ```c
 ./o2-analysistutorial-simple-analysis --aod-file <aod_file_name> --aod-memory-rate-limit 100000000000000 -b | o2-analysis-pid-tpc -b | o2-analysis-pid-tof -b | o2-analysis-trackselection -b | o2-analysis-tracksextention -b 
 ```

</p>
</details>

<details><summary>FDD conversion</summary>
<p>

For converted data when using train number larger than 267 we did not include fdd converter helper task. We received an error type: `Couldn't get TTree O2fdd`.

</p>
</details>


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

## Sending a debug file
Sometimes you may encounter an error. Screenshot of a problem may not be enough to help you. You will need to send a debug file, which simply contains everything that appears to you on the screen after building or running something. So if the command shows you something on the screen simply pipe it to debug.txt like that:
  ```c
command > debug.txt 2>&1
```

# Hyperloop <a name="Hyperloop"></a>
 You can find useful information on the [ALICE O2 documentation](https://aliceo2group.github.io/analysis-framework/docs/hyperloop/).
 
## Creating a JIRA ticket
Firstly, create a JIRA ticket for your analysis on the [alice.its.cern.ch/jira](https://alice.its.cern.ch/jira/secure/Dashboard.jspa). You have to log in. Click on the **Create** button on the top of the page. You need to fill the table. For example: <br>
- Project -> `PWG-CF (PWGCF)`, so here select your PWG<br>
- Issue Type -> `Analysis`, only those will be linked to the Hyperloop system,<br>
- Summary -> <name-of-your-analysis>, it should be as broad as the analysis you are doing, but it should be informative <br>
- Component/s - `PAG-CORRELATION-EBYE` choose your PAG<br>
- Run 3 analysis - `Yes`, always<br>
- Analyzers - add analyzers from your team, remember to add yourself ;), those people will be able to work with you on this project,<br> 
- Data Set - you can write there anything, <br>
- Public Result - `Yes`<br>

You do it once per analysis. Now you should be able to to see it on the [AliHyperloop page](https://alimonitor.cern.ch/hyperloop/). It can take some time to synchronize JIRA with Hyperloop.

## Creating wagons, adding dependencies
When you click on your analysis you will be able to see all thge wagons that are created for this analysis (now it's empty click on the `Add new wagon`). In wagon name write the wagon name, then version of the O2Physics (choose the latest version of the repo, but you can choose a different one if different version produces errors for you). In workflow name choose the main task you want to run. And then you save.<br>
 You can later configure it. As you know tasks usually need a lot of the helper tasks. You need to add them in the dependencies. You also need to add **Datasets**. Choose if you want to work with Data or MC, then choose type of collision and a period you are interested in. You will get a list of datasets available for now. But this part is also constantly evolving too, so there are a lot of new datasets comming ;) <br>
 
## Running a test
 Now, the good information for the people previously working with the AliPhysics, the tests are done here automatically ;) When the test is done you should see a :heavy_check_mark: :sunny: or :heavy_check_mark: :exclamation:. This means that the analysis went okay. But if you see a :bomb:, it means that something went wrong. Maybe there weren't enough helper tasks added or maybe the dataset is faulty. But if everything is okay you can click on the :sunny: and find _test output_ with the _AnalysisResults.root_.

## Running a long train
You have to create a request on the ALICE's `O2 Hyperloop Operation` mattermost channel simply by writing on the chat to the **train operators**. It is crucial that in order to turn on the long train beforehand to have an acccpeted test with small dataset linked to this long train. That's what I can see after this request: <br>
![image](https://user-images.githubusercontent.com/87480906/183655686-5bb0d603-9f17-4fe7-8444-6a1fa6e6ce4c.png)<br>
When it's finished it will say **Done**. Then you need to click on the **Train number** and then on the **Merge output** and you need to download the _AnalysisResults.root_ file and open it with root.
 
# Femtoscopy (FemtoWorld)<a name="Femtoscopy"></a>
Example codes are available here: `O2Physics/PWGCF/`.For data I've used 267 train.<br>
Here you can find: [FemtoWorld to do list](https://github.com/zchochul/AliceO2/blob/main/Tasks.md)

## Preparing AOD
First of all you need to _simplify_ your AOD. After typing `alienv enter O2Physics/latest` run a shell script called [`prod.sh`](https://github.com/zchochul/AliceO2/blob/main/prod.sh) I'm changing it quite frequently so it's just a concept, but I'll try keep it up to date. To run it you need to simply type `./prod.sh prod-config.json`.
 <br>
Make sure you have [`prod-config.json`](https://github.com/zchochul/AliceO2/blob/main/prod-config.json) file, you can download it from this repo. You need to change the "aod-file" part to match with your .root file/files (keep in mind this file also evolves, so this file may not be the latest version).<br>

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
To FemtoWorld task I'm using [`post.sh`](https://github.com/zchochul/AliceO2/blob/main/post.sh) and [`post-config.json`](https://github.com/zchochul/AliceO2/blob/main/post-config.json) files. You need to be in the O2Physics environment (type `alienv enter O2Physics/latest`).<br>

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
Here I've listed some steps but if it doesn't work in your case please follow [@saganatt's github instructions](https://github.com/zchochul/AliceO2/blob/main/gitInstructions.md) they are better ;)
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
When you use `git status` you will be able to display the state of the working directory and the staging area. It lets you see which changes have been staged, which haven't, and which files aren't being tracked by Git. If you want to add files to your branch (for example **femtoworld**) use:
 1. `git branch -vvv` <-- it tells you on which branch you are on <br>
 2. `git checkout femtoworld` <-- in this way you can switch to your desired branch <br>
 3. `git status`  <-- everything not added should be colored red <br>
 4. `git add <file>` <-- adding file to commit, now in `git status` they will be colored green<br>
 5.  `git commit -am "<commit-name>" `<-- submitting a commit, (a for all files and all changes)<br>
 6. `git push -f` <-- pushing <br>
 

The `-f` is actually required because of the rebase. Whenever you do a rebase you would need to do a force push because the remote branch cannot be fast-forwarded to your commit. You'd always want to make sure that you do a pull before pushing, but if you don't like to force push to master or dev for that matter, you can create a new branch to push to and then merge or make a PR. This action forces your remote repo to ignore differences in commits between your local and remote versions.<br>
                                       
If you want to add your file not to your branch only but also to main repo you need to make pull request.<br>
![image](https://user-images.githubusercontent.com/87480906/170225172-7c12f4bf-ea47-45e2-b060-d664a9ef46aa.png)<br>
and then after clicking on _1 commits ahead_ this page will show up and on it you need to create pull request<br>
![image](https://user-images.githubusercontent.com/87480906/170225312-9e5479a9-b3a0-4ac5-8a10-7ad6653ce84b.png)

### Clang formatting 
In order to add changes to official repo, your code needs to pass clang formatting test. 
What you need to do is quite easy, just enter clang environment by using `alienv enter Clang/latest` and then in the directory you have files you want to add, type `find . -iname "*.h" -o -iname "*.cxx" | xargs clang-format -style=file -i`. Keep in mind that this command changes only .h and .cxx, but you can add more or change to fit your files.

### Possible errors
<details><summary>`git reset --hard upstream/master`.</summary>
<p>

</p>
</details>
 
<details><summary>Branch is not following the branch on remote</summary>
<p>
 
You need to use:
 ```c
 git fetch origin
 git branch --set-upstream-to=origin/femtoworld
 ```
 
</p>
</details>



 


