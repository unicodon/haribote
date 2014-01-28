/*
    Copyright (C) 2005  University Of New South Wales
    Copyright (C) 2006  National ICT Australia

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software source code file, to deal in the file without restriction, 
    including without limitation the rights to use, copy, modify, merge, 
    publish, distribute, sublicense, and/or sell copies of the file, and to 
    permit persons to whom the file is furnished to do so, subject to the 
    following conditions:

    The above copyright notice and this permission notice shall be included in 
    all copies or substantial portions of the software source file.

    THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

/*******************************************************************************
*
* RoboCupGameControlData.h
*
* Broadcasted data structure and constants
*
* willu@cse.unsw.edu.au
* shnl327@cse.unsw.edu.au
*
*******************************************************************************/

#ifndef _RoboCupGameControlData_h_DEFINED
#define _RoboCupGameControlData_h_DEFINED

// data structure version number
#define STRUCT_VERSION              6

// data structure header
#define STRUCT_HEADER               "RGme"

// the maximum number of players per team
#define MAX_NUM_PLAYERS             11

// team numbers
#define TEAM_BLUE                   0
#define TEAM_RED                    1

// game states
#define STATE_INITIAL               0
#define STATE_READY                 1
#define STATE_SET                   2
#define STATE_PLAYING               3
#define STATE_FINISHED              4

// secondary game states
#define STATE2_NORMAL               0
#define STATE2_PENALTYSHOOT         1

// information that describes a player
struct RobotInfo {
    uint16 penalty;             // penalty state of the player
    uint16 secsTillUnpenalised; // estimate of time till unpenalised
};


// information that describes a team
struct TeamInfo {
    uint8 teamNumber;          // unique team number
    uint8 teamColour;          // colour of the team
    uint16 score;               // team's score
    RobotInfo players[MAX_NUM_PLAYERS];       // the team's players
};


struct RoboCupGameControlData {
    char   header[4];           // header to identify the structure
    uint32 version;             // version of the data structure
    uint8 playersPerTeam;       // The number of players on a team
    uint8 state;                // state of the game (STATE_READY, STATE_PLAYING, etc)
    uint8 firstHalf;            // 1 = game in first half, 0 otherwise
    uint8 kickOffTeam;          // the next team to kick off
    uint8 secondaryState;       // Extra state information - (STATE2_NORMAL, STATE2_PENALTYSHOOT, etc)
    uint8 dropInTeam;           // team that caused last drop in
    uint16 dropInTime;          // number of seconds passed since the last drop in.  -1 before first dropin
    uint32 secsRemaining;       // estimate of number of seconds remaining in the half
    TeamInfo teams[2];
};

#endif

