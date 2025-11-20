# dungeon-crawler

A simple dungeon crawler game for the terminal.

## How to run

This is a standard CMake project.

On Windows, download, install and open Visual Studio and install the appropriate tools (blank project -> Tools -> Get Tools and Features -> Desktop development with C++), then open the folder and click the green arrow to run.

On Linux, install CMake through your package manager and create a 'build' folder inside of the repo's directory. Using a terminal inside of the 'build' folder, run 'cmake ..' followed by 'cmake --build .', then simply './dungeon-crawler' to run.

## How to configure

The game is designed to be configured by the user, making this project actually a small game engine of sorts. The configuration takes place in a file called rooms.txt, which is generated in the same directory as the executable when you run it. By default, the configuration looks like this:

```
[ROOMS]

ROOM NUMBER: 100
MESSAGE: You enter a dark room. A light glimmers up north.
CONNECTIONS:
	NORTH: 101
	EAST: None
	SOUTH: None
	WEST: None
CHALLENGE: None

ROOM NUMBER: 101
MESSAGE: You enter a dingy living room. It is not well-kept.
CONNECTIONS:
	NORTH: None
	EAST: 1
	SOUTH: 100
	WEST: None
CHALLENGE: Physical, Puzzle

ROOM NUMBER: 1
MESSAGE: You enter the winning room. Congrats!
CONNECTIONS:
	NORTH: None
	EAST: None
	SOUTH: None
	WEST: None
CHALLENGE: None

[INTRODUCTORY TEXT]

You arise from a deep rest. There's a door up north.
```

The formatting is strictly enforced. You must use exactly the same capitalisation and whitespacing as the above template. The room connection lines (``NORTH: None``, ``SOUTH: 100``) must be indented with tabs.

Room number 1 is reserved for the final room of your game. Once this room is entered, its message is displayed followed by prompt asking you to exit the program.

As good practise, you should identify all other rooms by room number 100 and greater.

Currently, there are two types of challenges: physical and puzzle. The physical challenge spawns a delinquent in the room whom you must attack to proceed. The puzzle challenge spawns a note with a maths problem you must solve to proceed.

Add new rooms by adding more of these blocks after the last one, above the ``[INTRODUCTORY TEXT]`` line:

```
ROOM NUMBER: 102
MESSAGE: You enter another room. This is an example.
CONNECTIONS:
	NORTH: None
	EAST: None
	SOUTH: None
	WEST: None
CHALLENGE: None
```

## How to play

Launch the application in a terminal and follow the onscreen instructions.
