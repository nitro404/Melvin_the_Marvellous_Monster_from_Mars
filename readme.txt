===========================================
= Melvin the Marvellous Monster from Mars =
===========================================
= CLASS: COMP 2501                        =
= DATE: April 11, 2010                    =
=                                         =
= TEAM MEMBERS -------------------------- =
=  + Kevin Scroggins (100679071)          =
=  + Stephany Lay (100763723)             =
=  + Julie Powers (100758862)             =
=                                         =
= CONTACT INFORMATION ------------------- =
=  + kscrogg2@connect.carleton.ca         =
=  + slay@connect.carleton.ca             =
=  + jpowers@connect.carleton.ca          =
===========================================

DOCUMENTS

 + Our final project report and final presentation, as well as the documents
   from the project proposal can be found in the "Documents" folder.

 + The documentation for the Level Designer can be viewed by opening the file
   "index.html", located inside of the "/Level Designer/doc/" folder.
   Unfortunately due to time constraints, only the model is documented and not
   the GUI. If there are any questions regarding the 

INSTRUCTIONS

 + IMPORTANT: COMPILE THE GAME ENGINE UNDER RELEASE MODE ONLY. Debug mode
   contains extra debugging information / drawing, memory leak checking.

 + Requires Visual Studio 2008 and the latest copy of the DirectX SDK to
   compile our game engine. Pre-compiled executables have also been provided,
   just in case. When compiling, be sure that the paths are properly set up
   either in the development environment or the project properties to point to
   the DirectX Include and Library (x86) folders.

 + After compiling, it is much preferred if you use the provided batch files to
   play our game, instead of running it from inside of Visual Studio. Start by
   running "Release.bat" to create a playable version of the game, then run
   "Run Game.bat" to play the game.

 + If you wish to try our Level Designer, please ensure that the system's path
   variable is properly set up and includes an entry for the bin folder of the
   Java jdk (requires Java 1.6). Then run "Run Level Designer.bat" to open the
   Level Designer. This batch file also auto-compiles the Level Designer to
   ensure that your version is up to date. Pre-compiled class files will also
   be included just in case.

FINAL NOTES

 + Our Game Engine contains no memory leaks under standard execution.

 + The Game has very limited interaction - the most you can do is wear
   disguises.

 + There is no way to traverse between Levels, so this is why we temporarily
   have a level selection menu.

 + The AI characters have very limited collision detection (ie. the floating
   rat in the nuclear reactor) - if we had a few more minutes it would have
   been implemented. The code is there, just commented out be cause it does
   not function as expected.

 + All of our code is written from scratch by hand with the exception of our
   segmented line > line collision detection algorithm which was borrowed from
   a previous project and heavily re-written.

 + We hope you enjoy our game! Thanks!
