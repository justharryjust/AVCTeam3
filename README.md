# AVCTeam3 - Apollo 100
Our project repository for the ENGR101 autonomous vehicle challenge (AVC) 2016.
Members:
- Thomas Edwards - Lead/Manager
- Julian Schurhammer - Software
- Nicky McCleery - Hardware 
- Harry Just - Hardware
- Adam Jiang - Software


###Week 1 Checklist
- [x] Thomas: Write up Plan in proper table
- [x] Thomas: Check all have signed aggreement 
- [x] Thomas: photo of maze uploaded
- [x] Thomas: Research error correction and code for following line
- [x] Julian: Produce code for straight line
- [x] Julian: Research PID and SSH
- [x] Adam: Help out Julian 
- [x] Adam: Research networking communication signal to open gate
- [x] Harry+Thomas: Setup team github repository
- [x] Nicky+Harry: Develop prototype chassis with pi
- [x] Nicky+Harry: Try assemble first prototype
- [x] Everyone: Add each other on facebook and set up group chat

##Week 1 review
Everyone did their assigned tasks, we got a group chat going and everyone successfully showed up to our lab. The initial default robot was assembled and we managed to get the robot's wheels to run successfully. This week hopefully we can get our robot following the line and get the basic robot design sorted. 

##Week 2 Checklist
- [x] Thomas: Review and record progress
- [x] Nicky+Harry: Work on making upgraded chassis prototype 2 which can turn easily (180mm space) and hold sensors
- [x] Julian: Work on code for making robot follow the white line
- [x] Adam: Work on code which will open the network gate
- [x] Thomas: Help work on code or if not needed help with prototype 2
 
##Week 2 review
Week 2 we managed to get the robot to follow the line, but it used the "reverse backwards" method instead of PID so hopefully we can change it in future to use PID. On the hardware side we managed to make some plans for our final robot design, these need to be uploaded to github, and used to write our reports. Networking code managed to be completed but hasnt been tested yet.

##Week 3 Checklist
- [x] Thomas: Review and record progress
- [x] Nicky+Harry: Work on chassis design and try to start building it.
- [x] Julian: Work on code for making robot follow the white line through intersections and turns
- [x] Adam: Finalise and test networking code, ensure it works as expected, try and merge it with line following code
- [x] Thomas: Help work on code or if not needed help with prototype
- Overall robot should: move forward, stop at gate, send network signal, wait till gate is opened, start doing line following code

##Week 3 Review
Week 3 we managed to design all of our hardware additional features including the battery holder, sensor holders and the 3rd ball wheel. We are waiting for the 3D printers to become available so we can print these off. Code wise, our robot now follows the line through the first 2 quadrants successfully using well tuned PID. We have planned out the code for the third quadrant but have yet to test and implement it.

##Week 4 Checklist
- [x] Thomas: Review and record progress
- [x] Harry: Have our marble holder/third wheel printed out and screwed on robot.
- [x] Harry: Create a proper battery holder to hold the battery securely
- [ ] Nicky: Have all our IR sensor holders printed out and screwed on
- [x] Julian: Finish code to make robot completly follow white lines through quadrant 3.
- [x] Thomas: Write code so that robot responds to the walls of maze:
- There are 3 sensors facing the front, left and right. At 45 degree angles
- Robot should follow Right Hand wall to get to the end. (Doesnt have to stop at the end)
- Robot will need to balance itself so that it is always a constant distance from Right wall.
- Robot will need to have a method to turn itself left 90 degrees.
- [x] Thomas: Write full maze code with help from Adam
- [x] Julian: Help with Maze code id line code fully complete

##Week 4 Review
Week 4 we are behind slightly, we have the ball holder but still waiting on the infared sensor holders to be printed or improvised. Changing our thirdwheel had an unexpected setback of changing our line following PID code. Retesting and tweaking this code for the new wheel is well underway and it's able to make it though the first two quadrants again. The code for quadrant three is still being tested and adjusted. The maze code has been written in a first draft but not tested. Extra hours will need to be put in from each team member for the robot to be upto scratch for the day.

##Week 5 The Day.
- [x] Everything should be done. All code merged into single file.
- [ ] No changes made on the day.
- [x] Maze Code all done and working.
- During this weeks lab we will have to show Arthur how far our robot can get through the maze.

##Week 5 Review
Week 5, this week alot of hardwork and effort was put in by the team. The maze code managed to be completed and tested, the hardware was fully completed and everything was securely fastened in place. The hardwork paid off and we managed to make it through 100% of the course on the first try (Amazing), despite our practise runs only making it through about 30% of the time. The only change we made on the day was to uncomment our Network code. Our chassis broke during a test run that went wrong however by using another piece of chassis and some screws we were able to fix it.

##General Plan
 - First Week: Assign Roles, assemble basic robot, get it moving
 - Second Week: Robot moves through gate and in straght lines and turns, (Quadrant 1)
 - Third Week: Robot should be able to follow tape (Quadrant 2)
 - Fourth Week: Robot should be able to follow tape through right angle intersections and turns (Quadrant 3)
 - Fifth Week onwards: Work on Challenge part (Quadrant 4). Finnalise any code/hardware needing work
