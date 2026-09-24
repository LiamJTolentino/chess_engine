# SP-14 Chess bot library

This repo is for developing the library functions used for our Chess AI bot.


> [!todo] TO-DO
> - [ ] Figure out if we want to make a database to store previously calculated positions
> - [ ] Make the header files for stuff we already know we're gonna implement
> - [ ] Decide on a memory-efficient format to store positional data in the graph nodes
> - [ ] Test a library function in the Unreal project


## How to integrate into Unreal

As of writing this, I have not yet added the scripts yet, but basically, there will be console scripts for each operating system and they will compile the code into the following formats:

| OS      | File Format |
| ------- | ----------- |
| Windows | .lib        |
| MacOS   | .a          |
| Linux   | .a          |


  

## Algorithm Overview/what needs to be implemented
This part, I (Liam) will update as we implement more stuff, but basically I have already figured out most of the overall functionality of the algorithm and it's just about how to implement the individual parts at the low-level to make sure the bot doesn't just eat up a ton of memory.

Basically, the algorithm is split into these three parts:
- Manager
- Analyzer
- Graph

### Manager
This basically encompasses all the public library functions that the Unreal Engine will use to interact with the AI. Most likely, we can just implement it as a class so that when the game loads, we just create an instance and that object will have everything the bot needs.
But this highlights an important part of the design of the algorithm which is that I want to parallelize it and that's why I split the algorithm into these parts instead of just doing a usual recursive minimax function. This means that we'll have to have ==thread-safe implementations== of most of the stuff. But yeah, all the Mini-max logic will be done here. 

> [!todo] TO-DO
> - [ ] List the functions and classes and stuff we need to implement for the Manager

### Analyzer
The Analyzer class will have all the functions related to *heuristic calculations* and *board transpositions*. We'll have one for each concurrent thread and each one should be able to have a representation of the chess board (probably a 2d array), and a bunch of functions that I will describe in a bit.

##### bool isWhiteToMove
True if in the currently loaded position needs white to make a move, False if it is black's move.
##### Board
Instance variable that will probably be represented as an 8x8 2d array of signed integers with a piece's material value representing the piece at that position and the sign representing the color (negative being black) while 0 represents an unoccupied square. This variable will be used in most of the calculations.
##### node currentNode
This variable will simply be a reference to the node that is being analyzed at the moment. We will ==need to implement a thread-safe system for the nodes== since each analyzer thread will share access to the same graph, but that shouldn't be too hard.
##### void loadFromFEN(string FEN)
Loads the position from the Forsyth-Edwards Notation (FEN) representation of the position. This one shouldn't be too hard.
##### void loadFromNode(Node node)
This will make more sense when I explain the Node struct, but basically, nodes will need to store their respective position in a very memory-efficient format, like maybe a [64-bit Zobrist hash](https://en.wikipedia.org/wiki/Zobrist_hashing) or something else. This function will allow the Analyzer to load from the queue.
##### bool isPositionLegal()
Returns true if the currently loaded position is legal (i.e. opponent king should not be capturable by the current player's pieces).
##### void transpose(string move)
Okay, I'm not sure yet if we should have the move parameter be a string in ==algebraic notation/PGN format or if we should have a more compact representation==, but basically, we want to be able to calculate the new position that results from a move, and if it is a legal move, we add the node to the graph if it doesn't exist yet and then queue it up for calculation.
##### float getHeuristic()
This will probably be the most important and complex function. For now, we just need to calculate the material balance which just involves adding up all of the values (except for the king) in the `Board` matrix and returning that as a float. 
##### getLegalMoves()
I didn't specify the return type here yet because I'm still not even sure how to store the moves, but basically, this just finds all the legal moves in the current position. To save on memory, and to give the player a more "human-like" opponent, the bot will ==only analyze a limited number of moves== depending on the difficulty. Also, we need to ==organize them by riskiness==. Highest priority will be moves that just immediately take back material that was lost or just moves that immediately capture hanging pieces. Next are low risk moves which are just ones that move a piece to a safe or defended square. For higher difficulties, we'll actually have trades at a higher priority than the low-risk moves. Trades are basically when you capture an opponent's piece with a piece of equal material and expect them to take back. Finally, for only the higher difficulties, we'll have sacrifices which are moves that place a high-valued piece in a square that is being attacked by an opponent piece.


### Graph
The graph is basically gonna be our database. So we need to decide what data we actually need to store in each node as every byte counts. 