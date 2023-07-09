cd O2Physics
git reset --hard
git pull --rebase
cd ../alidist
git pull --rebase upstream master
cd ../
aliBuild build O2Physics --defaults o2 --debug
