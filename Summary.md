# Short summary
1. [Main informations](#Main)<br>
2. [Github repository with O2 instructions](#GithubO2Instructions)<br>
2. [O2 tutorial with Luca Barioglio and Maja Kabus](#PWGCFTutorial)<br>
3. [FemtoWorld](#PWGCFFemtoWorld)<br>

# Main informations <a name="Main"></a>
Affiliation: _Warsaw University of Technology_ <br>
Supervisor: _Dr inż. Łukasz Graczykowski_ <br>
Thesis topic: Analysis of angular correlations of protons with $\Phi$ mesons in proton-proton collisions in the ALICE experiment <br>

# Github repository with O2 instructions<a name="GithubO2Instructions"></a>
For almost a year now, I've been writing down all the information I thought is needed to better understand O2 software, on my repository on github. This mainly refers to basic operations in the O2 environment. For example, instructions with examples on how to build a system, upgrade, add your files and much more. In the instructions, I also included information on how to efficiently deal with errors that can sometimes occur. <br>
Link to this repository can be found here -> [Link to the repository](https://github.com/zchochul/AliceO2/#readme) <br>

# O2 tutorial with Luca Barioglio and Maja Kabus  <a name="PWGCFTutorial"></a>
I also helped organize a workshop with O2 along with Luca Barioglio and Maja Kabus. The code from the workshop is posted in the O2 tree.  The files can be found in the folder: `alice/O2Physics/PWGCF/Tutorial/` ([Link to the official github repository](https://github.com/AliceO2Group/O2Physics/tree/master/PWGCF/Tutorial)).<br>
My contribution was mainly to prepare a concise manual (in markdown) and to describe the structures learned in each of the smaller tutorials the workshop consisted of. <br>

# FemtoWorld <a name="PWGCFFemtoWorld"></a>
The code I added to the O2 tree can be seen here -> [`alice/O2Physics/PWGCF/FemtoWorld`](https://github.com/AliceO2Group/O2Physics/tree/master/PWGCF/FemtoWorld). The `FemtoWorld` folder code is based on a version from about 2 months ago of the `FemtoDream` code. I extended this base with functionalities that allow me to reconstruct phi mesons and get QA charts for them (as well as for their daughters). At this point, running my analysis already allows you to get a histogram of $\Delta \eta \Delta \varphi$ for $p-\Phi$. <br>

![image](https://user-images.githubusercontent.com/87480906/190134807-45d66984-5bd8-4806-bd90-fb30d67541e5.png) <br>

At this point, I have only done the analysis locally. The analysis has passed testing and I am waiting for results from hyperloop (they are being produced right now). Below I have provided a list of the histograms that form when you run the track-phi task.<br>

![image](https://user-images.githubusercontent.com/87480906/190138462-31223851-d02c-4983-bb25-6f8a904dd3d7.png)




