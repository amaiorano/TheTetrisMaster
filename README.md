# The Tetris Master

A tetris game for Windows that supports user-created themes.


## History

Development of The Tetris Master (TTM) began during the winter break of 1999, while I was still at University. One of the main goals of this project was to involve my friends in the creation process via the theming system.

Originally, the game used VESA BIOS Extension (VBE) 2 to allow for 16-bit colors at 320x240; however, many video cards had poor or no support for this mode. In 2004, I rewrote the graphics engine to use OpenGL so that it would run on all PCs running Windows.

Ten years later, 2014, I've decided to make the project available on Github as the source code is straightfoward, and can perhaps be a useful learning tool. The code has been mostly left in its original state, except for some minor tweaks to make it easier to build and play.


## Developer Setup

To build the game, all you need is Visual Studio 2012. Simply open the TheTetrisMaster.sln, select either Debug or Release configuration, and build the solution. Once the build is complete, the entire game should be in a generated in "_game/The Tetris Master/" folder. To debug any of the projects, set the Debugging properties to point to the exe within this folder, as well as the working directory.
