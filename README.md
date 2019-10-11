# BattleShips
An IT project for 8 people

Rules: https://docs.google.com/document/d/19VdngkIq-78Jn62Yd7Ij1xswsAuLyxXw2JkDBo1eDco/edit?usp=drivesdk&fbclid=IwAR0bfOl0phoEOxubo9isoxRdp_ziM-x1jJtgtR_lGBWsBFlrg2yT4a68KBo

Hi. :)

==========BACKEND==========

So I added a backend prototype which currently communicates in stdin/stdout. The input/output works like that:

INITIALISATION: 

Input: 2 tables (one for each player) containing the positions of the ships, as seen in an example below.

0 0 1 1 1 1 1 0 0 0
0 0 0 0 0 0 0 0 0 0
5 5 0 3 3 3 0 4 4 4
0 0 0 0 0 0 0 0 0 0
0 0 0 0 2 0 0 0 0 0
0 0 0 0 2 0 0 0 0 0
0 0 0 0 2 0 0 0 0 0
0 0 0 0 2 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0

0 0 0 0 0 0 0 5 0 0
0 0 0 0 0 0 0 5 0 0
0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 3 0 1
0 0 2 2 2 2 0 3 0 1
0 0 0 0 0 0 0 3 0 1
0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 4 0 0
0 0 0 0 0 0 0 4 0 0
0 0 0 0 0 0 0 4 0 0

the ships here are defined by their id numbers, which correspond to respective ships:

1 - 5 tile ship (cruiser)
2 - 4 tile ship (battleship)
3 and 4 - 3 tile ships (cruiser and submarine)
5 - 2 tile ship (destroyer)

GAME: Two players move alternately. Player 1 starts the game.

Input: int tileX, int tileY (1 <= tileX, tileY <= 10), the coordinates of a targeted tile.

Output: int responseID, [int shipID]: 

responseID is an integer which denotes the situation:

0 - miss
1 - hit
2 - ship sunk

shipID is only used when responseID is 2. It denotes which ship was sunk. shipID corresponds to a number showed earlier in an example.

The program quits when any of the players win.

Keep in mind that this is only a prototype of how a backend should work. Of course, feel free to change the code.
