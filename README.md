# Running-Lizard 

This game is built with WinBGIm (Windows BGI - with mouse) graphics.h library v6.0
[Documentation](https://home.cs.colorado.edu/~main/cs1300/doc/bgi/ "WinBGIm Doc")

```
 # build the exe file
 g++ -g app.cpp -o bin/Debug/"Running Lizard" -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
 
 # run the exe file
 cd ./bin/Debug
 .\'Running Lizard.exe'
 ```

## Screenshots
> Stage 1
![stage_1](https://github.com/Md7tz/Running-Lizard/blob/master/Screenshots/sc1.PNG)

### TODO
```
1. fix left wall to right transition border (make the lizard appear on the right side when position of head is on X is 0) [DONE]
2. Add a life counter with a 3 image sprite 
3. Create a poison class inherited from food class through polymorphism
4. Add a popup message upon winning/losing with a retry prompt with the keystroke R (Keystroke Added without popup)
5. Add an fps counter on top right (optional)
6. instantiate more than one fruit(red apple) as an array of objects with randomized position
7. instatiate one poisonous apple at a time (no array of objects needed)
```

### Contribution
1. Create a branch with your name from master branch
2. clone the repo to your local machine `git clone https://github.com/Md7tz/Running-Lizard.git`
3. pull the latest changes `git pull`
4. switch to your branch with `git branch BranchName` (your branch should have a *)
6. add changes to your local repo with `git add .`
7. commit your changes with a message declaring the changes `git commit -m "message"`
8. push to your branch `git commit origin BranchName`
9. Create a pull request to the master branch and issue a review without merging

**Important Notes** 
* run the same configuration provided in *.vscode* folder for vscode to avoid debugging issues
* Add [DONE] for every achievement in the todo list
* Do not merge with the master branch only issue a pull request with revision
