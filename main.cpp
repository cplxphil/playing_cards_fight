// version 7


// To debug the AI on this:  Go back to the prototype version and debug it there.

// I fixed the circle sprites problem!  The only big problem is, debugging the AI.

// Work on it tomorrow...don't give up.  Do a second version of the prototype, where the row and col are done properly.







//
//  main.cpp
//  Playing Cards Fight, Version 6.0            // It's now version 7.0
//
//  Created by Philip White on 11/7/25.
//


// SFML is the only graphics library I can use.

#include <iostream>
//#include <tbb/parallel_for.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <sstream>
#include <cassert>
//#include "omp.h"
#include "SFML/Graphics.hpp"
#include <optional>
#include <chrono>
#include <thread>
//#include "dijkstrafromgeeksforgeeks.cpp"

using namespace std;
//using WeightVertexPair = std::pair<int, int>; // this line of code seems to make the whole thing crash....


// The rule is, the first mentioned index in a 2D vector *must* be the first one that you iterate over.  You can still call it col.
// After you get the graphics up, test the underlying code again...I changed the indexes to be in accordance with the statement above.
// The idea is, "i" means "col," and "j" means "row," in any of my two index things.  You *must* iterate over row *first*.

struct player
{
    player()
    {
        username = "";
        playerID = -1;
        numPoints = -1;
        isHuman = true;
    }
    
    string username;
    int playerID;               // 1 , 2 , 3 , or 4
    int numPoints;              // first player to get 1000 (or some other number) points wins
    bool isHuman;
};


struct card
{
    void copy(card a)
    {
        rank = a.rank;
        suit = a.suit;
        playerOwner = a.playerOwner;
        colLocation = a.colLocation;
        rowLocation = a.rowLocation;
    }

    card()
    {
        rank = -1;
        suit = -1;
        playerOwner = -1;
        colLocation = -1;
        rowLocation = -1;
    }
    
    int rank;           // 2-10 = 2-10, jack = 11, queen = 12, king = 13, ace = 14 ; nothing is equal to 0 or 1
    char suit;          // 'h' for hearts, 'c' for clubs, 'd' for diamonds, 's' for spades
    int playerOwner;    // 0 = gray = no player, 1 = red = Player 1, 2 = blue = Player 2, 3 = green = Player 3
                        //      4 = yellow = Player 4; note, all blank squares are gray, but gray isn't displayed then
    int colLocation;    // 0-11 grid position column
    int rowLocation;    // 0-11 grid position row
    
    void intToSuit(int val);            // updates suit within this function
    void displayCard();                 // based on suit and rank
    void displayCard2();
};


struct grid
{
    grid()
    {
        theGrid.resize(0);
    }
    
    vector <vector <card> > theGrid;
};


struct gameMove
{
    gameMove()
    {
        row1 = col1 = row2 = col2 = 0;
    }
    
    // "from" square, 0-11
    int row1;
    int col1;
    // "to" square, 0-11
    int row2;
    int col2;
    
    bool isMoveLegal(grid gameGrid, int whoseTurn);         // from & to must be vert./horiz. adjacent, the from square
                                                            // must contain a card that is owned by var whoseTurn, and
                                                            // the to square must be empty...and all moves must be in
                                                            // bounds.
    
    void displayMove()
    {
        //cout << "From square (" << row1 << "," << col1 << " to square " << row2 << "," << col2 << ").\n";
    }
};

struct bestCardPairAndScore
{
    bestCardPairAndScore()
    {
        shortestPathSize = -1;
    }
    card card1;
    card card2;
    int shortestPathSize;
};


struct bestCardPairScoreWithMoves
{
    bestCardPairScoreWithMoves()
    {
        score = -1;
    }
    void displayPair()
    {
        //cout << "\ncard a = ";
        a.displayCard();
        //cout << "\ncard b = ";
        b.displayCard();
        //cout << "\nmove c = ";
        c.displayMove();
        //cout << "\nmove d = ";
        d.displayMove();
        //cout << "score = " << score;
    }
    
    card a;
    card b;
    gameMove c;             // the move a can make to get closer
    gameMove d;             // the move b can make to get closer
    int score;              // sum of shortest paths to the target card (don't make it the adjacent square)
};



struct cardOwnershipChange
{
    cardOwnershipChange()
    {
        newPlayerOwner = -1;
    }
    card theCard;
    int newPlayerOwner;                 // 1 - 4 (0 never takes over as the owner color)
};

/*struct graphicsBundle
{
    //sf::Texture t;
    
    
    
    
    
    vector < vector <sf::Sprite> > gridOfSquares;
    
    
    
    vector < vector <sf::Texture> > texturesForGrid;
    
};*/




struct square
{
    int row;
    int col;
    
    square()
    {
        col = -1;
        row = -1;
    }
};


struct cardAndMove
{
    card a;
    gameMove b;
};



// Remember and fix throughout:  it's [col][row], and [j][i]

struct gameState
{
    gameState()
    {
        whoseTurn = movementPointsTotal = turnNum = -1;
        players.resize(0);
    }
    
    vector <player> players;
    int whoseTurn;          // equal to the playerID of the player whose turn it is
    grid gameGrid;
    int movementPointsTotal;        // from the dice roll; governs how many moves a player can make before turn ends
    int turnNum;                    // how many turns have elapsed?
    
    bool areTwoCardsAdjacent(card a, card b);
    card oneWhoseTurnCardIsAdjacent(card Y);
    void initGame();
    void addPointsBasedOnBaseSquares();           // base squares are the 9 squares in each of the four corners
    void manageHumanFullTurn();
    vector <card> processCapturesAtEndOfTurn();          // return true if any captures happened, false otherwise
    //gameMove getAImove();
//    gameMove getHumanPlayerMove();
    //int getPositionStrength();
    void applyMove(gameMove theMove);
    void displayGame();
    
    cardAndMove bestCardToMoveHome();
    vector <card> allEmptyAdjacentSquaresToCardZ(card Z);
    //bool isCardAAdjacentToCardB(card a, card b);
};


struct dice
{
    dice()
    {
        die1 = die2 = -1;
    }
    
    int roll();         // randomizes die1 and die2 as ints between 1 and 6, and returns their sum
    
    int die1;
    int die2;
};





struct deckOfCards
{
    deckOfCards()
    {
        deck.resize(0);
    }
    
    vector <card> deck;
    
    void shuffle();
    void displayDeck();         // won't be needed in the final version
    
};

struct node
{
    node()
    {
        nodeID = gridCol = gridRow = -1;
        reachable = true;
    }
    
    int nodeID;
    int gridRow;        // for reference
    int gridCol;        // for reference
    bool reachable;
    
    void displayNode()
    {
        ;//cout << "\nNodeID = " << nodeID << ", gridRow = " << gridRow << ", gridCol = " << gridCol << ".\n";
    }
};

struct edge
{
    node fromNode;
    node toNode;
};


struct nodeAndDistanceFromSourceAndVisited
{
    node a;
    int distance;
    bool visited;
    
    nodeAndDistanceFromSourceAndVisited()
    {
        distance = -1;
        visited = false;
    }
};



struct DijkstraStorage
{
    node a;
    vector <nodeAndDistanceFromSourceAndVisited> dist;
    vector <node> nodeIDs;
};


struct digraph
{
    vector <DijkstraStorage> allSourceNodeDijkstraLists;
    vector <node> graphNodes;                 // each node has an integer ID
    vector <edge> allEdges;
    

    void displayDigraph(node Y);
    digraph(grid gameGrid);                           // constructor
// vector <node> Dijkstra(node source , node target);              // find list of nodes to visit to form the shortest path from node A to node B
    int findNodeIndexInGraphNodes(int row, int col);
    void customDijkstra(node source);           // updates allSourceNodeDijkstraLists
    node findNode(int row, int col);
    bool thereIsAnEdgeFromAToB(node a, node b);
    void displayGraphEdges();
    int minDistance(vector <int> dist, vector <bool> shortestPathTreeSet);
    vector <nodeAndDistanceFromSourceAndVisited> getAllUnvisitedNeighborVertices(node a, vector <nodeAndDistanceFromSourceAndVisited> allNodes);
    void initialDijkstra(node a);
    int getSourceToTargetDistance(node source, node target);        // before we call this, we *must* call customDjikstra on source.
};





void digraph::displayDigraph(node Y)
{
    customDijkstra(Y);
    int i, j;
    for ( i = 0; i < 12; i++)
    {
        for (j = 0; j < 12; j++)
        {
            for (int k = 0; k < graphNodes.size(); k++)
            {
                if (j == graphNodes[k].gridCol && i == graphNodes[k].gridRow)
                {
                    ;//cout << " " << getSourceToTargetDistance(Y, graphNodes[k]) << " ";
                }
            }
        }
        //cout << endl;
    }
}

node digraph::findNode(int row, int col)
{
    int i;
    for (i = 0; i < graphNodes.size(); i++)
    {
        assert(i >= 0 && i < graphNodes.size());
        if (graphNodes.at(i).gridRow == row && graphNodes.at(i).gridCol == col)
        
        {
            assert(i >= 0 && i < graphNodes.size());
            return graphNodes.at(i);
        }
    }
    node temp;              // if the node was not found
    temp.nodeID = -1;
    return temp;
}

int digraph::findNodeIndexInGraphNodes(int row, int col)
{
    int i;
    for (i = 0; i < graphNodes.size(); i++)
    {
        if (graphNodes[i].gridRow == row && graphNodes[i].gridCol == col)
        {
            return i;
        }
    }
    return -1;
}


struct distanceAndNodeID
{
    distanceAndNodeID()
    {
        nodeID = distance = -1;
    }
    
    int nodeID;
    int distance;
};



int getNodeIndex(node source,vector <node> graphNodes);


void outputPlayerColor(int playerOwner, int rank, int suit);

// vector <square> getEmptyAdjacentSquares(card c, vector< vector <card> > grid);


bestCardPairScoreWithMoves findClosestTwoCardCaptureOpportunityForAI(card Y, gameState game);

void manageAIfullTurn(gameState & game);

//void updateBoardCircles (gameState gs,vector < vector <sf::Sprite> > oldBoard, graphicsBundle & gb);


// gameMove getOneMoveCaptureOpportunities(gameState game);

//void updateBoardCards(gameState gs,vector < vector <sf::Sprite> > oldBoard, graphicsBundle & gb);

gameMove getAImoveForOneMovementPoint(gameState game);


bool gameState::areTwoCardsAdjacent(card a, card b)
{
    if (((a.colLocation - b.colLocation == 1 || b.colLocation - a.colLocation == 1)
        && a.rowLocation == b.rowLocation) || ((a.rowLocation - b.rowLocation == 1 || b.rowLocation - a.rowLocation == 1)
        && a.colLocation == b.colLocation))
    {
        return true;
    }
    else
    {
        return false;
    }
}





cardAndMove gameState::bestCardToMoveHome()        // recode this function too
{
    // just select any card that is ranked 10 or above that could be moved closer to the corner square that is associated with that player.
    
    vector <square> playerCorner;
    playerCorner.resize(4);
    playerCorner.at(0).row = 0;     // player 1, red
    playerCorner.at(0).col = 0;
    playerCorner.at(1).row = 0;       // player 2, blue
    playerCorner.at(1).col = 11;
    playerCorner.at(2).row = 11;          // player 3, green
    playerCorner.at(2).col = 11;
    playerCorner.at(3).row = 11;        // player 4, yellow     // now gray
    playerCorner.at(3).col = 0;
    
    int playerIndex = whoseTurn-1;
    
    vector <card> allWhoseTurnCards;
    
    int i, j;
    for (i = 0; i < gameGrid.theGrid.size(); i++)
    {
        for (j = 0; j < gameGrid.theGrid.at(i).size(); j++)
        {
            if (gameGrid.theGrid.at(i).at(j).playerOwner == whoseTurn)
            {
                allWhoseTurnCards.push_back(gameGrid.theGrid.at(i).at(j));
            }
        }
    }
    
    for (int i = 0; i < gameGrid.theGrid.size(); i++)
    {
        for (j = 0; j < gameGrid.theGrid[i].size(); j++)
        {
            //cout << "\t";
            gameGrid.theGrid[i][j].displayCard();
            //cout << "\t";
        }
        //cout << endl;
    }
    
    displayGame();
    
    vector <card> adjToWhoseTurnCardsWithMoves;
    cardAndMove toReturn;
    cardAndMove temp;
    vector <cardAndMove> allGoodOptions;        // we'll select randomly from this
    
    
    // BE **VERY CAREFUL** WITH THIS CODE SNIPPET!  DID YOU GET ALL THE i's AND j's RIGHT??
    // DOUBLE CHECK THIS LATER
    for (i = 0; i < allWhoseTurnCards.size(); i++)
    {
        adjToWhoseTurnCardsWithMoves = allEmptyAdjacentSquaresToCardZ(allWhoseTurnCards.at(i));
        for (j = 0; j < adjToWhoseTurnCardsWithMoves.size(); j++)
        {
            if (((playerCorner.at(playerIndex).col < adjToWhoseTurnCardsWithMoves.at(j).colLocation &&
                adjToWhoseTurnCardsWithMoves.at(j).colLocation < allWhoseTurnCards.at(i).colLocation)
                 || (playerCorner[playerIndex].col > adjToWhoseTurnCardsWithMoves[j].colLocation &&
                     adjToWhoseTurnCardsWithMoves[j].colLocation > allWhoseTurnCards[i].colLocation))
                 
                 
                 // if the adjacent empty square is "in the
                                                                                   // right direction" to be moving towards the player's corner...
                || ((playerCorner.at(playerIndex).row < adjToWhoseTurnCardsWithMoves.at(j).rowLocation &&
                    adjToWhoseTurnCardsWithMoves.at(j).rowLocation < allWhoseTurnCards.at(i).rowLocation)
                   || (playerCorner[playerIndex].row > adjToWhoseTurnCardsWithMoves[j].rowLocation &&
                       adjToWhoseTurnCardsWithMoves[j].rowLocation > allWhoseTurnCards[i].rowLocation))
                )
            {
                temp.a = allWhoseTurnCards.at(i);
                temp.b.col1 = allWhoseTurnCards.at(i).colLocation;
                temp.b.row1 = allWhoseTurnCards.at(i).rowLocation;
                temp.b.col2 = adjToWhoseTurnCardsWithMoves.at(j).colLocation;
                temp.b.row2 = adjToWhoseTurnCardsWithMoves.at(j).rowLocation;
                //if (temp.b.isMoveLegal(gameGrid, whoseTurn) == false)
                //{
                    //cout << "\n\nMajor error, bad move selected in bestcardtomovehome function.\n\nQuitting after you press enter.\n\n";
                    //displayGame();
                    //temp.b.displayMove();
                    
                    //string inputLine = "";
                    
                    //getline(cin, inputLine); // Grab whole line
                    //exit(0);
                //}
                if (temp.a.rank > 9 && temp.b.isMoveLegal(gameGrid, whoseTurn) == true)
                {
                    allGoodOptions.push_back(temp);
                }
            }
        }
    }
    
    if (allGoodOptions.size() == 0)
    {
        temp.a.rank = -1;
        temp.b.col1 = temp.b.col2 = temp.b.row1 = temp.b.row2 = -1;
        return temp;
    }
    return allGoodOptions[rand() % allGoodOptions.size()];
}



gameMove getAImoveForOneMovementPoint(gameState game)
{
    int value;
    string inputString;
    vector <card> allTargetCards;
    
    gameMove move1, move2, move3;
    allTargetCards.resize(0);
    int i;
    
//    #pragma omp parallel for
    for (i = 0; i < game.gameGrid.theGrid.size(); i++)
    {
        for (int j = 0; j < game.gameGrid.theGrid.at(i).size(); j++)
        {
            if (game.gameGrid.theGrid.at(i).at(j).rank != 0 && game.gameGrid.theGrid.at(i).at(j).playerOwner != game.whoseTurn)
            {
                allTargetCards.push_back(game.gameGrid.theGrid.at(i).at(j));
            }
        }
    }
    
    bestCardPairScoreWithMoves temp, best;
    int index = 0;
    
    best.score = INT_MAX;

    if (allTargetCards.size() == 0)
    {
        //cout << "ERROR:  No target cards exist, all cards belong to one player only.";
        exit(0);
    }
    

    //#pragma omp parallel for
    // Don't parallelize this...there is shared memory access, we both read and write the temp variable and the best variable during the loop.
    for (index = 0; index < allTargetCards.size(); index++)
    {
        
        temp = findClosestTwoCardCaptureOpportunityForAI(allTargetCards[index], game);
        
        if (temp.score != INT_MAX && temp.score != -1 && temp.score < best.score)
        {
            if ((temp.a.colLocation != 0 || temp.a.rowLocation != 0 || temp.b.colLocation != 0 || temp.b.rowLocation != 0)
                && temp.c.isMoveLegal(game.gameGrid, game.whoseTurn) &&/* temp.d.isMoveLegal(gameGrid,whoseTurn) && */ temp.a.colLocation != -1 &&
                temp.a.rowLocation != -1 /*&& temp.b.colLocation != -1 && temp.b.rowLocation != -1*/)
                
            {
                best = temp;
                
            }
            
        }
        // index++;     // What the heck was this doing in there??!!  I bet that caused most of the errors!!! :D.
    }
    
    cardAndMove m = game.bestCardToMoveHome();
    move1 = best.c;
    move2 = best.d;
    move3 = m.b;
    
    /*vector <gameMove> toReturnFrom;
    
    if (move1.isMoveLegal(game.gameGrid, game.whoseTurn))       // mute the cout output in the final version
    {
        toReturnFrom.push_back(move1);
    }
    
    if (move2.isMoveLegal(game.gameGrid, game.whoseTurn))
    {
        toReturnFrom.push_back(move2);
    }
    
    if (move3.isMoveLegal(game.gameGrid, game.whoseTurn))
    {
        toReturnFrom.push_back(move3);
    }                         // temporary commenting out, restore this later
    
    
    if (toReturnFrom.size() == 0)
    {
        cout << "ERROR, no AI move selected.";
        
        string inputLine = "";
        
        getline(cin, inputLine); // Grab whole line
        
        exit(0);
    }
    else
    {
        cout << "key flag";
    }
    */
    //move1.displayMove();
    
    //return toReturnFrom[rand()%toReturnFrom.size()];
    
    
    //cout << "KEY TEST:  game.whoseTurn = " << game.whoseTurn << endl;
    
    if (!move1.isMoveLegal(game.gameGrid, game.whoseTurn) && !move2.isMoveLegal(game.gameGrid, game.whoseTurn)
        && !move3.isMoveLegal(game.gameGrid, game.whoseTurn))
    {
        gameMove skipMove;
        skipMove.col1 = skipMove.col2 = skipMove.row1 = skipMove.row2 = -1;
        //cout << "MARKED POINT.";
        return skipMove;
    }
    
    if ((
    (!move1.isMoveLegal(game.gameGrid,game.whoseTurn
                        && !move2.isMoveLegal(game.gameGrid,game.whoseTurn)))))
    {
        //cout << "yikes, whoseTurn = " << game.whoseTurn << "\n"; move1.displayMove(); cout << " "; move2.displayMove();
    }
    
    
    // temporarily commented out, to focus on capture AI        // no longer commented out.
    if (move3.isMoveLegal(game.gameGrid,game.whoseTurn))
    {
        int value = rand() % 3;
        //cout << "Random value = " << value << ".\n";
        if ((value == 0) || (!move1.isMoveLegal(game.gameGrid,game.whoseTurn)
                            && !move2.isMoveLegal(game.gameGrid,game.whoseTurn))) // 1 in 3 chance of making this move, usually.
            
            // I think it was some subtle parentheses trouble in the condition above.
        {
            return move3;
        }
   }
    
    if (move1.isMoveLegal(game.gameGrid,game.whoseTurn) == true && move2.isMoveLegal(game.gameGrid,game.whoseTurn) == false)
    {
        return move1;
    }
    if (move2.isMoveLegal(game.gameGrid,game.whoseTurn) == true && move1.isMoveLegal(game.gameGrid,game.whoseTurn) == false)
    {
        return move2;
    }
    if (rand()%2 == 0)
    {
        return move1;
    }
    return move2;
}

// 11-11, 3:14 p.m.:  What happened during the last crash:  Move3 was legal, but move1 and move2 weren't found...so the AI returned a
//  coinflip of 1 and 2, and the return proceeded and the program didn't quit because there was one legal move.



void manageAIfullTurn(gameState & game)
{
    
    vector <gameMove> toReturnFrom;
    toReturnFrom.resize(0);
    int i;
    if (true)   // game.turnNum % 10 == 0)
    {
        game.addPointsBasedOnBaseSquares();
        //cout << "Score Update:\n";
        for (i = 0; i < 4; i++)
        {
            //cout << "\nPlayer " << game.players.at(i).playerID << " now has " << game.players.at(i).numPoints;
        }
        //cout << endl << endl;
    }
    
    
    dice theDice;
    int movementPointsLeft = theDice.roll();
    int value;
    string inputString;
    vector <card> allTargetCards;
    while (movementPointsLeft > 0)
    {
        toReturnFrom.resize(0);
        //cout << "\n\nGame Board\n\n";
        game.displayGame();
        gameMove move1, move2, move3;
        allTargetCards.resize(0);
        int i;
        for (i = 0; i < game.gameGrid.theGrid.size(); i++)
        {
            for (int j = 0; j < game.gameGrid.theGrid.at(i).size(); j++)
            {
                if (game.gameGrid.theGrid.at(i).at(j).rank != 0 && game.gameGrid.theGrid.at(i).at(j).playerOwner != game.whoseTurn)
                {
                    allTargetCards.push_back(game.gameGrid.theGrid.at(i).at(j));
                }
            }
        }
        
        bestCardPairScoreWithMoves temp, best;
        int index = 0;
        
        best.score = INT_MAX;

        if (allTargetCards.size() == 0)
        {
            //cout << "ERROR:  No target cards exist, all cards belong to one player only.";
            exit(0);
        }
        
        gameMove oneMoveCapture;
        bool oneMoveCaptureActivated = false;
        
        // This code is not even used in the final version of the game.
        //#pragma omp parallel for
        for (index = 0; index < allTargetCards.size(); index++)
        {
            
            temp = findClosestTwoCardCaptureOpportunityForAI(allTargetCards[index], game);
            if (temp.score != INT_MAX && temp.score < best.score)
            {
                if ((temp.a.colLocation != 0 || temp.a.rowLocation != 0 || temp.b.colLocation != 0 || temp.b.rowLocation != 0)
                    && temp.c.isMoveLegal(game.gameGrid, game.whoseTurn) &&/* temp.d.isMoveLegal(gameGrid,whoseTurn) && */ temp.a.colLocation != -1 &&
                    temp.a.rowLocation != -1 /*&& temp.b.colLocation != -1 && temp.b.rowLocation != -1*/)
                    
                {
                    best = temp;
                    
                }
                
            }
            //  index++;                    // This is bad...it didn't cause any effect b/c this function is no longer used.
        }
        
        best.c.displayMove();
        best.d.displayMove();
        
        cardAndMove m = game.bestCardToMoveHome();
        
        if (oneMoveCaptureActivated)
        {
            game.applyMove(oneMoveCapture);
            oneMoveCapture.displayMove();
            movementPointsLeft--;
            //cout << "The AI player has made its move, and now has " << movementPointsLeft << " moves remaining for this turn.\n\n";
            
            //cout << "\n\nProcessing captures....\n\n";
            
            if (game.processCapturesAtEndOfTurn().size() == 0)
            {
                //cout << "\nNo captures for this move, which is part of player " << game.whoseTurn << "'s turn.\n\n";
            }
            
        }
        
        move1 = best.c;
        move2 = best.d;
        move3 = m.b;
        
        
        
        if (move1.isMoveLegal(game.gameGrid,game.whoseTurn) &&
            move1.col1 != -1 && (move1.col1!=0 || move1.col2!=0 || move1.row1!=0 || move1.row2!=0))
        {
            toReturnFrom.push_back(move1);
        }
        if (move2.isMoveLegal(game.gameGrid,game.whoseTurn) &&
            move2.col1 != -1 && (move2.col1!=0 || move2.col2!=0 || move2.row1!=0 || move2.row2!=0))
        {
            toReturnFrom.push_back(move2);
        }
        if (move3.isMoveLegal(game.gameGrid,game.whoseTurn) &&
            move3.col1 != -1 && (move3.col1!=0 || move3.col2!=0 || move3.row1!=0 || move3.row2!=0))
        {
            toReturnFrom.push_back(move3);
        }
        // we're not really returning, we're just applying the move
        
        if (toReturnFrom.size() == 0)
        {
            //cout << "The AI player found no legal good moves.  The AI player skips the rest of its turn.";
            movementPointsLeft = 0;
        }
        else
        {
            if (best.score == 0)
            {
                value = 0;
            }
            else
            {
                if (toReturnFrom.size() >= 2 && toReturnFrom[1].isMoveLegal(game.gameGrid, game.whoseTurn) == false)
                {
                    value = 0;
                }
                else
                {
                    if (toReturnFrom.size() == 1)
                    {
                        value = 0;
                    }
                    else
                    {
                        if (toReturnFrom.size() == 0 || toReturnFrom[0].isMoveLegal(game.gameGrid, game.whoseTurn) == false)
                        {
                            //cout << "No legal moves found.\n\n";
                            return;
                        }
                        else
                        {
                            if (toReturnFrom.size() >= 2)
                            {
                                value = rand() % 2;
                            }
                        }
                    }
                }
                // THIS CODE JUST ABOVE FOR DEBUG MODE ONLY!!!!
                toReturnFrom[0] = best.c;
                toReturnFrom[1] = best.d;
                if (best.d.isMoveLegal(game.gameGrid,game.whoseTurn) == false)
                {
                    toReturnFrom.resize(1);
                    value = 0;
                }
                    
            }
            toReturnFrom[value].displayMove();
            game.applyMove(toReturnFrom[value]);
            movementPointsLeft--;
            //cout << "The AI player has made its move, and now has " << movementPointsLeft << " moves remaining for this turn.\n\n";
            
            getline(cin,inputString);
        }
        
        
        //cout << "\n\nProcessing captures....\n\n";
        
        if (game.processCapturesAtEndOfTurn().size() == 0)
        {
            ;//cout << "\nNo captures for this move, which is part of player " << game.whoseTurn << "'s turn.\n\n";
        }
        
    }
    
    
    //cout << "A new turn, turn #" << ++game.turnNum << ", is beginning.\n\n";
    
    
    for (i = 0; i < game.players.size(); i++)
    {
        ;//cout << game.players[i].playerID << ' ';
        
    }
    // Go to having it be the next player's turn.
    for (i = 0; i < game.players.size(); i++)
    {
        assert(i >= 0 && i < game.players.size());
        //cout << game.players[i].playerID << " , " << game.whoseTurn << ";";
        if (game.players.at(i).playerID == game.whoseTurn)
        {
            if (i+1 == game.players.size())
            {
                game.whoseTurn = game.players.at(0).playerID;
            }
            else
            {
                game.whoseTurn = game.players.at(i+1).playerID;
            }
            break;
        }
    }
    //cout << "\n It is now the turn of player:  " << game.whoseTurn;
    
    return;
}


// Problem with this function:  It is returning the best score, but not the best cards or moves associated with them.
bestCardPairScoreWithMoves findClosestTwoCardCaptureOpportunityForAI(card Y, gameState game)
{
    
    /* We are given a fixed card Y.  We want to find the two-card pair with the shortest summed distance
     to an adjacent square to Y.  It is OK if the two adjacent squares are the same one, even though it is not
     possible in practice to move both cards to the same square.
     
     The card Y must have at least two adjacent squares, or, one adjacent square and one square next to it that
     is owned by the player whose turn it is (the same player who owns the two capturing cards).
     
     What we must do is run a pair of nested for loops.  Within the for loops, we need to identify all 1-4
     empty adjacent squares that Y has, and then find:  A)  The shortest distance from CapturingCard1 to any of the empty adjacent squares,
     i.e., any path to an empty adjacent square can qualify as the shortest minimal one, or rather, as part of the sum.
     B)  The shortest distance from CapturingCard2 to any of the empty adjacent squares...the same as above.  We will take those two
     shortest distances and sum them.  We will minimize that variable using the for loops, i.e., each time, we will minimize the
     CapturingCard1 distance to an empty square adjacent to Y, and do the same for Capturing Card 2, and then minimize the sum of those two
     values.  When we find such a minimum, after the for loops are done, we will return the card pair for that minimum.  Also, we need
     to identify the two moves that CapturingCard1 and CapturingCard2 will make.  The way to identify that is, find all of the squares
     adjacent to (first) CapturingCard1, and then find one of those squares such that the shortest path from the square adjacent to
     CapturingCard1 to the empty square minus the shortest path from CapturingCard1 to the same adjacent square to Y is equal to 1.  Note, we
     will need to have previously saved the identity of the square adjacent to Y that yields, for both CC1 and CC2, the shortest distance
     between that adjacent square and the given card (CC1 or CC2).  Now, do this for CapturingCard2 also; find the empty adjacent square with a
     shortest path to a square adjacent to Y that is one less than the shortest path from CC2 to the saved adjacent-to-Y square.
     
     Once these calculations have been done, set a variable of type bestCardPairWithMoves, and pass the shortest path sum value as the score (which
     needs to be minimized to be better), and then pass the two cards and each 1-distance move that was found, back to the function that called
     this function.
     
     If no "good" return value is found for card Y--e.g., if Y is unreachable to all of the whoseTurn cards--return bestScore = INT_MAX.
     
     When you code this, break it up into sections, and use a comment to identify the beginning of each section.
     
     Here is a list of the sections:
     
     */
    
    
    
    // Update:  Fix the col/row issue.
     
    
    
    vector <card> allEmptyAdjacentSquaresToY = game.allEmptyAdjacentSquaresToCardZ(Y);
    
    
    bestCardPairScoreWithMoves toReturn;
    
    if (allEmptyAdjacentSquaresToY.size() == 0)
    {
        toReturn.score = INT_MAX;
        return toReturn;
    }
    
    //cout << "made it here 333";
    
    
    int i, j;
    
    vector <card> allCapturingCards;
    
    for (i = 0; i < game.gameGrid.theGrid.size(); i++)
    {
        for (j = 0; j < game.gameGrid.theGrid[i].size(); j++)
        {
            if (game.gameGrid.theGrid[i][j].playerOwner == game.whoseTurn)
            {
                allCapturingCards.push_back(game.gameGrid.theGrid[i][j]);
            }
        }
    }
    //cout << time(NULL) << endl << endl;
    
    node CC1;
    node adjToCC1;
    node CC2;
    node adjToCC2;
    node adjToY;
    
    gameMove bestGameMoveCC1;
    gameMove bestGameMoveCC2;
    
    string inputString;
    
    int foundMoves = -1;
    bool turnOffCC2flag = false;
    
    vector <card> allEmptyAdjacentSquaresToCC1;
    vector <card> allEmptyAdjacentSquaresToCC2;
    
    card capturingCard1;
    card capturingCard2;
    square savedEmptyAdjacentSquareToY;
    int shortestPathCC1, shortestPathCC2;
    // int TENTATIVEshortestPath1, TENTATIVEshortestPath2;
    
    shortestPathCC1 = INT_MAX;
    shortestPathCC2 = INT_MAX;
    
    int TENTATIVEshortestPathCC1, TENTATIVEshortestPathCC2;
    
    int currentDistance;
    int currShortestPathSum;
    int bestShortestPathSum = INT_MAX;
    int TENTATIVEbestShortestPathSum = INT_MAX;
    
    vector <card> tempadjacentsquaresvec;
    bool disqualduetoadjofanothercard;
    
    node CC1node, CC2node;
    node currentAdjacentToYNode;
    
    int criticalValue;
    
    square savedAdjacentToYSquare1, savedAdjacentToYSquare2;
    card TENTATIVEbestCapturingCard1;
    card TENTATIVEbestCapturingCard2;
    card bestCapturingCard1;
    card bestCapturingCard2;
    
    bool areAnyGoodMovesEverFound = false;
    
    
    gameMove TENTATIVEbestGameMoveCC1;
    gameMove TENTATIVEbestGameMoveCC2;
    
    bool justFoundPossibleNewBestScoreFlag = false;
    //int onewhosecardadjtoy = 0;
    
    digraph graph(game.gameGrid);
    
    node tempNode;
    for (int k = 0; k < allCapturingCards.size(); k++)
    {
        //cout << endl << endl << time(NULL) << ',';
        tempNode = graph.findNode(allCapturingCards[k].rowLocation,allCapturingCards[k].colLocation);
        graph.customDijkstra(tempNode);
    }
    card onewhoseturncardadjtoy;
    //#pragma omp parallel for collapse(2)

    bool disqualifyCC1, disqualifyCC2;
    
    for (i = 0; i < allCapturingCards.size(); i++)
    {
        // We need to consider every card as CC1, so j must be set equal to 0, even though that slows things down a little.
        for (j = 0; j < allCapturingCards.size(); j++)
            // could having had j set to i+1 been the problem?  It gets every pair, but it omits some cards as CC1, I think.
            // I think it is better to consider it like this.
            // The problem, either way, is, i doesn't get treated as the last card in the vector
        {
            justFoundPossibleNewBestScoreFlag = false;
            disqualifyCC1 = false;
            disqualifyCC2 = false;
            
            //areAnyGoodMovesEverFound = false;               // this was the missing code!  We must also re-init are any good moves found to false
            // NO, that code is bad.  We need to have it false if good moves are *ever* found.
            // We could have a separate variable for "were any good moves found this run through?"
            // And we have that:  justfoundpossiblebestnewscoreflag
            
            foundMoves = 0;
            capturingCard1 = allCapturingCards[i];
            capturingCard2 = allCapturingCards[j];
            
            
            onewhoseturncardadjtoy = game.oneWhoseTurnCardIsAdjacent(Y);
            //cout << "\n\nCapturing cards 1 and 2:  ";
            //capturingCard1.displayCard();
            //capturingCard2.displayCard();
            //cout << endl << endl;
            
            
            
            
            // 3.5.  Check both cards to make sure that both of them have rank <= 9.  If they do not, continue the for loop and omit that pair.
            
            //cout << "made it here 666";
            
            //cout << "flag4";
            if ((capturingCard1.rank > 9 || capturingCard2.rank > 9))// && onewhoseturncardadjtoy.rank == 0)
            {
                continue;
            }
            
            if (i == j && onewhoseturncardadjtoy.rank == 0)     // unneeded but leave it
            {
                continue;
            }
            
            
            
            if (onewhoseturncardadjtoy.rank > 9)
            {
                onewhoseturncardadjtoy.rank = 0;             // we will treat the card next to Y as non-existent
            }
            
            
            
            // 3.75:  I forgot this part at first!  Don't go with the cards if their ranks don't sum to 2 more than Y.rank at most.  Only small
            //  cards are excellent at forming capture teams.  I left this part of the re-code of this function at first.
            
            // OH!!!  Don't do this if we're only checking one card at a time!  What an omission!
            if (onewhoseturncardadjtoy.rank == 0 && capturingCard1.rank + capturingCard2.rank > Y.rank + 2)// && onewhoseturncardadjtoy.rank == 0)
            {
                continue;
            }
            
            //cout << "made it here 777";
            // We still need to do something, though.
            
            if (onewhoseturncardadjtoy.rank != 0 && capturingCard1.rank + onewhoseturncardadjtoy.rank > Y.rank + 2)
            {
                continue;
            }
            
            
            //4.  Now, find the shortest distance from CapturingCard1 (identified by the indexes i and j, i in this case) to a square in the vector of
            //    squares adjacent to Y...we want the shortest distance over all possible squares in the list of squares adjacent to Y.  Each time we
            //    find a shorter path, we will replace the minimum (for this iteration) shortest path with the newer, lower value.  Also, we must
            //    save the adjacent square that we were calculating the shortest path to; this is also updated when the minimum shortest path is updated.  (We save the best CapturingCard1 each time we find a new shortest path, too.  But we save all of these things only
            // TENTATIVELY, because the path might fail tests later on.  Wait to finalize until you're ready to return.
            // UPDATE:  Do update the individual shortest paths, but remember that we only care about the *sum* of the two shortest paths.
            
            //cout << "flag5";
            
            //digraph graph(gameGrid,capturingCard1.colLocation,capturingCard1);
            // THIS is the problem...you re-declare a new graph every time you enter this loop!
            
            
            // Convert the first capturing card under consideration to a node, for path finding purposes.
            
            CC1node = graph.findNode(capturingCard1.rowLocation,capturingCard1.colLocation);
            
            savedAdjacentToYSquare1.col = savedAdjacentToYSquare1.row = -1;
            
            
            int localShortestPathCC1 = INT_MAX, localShortestPathCC2 = INT_MAX;
            
            
            
            // this code finds the shortest path to any one of the up to 4 adjacent squares to card Y, and saves that value in var localSPCC1
            for (int k = 0; k < allEmptyAdjacentSquaresToY.size(); k++)
            {
                currentAdjacentToYNode = graph.findNode(allEmptyAdjacentSquaresToY[k].rowLocation, allEmptyAdjacentSquaresToY[k].colLocation);
                graph.customDijkstra(CC1node);     // this code is important because it ensures the paths from this node will be found
                currentDistance = graph.getSourceToTargetDistance(CC1node,currentAdjacentToYNode);
                
                if ((currentDistance < localShortestPathCC1 || localShortestPathCC1 == -1) && currentDistance != INT_MAX &&
                    currentDistance != -1)
                {
                    //cout << "flag6";
                    localShortestPathCC1 = currentDistance;
                    savedAdjacentToYSquare1.row = allEmptyAdjacentSquaresToY[k].rowLocation;
                    savedAdjacentToYSquare1.col = allEmptyAdjacentSquaresToY[k].colLocation;
                    
                }
                
                
            }
            
            //cout << "flag7";
            
            // 5.  Next, do the same thing for CapturingCard2.
            
            // DON'T DO IT IF WE'RE ONLY LOOKING AT ONE CARD!
            if (onewhoseturncardadjtoy.rank == 0)               // write it as == 0, not != 0.
            {
                CC2node = graph.findNode(capturingCard2.rowLocation,capturingCard2.colLocation);
                
                savedAdjacentToYSquare2.col = savedAdjacentToYSquare2.row = -1;
                for (int k = 0; k < allEmptyAdjacentSquaresToY.size(); k++)
                {
                    currentAdjacentToYNode = graph.findNode(allEmptyAdjacentSquaresToY[k].rowLocation, allEmptyAdjacentSquaresToY[k].colLocation);
                    graph.customDijkstra(CC2node);     // this code is important because it ensures the paths from this node will be found
                    currentDistance = graph.getSourceToTargetDistance(CC2node,currentAdjacentToYNode);
                    //cout << "\ncurrentDistance = " << currentDistance << endl;
                
                    
                    
                    if ((currentDistance < localShortestPathCC2 || localShortestPathCC1 == -1) && currentDistance != INT_MAX &&
                        currentDistance != -1)
                    {                                       // ^------I added this or in.
                        localShortestPathCC2 = currentDistance;
                        savedAdjacentToYSquare2.row = allEmptyAdjacentSquaresToY[k].rowLocation;
                        savedAdjacentToYSquare2.col = allEmptyAdjacentSquaresToY[k].colLocation;
                        
                    }
                    
                }
            }
            //int whichCard = 0;
            
            
            //cout << "Why aren't we making it below?  made it here 999";
            
            // This is where the problem is...this code is not always being called!
            
            // THIS LINE OF CODE IS THE PROBLEM
            //if (localShortestPathCC1 == -1 || localShortestPathCC2 == -1 || localShortestPathCC1 == INT_MAX || localShortestPathCC2 == INT_MAX)
            if (localShortestPathCC1 == -1 || localShortestPathCC1 == INT_MAX || (onewhoseturncardadjtoy.rank == 0 && (localShortestPathCC2 == -1 || localShortestPathCC2 == INT_MAX)))
            {
                currShortestPathSum = INT_MAX;
                //cout << "made it here 888";             // We *do* make it here...but the else statement never activates!
            }
            else
            {
                //cout << "flag 7.5";
                if (onewhoseturncardadjtoy.rank != 0)
                {
                    if (localShortestPathCC1 != INT_MAX && localShortestPathCC1 != -1)
                    {
                        currShortestPathSum = localShortestPathCC1;
                    }
                    else
                    {
                        currShortestPathSum = INT_MAX;
                    }
                }
                else
                {
                    if (localShortestPathCC1 != INT_MAX && localShortestPathCC1 != -1 &&
                        localShortestPathCC2 != INT_MAX && localShortestPathCC2 != -1)
                    {
                        currShortestPathSum = localShortestPathCC1 + localShortestPathCC2;
                    }
                    else
                    {
                        currShortestPathSum = INT_MAX;
                    }
                }
                //cout << "made it here 555";
                if (currShortestPathSum != INT_MAX && currShortestPathSum != -1 && currShortestPathSum < bestShortestPathSum)
                {
                    //cout << "made it here 444";
                    justFoundPossibleNewBestScoreFlag = true;
                    //cout << "flag8";
                    if (onewhoseturncardadjtoy.rank == 0)
                    {
                        TENTATIVEbestShortestPathSum = currShortestPathSum;
                        TENTATIVEshortestPathCC1 = localShortestPathCC1;
                        TENTATIVEshortestPathCC2 = localShortestPathCC2;
                        TENTATIVEbestCapturingCard1 = capturingCard1;
                        TENTATIVEbestCapturingCard2 = capturingCard2;
                        justFoundPossibleNewBestScoreFlag = true;
                        //cout << "good news!"
                    }
                    
                    else                // this code might contain a problem
                    {
                        turnOffCC2flag = true;      // turn off CC2 immediately.
                        
                        //cout << "flag8.5";
                        /* this code is toxic and must be removed
                        
                        // disqualify cc1 also if cc1 is adjacent to a *different* card Y.  I.e., check for any captures.
                        // Also, you *could* process captures *immediately*, before the turn ends...that might make the game more fun.  :)
                        // Makes the AI workload lighter, too! :D
                        
                        tempadjacentsquaresvec = game.allEmptyAdjacentSquaresToCardZ(capturingCard1);
                        for (int s = 0; s < tempadjacentsquaresvec.size(); s++)
                        {
                            if (capturingCard1.rank + 6 <= tempadjacentsquaresvec[s].rank + 2)
                            {
                                disqualifyCC1 = true;
                            }
                        }
                        
                        tempadjacentsquaresvec = game.allEmptyAdjacentSquaresToCardZ(capturingCard2);
                        for (int s = 0; s < tempadjacentsquaresvec.size(); s++)
                        {
                            if (capturingCard2.rank + 6 <= tempadjacentsquaresvec[s].rank + 2)
                            {
                                disqualifyCC2 = true;
                            }
                        }*/
                        
                        // this condition is absurd.  It never happens unless we are capturing.
                        //if (game.areTwoCardsAdjacent(capturingCard1,Y) && onewhoseturncardadjtoy.rank + capturingCard1.rank <= Y.rank + 2)
                            // very foolish error!  I wrote "||" instead of "&&".  :)
                            // also:  disqualify movement if the adj to y card is *good* at capturing, not if it is *bad* at capturing
                        if (onewhoseturncardadjtoy.colLocation == capturingCard1.colLocation &&
                            onewhoseturncardadjtoy.rowLocation == capturingCard1.rowLocation)
                        {
                            // The problem is, a card can be torn between two options.  If a *different* card Y is adjacent to CC1, then at this
                            // point, CC1 will not be disqualified from moving, and an opportunity to capture a card might be disrupted.
                            // The answer:  Run a general test to see if there are any non-whose-turn cards adjacent to CC1 where the rank formula
                            // could be satisfied by a card from 2-ace.  Maybe keep the card if it could be satisfied by 6 or lower.
                                // NO--this created problems.  Ditch this approach.
                            
                            // This part is fine, but the other part needs to be left commented out.
                            
                            //disqualifyCC1 = true;       // this is unnecessary, just continue the for loop
                            continue;
                        }
                        
                        //disqualifyCC2 = true;           // we *always* disqualify CC2 when onewhoseturncardadjtoy.rank != 0
                        /*if (game.areTwoCardsAdjacent(capturingCard2,Y) && onewhoseturncardadjtoy.rank + capturingCard2.rank <= Y.rank + 2)
                        {
                            disqualifyCC2 = true;
                        }*/
                        if (localShortestPathCC1 < bestShortestPathSum
                            && localShortestPathCC1 != -1       // added this as a potential fix to all the issues, 11-12, 9:40 a.m.
                            ) // && !disqualifyCC1)  // we don't need this
                        {
                            // We aren't using tentative for the onewhoseturnadjtoy!=0 case.  Remember that.
                            // We are ignoring the value of currshortestpath sum when whoseturncardadjtoy != 0 .
                            bestShortestPathSum = localShortestPathCC1;
                            // TENTATIVEbestShortestPathSum = localShortestPathCC1;
                            // cout << "localshortestpathcc1 = " << localShortestPathCC1 << ", onewhoseturncardadjtoy.rank = "
                            // << onewhoseturncardadjtoy.rank << endl;
                            //TENTATIVEbestCapturingCard1 = capturingCard1;
                            bestCapturingCard1 = capturingCard1;
                            //cout << "flag8";
                            justFoundPossibleNewBestScoreFlag = true;
                            //whichCard = 1;
                        }
                        /*if (localShortestPathCC2 < bestShortestPathSum && !disqualifyCC2)
                        {
                            bestShortestPathSum = localShortestPathCC2;
                            TENTATIVEbestShortestPathSum = localShortestPathCC2;
                            TENTATIVEbestCapturingCard1 = capturingCard2;
                            bestCapturingCard1 = capturingCard2;
                            allEmptyAdjacentSquaresToCC1 = allEmptyAdjacentSquaresToCC2;
                            justFoundPossibleNewBestScoreFlag = true;
                            //whichCard = 2;
                            cout << "local shortest path cc2:  " << localShortestPathCC2 << endl;
                            
                            
                            CC1 = CC2;
                            localShortestPathCC1 = localShortestPathCC2;
                            allEmptyAdjacentSquaresToCC1 = allEmptyAdjacentSquaresToCC2;
                            capturingCard1 = capturingCard2;
                            shortestPathCC1 = shortestPathCC2;
                            bestCapturingCard1 = bestCapturingCard2;
                            // TENTATIVEshortestPathCC1 = TENTATIVEshortestPathCC2;
                            shortestPathCC1 = shortestPathCC2;
                            disqualifyCC1 = false;
                            
                            savedAdjacentToYSquare1 = savedAdjacentToYSquare2;      // don't forget this!
                            justFoundPossibleNewBestScoreFlag = true;
                            bestShortestPathSum = localShortestPathCC2;
                        }*/
                    }
                }
                //cout << "onewhoseturncardadjtoy = ";
                onewhoseturncardadjtoy.displayCard2();
                //cout << endl << endl;
                if (onewhoseturncardadjtoy.rank != 0)      // only move one card nearer...the closer one  // YES, and set the good card to CC1!
                {
                    /*
                    if (!disqualifyCC1 && localShortestPathCC1 <= bestShortestPathSum && onewhoseturncardadjtoy.rank + capturingCard1.rank <= Y.rank + 2 && capturingCard1.rank < 10)*/
                    //{
                        // We need to update the shortest path score, too, so I'm adding in the following line of code:     //NO.  Don't use tent.
                        // TENTATIVEbestShortestPathSum = localShortestPathCC1;
                        // TENTATIVEbestCapturingCard1 = capturingCard1;
                        // we don't have the game move yet
                        
                        
                    //}
                    
                    // For this case, we have eliminated card2 essentially...card1 got set to the values of card1.
                    /*if (!disqualifyCC2 && localShortestPathCC2 < bestShortestPathSum && localShortestPathCC2 < localShortestPathCC1 && onewhoseturncardadjtoy.rank +
                        capturingCard2.rank <= Y.rank + 2 && capturingCard2.rank < 10)
                    {
                        justFoundPossibleNewBestScoreFlag = true;
                        
                        
                    }*/
                    
                    /* Quick Interruption...since we have CC1, CC2, and Y identified, run the check for 1-m.p. captures.*/
                    
                    // re-do these in case they're out of date!
                    
                    
                    // Switch this off for now...it shouldn't be needed unless the AI for onewhoseturncardadjtoy != 0 isn't working right
                    /*
                    allEmptyAdjacentSquaresToY = game.allEmptyAdjacentSquaresToCardZ(Y);
                    allEmptyAdjacentSquaresToCC1 = game.allEmptyAdjacentSquaresToCardZ(capturingCard1);
                    allEmptyAdjacentSquaresToCC2 = game.allEmptyAdjacentSquaresToCardZ(capturingCard2);
                    
                    if (onewhoseturncardadjtoy.rank != 0)
                    {
                        //cout << "flagabc";
                        for (int qqq = 0; qqq < allEmptyAdjacentSquaresToY.size(); qqq++)
                        {
                            if (onewhoseturncardadjtoy.rank + capturingCard1.rank <= Y.rank + 2)
                            {
                                //cout << "flagdef";
                                for (int www = 0; www < allEmptyAdjacentSquaresToCC1.size(); www++)
                                {
                                
                                    
                                    //cout << "flagghi";
                                    if (allEmptyAdjacentSquaresToCC1[www].colLocation == allEmptyAdjacentSquaresToY[qqq].colLocation
                                        && allEmptyAdjacentSquaresToCC1[www].rowLocation == allEmptyAdjacentSquaresToY[qqq].rowLocation)
                                    {
                                        //cout << "flagzzz";
                                        toReturn.score = 0;
                                        toReturn.a = capturingCard1;
                                        toReturn.b.colLocation = -1;
                                        toReturn.c.col1 = capturingCard1.colLocation;
                                        toReturn.c.row1 = capturingCard1.rowLocation;
                                        toReturn.c.col2 = allEmptyAdjacentSquaresToCC1[www].colLocation;
                                        toReturn.c.row2 = allEmptyAdjacentSquaresToCC1[www].rowLocation;
                                        return toReturn;
                                    }
                                    
                                }
                            }
                            if (onewhoseturncardadjtoy.rank + capturingCard2.rank <= Y.rank + 2)
                            {
                                //cout << "flag jkl";
                                for (int www = 0; www < allEmptyAdjacentSquaresToCC2.size(); www++)
                                {
                                    //cout << "flag mno";
                                    if (allEmptyAdjacentSquaresToCC2[www].colLocation == allEmptyAdjacentSquaresToY[qqq].colLocation
                                        && allEmptyAdjacentSquaresToCC2[www].rowLocation == allEmptyAdjacentSquaresToY[qqq].rowLocation)
                                    {
                                        //cout << "flag pqr";
                                        toReturn.score = 0;
                                        toReturn.a = capturingCard2;
                                        toReturn.b.colLocation = -1;
                                        toReturn.c.col1 = capturingCard2.colLocation;
                                        toReturn.c.row1 = capturingCard2.rowLocation;
                                        toReturn.c.col2 = allEmptyAdjacentSquaresToCC2[www].colLocation;
                                        toReturn.c.row2 = allEmptyAdjacentSquaresToCC2[www].rowLocation;
                                        return toReturn;
                                    }
                                    
                                }
                            }
                        }
                    }
                    */
                    
                    
                    
                    // int loopvalue;

                    
                    // having previously omitted the line of code below was a serious error
                    
                    // We are only examining CC1 here, it's the one-card-only version.
                    
                    allEmptyAdjacentSquaresToCC1 = game.allEmptyAdjacentSquaresToCardZ(capturingCard1);
                    //allEmptyAdjacentSquaresToCC2 = game.allEmptyAdjacentSquaresToCardZ(capturingCard2);
                    allEmptyAdjacentSquaresToY = game.allEmptyAdjacentSquaresToCardZ(Y);
                    
                    
                    //loopvalue = allEmptyAdjacentSquaresToCC1.size();
                    //areAnyGoodMovesEverFound = false;     // only reset this before the two main nested for loops...if we find cards, we don't
                    // want to over-write that every time we restart this code.
                
                    //cout << "justfoundpossiblenewbestscoreflag = " << justFoundPossibleNewBestScoreFlag << endl;
                    //cout << "flag9";
                    
                    if (justFoundPossibleNewBestScoreFlag)      // we have to reject cards that violate the capture rank formula
                    {                                           // ^--- we've already done that, removing the whoseturncard...==0 condition above
                        //cout << "advanced time start 1";
                        //cout << "loopvalue = " << loopvalue << endl;
                        
                        // Find which move takes us one shortest path point closer to the goal square adjacent to Y.
                        for (int k = 0; k < allEmptyAdjacentSquaresToCC1.size(); k++)
                        {
                            //cout << "MADE IT HERE";
                            //cout << "flag special #3.";
                            CC1 = graph.findNode(capturingCard1.rowLocation,capturingCard1.colLocation);
                            //cout << "CC1 Node ID = "; CC1.displayNode();
                            adjToCC1 = graph.findNode(allEmptyAdjacentSquaresToCC1[k].rowLocation,allEmptyAdjacentSquaresToCC1[k].colLocation);
                            //cout << "adjToCC1 Node ID = "; adjToCC1.displayNode();
                            
                            
                            if (savedAdjacentToYSquare1.col != -1)
                            {
                                adjToY = graph.findNode(savedAdjacentToYSquare1.row,savedAdjacentToYSquare1.col);
                            }
                            else
                            {
                                //cout << "ERROR, saved adjacent to Y node not found";
                                break;
                            }
                            //cout << "adjtoY Node ID = ";
                            graph.customDijkstra(CC1);
                            graph.customDijkstra(adjToCC1);
                            
                            // Be careful.  What if GSTTD returns -1 in one case, and 0 in another??
                            
                            int valAAA = graph.getSourceToTargetDistance(CC1,adjToY);
                            int valBBB = graph.getSourceToTargetDistance(adjToCC1,adjToY);
                            
                            if (valAAA == -1 || valBBB == -1 || valAAA == INT_MAX || valBBB == INT_MAX)
                            {
                                criticalValue = INT_MAX;
                            }
                            else
                            {
                                criticalValue = valAAA - valBBB;
                            }
                            
                            // Warning:  What if the critical value can equal 1 when we aren't actually moving 1 closer?
                            // Are all of the two-card combinations correct in terms of what the *moves* are?  Check again.
                            // I don't think that's the error.  The shortest path formula is right.
                            // What is going on??  I can now get the right card, but not the right move for 1-card only.
                            
                            //cout << "\ncriticalvalue = " << criticalValue << endl;
                            //cout << "\n\nMore Data:\n\ncapturing card 1 = ";
                            //capturingCard1.displayCard();
                            //cout << "\nadjtoy = ";
                            
                            //adjToY.displayNode();
                            //cout << "\nadjtocc1 = ";
                            //adjToCC1.displayNode();
                            //cout << endl << endl;
                            
                            
                            if (criticalValue == 1)
                            {
                                if (justFoundPossibleNewBestScoreFlag)
                                {
                                    bestGameMoveCC1.col1 = capturingCard1.colLocation;
                                    bestGameMoveCC1.row1 = capturingCard1.rowLocation;
                                    
                                    bestGameMoveCC1.col2 = adjToCC1.gridCol;
                                    bestGameMoveCC1.row2 = adjToCC1.gridRow;
                                    
                                    //cout << "good flag";
                                    
                                    
                                    // THESE TWO ARE WRONG!     // not really, we can use them interchangeably with adjtocc1
                                    // bestGameMoveCC1.col2 = allEmptyAdjacentSquaresToCC1[k].colLocation;
                                    // bestGameMoveCC1.row2 = allEmptyAdjacentSquaresToCC1[k].rowLocation;
                                    
                                    
                                    foundMoves = 2;                 // it's not literally two moves here, it's just the one and only move
                                    areAnyGoodMovesEverFound = true;
                                    
                                    // WE HAVE FOUND THE MOVES, BUT MAKE THE RIGHT ASSIGNMENT!
                                }
                            }
                        }
                    }
                    
                    
                }
                
                else                // THIS is the code to focus on...the code addressing one card only.  Go through this code and
                {                   //      annotate what each block of code is doing.
                    
                    //cout << "made it here 222";
                    // This code is for 2 cards.

                    foundMoves = 0;
                    
                    allEmptyAdjacentSquaresToCC1 = game.allEmptyAdjacentSquaresToCardZ(capturingCard1);
                    
                    
                    
                    for (int k = 0; k < allEmptyAdjacentSquaresToCC1.size(); k++)
                    {
                        
                        CC1 = graph.findNode(capturingCard1.rowLocation,capturingCard1.colLocation);
                        
                        adjToCC1 = graph.findNode(allEmptyAdjacentSquaresToCC1[k].rowLocation,allEmptyAdjacentSquaresToCC1[k].colLocation);
                        
                        
                        if (savedAdjacentToYSquare1.col != -1)
                        {
                            adjToY = graph.findNode(savedAdjacentToYSquare1.row,savedAdjacentToYSquare1.col);
                        }
                        else
                        {
                            break;
                        }
                        
                        // Check all the squares adjacent to CC1 to see which one(s) brings us closer to the destination node by 1 unit.
                        
                        
                        graph.customDijkstra(CC1);
                        graph.customDijkstra(adjToCC1);
                        
                        // What if one of these equals 0 and the other equals -1??  Big problem!
                        // Do arithmetic warily in this program.  You have to watch out for "failure values" like INT_MAX and -1.
                        
                        int valAAA = graph.getSourceToTargetDistance(CC1,adjToY);
                        int valBBB = graph.getSourceToTargetDistance(adjToCC1,adjToY);
                        
                        if (valAAA == -1 || valBBB == -1 || valAAA == INT_MAX || valBBB == INT_MAX)
                        {
                            criticalValue = INT_MAX;
                        }
                        else
                        {
                            criticalValue = valAAA - valBBB;
                        }
                        //cout << "criticalvalue = " << criticalValue;
                        if (justFoundPossibleNewBestScoreFlag && criticalValue == 1
                            )
                        {
                     
                            TENTATIVEbestGameMoveCC1.col1 = capturingCard1.colLocation;
                            TENTATIVEbestGameMoveCC1.row1 = capturingCard1.rowLocation;
                            TENTATIVEbestGameMoveCC1.col2 = adjToCC1.gridCol; //allEmptyAdjacentSquaresToCC1[k].colLocation;
                            TENTATIVEbestGameMoveCC1.row2 = adjToCC1.gridRow; //allEmptyAdjacentSquaresToCC1[k].rowLocation;
                            
                            //cout << "good place reached flag";
                            
                            if (/*foundMoves != 1 &&*/ justFoundPossibleNewBestScoreFlag)
                            {
                                //foundMoves = 2;     // just set it to 2, even though there's only one move
                                // I think this was tripping everything up...start with foundMoves at 1, not 2
                                foundMoves = 1;
                                
                            }
                        }
                    }
                    
                    
                    allEmptyAdjacentSquaresToCC2 = game.allEmptyAdjacentSquaresToCardZ(capturingCard2);
                    

                    for (int k = 0; k < allEmptyAdjacentSquaresToCC2.size(); k++)
                    {
                        CC2 = graph.findNode(capturingCard2.rowLocation,capturingCard2.colLocation);
                        adjToCC2 = graph.findNode(allEmptyAdjacentSquaresToCC2[k].rowLocation,allEmptyAdjacentSquaresToCC2[k].colLocation);
                        if (savedAdjacentToYSquare2.col != -1)
                        {
                            adjToY = graph.findNode(savedAdjacentToYSquare2.row,savedAdjacentToYSquare2.col);
                        }
                        else
                        {
                            break;
                        }
                        graph.customDijkstra(CC2);
                        graph.customDijkstra(adjToCC2);
                        //cout << "made it here 111";
                        if (graph.getSourceToTargetDistance(CC2,adjToY) - graph.getSourceToTargetDistance(adjToCC2,adjToY) == 1)
                        {
                            TENTATIVEbestGameMoveCC2.col1 = capturingCard2.colLocation;
                            TENTATIVEbestGameMoveCC2.row1 = capturingCard2.rowLocation;
                            TENTATIVEbestGameMoveCC2.col2 = adjToCC2.gridCol; // allEmptyAdjacentSquaresToCC2[k].colLocation;
                            TENTATIVEbestGameMoveCC2.row2 = adjToCC2.gridRow; //allEmptyAdjacentSquaresToCC2[k].rowLocation;
                            // toReturn.d = bestGameMoveCC2;
                        
                            // if (foundMoves == 1 && justFoundPossibleNewBestScoreFlag)
//                            {
                                foundMoves++;
                                areAnyGoodMovesEverFound = true;
                                
//                            }
                        }
                    }
                }
                
                // cout << "foundmoves = " << foundMoves << ", onewhoseturncardadjtoy.rank = " << onewhoseturncardadjtoy.rank
                // << " and justfoundpossiblenewbestscoreflag = " << justFoundPossibleNewBestScoreFlag << endl << endl;
                // << " and justfoundpossiblenewbestscoreflag = " << justFoundPossibleNewBestScoreFlag << endl << endl;
                if (foundMoves == 2 && onewhoseturncardadjtoy.rank == 0 && justFoundPossibleNewBestScoreFlag)
                {
                    //cout << "better news!";
                    bestCapturingCard1 = TENTATIVEbestCapturingCard1;
                    bestCapturingCard2 = TENTATIVEbestCapturingCard2;
                    bestGameMoveCC1 = TENTATIVEbestGameMoveCC1;
                    bestGameMoveCC2 = TENTATIVEbestGameMoveCC2;
                    bestShortestPathSum = TENTATIVEbestShortestPathSum;
                    
                    areAnyGoodMovesEverFound = true;
                    
                    //cout << endl << bestShortestPathSum << endl;
                    // why don't we have "are any good moves found = true" here?
                    // because we already set it above
                    
                    //cout << "flag1 and areanygoodmoveseverfound = " << areAnyGoodMovesEverFound << endl;
                }
                else
                {
                    if (foundMoves == 2 && onewhoseturncardadjtoy.rank != 0 && justFoundPossibleNewBestScoreFlag)
                    {
                        
                        areAnyGoodMovesEverFound = true;        // I think this code should be here but I'm not sure.
                        bestCapturingCard1 = capturingCard1;
                        
                        
                        // NO!  We have already set bestgamemove, don't overwrite it with a tentative value!!!
                        
                        //bestGameMoveCC1 = TENTATIVEbestGameMoveCC1;     // we actually do use tentative here just this once; don't change this
                                                                        //      carelessly
                        
                        // NO--we aren't using tentative vars here!!!  This code below over-writes good values!
                        
                        // We need to also update the "tentative" variables to be final
                        //bestGameMoveCC1.col1 = TENTATIVEbestGameMoveCC1.col1;
                        //bestGameMoveCC1.row1 = TENTATIVEbestGameMoveCC1.row1;
                        //bestGameMoveCC1.col2 = TENTATIVEbestGameMoveCC1.col2;
                        //bestGameMoveCC1.row2 = TENTATIVEbestGameMoveCC1.row2;
                        //bestCapturingCard1 = TENTATIVEbestCapturingCard1;
                        //bestShortestPathSum = TENTATIVEbestShortestPathSum;
                        
                    }
                }
                
            }
        }
        
    }
    
    //cout << "areanygoodmoveseverfound = " << areAnyGoodMovesEverFound << endl;
    
    if (areAnyGoodMovesEverFound == false)
    {
        //cout << "big flag";
        //cout << "\n\nfoundMoves = " << foundMoves;
        toReturn.score = INT_MAX;
        return toReturn;
    }
    
    if (turnOffCC2flag == false)
    {
        toReturn.b = bestCapturingCard2;
        toReturn.d = bestGameMoveCC2;
    }
    else
    {
        toReturn.b.colLocation = -1;
        toReturn.d.col1 = -1;
    }
    toReturn.a = bestCapturingCard1;
    
    toReturn.c = bestGameMoveCC1;
    
    toReturn.score = bestShortestPathSum;
    if (onewhoseturncardadjtoy.rank != 0)
    {
        ; //cout << "toReturn score = " << toReturn.score;
    }
    
    if (toReturn.c.col1 == 0 && toReturn.c.row1 == 0 && toReturn.c.col2 == 0 && toReturn.c.row2 == 0)
    {
        toReturn.score = INT_MAX;
    }
    
    return toReturn;

}



void digraph::displayGraphEdges()
{
    int i;
    for (i = 0; i < allEdges.size(); i++)
    {
        ;//cout << endl << allEdges[i].fromNode.gridRow << "," << allEdges[i].fromNode.gridCol << "; "
        //<< allEdges[i].toNode.gridRow << "," << allEdges[i].toNode.gridCol << endl;
    }
}


digraph::digraph(grid gameGrid)
{
    // construct all the nodes
    allSourceNodeDijkstraLists.resize(0);
    //int nodeCounter = 1;
    int nodeCounter = 0;        // big change, from nodeID starting at 0 to 1...to comply with Google AI Dijkstra code
    graphNodes.resize(0);       //      and avoid segmentation faults.  My "debugging intuition" is good here. :)
    int i, j;
//    #pragma omp parallel for collapse(2)

    for ( i = 0; i < gameGrid.theGrid.size(); i++)                // OLD VERSION   // -- reinstated
    {
        for (j = 0; j < gameGrid.theGrid./*at(i).*/size(); j++)
        {
            node newNode;
            newNode.gridRow = i;
            newNode.gridCol = j;
            newNode.nodeID = nodeCounter;
            nodeCounter++;
            graphNodes.push_back(newNode);
        }
    }
    
    /*for (int i = centerRow - radius; i <= centerRow + radius; i++)
    {
        for (int j = centerCol - radius; j <= centerRow + radius; j++)
        {
            if (i >= 0 && i <= 11 && j >= 0 && j <= 11)
            {
                node newNode;
                newNode.gridCol = j;
                newNode.gridRow = i;
                newNode.nodeID = nodeCounter;
                nodeCounter++;
                graphNodes.push_back(newNode);
            }
        }
    }*/
    
    //cout << "flagA";

    edge newEdge;
    for (i = 0; i < graphNodes.size(); i++)
    {
        for (j = 0; j < graphNodes.size(); j++)
        {
            if (((graphNodes[i].gridCol - graphNodes[j].gridCol == 1 || graphNodes[i].gridCol - graphNodes[j].gridCol == -1)
                && (graphNodes[i].gridRow == graphNodes[j].gridRow)) || ((graphNodes[i].gridRow - graphNodes[j].gridRow == 1 || graphNodes[i].gridRow - graphNodes[j].gridRow == -1) && graphNodes[i].gridCol == graphNodes[j].gridCol))
            {
                /*
                if (gameGrid.theGrid[j][i].rank == 0)      // the square is empty
                 */             // this line of code is the error!
                // We need to check if the "to" square is empty.
                //cout << "IMPORTANT:  " << graphNodes[j].gridCol  << " " << graphNodes[j].gridRow << endl;
                assert (graphNodes[j].gridCol >= 0 && graphNodes[j].gridCol < 12 && graphNodes[j].gridRow >= 0 && graphNodes[j].gridRow < 12);
                if (gameGrid.theGrid[graphNodes[j].gridRow][graphNodes[j].gridCol].rank == 0)
                {
                    newEdge.fromNode = graphNodes[i];
                    newEdge.toNode = graphNodes[j];
                    allEdges.push_back(newEdge);
                    // cout << newEdge.fromNode.gridCol << " " << newEdge.fromNode.gridRow << " ; " << newEdge.toNode.gridCol << " " << newEdge.toNode.gridRow << " ";
                }
            }
        }
    }
}




int dice::roll()
{
    die1 = rand() % 6 + 1;
    die2 = rand() % 6 + 1;
    return (die1+die2);
}





void gameState::manageHumanFullTurn()
{
    if (turnNum % 10 == 0)
    {
        addPointsBasedOnBaseSquares();
        //cout << "Score Update:\n";
        int i;
        for (i = 0; i < 4; i++)
        {
            ;//cout << "\nPlayer " << players.at(i).playerID << " now has " << players.at(i).numPoints;
        }
        //cout << endl << endl;
    }
    
    
    
    dice theDice;
    int movementPointsLeft = theDice.roll();
    //cout << "FULL GAME BOARD:\n\n";
    displayGame();
    // cout << "Rolling the dice yields " << movementPointsLeft << ", i.e., " << theDice.die1 << " and " << theDice.die2 << ".\n";
    
    // Let's roll 6 dice, just for now:
    
    // movementPointsLeft = theDice.roll() + theDice.roll() + theDice.roll();
    
    int row = -1, col = -1;
    char direction = ' ';
    
    bool skipFlag = false;
    
    do {
        //cout << "\nPlayer " << whoseTurn << ", your cards are marked ";
        outputPlayerColor(whoseTurn,1,1);
        // inaccurate fluff values for rank and suit...they just need to not be equal to 0 for the function.
        //cout << ".\nYou have " << movementPointsLeft << " vertical and horizontal one-square moves of your cards left.";
        //cout << "\nPlease enter the column (1-12) and then the row (1-12) of one of your cards, followed by the direction you would like to"
           //  << " move it in...U for up, D for down, L for left, R for right.  You cannot move your card to a square that is "
           //  << "occupied by any other card, whether it is your card or another player's or an unowned GRAY card.  To capture a card, move "
          //   << "two cards adjacent to it (vertically/horizontally) so that the sum of your two cards is at least as large as the adjacent "
         //    << "card you want to capture plus two.  (To skip the rest of your turn, type '0 0 s', without the quotes.  To quit, type '0 0 q'"
        //<< " without the quotes).";

        //cout << "\n[COLUMN] [ROW] [DIRECTION]:  ";
        
        
        string inputLine = "";
        
        getline(cin, inputLine); // Grab whole line
        stringstream ss(inputLine);
        
        col = -1;
        row = -1;
        direction = '\0';
        
        ss >> col >> row >> direction;
        
        if (direction == 'z')
        {
            // output some debugging information
            //cout << "\n\nEnter two squares to find the shortest path from the first square to the second square:  ";
            getline(cin, inputLine);
            stringstream ss(inputLine);
            int a, b, c, d;
            ss >> a >> b >> c >> d;
            digraph tempgraph(gameGrid);
            node node1 = tempgraph.findNode(a,b);
            node node2 = tempgraph.findNode(c,d);
            tempgraph.customDijkstra(node1);
            //cout << "shortest path size = " << tempgraph.getSourceToTargetDistance(node1, node2) << endl << endl;
            getline(cin, inputLine);
        
        }
        if (direction == 'y')
        {
            //cout << "\n\nEnter col,row, to look up what card is in that square:  ";
            int theRow, theCol;
            getline(cin, inputLine);
            stringstream ss(inputLine);
            ss >> theCol >> theRow;
            //cout << "Grid card at row=" << theRow << ", col=" << theCol << " is:  ";
            gameGrid.theGrid.at(theRow).at(theCol).displayCard();               // why is it always the one at 0,0?
            //cout << "\n\nFull grid: \n\n";                                  // I relied on ChatGPT to fix col vs. theCol.  Oh well.
            // Remember:  minus 1 is up, minus 1 is left
            displayGame();
            //cout << endl << endl;
            getline(cin, inputLine);
        }
        
        if (col == 0 && row == 0 && direction == 'q')
        {
            //cout << "\n\nGoodbye, thanks for playing this game!\n\n";
            exit(0);
        }
        
        
        
        skipFlag = false;
        if (col == 0 && row == 0 && direction == 's')
        {
            skipFlag = true;
        }
        
        if (skipFlag == true)
        {
            break;
        }

        
        // Check move for legality, then use ApplyMove if it is legal.
        
        gameMove theGameMove;
        theGameMove.row1 = row;
        theGameMove.col1 = col;
        
        bool moveIsLegal = true;
        
        //cout << "***" << direction << "***";
        //cout << "***" << col << "," << row << "***";
        
        switch (direction)
                    // I think this is right.
        {
            case 'U':
            case 'u':
                theGameMove.col2 = col;
                theGameMove.row2 = row-1;
                break;
            case 'D':
            case 'd':
                theGameMove.col2 = col;
                theGameMove.row2 = row+1;
                break;
            case 'R':
            case 'r':
                theGameMove.col2 = col+1;
                theGameMove.row2 = row;
                break;
            case 'L':
            case 'l':
                theGameMove.col2 = col-1;
                theGameMove.row2 = row;
                break;
            default:
                //cout << "\n\nERROR:  Invalid character submitted.  Try again.\n\n";
                moveIsLegal = false;
        }
        
        if (moveIsLegal)
        {
            moveIsLegal = theGameMove.isMoveLegal(gameGrid, whoseTurn);
        }
        
        if (!moveIsLegal)
        {
            //cout << "\n\nERROR:  Illegal move specified.  Please try again.\n\n";
            theGameMove.displayMove();
        }
        else
        {
            applyMove(theGameMove);
            //cout << "Processing captures....\n\n";
            
            processCapturesAtEndOfTurn();
            movementPointsLeft--;
        }
        
        displayGame();
        
    } while (movementPointsLeft > 0);
    
    //cout << "\n\nAll movement points used by human player " << whoseTurn << ".\n";
    
    
    //cout << "A new turn, turn #" << ++turnNum << ", is beginning.\n\n";
    
    
    // Go to having it be the next player's turn.
    int i;
    for (i = 0; i < players.size(); i++)
    {
        assert(i >= 0 && i < players.size());
        if (players.at(i).playerID == whoseTurn)
        {
            if (i+1 == players.size())
            {
                whoseTurn = players.at(0).playerID;
            }
            else
            {
                whoseTurn = players.at(i+1).playerID;
            }
            break;
        }
    }
} // This function does the following:  Check if it's every 10 turns and display score ; roll the dice ; display the game board ; track
    // movement points remaining for the player ; give the user instructions on how to move ; get user move selected ; check if user selected
    // move is illegal ; process captures at the end of each movementpoint-turn (i.e, on turns within one user's moves) ; update the game board
    // if a legal move was selected ; change whose turn it is at the end of the human turn, when all movement points have run out.

// My graphics version of this will be:  Skip score for now (add that in later), roll the dice and output the number on the RHS of the screen,
//    get a user move based on clicking two cards.  (You may wish to display a larger version of the card on the RHS when it's selected.)
//    Don't let the user highlight/select a card that is not legal to select.  Also, don't let the user choose a square that's illegal.
//    Also, this function must *check for captures*, and, it must update the game grid.
void gameState::applyMove(gameMove theMove)
{
    if (theMove.isMoveLegal(gameGrid, whoseTurn) == false)
    {
        //cout << "\n\nIllegal move specified.  Please try again.";
    }
    else
    {
        assert(theMove.col2 >= 0 && theMove.col2 < gameGrid.theGrid.size() &&
               theMove.col1 >= 0 && theMove.col1 < gameGrid.theGrid.size() &&
               theMove.row1 >= 0 && theMove.row1 < gameGrid.theGrid.at(theMove.col1).size() &&
                theMove.row2 >= 0 && theMove.row2 < gameGrid.theGrid.at(theMove.col2).size());
        gameGrid.theGrid.at(theMove.row2).at(theMove.col2) = (gameGrid.theGrid.at(theMove.row1).at(theMove.col1));
        gameGrid.theGrid.at(theMove.row2).at(theMove.col2).rowLocation = theMove.row2;
        gameGrid.theGrid.at(theMove.row2).at(theMove.col2).colLocation = theMove.col2;
        
        gameGrid.theGrid.at(theMove.row1).at(theMove.col1).rank = 0;
        gameGrid.theGrid.at(theMove.row1).at(theMove.col1).suit = 0;
        gameGrid.theGrid.at(theMove.row1).at(theMove.col1).playerOwner = 0;
    }
    return;
}

vector <card> gameState::processCapturesAtEndOfTurn()
{
    vector <card> toReturn;
    int x1, x2, x3, x4;
    int targetCardRank;
    int numCapturesThisRound;
    int i;
    card targetCard;
    do
    {
        numCapturesThisRound = 0;
        for (i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                // cout << "\n\ni = " << i << ", j = " << j;
                assert (i >= 0 && i < gameGrid.theGrid.size() && j >= 0 && j < gameGrid.theGrid.at(j).size());
                if (gameGrid.theGrid.at(i).at(j).playerOwner != whoseTurn && gameGrid.theGrid.at(i).at(j).rank != 0)
                    /* ^---- blank -- gray cards can be captured */
                {
                    targetCard = gameGrid.theGrid.at(i).at(j);
                    targetCardRank = gameGrid.theGrid.at(i).at(j).rank;
                    // what do you do if there are 3 or 4 cards surrounding the card?  Answer:  Choose the best 2.
                    //assert (j < gameGrid.theGrid.size() && j > -1 && i+1 > -1 && i+1 < gameGrid.theGrid.at(j).size());
                    if (i < 11 && gameGrid.theGrid.at(i+1).at(j).playerOwner == whoseTurn)  // second condition won't run if first fails
                    {
                        x1 = gameGrid.theGrid.at(i+1).at(j).rank;
                    }
                    else
                    {
                        x1 = 20;                // a large number that won't work in the formula
                    }
                    //assert (i-1 >= 0 && i-1 < gameGrid.theGrid.at(j).size() && j >= 0 && j < gameGrid.theGrid.size());
                    // this assertion is too strict...we test for i > 0 to avoid this issue anyway n the condition.
                    if (i > 0 && gameGrid.theGrid.at(i-1).at(j).playerOwner == whoseTurn)
                    {
                        x2 = gameGrid.theGrid.at(i-1).at(j).rank;
                    }
                    else
                    {
                        x2 = 20;                // a large number that won't work in the formula
                    }
                    //assert (j+1 >= 0 && j+1 < gameGrid.theGrid.size() && i >= 0 && i < gameGrid.theGrid.at(j).size());
                    if (j < 11 && gameGrid.theGrid.at(i).at(j+1).playerOwner == whoseTurn)
                    {
                        x3 = gameGrid.theGrid.at(i).at(j+1).rank;
                    }
                    else
                    {
                        x3 = 20;                // a large number that won't work in the formula
                    }
                    //assert(j-1 >= 0 && j-1 < gameGrid.theGrid.size() && i >= 0 && i < gameGrid.theGrid.at(j-1).size());
                    if (j > 0 && gameGrid.theGrid.at(i).at(j-1).playerOwner == whoseTurn)
                    {
                        x4 = gameGrid.theGrid.at(i).at(j-1).rank;
                    }
                    else
                    {
                        x4 = 20;                // a large number that won't work in the formula
                    }
                    
                    // Formula:  card1 + card2 <= card3 + 2
                    // If there are two ways to take the card...it doesn't matter
                    // Check every pair of vertically/horizontally adjacent cards with the formula.
                    
                    if (x1 + x2 <= targetCardRank + 2 || x1 + x3 <= targetCardRank + 2 || x1 + x4 <= targetCardRank + 2 ||
                        x2 + x3 <= targetCardRank + 2 || x2 + x4 <= targetCardRank + 2 || x3 + x4 <= targetCardRank + 2)
                    {
                        //toReturn = true;
                        //cout << "Player " << whoseTurn << " has captured the card ";
                        targetCard.displayCard();
                        targetCard.playerOwner = whoseTurn;
                        //cout << ".\n";
                        
                        toReturn.push_back(targetCard);
                        
                        
                        assert (j >= 0 && j < gameGrid.theGrid.size() && i >= 0 && i < gameGrid.theGrid.at(j).size());
                        gameGrid.theGrid.at(i).at(j) = targetCard;
                        
                        numCapturesThisRound++;
                        
                    }
                    /*else
                     {
                        cout << "x1 = " << x1 << ", x2 = " << x2 << ", x3 = " << x3 << ", x4 = " << x4 << endl;
                     }*/
                }
            }
        }
    } while (numCapturesThisRound != 0);             // repeat the loop to do successive captures enabled after earlier captures
    return toReturn;
}

void gameState::addPointsBasedOnBaseSquares()
{
    for (int h = 1; h <= 4; h++)
    {
        int x1, x2, x3, x4;
        switch(h)
        {
                // Check over these values later
            case 1:                 // red
                x1 = 0;
                x2 = 4;
                x3 = 0;
                x4 = 4;
                break;
            case 2:
                x1 = 0;
                x2 = 4;
                x3 = 8;
                x4 = 12;
                break;
            case 3:
                x1 = 8;
                x2 = 12;
                x3 = 8;
                x4 = 12;
                break;
            default:        // 4
                x1 = 8;
                x2 = 12;
                x3 = 0;
                x4 = 4;
        }
        for (int i = x1; i < x2; i++)
        {
            for (int j = x3; j < x4; j++)
            {
                assert(j >= 0 && j < gameGrid.theGrid.size() && i >= 0 && i < gameGrid.theGrid.at(j).size());
                if (gameGrid.theGrid.at(i).at(j).playerOwner == h)
                {
                    int k;
                    for (k = 0; k < players.size(); k++)
                    {
                        assert (k > -1 && k < players.size());
                        if (players.at(k).playerID == h)
                        {
                            break;
                        }
                    }
                    assert (k >= 0 && k < players.size() && i >= 0 && i < gameGrid.theGrid.size() && j >= 0 && j < gameGrid.theGrid.at(j).size());
                    players.at(k).numPoints += gameGrid.theGrid.at(i).at(j).rank;
                }
            }
        }
    }
}


void gameState::initGame()              // skip the asserts for this function
{
    deckOfCards gameInitDeck;
    gameInitDeck.deck.resize(52);
    gameGrid.theGrid.resize(12);
    turnNum = 1;
    whoseTurn = 1;
    int i;
    
    players.resize(4);
    
    
    // change this later.
    players[0].isHuman = true;
    players[1].isHuman = false;
    players[2].isHuman = false;
    players[3].isHuman = false;
    
    
    
    for (i = 0; i < gameGrid.theGrid.size(); i++)
    {
        gameGrid.theGrid.at(i).resize(12);
    }
    
    // First run through:  Change all squares to blank.
    
    int j;
    for (i = 0; i < 12; i++)
    {
        for (j = 0; j < 12; j++)
        {
            gameGrid.theGrid.at(i).at(j).playerOwner = 0;
            gameGrid.theGrid.at(i).at(j).rank = 0;
            gameGrid.theGrid.at(i).at(j).suit = 0;
            gameGrid.theGrid[i][j].rowLocation = i;
            gameGrid.theGrid[i][j].colLocation = j;
        }
    }
    

    
    int val = 0;
    for (i = 0; i < 4; i++)
    {
        for (j = 2; j < 15; j++)
        {
            gameInitDeck.deck.at(val).rank = j;
            gameInitDeck.deck.at(val).intToSuit(i);            // a special function handles the suit char
            val++;
        }
    }
    
    

    
    for ( i = 0; i < 4; i++)
    {
        players.at(i).username = "Player ";               // for this prototype version, do it like this
        players.at(i).username = players.at(i).username + char('0'+i+1);
        players.at(i).playerID = i+1;
        players.at(i).numPoints = 0;
    }
    
    
    
    /*for (int i = 0; i < 52; i++)
    {
        gameInitDeck.deck[i].displayCard();
        cout << endl;
    }*/
    
    // gameInitDeck.displayDeck();
    gameInitDeck.shuffle();
    // gameInitDeck.displayDeck();
    
    vector < vector <int> > squaresThatGetCardsDealtToThem;
    
    squaresThatGetCardsDealtToThem.resize(52);

    card theCard;
    bool cardPlacedSuccessfullyFlag;
    int q;
    int c, r;
    for (i = 0; i < 12; i++)
    {
        for (j = 0; j < 12; j++)
        {
            gameGrid.theGrid[i][j].playerOwner = 0;
        }
    }
    
    for (q = 1; q <= 4/*5*/; q++)
    {
        //cout << "flag1";
        for ( i = 0; i < 9 + (q==5)*7; i++)      // do 16 cards for the gray unmoving cards
        {
            //cout << gameInitDeck.deck.size();

            //cout << "i = " << i << endl << "gameInitDeck.deck.size() = " << gameInitDeck.deck.size() << endl;
            theCard = gameInitDeck.deck[gameInitDeck.deck.size()-1];
            theCard.playerOwner = q;
            theCard.rowLocation = -1;
            theCard.colLocation = -1;
            // rank and suit are already set
            
            
            gameInitDeck.deck.resize(gameInitDeck.deck.size()-1);
            //cout << gameInitDeck.deck.size();
            do
            {
                //cout << "flag3";
                c = rand() % 12;
                r = rand() % 12;
                //cout << " c = " << c << endl;
                cardPlacedSuccessfullyFlag = false;
                if (gameGrid.theGrid[r][c].rank == 0)
                {
                    //cout << "flag4";
                    gameGrid.theGrid[r][c].rank = theCard.rank;
                    gameGrid.theGrid[r][c].suit = theCard.suit;
                    cardPlacedSuccessfullyFlag = true;
                    gameGrid.theGrid[r][c].colLocation = c;
                    gameGrid.theGrid[r][c].rowLocation = r;
                    gameGrid.theGrid[r][c].playerOwner = q;
                }
                else
                {
                    ;       // try again with two new random numbers
                }
            } while (!cardPlacedSuccessfullyFlag);
        }
    }
     

}


void card::displayCard()
{
    if (rank == 0 && suit == 0)
    {
        //cout << "BLANK";
        return;
    }
    if (rank >= 2 && rank <= 10)
    {
        //cout << rank;
    }
    else
    {
        switch(rank)
        {
            case 11:
                //cout << "jack";
                break;
            case 12:
                //cout << "queen";
                break;
            case 13:
                //cout << "king";
                break;
            case 14:
                //cout << "ace";
                break;
            default:
                ;//cout << "blank";
        }
    }
    //cout << " of ";
    switch(suit)
    {
        case 'c':
            //cout << "clubs";
            break;
        case 'd':
            //cout << "diamonds";
            break;
        case 'h':
            //cout << "hearts";
            break;
        case 's':        // 's'
            //cout << "spades";
            break;
        default:
            ;
            //cout << "blank";
    }
    //cout << ":";
    outputPlayerColor(playerOwner, rank, suit);
    return;
}

void card::displayCard2()
{
    if (rank == 0 && suit == 0)
    {
        //cout << "---------   ";
        return;
    }
    if (rank >= 2 && rank <= 9)
    {
        //cout << rank << "_";
    }
    else if (rank == 10)
    {
        //cout << rank;
    }
    else
    {
        switch(rank)
        {
            case 11:
                //cout << "JA";
                break;
            case 12:
                //cout << "QU";
                break;
            case 13:
                //cout << "KI";
                break;
            case 14:
                //cout << "AC";
                break;
            default:
                ;//cout << "RANK_ERROR";
        }
    }
    //cout << "of";
    switch(suit)
    {
        case 'c':
            //cout << "C";
            break;
        case 'd':
            //cout << "D";
            break;
        case 'h':
            //cout << "H";
            break;
        case 's':        // 's'
            //cout << "S";
            break;
        default:
            ;//cout << "SUIT_ERROR";
    }
    
    outputPlayerColor(playerOwner, rank, suit);
    //cout << "   ";
    return;
}



void outputPlayerColor(int playerOwner, int rank, int suit)
{
    switch (playerOwner)
    {
        case 5:
            if (rank != 0 && suit != 0)
            {
                //cout << "GRAY";
            }
            break;
        case 1:
            //cout << "RED_";
            break;
        case 2:
            //cout << "BLUE";
            break;
        case 3:
            //cout << "GREN";
            break;
        case 4:
            //cout << "YELW";
            break;
        default:
            ;//cout << "NO_COLOR (switch value = " << playerOwner << ")";
    }
    return;
}

void card::intToSuit(int num)
{
    switch (num)
    {
        case 0:
            suit = 'c';
            break;
        case 1:
            suit = 'd';
            break;
        case 2:
            suit = 'h';
            break;
        default:            // 3
            suit = 's';
    }
    return;
}


void deckOfCards::shuffle()
{
    vector <card> newDeck;
    newDeck.resize(52);
    int whichCard;
    int i;
    for (i = 0; i < 52; i++)
    {
        whichCard = rand() % deck.size();
        assert(i >= 0 && i < newDeck.size());
        newDeck.at(i) = deck.at(whichCard);
        deck.erase(deck.begin() + whichCard);
    }
    deck = newDeck;
}

void deckOfCards::displayDeck()
{
    int i;
    for (i = 0; i < deck.size(); i++)
    {
        //cout << endl;
        deck.at(i).displayCard();
    }
    //cout << endl;
    //cout << endl;
}

bool gameMove::isMoveLegal(grid gameGrid, int whoseTurn)
{
    // It is OK if this function is called and outputs that a move is illegal; as long as a move is successfully made, this code just verifies
    //  that one of the AI function did not return all legal moves.
    
    
    
    if (col1 < 0 || col1 > 11 || col2 < 0 || col2 > 11 || row1 < 0 || row1 > 11 || row2 < 0 || row2 > 11)
    {
        //cout << "illegalmoveflagONE";
        //cout << "\n\nThe move specified:  ";
        displayMove();
        return false;
    }
    // [col][row], like in chess
    assert (col1 >= 0 && col1 < gameGrid.theGrid.size() && row1 >= 0 && row1 < gameGrid.theGrid.at(col1).size());
    if (gameGrid.theGrid.at(row1).at(col1).playerOwner != whoseTurn)
    {
        //cout << "illegalmoveflagTWO" << "   " << gameGrid.theGrid.at(row1).at(col1).playerOwner << "  " << whoseTurn << endl;
        gameGrid.theGrid.at(row1).at(col1).displayCard();
        // cout << endl << col1 << " " << row1 << endl;
        // cout << " <<<";
        //cout << "\n\nThe move specified:  ";
        displayMove();
        //cout << "... is illegal.\n\n";
        return false;
    }
    assert (row2 >= 0 && row2 < gameGrid.theGrid.size() && col2 >= 0 && col2 < gameGrid.theGrid.at(col2).size());
    if (gameGrid.theGrid.at(row2).at(col2).rank != 0)     // we don't need to check both rank and suit for empty square
    {
        //cout << "illegalmoveflagTHREE";
        //cout << "\n\nThe move specified:  ";
        displayMove();
        //cout << "... is illegal.\n\n";
        return false;
    }
    if ( !((((col1 - col2 == 1) || (col1 - col2 == -1)) && (row1 == row2)) ||
           (((row1 - row2 == 1) || (row1 - row2 == -1)) && (col1 == col2))) )
    {
        //cout << "illegalmoveflagFOUR";
        //cout << "\n\nThe move specified:  ";
        displayMove();
        //cout << "... is illegal.\n\n";
        return false;
    }
    return true;
}



void gameState::displayGame()
{
    //cout << endl << endl;
    int i, j;
//#pragma omp parallel for collapse(2)
    for (i = 0 ; i < 12 ; i++)
    {
        for (j = 0; j < 12; j++)
        {
            //gameGrid.theGrid.at(j).at(i).displayCard2();         // col, row
            gameGrid.theGrid[i][j].displayCard2();                 // this is the (simple) and correct way to do col, row.
            
            // NO--this is clearly row,col.  This is a problem!  Fix it!
            
            
            // we need a better output screen, even just for debugging and testing
            //if (gameGrid.theGrid[j][i].playerOwner == 0) //blank
            //{
            //    cout << '\t';
            //}
            //cout << '\t';
            
        }
        //cout << endl;
    }
}


card gameState::oneWhoseTurnCardIsAdjacent(card Y)      // do not select it if the rank is > 9
{
    // This function had a number of typos before I redid the col-row thing.
    // A problem:  We don't learn the rank of the card, so it might not be worth it to send another card over.
    if ((((Y.rowLocation+1 < gameGrid.theGrid.size()) && Y.rowLocation+1 >= 0 && Y.colLocation >= 0 && Y.colLocation < gameGrid.theGrid.size()) && gameGrid.theGrid.at(Y.rowLocation+1).at(Y.colLocation).playerOwner == whoseTurn)
        && gameGrid.theGrid[Y.rowLocation+1][Y.colLocation].rank <= 9)
    {
        return gameGrid.theGrid[Y.rowLocation+1][Y.colLocation];
    }
    if ((((Y.rowLocation-1 < gameGrid.theGrid.size()) && Y.rowLocation-1 >= 0 && Y.colLocation >= 0 && Y.colLocation < gameGrid.theGrid.size()) && gameGrid.theGrid.at(Y.rowLocation-1).at(Y.colLocation).playerOwner == whoseTurn)
        && gameGrid.theGrid[Y.rowLocation-1][Y.colLocation].rank <= 9)
    {
        return gameGrid.theGrid[Y.rowLocation-1][Y.colLocation];
    }
    if ((((Y.rowLocation < gameGrid.theGrid.size()) && Y.rowLocation >= 0 && Y.colLocation+1 >= 0 && Y.colLocation+1 < gameGrid.theGrid.size()) && gameGrid.theGrid.at(Y.rowLocation).at(Y.colLocation+1).playerOwner == whoseTurn)
        && gameGrid.theGrid[Y.rowLocation][Y.colLocation+1].rank <= 9)
    {
        return gameGrid.theGrid[Y.rowLocation][Y.colLocation+1];
    }
    if ((((Y.rowLocation < gameGrid.theGrid.size()) && Y.rowLocation >= 0 && Y.colLocation-1 >= 0 && Y.colLocation-1 < gameGrid.theGrid.size()) && gameGrid.theGrid.at(Y.rowLocation).at(Y.colLocation-1).playerOwner == whoseTurn)
        && gameGrid.theGrid[Y.rowLocation][Y.colLocation-1].rank <= 9)
    {
        return gameGrid.theGrid[Y.rowLocation][Y.colLocation-1];
    }
    card temp;
    temp.rank = 0;
    return temp;
}



// THE vector INDEXING PROBLEM WAS HERE!!!!  I FIXED IT! :-D
// 10-18-2025, 5:38 p.m. ...take a nice break.  :)
vector <card> gameState::allEmptyAdjacentSquaresToCardZ(card Z)
{
    vector <card> toReturn;
    toReturn.resize(0);
    if (Z.colLocation+1 <= 11 && Z.colLocation+1 >= 0 && Z.rowLocation <= 11 &&
        Z.rowLocation >= 0 && gameGrid.theGrid.at(Z.rowLocation).at(Z.colLocation+1).rank == 0)
    {
        toReturn.push_back(gameGrid.theGrid[Z.rowLocation][Z.colLocation+1]);
    }
   // else
   // {
    //    cout << "badflag1";
    //}
    if (Z.colLocation-1 >= 0 && Z.colLocation-1 <= 11 && Z.rowLocation >= 0 && Z.rowLocation <= 11 && gameGrid.theGrid[Z.rowLocation][Z.colLocation-1].rank == 0)
    {
        toReturn.push_back(gameGrid.theGrid[Z.rowLocation][Z.colLocation-1]);
    }
   // else
   // {
    //    cout << "badflag2";
    //}
    if (Z.rowLocation+1 <= 11 && Z.rowLocation+1 >= 0 && Z.colLocation >= 0 && Z.colLocation <= 11 && gameGrid.theGrid[Z.rowLocation+1][Z.colLocation].rank == 0)
    {
        toReturn.push_back(gameGrid.theGrid[Z.rowLocation+1][Z.colLocation]);
    }
    //else
   // {
    //    cout << "badflag3";
    //}
    if (Z.rowLocation-1 >= 0 && Z.rowLocation-1 <= 11 && Z.colLocation >= 0 && Z.colLocation <= 11 && gameGrid.theGrid[Z.rowLocation-1][Z.colLocation].rank == 0)
    {
        toReturn.push_back(gameGrid.theGrid[Z.rowLocation-1][Z.colLocation]);
    }
    //else
   // {
    //    cout << "badflag4";
    //}
    return toReturn;
}

//bool gameState::isCardAAdjacentToCardB(card a, card b)
//{
 //   if (((a.rowLocation - b.rowLocation == 0) && ((a.colLocation - b.colLocation == -1 || a.colLocation - b.colLocation == 1)))
  //      || ((a.colLocation - b.colLocation == 0) && (a.rowLocation - b.rowLocation == -1 || a.rowLocation - b.rowLocation == 1)))
   // {
    //    return true;
    //}
    //return false;
//}




int digraph::minDistance(vector <int> dist, vector <bool> shortestPathTreeSet)      // gets the *index* of the min distance value in the vector
{
    int min = INT_MAX;
    int min_index = -1;
    
    int i;
    for (i = 0; i < graphNodes.size(); i++)
    {
        if (shortestPathTreeSet[i] == false && dist[i] <= min)
        {
            min = dist[i];
            min_index = i;
        }
    }
    return min_index;
}


int getNodeIndex(node a, vector <node> theVector)
{
    int i;
    for (i = 0; i < theVector.size(); i++)
    {
        if (theVector[i].nodeID == a.nodeID)
        {
            return i;
        }
    }
    return -1;
}



vector <nodeAndDistanceFromSourceAndVisited> digraph::getAllUnvisitedNeighborVertices(node a, vector <nodeAndDistanceFromSourceAndVisited> allNodes)
{
    vector <nodeAndDistanceFromSourceAndVisited> toReturn;
    int i;
    for (i = 0; i < allNodes.size(); i++)
    {
        // if there exists an edge from a to the node in the vector, and the node in the vector is unvisited, i.e., bool visited == false
        if (thereIsAnEdgeFromAToB(a,allNodes[i].a) && allNodes[i].visited == false)
        {
            toReturn.push_back(allNodes[i]);
        }
    }
    return toReturn;
}





bool digraph::thereIsAnEdgeFromAToB(node a, node b)
{
    int i;
    for (i = 0; i < allEdges.size(); i++)
    {
        if (allEdges[i].fromNode.nodeID == a.nodeID && allEdges[i].toNode.nodeID == b.nodeID)
        {
            return true;
        }
    }
    return false;
}

    // New source for Dijkstra's shortest path algorithm:  https://www.geeksforgeeks.org/cpp/c-program-for-Dijkstras-shortest-path-algorithm-greedy-algo-7/

    // Make sure to cite ChatGPT, w3schools, and geeksforgeeks.org for helping with the coding of this program.
// https://cplusplus.com/forum/beginner/254328/#google_vignette , that too




/*int digraph::customDijkstra(node source, node target)
{
    // Look up the source node in the vector stored in the struct.
    int sourceIndex = -1, targetIndex = -1;
    for (int i = 0; i < allSourceNodeDijkstraLists.size(); i++)
    {
        if (allSourceNodeDijkstraLists[i].a.nodeID == source.nodeID)
        {
            sourceIndex = i;
            //cout << "i = " << i << " and source.nodeID = " << source.nodeID << " and "
            //    << "allsourcenodedjistkralists[i].a.nodeID = " << allSourceNodeDijkstraLists[i].a.nodeID
            //    << " and target.nodeID = " << target.nodeID;
            break;
        }
    }
    //cout << "sourceIndex = " << sourceIndex;
    for (int i = 0; i < allSourceNodeDijkstraLists.size(); i++)
    {
        for (int j = 0; j < allSourceNodeDijkstraLists[i].nodeIDs.size(); j++)
        {
            if (allSourceNodeDijkstraLists[i].nodeIDs[j].nodeID == target.nodeID)
            {
                targetIndex = j;
                break;
            }
        }
    }
    //cout << "targetIndex = " << targetIndex;
    
    return allSourceNodeDijkstraLists[sourceIndex].dist[targetIndex];
}
*/



// REVIEW THIS CODE BY HAND CAREFULLY AND REPEATEDLY!  TEST IT AFTER YOU'RE SATISFIED IT'S CORRECT, LINE BY LINE.  11-10, 5:29 p.m.
void digraph::customDijkstra(node source)
{
    // To build this as the fast version:  We will need to have and maintain two priority queues...one ordered by distance, and one ordered
    //      by NodeID.  We need to look up nodes by both of those different measures at certain times.  We *can* obtain the running time listed
    //      on the internet, but not without using *two* different priority queues.
    
    DijkstraStorage abc;
    
    vector <nodeAndDistanceFromSourceAndVisited> allNodes;
    
    for (int i = 0; i < graphNodes.size(); i++)
    {
        nodeAndDistanceFromSourceAndVisited temp;
        temp.a = graphNodes[i];
        allNodes.push_back(temp);
        if (graphNodes[i].nodeID != source.nodeID)
        {
            allNodes[allNodes.size()-1].distance = -1;          // infinity
        }
        else
        {
            allNodes[allNodes.size()-1].distance = 0;           // set the source node distance to 0
        }
        allNodes[allNodes.size()-1].visited = false;
    }
    
    
    bool someReachableNodesUnvisited = true;
    // Beware:  We have some nodes that are unreachable in this graph.  Thus, we will not quite visit *all* nodes.
    
    while (someReachableNodesUnvisited == true)
    {
        
        // Find the smallest unvisited node, distance-wise.
        int minDistance = -1;                                   // initialize to infinity
        int savedIndex = -1;
        // Rather than using a priority queue, we could also speed up the linear sub-routines simply with parallel processing.
//#pragma omp parallel for
        // We don't need to speed this code up, it's already linear, and, it's a minimization one...can't be run simultaneously AFAIK.
        for (int i = 0; i < allNodes.size(); i++)
        {
            if (allNodes[i].visited == false && (minDistance == -1 || (allNodes[i].distance < minDistance && allNodes[i].distance != -1)))
            {
                minDistance = allNodes[i].distance;
                savedIndex = i;             // Save the index of the newly selected current node.
                //cout << "FLAG, and allnodes[i].visisted = " << allNodes[i].visited << endl;
                //cout << "...and, i = " << i << " and minDistance = " << minDistance << " and savedIndex = " << savedIndex << endl;
            }
            //cout << "\nminDistance = " << minDistance << " , savedIndex = " << savedIndex << endl;
        }
        //cout << "\nEND FOR LOOP.";
        
        for (int i = 0; i < allEdges.size(); i++)
        {
            if (allEdges[i].fromNode.nodeID == allNodes[savedIndex].a.nodeID)       // If the edge is from the current selected node.
            {
                // YES, parallelize this part.
#pragma omp parallel for          // NO, it causes test failures.         // Actually, pragma might not be the cause.
                for (int j = 0; j < allNodes.size(); j++)
                {
                    if (allEdges[i].toNode.nodeID == allNodes[j].a.nodeID &&        // the toNode is the neighbor
                        allNodes[j].visited == false)          // Find the "to" node in AllNodes, if unvisited.
                    {
                        if (allNodes[savedIndex].distance != -1 && (allNodes[savedIndex].distance + 1 < allNodes[j].distance || allNodes[j].distance == -1))
                            // compare the saved distance to the new calculated dist.
                        {
                            allNodes[j].distance = allNodes[savedIndex].distance+1;     // Update the shortest distance if we can do better.
                        }
                    }
                }
            }
        }
        allNodes[savedIndex].visited = true;            // Why isn't this value sticking??  It's not updating!
        //cout << "***saved index = " << savedIndex << endl;
        //cout << "allnodes[savedindex].visisted = " << allNodes[savedIndex].visited << endl;
        someReachableNodesUnvisited = false;
        for (int i = 0; i < allNodes.size(); i++)
        {
            if (allNodes[i].visited == false && allNodes[i].a.reachable == true)
            {
                someReachableNodesUnvisited = true;
            }
        }
    }
    
    abc.dist = allNodes;
    abc.a = source;
    abc.nodeIDs = graphNodes;               // We don't really need this in the data structure, but it's fine to include it.
    
    allSourceNodeDijkstraLists.push_back(abc);
}
    

int digraph::getSourceToTargetDistance(node source, node target)
{
    // I think the problem is with looking up the node.
    
    // special case
    if (source.gridCol == target.gridCol && source.gridRow == target.gridRow)
    {
        return 0;
    }
    
    int i;
    for (i = 0; i < allSourceNodeDijkstraLists.size(); i++)
    {
        if (source.nodeID == allSourceNodeDijkstraLists[i].a.nodeID)
        {
            //allSourceNodeDijkstraLists[i].a.displayNode();
            for (int j = 0; j < allSourceNodeDijkstraLists[i].dist.size(); j++)
            {
                if (allSourceNodeDijkstraLists[i].dist[j].a.nodeID == target.nodeID)
                {
                    //return allSourceNodeDijkstraLists[source.nodeID].dist[
                    //                                                      allSourceNodeDijkstraLists[i].nodeIDs[j].nodeID
                    //                                                      ];       // BE CAREFUL...BIG CHANGE MADE HERE, PREVIOUSLY WAS:
                    return allSourceNodeDijkstraLists[i].dist[j].distance;
                    // the Google AI Dijkstra code indexes by nodeID.
                }
            }
        }
    }
    return -1;
}


// 10-26, 5:24 p.m.:   A comment:  Terminal is glitchy.  Don't hit Command+F and try to find text within it unless you have maximized the window


// I'm going to try something with parallel computing, and will back up this source code again.



string getCardFileName(int row, int col, vector < vector <card> > grid)
{
    string toReturn = "./";
    switch (grid[row][col].rank)
    {
        case 2:
            toReturn += "Two";
            break;
        case 3:
            toReturn += "Three";
            break;
        case 4:
            toReturn += "Four";
            break;
        case 5:
            toReturn += "Five";
            break;
        case 6:
            toReturn += "Six";
            break;
        case 7:
            toReturn += "Seven";
            break;
        case 8:
            toReturn = "Eight";
            break;
        case 9:
            toReturn = "Nine";
            break;
        case 10:
            toReturn = "Ten";
            break;
        case 11:
            toReturn = "Jack";
            break;
        case 12:
            toReturn = "Queen";
            break;
        case 13:
            toReturn = "King";
            break;
        case 14:
            toReturn = "Ace";
            break;
        default:
            toReturn = "Blank";
    }
    toReturn += "Of";
    switch (grid[row][col].suit)
    {
        case 'c':
            toReturn += "Clubs";
            break;
        case 'd':
            toReturn += "Diamonds";
            break;
        case 'h':
            toReturn += "Hearts";
            break;
        case 's':
            toReturn += "Spades";
            break;
        default:
            toReturn += "Blank";
            break;
    }
    toReturn += ".png";
    return toReturn;
}

/*
int main()
{
    sf::RenderWindow window(sf::VideoMode({2560, 1664}), "Playing Cards Fight");
    
    
    bool permanentChangeFlag = false;
    while (window.isOpen())
    {
        
        sf::Texture textureA;
        sf::Texture textureB;
    
        
        if (!textureA.loadFromFile("RedCircle.png"))
        {
            cout << "ERROR";
            exit(0);
        }
        if (!textureB.loadFromFile("BlueCircle.png"))
        {
            cout << "ERROR";
            exit(0);
        }
    
        
        sf::Sprite spriteA(textureA);
        sf::Sprite spriteB(textureB);
        
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                permanentChangeFlag = true;
            }
        }
            
        
        spriteA.setPosition({100.0,100.0});
        
        
        if (permanentChangeFlag)
        {
            spriteB = spriteA;
            spriteB.setPosition({200.0,200.0});
            
        }
        window.clear(sf::Color::White);
        window.draw(spriteA);
        window.draw(spriteB);
        window.display();
        
        
        
        
    }
    
    
    
    return 0;
}

*/





int main()
{
    srand(time(0));
    
/*    while(1)
    {
        gameState gs;
        
        gs.initGame();
        
        gs.gameGrid.theGrid[0][0].rank = 0; gs.gameGrid.theGrid[0][0].suit = 0; gs.gameGrid.theGrid[0][0].playerOwner = 0;
        gs.gameGrid.theGrid[0][1].rank = 0; gs.gameGrid.theGrid[0][1].suit = 0; gs.gameGrid.theGrid[0][1].playerOwner = 0;
        gs.gameGrid.theGrid[0][2].rank = 12; gs.gameGrid.theGrid[0][2].suit = 'c'; gs.gameGrid.theGrid[0][2].playerOwner = 3;
        gs.gameGrid.theGrid[0][3].rank = 0; gs.gameGrid.theGrid[0][3].suit = 0; gs.gameGrid.theGrid[0][3].playerOwner = 0;
        gs.gameGrid.theGrid[0][4].rank = 0; gs.gameGrid.theGrid[0][4].suit = 0; gs.gameGrid.theGrid[0][4].playerOwner = 0;
        gs.gameGrid.theGrid[0][5].rank = 0; gs.gameGrid.theGrid[0][5].suit = 0; gs.gameGrid.theGrid[0][5].playerOwner = 0;
        gs.gameGrid.theGrid[0][6].rank = 0; gs.gameGrid.theGrid[0][6].suit = 0; gs.gameGrid.theGrid[0][6].playerOwner = 0;
        gs.gameGrid.theGrid[0][7].rank = 0; gs.gameGrid.theGrid[0][7].suit = 0; gs.gameGrid.theGrid[0][7].playerOwner = 0;
        gs.gameGrid.theGrid[0][8].rank = 0; gs.gameGrid.theGrid[0][8].suit = 0; gs.gameGrid.theGrid[0][8].playerOwner = 0;
        gs.gameGrid.theGrid[0][9].rank = 14; gs.gameGrid.theGrid[0][9].suit = 'c'; gs.gameGrid.theGrid[0][9].playerOwner = 1;
        gs.gameGrid.theGrid[0][10].rank = 0; gs.gameGrid.theGrid[0][10].suit = 0; gs.gameGrid.theGrid[0][10].playerOwner = 0;
        gs.gameGrid.theGrid[0][11].rank = 0; gs.gameGrid.theGrid[0][11].suit = 0; gs.gameGrid.theGrid[0][11].playerOwner = 0;
        
        gs.gameGrid.theGrid[1][0].rank = 13; gs.gameGrid.theGrid[1][0].suit = 'c'; gs.gameGrid.theGrid[1][0].playerOwner = 3;
        gs.gameGrid.theGrid[1][1].rank = 0; gs.gameGrid.theGrid[1][1].suit = 0; gs.gameGrid.theGrid[1][1].playerOwner = 0;
        gs.gameGrid.theGrid[1][2].rank = 5; gs.gameGrid.theGrid[1][2].suit = 'c'; gs.gameGrid.theGrid[1][2].playerOwner = 3;
        gs.gameGrid.theGrid[1][3].rank = 0; gs.gameGrid.theGrid[1][3].suit = 0; gs.gameGrid.theGrid[1][3].playerOwner = 0;
        gs.gameGrid.theGrid[1][4].rank = 0; gs.gameGrid.theGrid[1][4].suit = 0; gs.gameGrid.theGrid[1][4].playerOwner = 0;
        gs.gameGrid.theGrid[1][5].rank = 0; gs.gameGrid.theGrid[1][5].suit = 0; gs.gameGrid.theGrid[1][5].playerOwner = 0;
        gs.gameGrid.theGrid[1][6].rank = 0; gs.gameGrid.theGrid[1][6].suit = 0; gs.gameGrid.theGrid[1][6].playerOwner = 0;
        gs.gameGrid.theGrid[1][7].rank = 0; gs.gameGrid.theGrid[1][7].suit = 0; gs.gameGrid.theGrid[1][7].playerOwner = 0;
        gs.gameGrid.theGrid[1][8].rank = 0; gs.gameGrid.theGrid[1][8].suit = 0; gs.gameGrid.theGrid[1][8].playerOwner = 0;
        gs.gameGrid.theGrid[1][9].rank = 14; gs.gameGrid.theGrid[1][9].suit = 'c'; gs.gameGrid.theGrid[1][9].playerOwner = 1;
        gs.gameGrid.theGrid[1][10].rank = 5; gs.gameGrid.theGrid[1][10].suit = 'c'; gs.gameGrid.theGrid[1][10].playerOwner = 1;
        gs.gameGrid.theGrid[1][11].rank = 0; gs.gameGrid.theGrid[1][11].suit = 0; gs.gameGrid.theGrid[1][11].playerOwner = 0;
        
        gs.gameGrid.theGrid[2][0].rank = 0; gs.gameGrid.theGrid[2][0].suit = 0; gs.gameGrid.theGrid[2][0].playerOwner = 0;
        gs.gameGrid.theGrid[2][1].rank = 13; gs.gameGrid.theGrid[2][1].suit = 'c'; gs.gameGrid.theGrid[2][1].playerOwner = 3;
        gs.gameGrid.theGrid[2][2].rank = 0; gs.gameGrid.theGrid[2][2].suit = 0; gs.gameGrid.theGrid[2][2].playerOwner = 0;
        gs.gameGrid.theGrid[2][3].rank = 0; gs.gameGrid.theGrid[2][3].suit = 0; gs.gameGrid.theGrid[2][3].playerOwner = 0;
        gs.gameGrid.theGrid[2][4].rank = 0; gs.gameGrid.theGrid[2][4].suit = 0; gs.gameGrid.theGrid[2][4].playerOwner = 0;
        gs.gameGrid.theGrid[2][5].rank = 0; gs.gameGrid.theGrid[2][5].suit = 0; gs.gameGrid.theGrid[2][5].playerOwner = 0;
        gs.gameGrid.theGrid[2][6].rank = 0; gs.gameGrid.theGrid[2][6].suit = 0; gs.gameGrid.theGrid[2][6].playerOwner = 0;
        gs.gameGrid.theGrid[2][7].rank = 8; gs.gameGrid.theGrid[2][7].suit = 'c'; gs.gameGrid.theGrid[2][7].playerOwner = 3;
        gs.gameGrid.theGrid[2][8].rank = 11; gs.gameGrid.theGrid[2][8].suit = 'c'; gs.gameGrid.theGrid[2][8].playerOwner = 1;
        gs.gameGrid.theGrid[2][9].rank = 0; gs.gameGrid.theGrid[2][9].suit = 0; gs.gameGrid.theGrid[2][9].playerOwner = 0;
        gs.gameGrid.theGrid[2][10].rank = 0; gs.gameGrid.theGrid[2][10].suit = 0; gs.gameGrid.theGrid[2][10].playerOwner = 0;
        gs.gameGrid.theGrid[2][11].rank = 0; gs.gameGrid.theGrid[2][11].suit = 0; gs.gameGrid.theGrid[2][11].playerOwner = 0;
        
        
        gs.gameGrid.theGrid[3][0].rank = 0; gs.gameGrid.theGrid[3][0].suit = 0; gs.gameGrid.theGrid[3][0].playerOwner = 0;
        gs.gameGrid.theGrid[3][1].rank = 0; gs.gameGrid.theGrid[3][1].suit = 0; gs.gameGrid.theGrid[3][1].playerOwner = 0;
        gs.gameGrid.theGrid[3][2].rank = 7; gs.gameGrid.theGrid[3][2].suit = 'c'; gs.gameGrid.theGrid[3][2].playerOwner = 4;
        gs.gameGrid.theGrid[3][3].rank = 0; gs.gameGrid.theGrid[3][3].suit = 0; gs.gameGrid.theGrid[3][3].playerOwner = 0;
        gs.gameGrid.theGrid[3][4].rank = 0; gs.gameGrid.theGrid[3][4].suit = 0; gs.gameGrid.theGrid[3][4].playerOwner = 0;
        gs.gameGrid.theGrid[3][5].rank = 0; gs.gameGrid.theGrid[3][5].suit = 0; gs.gameGrid.theGrid[3][5].playerOwner = 0;
        gs.gameGrid.theGrid[3][6].rank = 0; gs.gameGrid.theGrid[3][6].suit = 0; gs.gameGrid.theGrid[3][6].playerOwner = 0;
        gs.gameGrid.theGrid[3][7].rank = 6; gs.gameGrid.theGrid[3][7].suit = 'c'; gs.gameGrid.theGrid[3][7].playerOwner = 3;
        gs.gameGrid.theGrid[3][8].rank = 0; gs.gameGrid.theGrid[3][8].suit = 0; gs.gameGrid.theGrid[3][8].playerOwner = 0;
        gs.gameGrid.theGrid[3][9].rank = 8; gs.gameGrid.theGrid[3][9].suit = 'c'; gs.gameGrid.theGrid[3][9].playerOwner = 3;
        gs.gameGrid.theGrid[3][10].rank = 4; gs.gameGrid.theGrid[3][10].suit = 'c'; gs.gameGrid.theGrid[3][10].playerOwner = 3;
        gs.gameGrid.theGrid[3][11].rank = 3; gs.gameGrid.theGrid[3][11].suit = 'c'; gs.gameGrid.theGrid[3][11].playerOwner = 1;
        
        
        gs.gameGrid.theGrid[4][0].rank = 0; gs.gameGrid.theGrid[4][0].suit = 0; gs.gameGrid.theGrid[4][0].playerOwner = 0;
        gs.gameGrid.theGrid[4][1].rank = 0; gs.gameGrid.theGrid[4][1].suit = 0; gs.gameGrid.theGrid[4][1].playerOwner = 0;
        gs.gameGrid.theGrid[4][2].rank = 0; gs.gameGrid.theGrid[4][2].suit = 0; gs.gameGrid.theGrid[4][2].playerOwner = 0;
        gs.gameGrid.theGrid[4][3].rank = 0; gs.gameGrid.theGrid[4][3].suit = 0; gs.gameGrid.theGrid[4][3].playerOwner = 0;
        gs.gameGrid.theGrid[4][4].rank = 0; gs.gameGrid.theGrid[4][4].suit = 0; gs.gameGrid.theGrid[4][4].playerOwner = 0;
        gs.gameGrid.theGrid[4][5].rank = 0; gs.gameGrid.theGrid[4][5].suit = 0; gs.gameGrid.theGrid[4][5].playerOwner = 0;
        gs.gameGrid.theGrid[4][6].rank = 0; gs.gameGrid.theGrid[4][6].suit = 0; gs.gameGrid.theGrid[4][6].playerOwner = 0;
        gs.gameGrid.theGrid[4][7].rank = 2; gs.gameGrid.theGrid[4][7].suit = 'c'; gs.gameGrid.theGrid[4][7].playerOwner = 3;
        gs.gameGrid.theGrid[4][8].rank = 0; gs.gameGrid.theGrid[4][8].suit = 0; gs.gameGrid.theGrid[4][8].playerOwner = 0;
        gs.gameGrid.theGrid[4][9].rank = 0; gs.gameGrid.theGrid[4][9].suit = 0; gs.gameGrid.theGrid[4][9].playerOwner = 0;
        gs.gameGrid.theGrid[4][10].rank = 4; gs.gameGrid.theGrid[4][10].suit = 'c'; gs.gameGrid.theGrid[4][10].playerOwner = 3;
        gs.gameGrid.theGrid[4][11].rank = 12; gs.gameGrid.theGrid[4][11].suit = 'c'; gs.gameGrid.theGrid[4][11].playerOwner = 1;
        
        
        gs.gameGrid.theGrid[5][0].rank = 0; gs.gameGrid.theGrid[5][0].suit = 0; gs.gameGrid.theGrid[5][0].playerOwner = 0;
        gs.gameGrid.theGrid[5][1].rank = 0; gs.gameGrid.theGrid[5][1].suit = 0; gs.gameGrid.theGrid[5][1].playerOwner = 0;
        gs.gameGrid.theGrid[5][2].rank = 0; gs.gameGrid.theGrid[5][2].suit = 0; gs.gameGrid.theGrid[5][2].playerOwner = 0;
        gs.gameGrid.theGrid[5][3].rank = 0; gs.gameGrid.theGrid[5][3].suit = 0; gs.gameGrid.theGrid[5][3].playerOwner = 0;
        gs.gameGrid.theGrid[5][4].rank = 0; gs.gameGrid.theGrid[5][4].suit = 0; gs.gameGrid.theGrid[5][4].playerOwner = 0;
        gs.gameGrid.theGrid[5][5].rank = 0; gs.gameGrid.theGrid[5][5].suit = 0; gs.gameGrid.theGrid[5][5].playerOwner = 0;
        gs.gameGrid.theGrid[5][6].rank = 7; gs.gameGrid.theGrid[5][6].suit = 'c'; gs.gameGrid.theGrid[5][6].playerOwner = 3;
        gs.gameGrid.theGrid[5][7].rank = 0; gs.gameGrid.theGrid[5][7].suit = 0; gs.gameGrid.theGrid[5][7].playerOwner = 0;
        gs.gameGrid.theGrid[5][8].rank = 11; gs.gameGrid.theGrid[5][8].suit = 'c'; gs.gameGrid.theGrid[5][8].playerOwner = 3;
        gs.gameGrid.theGrid[5][9].rank = 0; gs.gameGrid.theGrid[5][9].suit = 0; gs.gameGrid.theGrid[5][9].playerOwner = 0;
        gs.gameGrid.theGrid[5][10].rank = 0; gs.gameGrid.theGrid[5][10].suit = 0; gs.gameGrid.theGrid[5][10].playerOwner = 0;
        gs.gameGrid.theGrid[5][11].rank = 0; gs.gameGrid.theGrid[5][11].suit = 0; gs.gameGrid.theGrid[5][11].playerOwner = 0;
        
        
        gs.gameGrid.theGrid[6][0].rank = 0; gs.gameGrid.theGrid[6][0].suit = 0; gs.gameGrid.theGrid[6][0].playerOwner = 0;
        gs.gameGrid.theGrid[6][1].rank = 0; gs.gameGrid.theGrid[6][1].suit = 0; gs.gameGrid.theGrid[6][1].playerOwner = 0;
        gs.gameGrid.theGrid[6][2].rank = 0; gs.gameGrid.theGrid[6][2].suit = 0; gs.gameGrid.theGrid[6][2].playerOwner = 0;
        gs.gameGrid.theGrid[6][3].rank = 0; gs.gameGrid.theGrid[6][3].suit = 0; gs.gameGrid.theGrid[6][3].playerOwner = 0;
        gs.gameGrid.theGrid[6][4].rank = 0; gs.gameGrid.theGrid[6][4].suit = 0; gs.gameGrid.theGrid[6][4].playerOwner = 0;
        gs.gameGrid.theGrid[6][5].rank = 0; gs.gameGrid.theGrid[6][5].suit = 0; gs.gameGrid.theGrid[6][5].playerOwner = 0;
        gs.gameGrid.theGrid[6][6].rank = 9; gs.gameGrid.theGrid[6][6].suit = 'c'; gs.gameGrid.theGrid[6][6].playerOwner = 1;
        gs.gameGrid.theGrid[6][7].rank = 10; gs.gameGrid.theGrid[6][7].suit = 'c'; gs.gameGrid.theGrid[6][7].playerOwner = 1;
        gs.gameGrid.theGrid[6][8].rank = 0; gs.gameGrid.theGrid[6][8].suit = 0; gs.gameGrid.theGrid[6][8].playerOwner = 0;
        gs.gameGrid.theGrid[6][9].rank = 0; gs.gameGrid.theGrid[6][9].suit = 0; gs.gameGrid.theGrid[6][9].playerOwner = 0;
        gs.gameGrid.theGrid[6][10].rank = 0; gs.gameGrid.theGrid[6][10].suit = 0; gs.gameGrid.theGrid[6][10].playerOwner = 0;
        gs.gameGrid.theGrid[6][11].rank = 0; gs.gameGrid.theGrid[6][11].suit = 0; gs.gameGrid.theGrid[6][11].playerOwner = 0;
        
        
        gs.gameGrid.theGrid[7][0].rank = 0; gs.gameGrid.theGrid[7][0].suit = 0; gs.gameGrid.theGrid[7][0].playerOwner = 0;
        gs.gameGrid.theGrid[7][1].rank = 0; gs.gameGrid.theGrid[7][1].suit = 0; gs.gameGrid.theGrid[7][1].playerOwner = 0;
        gs.gameGrid.theGrid[7][2].rank = 0; gs.gameGrid.theGrid[7][2].suit = 0; gs.gameGrid.theGrid[7][2].playerOwner = 0;
        gs.gameGrid.theGrid[7][3].rank = 0; gs.gameGrid.theGrid[7][3].suit = 0; gs.gameGrid.theGrid[7][3].playerOwner = 0;
        gs.gameGrid.theGrid[7][4].rank = 0; gs.gameGrid.theGrid[7][4].suit = 0; gs.gameGrid.theGrid[7][4].playerOwner = 0;
        gs.gameGrid.theGrid[7][5].rank = 0; gs.gameGrid.theGrid[7][5].suit = 0; gs.gameGrid.theGrid[7][5].playerOwner = 0;
        gs.gameGrid.theGrid[7][6].rank = 3; gs.gameGrid.theGrid[7][6].suit = 'c'; gs.gameGrid.theGrid[7][6].playerOwner = 1;
        gs.gameGrid.theGrid[7][7].rank = 0; gs.gameGrid.theGrid[7][7].suit = 0; gs.gameGrid.theGrid[7][7].playerOwner = 0;
        gs.gameGrid.theGrid[7][8].rank = 0; gs.gameGrid.theGrid[7][8].suit = 0; gs.gameGrid.theGrid[7][8].playerOwner = 0;
        gs.gameGrid.theGrid[7][9].rank = 0; gs.gameGrid.theGrid[7][9].suit = 0; gs.gameGrid.theGrid[7][9].playerOwner = 0;
        gs.gameGrid.theGrid[7][10].rank = 0; gs.gameGrid.theGrid[7][10].suit = 0; gs.gameGrid.theGrid[7][10].playerOwner = 0;
        gs.gameGrid.theGrid[7][11].rank = 0; gs.gameGrid.theGrid[7][11].suit = 0; gs.gameGrid.theGrid[7][11].playerOwner = 0;
        
        
        
        gs.gameGrid.theGrid[8][0].rank = 11; gs.gameGrid.theGrid[8][0].suit = 'c'; gs.gameGrid.theGrid[8][0].playerOwner = 4;
        gs.gameGrid.theGrid[8][1].rank = 0; gs.gameGrid.theGrid[8][1].suit = 0; gs.gameGrid.theGrid[8][1].playerOwner = 0;
        gs.gameGrid.theGrid[8][2].rank = 0; gs.gameGrid.theGrid[8][2].suit = 0; gs.gameGrid.theGrid[8][2].playerOwner = 0;
        gs.gameGrid.theGrid[8][3].rank = 0; gs.gameGrid.theGrid[8][3].suit = 0; gs.gameGrid.theGrid[8][3].playerOwner = 0;
        gs.gameGrid.theGrid[8][4].rank = 0; gs.gameGrid.theGrid[8][4].suit = 0; gs.gameGrid.theGrid[8][4].playerOwner = 0;
        gs.gameGrid.theGrid[8][5].rank = 10; gs.gameGrid.theGrid[8][5].suit = 'c'; gs.gameGrid.theGrid[8][5].playerOwner = 4;
        gs.gameGrid.theGrid[8][6].rank = 6; gs.gameGrid.theGrid[8][6].suit = 'c'; gs.gameGrid.theGrid[8][6].playerOwner = 1;
        gs.gameGrid.theGrid[8][7].rank = 0; gs.gameGrid.theGrid[8][7].suit = 0; gs.gameGrid.theGrid[8][7].playerOwner = 0;
        gs.gameGrid.theGrid[8][8].rank = 0; gs.gameGrid.theGrid[8][8].suit = 0; gs.gameGrid.theGrid[8][8].playerOwner = 0;
        gs.gameGrid.theGrid[8][9].rank = 0; gs.gameGrid.theGrid[8][9].suit = 0; gs.gameGrid.theGrid[8][9].playerOwner = 0;
        gs.gameGrid.theGrid[8][10].rank = 0; gs.gameGrid.theGrid[8][10].suit = 0; gs.gameGrid.theGrid[8][10].playerOwner = 0;
        gs.gameGrid.theGrid[8][11].rank = 0; gs.gameGrid.theGrid[8][11].suit = 0; gs.gameGrid.theGrid[8][11].playerOwner = 0;
        
    
        gs.gameGrid.theGrid[9][0].rank = 0; gs.gameGrid.theGrid[9][0].suit = 0; gs.gameGrid.theGrid[9][0].playerOwner = 0;
        gs.gameGrid.theGrid[9][1].rank = 0; gs.gameGrid.theGrid[9][1].suit = 0; gs.gameGrid.theGrid[9][1].playerOwner = 0;
        gs.gameGrid.theGrid[9][2].rank = 0; gs.gameGrid.theGrid[9][2].suit = 0; gs.gameGrid.theGrid[9][2].playerOwner = 0;
        gs.gameGrid.theGrid[9][3].rank = 0; gs.gameGrid.theGrid[9][3].suit = 0; gs.gameGrid.theGrid[9][3].playerOwner = 0;
        gs.gameGrid.theGrid[9][4].rank = 2; gs.gameGrid.theGrid[9][4].suit = 'c'; gs.gameGrid.theGrid[9][4].playerOwner = 2;
        gs.gameGrid.theGrid[9][5].rank = 0; gs.gameGrid.theGrid[9][5].suit = 0; gs.gameGrid.theGrid[9][5].playerOwner = 0;
        gs.gameGrid.theGrid[9][6].rank = 5; gs.gameGrid.theGrid[9][6].suit = 'c'; gs.gameGrid.theGrid[9][6].playerOwner = 2;
        gs.gameGrid.theGrid[9][7].rank = 3; gs.gameGrid.theGrid[9][7].suit = 'c'; gs.gameGrid.theGrid[9][7].playerOwner = 4;
        gs.gameGrid.theGrid[9][8].rank = 0; gs.gameGrid.theGrid[9][8].suit = 0; gs.gameGrid.theGrid[9][8].playerOwner = 0;
        gs.gameGrid.theGrid[9][9].rank = 0; gs.gameGrid.theGrid[9][9].suit = 0; gs.gameGrid.theGrid[9][9].playerOwner = 0;
        gs.gameGrid.theGrid[9][10].rank = 7; gs.gameGrid.theGrid[9][10].suit = 'c'; gs.gameGrid.theGrid[9][10].playerOwner = 4;
        gs.gameGrid.theGrid[9][11].rank = 0; gs.gameGrid.theGrid[9][11].suit = 0; gs.gameGrid.theGrid[9][11].playerOwner = 0;
        
        
        gs.gameGrid.theGrid[10][0].rank = 0; gs.gameGrid.theGrid[10][0].suit = 0; gs.gameGrid.theGrid[10][0].playerOwner = 0;
        gs.gameGrid.theGrid[10][1].rank = 0; gs.gameGrid.theGrid[10][1].suit = 0; gs.gameGrid.theGrid[10][1].playerOwner = 0;
        gs.gameGrid.theGrid[10][2].rank = 2; gs.gameGrid.theGrid[10][2].suit = 'c'; gs.gameGrid.theGrid[10][2].playerOwner = 2;
        gs.gameGrid.theGrid[10][3].rank = 0; gs.gameGrid.theGrid[10][3].suit = 0; gs.gameGrid.theGrid[10][3].playerOwner = 0;
        gs.gameGrid.theGrid[10][4].rank = 0; gs.gameGrid.theGrid[10][4].suit = 0; gs.gameGrid.theGrid[10][4].playerOwner = 0;
        gs.gameGrid.theGrid[10][5].rank = 0; gs.gameGrid.theGrid[10][5].suit = 0; gs.gameGrid.theGrid[10][5].playerOwner = 0;
        gs.gameGrid.theGrid[10][6].rank = 5; gs.gameGrid.theGrid[10][6].suit = 'c'; gs.gameGrid.theGrid[10][6].playerOwner = 1;
        gs.gameGrid.theGrid[10][7].rank = 12; gs.gameGrid.theGrid[10][7].suit = 'c'; gs.gameGrid.theGrid[10][7].playerOwner = 2;
        gs.gameGrid.theGrid[10][8].rank = 6; gs.gameGrid.theGrid[10][8].suit = 'c'; gs.gameGrid.theGrid[10][8].playerOwner = 1;
        gs.gameGrid.theGrid[10][9].rank = 0; gs.gameGrid.theGrid[10][9].suit = 0; gs.gameGrid.theGrid[10][9].playerOwner = 0;
        gs.gameGrid.theGrid[10][10].rank = 0; gs.gameGrid.theGrid[10][10].suit = 0; gs.gameGrid.theGrid[10][10].playerOwner = 0;
        gs.gameGrid.theGrid[10][11].rank = 0; gs.gameGrid.theGrid[10][11].suit = 0; gs.gameGrid.theGrid[10][11].playerOwner = 0;
        
        
        gs.gameGrid.theGrid[11][0].rank = 0; gs.gameGrid.theGrid[11][0].suit = 0; gs.gameGrid.theGrid[11][0].playerOwner = 0;
        gs.gameGrid.theGrid[11][1].rank = 0; gs.gameGrid.theGrid[11][1].suit = 0; gs.gameGrid.theGrid[11][1].playerOwner = 0;
        gs.gameGrid.theGrid[11][2].rank = 0; gs.gameGrid.theGrid[11][2].suit = 0; gs.gameGrid.theGrid[11][2].playerOwner = 0;
        gs.gameGrid.theGrid[11][3].rank = 8; gs.gameGrid.theGrid[11][3].suit = 'c'; gs.gameGrid.theGrid[11][3].playerOwner = 2;
        gs.gameGrid.theGrid[11][4].rank = 0; gs.gameGrid.theGrid[11][4].suit = 0; gs.gameGrid.theGrid[11][4].playerOwner = 0;
        gs.gameGrid.theGrid[11][5].rank = 0; gs.gameGrid.theGrid[11][5].suit = 0; gs.gameGrid.theGrid[11][5].playerOwner = 0;
        gs.gameGrid.theGrid[11][6].rank = 0; gs.gameGrid.theGrid[11][6].suit = 0; gs.gameGrid.theGrid[11][6].playerOwner = 0;
        gs.gameGrid.theGrid[11][7].rank = 4; gs.gameGrid.theGrid[11][7].suit = 'c'; gs.gameGrid.theGrid[11][7].playerOwner = 2;
        gs.gameGrid.theGrid[11][8].rank = 0; gs.gameGrid.theGrid[11][8].suit = 0; gs.gameGrid.theGrid[11][8].playerOwner = 0;
        gs.gameGrid.theGrid[11][9].rank = 0; gs.gameGrid.theGrid[11][9].suit = 0; gs.gameGrid.theGrid[11][9].playerOwner = 0;
        gs.gameGrid.theGrid[11][10].rank = 0; gs.gameGrid.theGrid[11][10].suit = 0; gs.gameGrid.theGrid[11][10].playerOwner = 0;
        gs.gameGrid.theGrid[11][11].rank = 9; gs.gameGrid.theGrid[11][11].suit = 'c'; gs.gameGrid.theGrid[11][11].playerOwner = 4;
      
        
        
        
        
        
        gs.displayGame();
        
        cout << "\n\nEnter the row,col of the target card you're interested in:  ";
        
        string is;
        getline(cin, is);
        
        stringstream ss0(is);
        
        int row, col;
        
        ss0 >> row >> col;
        
        
        
        bestCardPairScoreWithMoves qqq = findClosestTwoCardCaptureOpportunityForAI(gs.gameGrid.theGrid[row][col], gs);
        
        qqq.a.displayCard();
        qqq.b.displayCard();
        qqq.c.displayMove();
        qqq.d.displayMove();
        cout << "\n\nPress enter to continue.\n\n";
        
        getline(cin,is);
        
        cout << "\n\nEnter the row,col of the source node you want to generate shortest paths from:  ";
        
        getline(cin,is);
        stringstream ss1(is);
        
        ss1 >> row >> col;
        
        digraph theGraph(gs.gameGrid);
        
        node temp;
        
        for (int i = 0; i < theGraph.graphNodes.size(); i++)
        {
            if (theGraph.graphNodes[i].gridRow == row && theGraph.graphNodes[i].gridCol == col)
            {
                temp = theGraph.graphNodes[i];
            }
        }
        
        //theGraph.customDijkstra(temp);
        
        theGraph.displayDigraph(temp);
    }
 */


    
    
    
    
    
    
    
    
    
    
    

    
    sf::RenderWindow window(sf::VideoMode({2560, 1664}), "Playing Cards Fight");
    
    
    window.setKeyRepeatEnabled(false);
    
    
    
    gameMove moveToMake;
    
    vector <int> winners;
    
    bool wasAIMoveLegal;
    bool cardJustCaptured = false;
    bool skipButtonInitializedFlag = false;
    //bool dirButtonInitializedFlag = false;
    
    bool isACurrentPlayerCardHighlightedInPink = false;
    bool isACurrentPlayerCardHighlightedInBlue = false;
    // both of these two variables can be true at the same time, but there can never be two pink cards or two light blue cards
    
    vector < vector <sf::Texture> > boardCardTextures;
    vector < vector <sf::Sprite> > boardCards;
    
    vector < vector <sf::Texture> > circleSpriteTextures;
    vector < vector <sf::Sprite> > circleSprites;
    
    sf::Texture fillerTexture;
    
    if (!fillerTexture.loadFromFile("./ClearRectangle.png"))
    {
        //cout << "FILE LOAD ERROR";
        exit(0);
    }
    if (!fillerTexture.loadFromFile("./ClearCircle.png"))
    {
        //cout << "FILE LOAD ERROR";
        exit(0);
    }

    bool waitForUpdate = false;
    
    sf::Sprite swapTemp(fillerTexture);
    
    gameState gs;
    gs.initGame();
    
    bool drawMovementArrowAndPause;
    
    bool AIskippingTurn = false;
    
//    graphicsBundle gb1, gb2;        // Anticipation:  One graphicsBundle for the cards, one for the circles drawn on them.
    
    //gb1.gridOfSquares.resize(12);
    //gb1.texturesForGrid.resize(12);
    //gb2.gridOfSquares.resize(12);
    //gb2.gridOfSquares.resize(12);
    
    sf::Texture RedTexture;
    sf::Texture BlueTexture;
    sf::Texture GreenTexture;
    sf::Texture GrayTexture;
    
    sf::Texture currTexture;
    
    circleSpriteTextures.resize(12);
    for (int i = 0; i < 12; i++)
    {
        circleSpriteTextures[i].resize(12);
        for (int j = 0; j < 12; j++)
        {
            string filename;
            switch(gs.gameGrid.theGrid[i][j].playerOwner)
            {
                case 1:
                    filename = "./RedCircle.png";
                    break;
                case 2:
                    filename = "./BlueCircle.png";
                    break;
                case 3:
                    filename = "./GreenCircle.png";
                    break;
                case 4:
                    filename = "./GrayCircle.png";
                    break;
                default:
                    filename = "./ClearCircle.png";
            }
            if (!circleSpriteTextures[i][j].loadFromFile(filename))
            {
                //cout << "Failed to load file.\n";
                exit(0);
            }
            circleSpriteTextures[i][j].setSmooth(true);
        }
    }
    
    circleSprites.resize(12);
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            circleSprites[i].push_back(sf::Sprite(circleSpriteTextures[i][j]));
            circleSprites[i][j].setPosition({static_cast<float>(50.0*j+5.0f) , static_cast<float>(60.0*i+45.0f)});
            circleSprites[i][j].setScale({1.0/11.0f,1.0/11.0f});
        }
    }
    
    
    boardCardTextures.resize(12);
    for (int i = 0; i < 12; i++)
    {
        boardCardTextures[i].resize(12);
        for (int j = 0; j < 12; j++)
        {
            string filename = "./" + getCardFileName(i,j,gs.gameGrid.theGrid);
            if (filename == "./BlankOfBlank.png")
            {
                filename = "./ClearRectangle.png";
            }
            if (!currTexture.loadFromFile(filename))
            {
                //cout << "ERROR, file failed to load.";
                exit (0);
            }
            boardCardTextures[i][j] = currTexture;
            boardCardTextures[i][j].setSmooth(true);
        }
    }
    

    
    boardCards.resize(12);
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            boardCards[i].push_back(sf::Sprite(boardCardTextures[i][j]));
            boardCards[i][j].setPosition({static_cast<float> (50.0*j+10.0f),static_cast<float>(60.0*i+30.0f)});
            boardCards[i][j].setScale({1.0/8.0f,1.0/9.0f});
        }
    }
    
    int selectedSquareCol = -1;
    int selectedSquareRow = -1;
    int mouseOverCol, mouseOverRow;
    bool squareSelected = false;
    bool moveJustMade = false;
    
    bool flashFlag = false;
    //bool specialFlag = false;
    
    string textToDisplay = "";
    
    /*
    sf::Clock clock;
    sf::Time time1;
    sf::Time time2;
     */
    
    /*sf::Texture directionsButtonTexture;
    if (!directionsButtonTexture.loadFromFile("DirectionsButton.png"))
    {
        cout << "ERROR, image file failed to load.";
        exit(0);
    }
    sf::Sprite dirButton(directionsButtonTexture);*/
    
    sf::Texture skipButtonTexture;
    if (!skipButtonTexture.loadFromFile("SkipButton.png"))
    {
        //cout << "ERROR, image file failed to load.";
        exit(0);
    }
    sf::Sprite skipButtonSprite(skipButtonTexture);
    
    
    

    vector <card> cardsToUpdate;
    
    bool openToMouseSelectionChange = false;
    bool keyCodeBlockVisited = false;
    
    int gameJustLoaded = 0;
    
    bool clickOn = false;
    
    
    sf::Clock clock;
    //sf::Time lastClickOnTime = clock.getElapsedTime() - clock.getElapsedTime();     // 0, basically
    
    while (window.isOpen())
    {
        if (gameJustLoaded == 0)
        {
            // Screen -1, before the credits and before the player selections.
            
            while (const std::optional event = window.pollEvent())
            {
                // "close requested" event: we close the window
                if (event->is<sf::Event::Closed>())
                    window.close();
                
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    clickOn = true;
                    
                }
                else
                {
                    clickOn = false;
                }
                
                
            }
            
            
            
         
            sf::Font font("NK57 Monospace Cd Rg It.otf");
            sf::Text textString(font);
            sf::Text textString0(font);
            textString0.setString("\nPlaying Cards Fight\n\na game by Philip J. White\n");
            textString.setString("Click to continue.\nCopyright 2025, Philip J. White\nAll rights reserved.\nPlease do not pirate this game.  It costs only $3.00 to purchase.\n\n\nCredits:\n\nGraphics....Playing card graphics designed by Freepik.  Graphics used based on on-site written permission for use.  Check out freepik.com.\nC++ Programming............................................................................................................Philip J. White\nGame Development...........................................................................................................Philip J. White\nGraphics.....................................................SFML (Simple and Fast Multimedia Library), check out https://www.sfml-dev.org\nDevelopment Environment..............................................................................................Xcode and MacBook Air\nOther tools used........................................................................................Open MP (for code parallelization)\n");
                                 

            sf::Texture logoTexture;
            if (!logoTexture.loadFromFile("TwoOfDiamonds.png"))
            {
                //cout << "FILE LOAD ERROR";
                exit(0);
            }
            
            sf::Texture introBlueCircleTexture;
            if (!introBlueCircleTexture.loadFromFile("BlueCircle.png"))
            {
                //cout << "FILE LOAD ERROR";
                exit(0);
            }
            
            sf::Sprite introBlueCircle(introBlueCircleTexture);
            sf::Sprite illustration(logoTexture);
            
            
            introBlueCircle.setPosition({495, 300});
            introBlueCircle.setScale({0.45f,0.45f});
            
            
            
            
            illustration.setPosition({525,310});
            illustration.setScale({0.40f,0.40f});

            textString0.setPosition({300,0});
            textString0.setFillColor(sf::Color(255,0,0));
            textString0.setScale({2.0f,2.0f});
                                  
            textString.setPosition({270,570});
            textString.setFillColor(sf::Color(7,148,104));
            textString.setScale({0.45f,0.45f});
            
            window.clear(sf::Color(80,80,80));
            
            window.draw(textString0);
            window.draw(textString);
            window.draw(illustration);
            window.draw(introBlueCircle);
            
            
            
            window.display();
            
            
            if (clickOn == true)
            {
                this_thread::sleep_for(chrono::milliseconds(300));
                gameJustLoaded = 1;
            }
            
            
        }
        else if (gameJustLoaded == 1)
        {
            
            while (const std::optional event = window.pollEvent())
            {
                // "close requested" event: we close the window
                if (event->is<sf::Event::Closed>())
                    window.close();
                
                
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    clickOn = true;
                    
                }
                else
                {
                    clickOn = false;
                }
                
                
            }
            sf::Font font("NK57 Monospace Cd Rg It.otf");
            sf::Text directions(font);
            directions.setScale({0.5f,0.5f});
            directions.setString("HOW TO PLAY.\n\nPlaying Cards Fight is a game about capturing material--other players' cards--and bringing it home to home base to score\npoints.  The players proceed from red, to blue,to green, to gray, making moves, one square at a time, horizontally and vertically.\nEach card moves to an empty square.\n\nOf critical importance, whenever two cards of a particular rank are such that their ranks sum to a certain value and\nthey are both adjacent to an opponent's card where the opponent's card is at least 2 greater than the aforementioned\nsum of two cards, then the opponent's card will be captured by the two capturing cards, and the capturing player\nwill be able to move that card.  For example, if I am player Blue and I move the 4 of clubs and the 8 of diamonds,\nand both of these cards are owned by player Blue (ownership is portrayed based on a circle at the bottom left of the\ncard), and I move both of those cards next to the King of Diamonds, owned by Player Gray, then I will capture the\nKing of Diamonds, because 4 + 8 < 13 + 2.  (The rank of each card is, for 2-10, the number on the card, and, 11 for\njack, 12 for queen, 13 for king, and 14 for ace.  The ace does not have a rank of 1 in this game.)\n\nThe two main activities in the game are trying to capture opponent cards, as described above, and trying to\nmove cards you control to your home base.  If you are a particular player with a certain color, your base is\nmarked as the 4x4 corner in the board that you ought to move your cards to to score points.  If you have a card\nstationed at your base, whenever a player finishes moving and a new turn begins, you get to add points based on the\nrank of each card in your home base, so, e.g., if you have the Ace of Clubs and the King of Spades in your home base\nand those are the only cards controlled by you there, then you will get 14+13 = 27 points per turn.\n\nThe first player to score 2000 points wins the game.\n\nSee if you can win 5 games in a row against 3 computer players!\n\nAlso, can you beat the AI players by more than 1000 points?\n\nThe movement points are determined by simulated dice.  That is, die one can have as its outcome be one to six, and\ndie two can also have its outcome be one to six.  The dice are summed by the computer and presented as\nthe number of movement points each player has for his/her turn.\n\nPlease enjoy the game!\n\nThe developer believes that the game is more fun than Minesweepr or Solitaire, and easier to learn than chess.");
            
            directions.setPosition({400,100});
            directions.setFillColor(sf::Color(0,255,0));
            
            
            
            window.clear(sf::Color::Black);
            window.draw(directions);
            
            window.display();
            
            
            if (clickOn)
            {
                gameJustLoaded = 2;
            }

            
        }
        // Screen 1, before the game.  Remember to display the credits!
        else if (gameJustLoaded == 2)
        {

            while (const std::optional event = window.pollEvent())
            {
                // "close requested" event: we close the window
                if (event->is<sf::Event::Closed>())
                    window.close();
                
                
                
                bool mouseOverScreen1;
                
                mouseOverCol = -1;
                mouseOverRow = -1;
                
                
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))// && !clickOn)
                {
                    // Extend the duration of the click to 0.1 seconds, then, refresh and set click to off.
                    //if (lastClickOnTime.asMilliseconds() < 100)
                    //{
                        clickOn = true;
                    //}
                    //else
                    //{
                      //  clickOn = false;
                        // lastClickOnTime = clock.getElapsedTime() - lastClickOnTime;
                        // I am not that good with the mouse clicks.  Try to be empirical and do what works smoothly.
                        // the lastClickOnTime caused some freezes.
                    //}
                }
                else
                {
                    clickOn = false;
                }
                
                
                sf::Font font("NK57 Monospace Cd Rg It.otf");
                sf::Text textString(font);
                textString.setString("Welcome to Playing Cards Fight\n\nThis game has four players--red, blue, green, and gray.\n\nPlease select which of these players will be human and which will be\ncomputer AI players:\n\n");
                textString.setPosition({300,50});
                textString.setFillColor(sf::Color(7,148,104));
                
                sf::Texture startButton;
                if (!startButton.loadFromFile("./StartButton.png"))
                {
                    //cout << "ERROR, image file not found.";
                    exit(0);
                }
                sf::Sprite startButtonSprite(startButton);
                
                sf::Texture redButtonTexture;
                if (!redButtonTexture.loadFromFile("./RedButton.png"))
                {
                    //cout << "ERROR, image file not found.";
                    exit(0);
                }
                sf::Sprite redButton(redButtonTexture);
                
                sf::Texture blueButtonTexture;
                if (!blueButtonTexture.loadFromFile("./BlueButton.png"))
                {
                    //cout << "ERROR, image file not found.";
                    exit(0);
                }
                sf::Sprite blueButton(blueButtonTexture);
                
                sf::Texture greenButtonTexture;
                if (!greenButtonTexture.loadFromFile("GreenButton.png"))
                {
                    //cout << "ERROR, image file not found.";
                    exit(0);
                }
                sf::Sprite greenButton(greenButtonTexture);
                
                sf::Texture grayButtonTexture;
                if (!grayButtonTexture.loadFromFile("./GrayButton.png"))
                {
                    //cout << "ERROR, image file not found.";
                    exit(0);
                }
                sf::Sprite grayButton(grayButtonTexture);
                
                sf::Text redButtonText(font);
                redButtonText.setFillColor(sf::Color::Black);
                if (gs.players[0].isHuman == true)
                {
                    redButtonText.setString("Red Player is Human\n   (click to switch)");
                }
                else
                {
                    redButtonText.setString("Red Player is Computer\n   (click to switch)");
                }
                
                sf::Text blueButtonText(font);
                blueButtonText.setFillColor(sf::Color::Black);
                if (gs.players[1].isHuman == true)
                {
                    blueButtonText.setString("Blue Player is Human\n   (click to switch)");
                }
                else
                {
                    blueButtonText.setString("Blue Player is Computer\n   (click to switch)");
                }
                
                sf::Text greenButtonText(font);
                greenButtonText.setFillColor(sf::Color::Black);
                if (gs.players[2].isHuman == true)
                {
                    greenButtonText.setString("Green Player is Human\n   (click to switch)");
                }
                else
                {
                    greenButtonText.setString("Green Player is Computer\n   (click to switch)");
                }
                
                sf::Text grayButtonText(font);
                grayButtonText.setFillColor(sf::Color::Black);
                if (gs.players[3].isHuman == true)
                {
                    grayButtonText.setString("Gray Player is Human\n   (click to switch)");
                }
                else
                {
                    grayButtonText.setString("Gray Player is Computer\n  (click to switch)");
                }
                
                
                startButtonSprite.setPosition({450, 255});
                
                redButton.setPosition({200,100});
                blueButton.setPosition({700,100});
                greenButton.setPosition({200,400});
                grayButton.setPosition({700,400});
                
                redButtonText.setPosition({340,360});
                redButtonText.setScale({1.0/2.0f,1.0/2.0f});
                blueButtonText.setPosition({840,360});
                blueButtonText.setScale({1.0/2.0f,1.0/2.0f});
                greenButtonText.setPosition({340,660});
                greenButtonText.setScale({1.0/2.0f,1.0/2.0f});
                grayButtonText.setPosition({840,660});
                grayButtonText.setScale({1.0/2.0f,1.0/2.0f});
                
                
                
                
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                float mouseX = localPosition.x;
                float mouseY = localPosition.y;
                
                if (mouseX >= 547 && mouseX <= 735 && mouseY >= 440 && mouseY <= 615)
                {
                    startButtonSprite.setColor(sf::Color(0,0,255));
                    if (clickOn == true)
                    {
                        gameJustLoaded = 3;         // proceed to the main game screen.
                        this_thread::sleep_for(chrono::milliseconds(1000));
                    }
                }
                else
                {
                    startButtonSprite.setColor(sf::Color::White);
                }
                
                
                if (mouseX >= 200 && mouseX <= 600 && mouseY >= 275 && mouseY <= 450)
                {
                    redButton.setColor(sf::Color(255,0,255));
                    if (clickOn == true)
                    {
                        gs.players[0].isHuman = !gs.players[0].isHuman;
                    }
                }
                else
                {
                    redButton.setColor(sf::Color::White);
                }
                
                if (mouseX >= 700 && mouseX <= 1100 && mouseY >= 275 && mouseY <= 450)
                {
                    blueButton.setColor(sf::Color(255,0,255));
                    if (clickOn == true)
                    {
                        gs.players[1].isHuman = !gs.players[1].isHuman;
                    }
                }
                else
                {
                    blueButton.setColor(sf::Color::White);
                }
                
                if (mouseX >= 200 && mouseX <= 600 && mouseY >= 575 && mouseY <= 750)
                {
                    greenButton.setColor(sf::Color(0,255,255));
                    if (clickOn == true)
                    {
                        gs.players[2].isHuman = !gs.players[2].isHuman;
                    }
                    
                }
                else
                {
                    greenButton.setColor(sf::Color::White);
                }
                
                if (mouseX >= 700 && mouseX <= 1100 && mouseY >= 575 && mouseY <= 750)
                {
                    grayButton.setColor(sf::Color(255,0,255));
                    if (clickOn == true)
                    {
                        gs.players[3].isHuman = !gs.players[3].isHuman;
                    }
                    
                }
                else
                {
                    grayButton.setColor(sf::Color::White);
                }
                
                window.clear(sf::Color(80,80,120));
            
                window.draw(redButton); window.draw(blueButton); window.draw(greenButton); window.draw(grayButton);
                window.draw(redButtonText); window.draw(blueButtonText); window.draw(greenButtonText); window.draw(grayButtonText);
                window.draw(textString);
                window.draw(startButtonSprite);
                
                
                window.display();
            }
        }
        else if (gameJustLoaded == 3)
        {
            if (gs.players[0].numPoints >= 2000 || gs.players[1].numPoints >= 2000 || gs.players[2].numPoints >= 2000
                || gs.players[3].numPoints >= 2000)
            {
                for (int i = 0; i < gs.players.size(); i++)
                {
                    if (gs.players[i].numPoints >= 2000)
                    {
                        winners.push_back(i+1);
                    }
                }
                gameJustLoaded = 4;
            }
            
            
            
            sf::Texture arrowTex;
            if (!arrowTex.loadFromFile("MovementArrow.png"))
            {
                //cout << "ERROR, image file not found.\n\n";
                exit(0);
            }
            sf::Sprite arrow(arrowTex);
            
            
            drawMovementArrowAndPause = false;
            //specialFlag = false;
            // check all the window's events that were triggered since the last iteration of the loop
            moveJustMade = false;
            
            //put this code outside of the window.pollevent section.
            if (gs.players[gs.whoseTurn-1].isHuman == true)
            {
                skipButtonSprite.setPosition({700.0f,450.0f});
                skipButtonInitializedFlag = true;
                
                //dirButton.setPosition({700.0f,-100.0f});
                //dirButton.setScale({1.0/2.0f,1.0/2.0f});
                //dirButtonInitializedFlag = true;
            }
                
                
                
                
            while (const std::optional event = window.pollEvent())
            {
                // "close requested" event: we close the window
                if (event->is<sf::Event::Closed>())
                    window.close();
                
                
                
                // SECTION 1:  GOVERNING INPUT VARIABLE UPDATE AND MAINTENANCE (mostly).
                
                bool mouseOver;
                
                mouseOverCol = -1;
                mouseOverRow = -1;
                
                /*if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                 {
                 if (clickOn == false && openToMouseSelectionChange == false)
                 {
                 time1 = clock.getElapsedTime();
                 keyCodeBlockVisited = true;
                 }
                 else
                 {
                 clickOn = true;
                 }
                 }
                 else if (keyCodeBlockVisited)
                 {
                 time2 = clock.getElapsedTime();
                 if (time2.asMilliseconds() - time1.asMilliseconds() >= 500)
                 // if it has been 1/2 a second since we last clicked when we hadn't been clicking before that time
                 {
                 openToMouseSelectionChange = true;
                 }
                 else
                 {
                 openToMouseSelectionChange = false;
                 }
                 clickOn = false;
                 }
                 else
                 {
                 openToMouseSelectionChange = true;
                 }*/
                
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))// && !clickOn)
                {
                    clickOn = true;
                    //cout << "HI!";
                }
                else
                {
                    clickOn = false;
                }
                
                
                
                
                
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                float mouseX = localPosition.x;
                float mouseY = localPosition.y;
                
                mouseOver = false;
                
                for (int col = 0; col < 12; col++)
                {
                    for (int row = 0; row < 12; row++)
                    {
                        if (mouseX < 50.0*(col+1)+10.0f && mouseX >= 50.0*(col)+10.0f
                            && mouseY < 60.0*(row+1)+30.0f && mouseY >= 60.0*(row)+30.0f)
                        {
                            mouseOverCol = col;
                            mouseOverRow = row;
                            mouseOver = true;
                            break;
                        }
                    }
                    if (mouseOver==true)
                    {
                        break;
                    }
                }
                
                // Special case for clicking the "skip turn" button.
                
                
                
                //if (mouseX ... && mouseY ... && clickOn == true)
                //{
                //    gs.movementPointsTotal = 1;
                //}
                
                
                
                
                
                if (gs.players[gs.whoseTurn-1].isHuman == true)
                {
                    if (clickOn == true && mouseOver == false)          // if we clicked somewhere off the grid, deactivate the selected square
                    {
                        //cout << "a";
                        selectedSquareCol = -1;
                        selectedSquareRow = -1;
                        squareSelected = false;
                        isACurrentPlayerCardHighlightedInBlue = false;
                        isACurrentPlayerCardHighlightedInPink = false;
                        
                        
                        // special case...if we clicked the skip button
                        // paste this code in both clickon==true&&mouseover==false && clickon==false&&mouseover==false
                        if (clickOn == true && mouseX >= 700 && mouseX <= 1100 && mouseY >= 635 && mouseY <= 795)
                        {
                            gs.movementPointsTotal = 0;
                            moveJustMade = false;
                        }
                        else if (mouseX >= 700 && mouseX <= 1100 && mouseY >= 635 && mouseY <= 795)
                        {
                            skipButtonSprite.setColor(sf::Color(0,255,0));
                        }
                        else
                        {
                            skipButtonSprite.setColor(sf::Color::White);
                        }
                        
                        
                    }
                    if (clickOn == false && mouseOver == true)
                    {
                        //cout << "z";
                        
                        
                        
                        
                        // special case...if we clicked the skip button     // we need to include this under clickon=false&mouseover=true!
                        if (clickOn == true && mouseX >= 700 && mouseX <= 1100 && mouseY >= 635 && mouseY <= 795)
                        {
                            gs.movementPointsTotal = 0;
                            moveJustMade = false;
                        }
                        else if (mouseX >= 700 && mouseX <= 1100 && mouseY >= 635 && mouseY <= 795)
                        {
                            skipButtonSprite.setColor(sf::Color(0,255,0));
                        }
                        else
                        {
                            skipButtonSprite.setColor(sf::Color::White);
                        }
                        
                        
                        
                        
                        // special case
                        // Problem:  We don't have for sure that a card is selected here.
                        
                        // I missed a spot in the col-row reversal!
                        if (squareSelected == true && gs.gameGrid.theGrid[mouseOverRow][mouseOverCol].playerOwner == 0 &&
                            gs.areTwoCardsAdjacent(gs.gameGrid.theGrid[selectedSquareRow][selectedSquareCol],
                                                   gs.gameGrid.theGrid[mouseOverRow][mouseOverCol]))
                            // A square is selected and we are mousing over an adjacent blank square, which should be highlighted.
                            
                        {
                            //specialFlag = true;
                            isACurrentPlayerCardHighlightedInPink = true;       // it's actually a blank square, not a card
                            isACurrentPlayerCardHighlightedInBlue = true;
                            // squareSelected = true;           // NO, not this time, we don't know this.
                        }
                        else if (squareSelected == false && gs.gameGrid.theGrid[mouseOverRow][mouseOverCol].playerOwner == 0)
                        {
                            isACurrentPlayerCardHighlightedInPink = false;
                            isACurrentPlayerCardHighlightedInBlue = false;      // caught this during Dana's test run.
                        }
                        else if (squareSelected)            // We can completely determine the state in the if statement above...so this is else if.
                            
                            // this condition is:  We have a card selected, and our mouse is over a card that is not blank and adjacent
                        {
                            if (mouseOverCol == selectedSquareCol && mouseOverRow == selectedSquareRow)
                            {
                                // this is right...our mouse is over the selected card, but we're not clicking.
                                isACurrentPlayerCardHighlightedInPink = false;
                                isACurrentPlayerCardHighlightedInBlue = true;
                            }
                            else        // if the mouse is over a different card
                            {
                                if (gs.gameGrid.theGrid[mouseOverRow][mouseOverCol].playerOwner == gs.whoseTurn)
                                {
                                    isACurrentPlayerCardHighlightedInPink = true;
                                }
                                else
                                {
                                    isACurrentPlayerCardHighlightedInPink = false;
                                }
                                isACurrentPlayerCardHighlightedInBlue = true;
                            }
                        }
                        else
                            // no square is selected, and we are mousing over the human player's card, but not clicking
                        {
                            if (gs.gameGrid.theGrid[mouseOverRow][mouseOverCol].playerOwner == gs.whoseTurn)// && openToMouseSelectionChange)
                            {
                                isACurrentPlayerCardHighlightedInBlue = false;
                                isACurrentPlayerCardHighlightedInPink = true;
                            }
                            else
                                // no square is selected, and we are mousing over a blank space or another player's card
                            {
                                isACurrentPlayerCardHighlightedInBlue = false;
                                isACurrentPlayerCardHighlightedInPink = false;
                            }
                        }
                    }
                    else if (clickOn == false && mouseOver == false)
                    {
                        //cout << "w";
                        isACurrentPlayerCardHighlightedInPink = false;
                        isACurrentPlayerCardHighlightedInBlue = squareSelected;
                        
                        
                        // special case...if we clicked the skip button     // we need to include this under clickon=false&mouseover=true!
                        if (clickOn == true && mouseX >= 700 && mouseX <= 1100 && mouseY >= 635 && mouseY <= 795)
                        {
                            gs.movementPointsTotal = 0;
                            moveJustMade = false;
                        }
                        else if (mouseX >= 700 && mouseX <= 1100 && mouseY >= 635 && mouseY <= 795)
                        {
                            skipButtonSprite.setColor(sf::Color(0,255,0));
                        }
                        else
                        {
                            skipButtonSprite.setColor(sf::Color::White);
                        }
                        
            
                        
                        
                        
                    }
                    //  else if (clickOn==true && mouseOver==false)     // case already covered
                    else if (clickOn == true && mouseOver == true)
                    {
                        
                        // special case
                        if (squareSelected && mouseOverCol == selectedSquareCol && mouseOverRow == selectedSquareRow)
                        {
                            
                            if (true)//openToMouseSelectionChange)
                            {
                                //cout << "g";
                                isACurrentPlayerCardHighlightedInBlue = false;
                                isACurrentPlayerCardHighlightedInPink = true;
                                squareSelected = false;
                                selectedSquareCol = -1;
                                selectedSquareRow = -1;
                            }
                        }
                        
                        else if (gs.gameGrid.theGrid[mouseOverRow][mouseOverCol].playerOwner == 0 && squareSelected == true &&
                                 gs.areTwoCardsAdjacent(gs.gameGrid.theGrid[selectedSquareRow][selectedSquareCol],
                                                        gs.gameGrid.theGrid[mouseOverRow][mouseOverCol]))
                        {
                            //cout << "e";
                            //    isACurrentPlayerCardHighlightedInPink = true;       // it's actually a blank square, not a card
                            //    isACurrentPlayerCardHighlightedInBlue = true;
                            
                            // This isn't right...we should make a move when this code is called.
                            
                            // squareSelected = true;       // NO, we don't know this for sure.
                            // We only update squareSelected when we are *changing* it.  It's not a simple information submission.
                            
                            
                            gameMove moveToMake;
                            moveToMake.row1 = selectedSquareRow;
                            moveToMake.col1 = selectedSquareCol;
                            moveToMake.row2 = mouseOverRow;
                            moveToMake.col2 = mouseOverCol;
                            
                            if (!moveToMake.isMoveLegal(gs.gameGrid, gs.whoseTurn))
                            {
                                //cout << "ERROR, invalid human move selected.";
                                exit (0);
                            }
                            
                            // I haven't applied the game move!
                            
                            gs.applyMove(moveToMake);
                            
                            
                            
                            
                            
                            // It is swapping the Sprites, but not updating their properties :).
                            
                            swapTemp = boardCards[moveToMake.row1][moveToMake.col1];
                            boardCards[moveToMake.row1][moveToMake.col1] = boardCards[moveToMake.row2][moveToMake.col2];
                            
                            // The line of commented out code below is from above, pasted for reference only.
                            // boardCards[i][j].setPosition({static_cast<float> (50.0*j+10.0f),static_cast<float>(60.0*i+30.0f)});
                            
                            boardCards[moveToMake.row1][moveToMake.col1].setPosition({static_cast<float>(50.0*(moveToMake.col1)+10.0f),static_cast<float> (60.0*(moveToMake.row1)+30.0f)});
                            boardCards[moveToMake.row1][moveToMake.col1].setScale({1.0/8.0f,1.0/9.0f});
                            boardCards[moveToMake.row1][moveToMake.col1].setColor(sf::Color::White);
                            
                            boardCards[moveToMake.row2][moveToMake.col2] = swapTemp;
                            boardCards[moveToMake.row2][moveToMake.col2].setPosition({static_cast<float>(50.0*(moveToMake.col2)+10.0f),static_cast<float> (60.0*(moveToMake.row2)+30.0f)});
                            boardCards[moveToMake.row2][moveToMake.col2].setScale({1.0/8.0f,1.0/9.0f});
                            boardCards[moveToMake.row2][moveToMake.col2].setColor(sf::Color::White);
                            
                            
                            
                            
                            
                            
                            
                            // The line of code below is, too.
                            
                            // circleSprites[i][j].setPosition({static_cast<float>(50.0*j+5.0f) , static_cast<float>(60.0*i+45.0f)});
                            
                            // col actually does precede row in SetPosition statements
                            swapTemp = circleSprites[moveToMake.row1][moveToMake.col1];
                            circleSprites[moveToMake.row1][moveToMake.col1] = circleSprites[moveToMake.row2][moveToMake.col2];
                            circleSprites[moveToMake.row1][moveToMake.col1].setPosition({static_cast<float>(50.0*moveToMake.col1+5.0f), static_cast<float>(60.0*moveToMake.row1+45.0f)});
                            circleSprites[moveToMake.row1][moveToMake.col1].setScale({1.0/11.0f,1.0/11.0f});
                            circleSprites[moveToMake.row1][moveToMake.col1].setColor(sf::Color::White);
                            
                            circleSprites[moveToMake.row2][moveToMake.col2] = swapTemp;
                            circleSprites[moveToMake.row2][moveToMake.col2].setPosition({static_cast<float>(50.0*moveToMake.col2+5.0f), static_cast<float>(60.0*moveToMake.row2+45.0f)});
                            circleSprites[moveToMake.row2][moveToMake.col2].setScale({1.0/11.0f,1.0/11.0f});
                            circleSprites[moveToMake.row2][moveToMake.col2].setColor(sf::Color::White);
                            
                            
                            
                            
                            isACurrentPlayerCardHighlightedInBlue = false;
                            isACurrentPlayerCardHighlightedInPink = false;  // at least just for a moment, turn off mouseover color
                            squareSelected = false;
                            selectedSquareCol = -1;
                            selectedSquareRow = -1;
                            mouseOver = false;                              // this will be reset in a moment if the mouse stays over a card
                            
                            moveJustMade = true;
                            gs.movementPointsTotal--;
                            
                        }
                        else if (gs.gameGrid.theGrid[mouseOverRow][mouseOverCol].playerOwner != gs.whoseTurn && squareSelected == true) // &&
                            //                             ! gs.areTwoCardsAdjacent(gs.gameGrid.theGrid[selectedSquareRow][selectedSquareCol],
                            //                                                    gs.gameGrid.theGrid[mouseOverRow][mouseOverCol]))
                            // It doesn't matter if the two cards are adjacent; we flip off the selection anyway.
                            // This is:  We clicked an opponent card while one of our cards was selected and highlighted in blue,
                            // so turn the selection off.  DON'T turn pink on, we never do that for opponents' cards.
                        {
                            //cout << "d";
                            if (true) // openToMouseSelectionChange == true)
                            {
                                //cout << "c";
                                // we selected a blank card in the grid, or another player's card, so turn off the selection.
                                isACurrentPlayerCardHighlightedInBlue = false;
                                isACurrentPlayerCardHighlightedInPink = false;
                                squareSelected = false;
                                selectedSquareCol = -1;
                                selectedSquareRow = -1;
                            }
                        }
                        else if (gs.gameGrid.theGrid[mouseOverRow][mouseOverCol].playerOwner == gs.whoseTurn)
                            
                            //&& openToMouseSelectionChange == true)
                        {
                            // toggle square selected
                            if (squareSelected == false)
                            {
                                //cout << "b";
                                squareSelected = true;
                                selectedSquareRow = mouseOverRow;
                                selectedSquareCol = mouseOverCol;
                                isACurrentPlayerCardHighlightedInBlue = true;
                                isACurrentPlayerCardHighlightedInPink = false;
                            }
                            else
                            {
                                squareSelected = false;
                                selectedSquareCol = -1;
                                selectedSquareRow = -1;
                                isACurrentPlayerCardHighlightedInBlue = false;
                                isACurrentPlayerCardHighlightedInPink = true;
                            }
                        }
                        else
                        {
                            //cout << "q";
                            //cout << gs.gameGrid.theGrid[mouseOverRow][mouseOverCol].playerOwner << " , " << gs.whoseTurn << " , " <<
                            //squareSelected << " , " << openToMouseSelectionChange << endl;
                            //cout << time2.asMilliseconds() - time1.asMilliseconds() << endl;
                        }
                        
                        
                        // ONLY DO ELSE IFS...we don't want to run the code straight through sequentially, becuase var changes in the middle might
                        // change whether or not code blocks will be activated.
                        
                        // we already handled the case below
                        
                        //else if (isACurrentPlayerCardHighlightedInBlue == true && selectedSquareCol == mouseOverCol && selectedSquareRow == mouseOverRow
                        // we just had the user clicking the card that was already selected; so now it's de-selected
                        //)
                        // ...so that we don't de-select the card too quickly if it's a sudden brief mouse click
                        //{
                        
                        // add this condition here, not &&'d with the line above, to avoid disrupting the else statement. :).
                        //cout << "a";
                        // if (openToMouseSelectionChange == true)
                        // {
                        //isACurrentPlayerCardHighlightedInBlue = false;
                        //  isACurrentPlayerCardHighlightedInPink = true;
                        //    squareSelected = false;
                        //      selectedSquareCol = -1;
                        //        selectedSquareRow = -1;
                        //      }
                        //}
                        //else if (isACurrentPlayerCardHighlightedInBlue == true && gs.players[gs.whoseTurn-1].isHuman == true)            // we  just clicked a different card, but there is a blue card
                        //{
                        //  if (gs.gameGrid.theGrid[mouseOverCol][mouseOverRow].playerOwner == 0)     // a blank square
                        //{
                        //  if (gs.areTwoCardsAdjacent(gs.gameGrid.theGrid[mouseOverCol][mouseOverRow],
                        //                           gs.gameGrid.theGrid[selectedSquareCol][selectedSquareRow]) && !moveJustMade)
                        //{
                        
                        //  ;
                        // case already handled above.
                        // This code is sort of in the wrong section.
                        /*
                         gameMove moveToMake;
                         moveToMake.col1 = selectedSquareCol;
                         moveToMake.row1 = selectedSquareRow;
                         moveToMake.col2 = mouseOverCol;
                         moveToMake.row2 = mouseOverRow;
                         if (!moveToMake.isMoveLegal(gs.gameGrid, gs.whoseTurn))
                         {
                         cout << "ERROR, invalid human move selected.";
                         (exit (0));
                         }
                         
                         // I haven't applied the game move!
                         
                         gs.applyMove(moveToMake);
                         
                         
                         
                         
                         // It is swapping the Sprites, but not updating their properties :).
                         
                         swapTemp = boardCards[moveToMake.col1][moveToMake.row1];
                         boardCards[moveToMake.col1][moveToMake.row1] = boardCards[moveToMake.col2][moveToMake.row2];
                         boardCards[moveToMake.col1][moveToMake.row1].setPosition({static_cast<float> (50.0*(moveToMake.row1)+10.0f),static_cast<float>(60.0*(moveToMake.col1)+30.0f)});
                         boardCards[moveToMake.col1][moveToMake.row1].setScale({1.0/8.0f,1.0/9.0f});
                         boardCards[moveToMake.col1][moveToMake.row1].setColor(sf::Color::White);
                         
                         boardCards[moveToMake.col2][moveToMake.row2] = swapTemp;
                         boardCards[moveToMake.col2][moveToMake.row2].setPosition({static_cast<float> (50.0*(moveToMake.row2)+10.0f),static_cast<float>(60.0*(moveToMake.col2)+30.0f)});
                         boardCards[moveToMake.col2][moveToMake.row2].setScale({1.0/8.0f,1.0/9.0f});
                         boardCards[moveToMake.col2][moveToMake.row2].setColor(sf::Color::White);
                         
                         
                         swapTemp = circleSprites[moveToMake.col1][moveToMake.row1];
                         circleSprites[moveToMake.col1][moveToMake.row1] = circleSprites[moveToMake.col2][moveToMake.row2];
                         circleSprites[moveToMake.col1][moveToMake.row1].setPosition({static_cast<float>(50.0*moveToMake.row1+5.0f) , static_cast<float>(60.0*moveToMake.col1+45.0f)});
                         circleSprites[moveToMake.col1][moveToMake.row1].setScale({1.0/11.0f,1.0/11.0f});
                         circleSprites[moveToMake.col1][moveToMake.row1].setColor(sf::Color::White);
                         
                         circleSprites[moveToMake.col2][moveToMake.row2] = swapTemp;
                         circleSprites[moveToMake.col2][moveToMake.row2].setPosition({static_cast<float>(50.0*moveToMake.row2+5.0f) , static_cast<float>(60.0*moveToMake.col2+45.0f)});
                         circleSprites[moveToMake.col2][moveToMake.row2].setScale({1.0/11.0f,1.0/11.0f});
                         circleSprites[moveToMake.col2][moveToMake.row2].setColor(sf::Color::White);
                         
                         
                         
                         isACurrentPlayerCardHighlightedInBlue = false;
                         isACurrentPlayerCardHighlightedInPink = false;  // at least just for a moment, turn off mouseover color
                         squareSelected = false;
                         selectedSquareCol = -1;
                         selectedSquareRow = -1;
                         mouseOver = false;                              // this will be reset in a moment if the mouse stays over a card
                         
                         moveJustMade = true;
                         gs.movementPointsTotal--;*/
                        //}
                        //else        // it was not a legal move, so, deselect the blue card
                        // add in this line of code
                        
                        /*else if (openToMouseSelectionChange == true)
                         {
                         isACurrentPlayerCardHighlightedInBlue = false;
                         isACurrentPlayerCardHighlightedInPink = true;
                         squareSelected = false;
                         selectedSquareCol = -1;
                         selectedSquareRow = -1;
                         }
                         else
                         {
                         ;
                         }*/
                        /*}
                         else
                         {
                         // WHAT CONDITION EXACTLY TRIGGERS THIS CODE??
                         
                         isACurrentPlayerCardHighlightedInBlue = false;
                         if (gs.gameGrid.theGrid[mouseOverCol][mouseOverRow].playerOwner == gs.whoseTurn)
                         {
                         isACurrentPlayerCardHighlightedInPink = true;
                         }
                         else
                         {
                         isACurrentPlayerCardHighlightedInPink = false;
                         }
                         squareSelected = false;
                         selectedSquareCol = -1;
                         selectedSquareRow = -1;
                         }*/
                        
                        //}
                        //else            // there was no selected card...until now.
                        //{
                        //   if (gs.gameGrid.theGrid[mouseOverCol][mouseOverRow].playerOwner == gs.whoseTurn)
                        //  {
                        //     isACurrentPlayerCardHighlightedInBlue = true;
                        //    isACurrentPlayerCardHighlightedInPink = false;
                        //   squareSelected = true;
                        //  selectedSquareCol = mouseOverCol;
                        // selectedSquareRow = mouseOverRow;
                        // }
                        // else            // it's someone else's card, and no card is selected
                        //           {
                        //             isACurrentPlayerCardHighlightedInBlue = false;
                        //           isACurrentPlayerCardHighlightedInPink = false;
                        //         squareSelected = false;
                        //       selectedSquareCol = -1;
                        //     selectedSquareRow = -1;
                        
                        // }
                        //}
                    }
                }
            } // the end of the window poll event, I believe.
            
            // SECTION 2:  GAME MECHANICS VARIABLES UPDATE AND MAINTENANCE.
            
            textToDisplay = "";
            
            textToDisplay += "Pinned Directions Reminder:  First adjacent capturing card rank Plus\nSecond adjacent capturing card rank must be less than or equal to Two\nPlus the Captured Card Rank for the capture to take effect.\n\n";
            
            
            
            if (gs.players[gs.whoseTurn-1].isHuman == false && waitForUpdate == false)
                // Ohhhh...this code is getting called *repeatedly*, before the screen updates, and it makes the
                // system try to make the same move twice. :)
            {
                waitForUpdate = true;
                moveToMake = getAImoveForOneMovementPoint(gs);
                
                skipButtonInitializedFlag = false;
                //dirButtonInitializedFlag = false;
                
                
                // run this code *before* you apply the move. :)
                if (!moveToMake.isMoveLegal(gs.gameGrid, gs.whoseTurn))     // the AI player selects no move and skips its turn
                {
                    //gs.displayGame();
                    //cout << "\n\nERROR, illegal AI move attempted.";
                    //moveToMake.displayMove();
                    //
                    //string inputLine = "";
                    //
                    //getline(cin, inputLine); // Grab whole line
                    
                    textToDisplay += "\nThe AI player (";
                    switch(gs.whoseTurn)
                    {
                        case 1:
                            textToDisplay += "Red";
                            break;
                        case 2:
                            textToDisplay += "Blue";
                            break;
                        case 3:
                            textToDisplay += "Green";
                            break;
                        case 4:
                            textToDisplay += "Gray";
                            break;
                        default:
                            //cout << "ERROR";
                            exit(0);
                    }
                    
                    
                    
                    textToDisplay += ") skips\nthe rest of its turn.\n";
                    
                    AIskippingTurn = true;
                    
                    
                    //cout << "qez";
                    gs.movementPointsTotal = 1;         // this will be subtracted by 1 to 0 soon
                    wasAIMoveLegal = false;
                }
                else
                {
                    AIskippingTurn = false;
                    wasAIMoveLegal = true;
                    gs.applyMove(moveToMake);
                    moveToMake.displayMove();
                    drawMovementArrowAndPause = true;
                }
                
                // don't do it twice! :)
                cardsToUpdate = gs.processCapturesAtEndOfTurn();
                
                cardJustCaptured = false;
                for (int i = 0; i < cardsToUpdate.size(); i++)
                {
                    cardJustCaptured = true;
                    textToDisplay += "\nPlayer ";
                    switch (gs.whoseTurn)
                    {
                        case 1:
                            textToDisplay += "Red";
                            break;
                        case 2:
                            textToDisplay += "Blue";
                            break;
                        case 3:
                            textToDisplay += "Green";
                            break;
                        case 4:
                            textToDisplay += "Gray";
                            break;
                        default:
                            //cout << "ERROR, unhandled case.";
                            exit(0);
                    }
                    textToDisplay += " has just captured the ";
                    switch(cardsToUpdate[i].rank)
                    {
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                        case 7:
                        case 8:
                        case 9:
                        case 10:
                            textToDisplay += to_string(cardsToUpdate[i].rank);
                            break;
                        case 11:
                            textToDisplay += "Jack";
                            break;
                        case 12:
                            textToDisplay += "Queen";
                            break;
                        case 13:
                            textToDisplay += "King";
                            break;
                        case 14:
                            textToDisplay += "Ace";
                            break;
                        default:
                            //cout << "ERROR, unhandled case.";
                            exit(0);
                    }
                    textToDisplay += " of ";
                    switch(cardsToUpdate[i].suit)
                    {
                        case 'c':
                            textToDisplay += "Clubs.\n";
                            break;
                        case 'd':
                            textToDisplay += "Diamonds.\n";
                            break;
                        case 'h':
                            textToDisplay += "Hearts.\n";
                            break;
                        case 's':
                            textToDisplay += "Spades.\n";
                            break;
                        default:
                            //cout << "ERROR, unhandled case.";
                            exit(0);
                    }
                }
                
                // redo the position to match the latest computer move
                if (!arrowTex.loadFromFile("./MovementArrow.png"))     // reload the texture to reset rotation
                {
                    //cout << "FILE LOAD ERROR";
                    exit(0);
                }
                
                arrow.setScale({1.0/6.0f,1.0/6.0f});
                
                // I figured this code out empirically...it seems the board is transposed, but it works out consistently so it's OK.
                if (moveToMake.row1 > moveToMake.row2)
                {
                    
                    arrow.rotate(sf::degrees(90));
                    
                    arrow.setPosition({static_cast<float> (50.0*(moveToMake.col1)+10.0+70.f),static_cast<float>(60.0*(moveToMake.row1)+30.0-20.f)});
                    
                }
                if (moveToMake.row2 > moveToMake.row1)
                {
                    arrow.rotate(sf::degrees(270));
                    
                    arrow.setPosition({static_cast<float> (50.0*(moveToMake.col1)+10.0-20.f),static_cast<float>(60.0*(moveToMake.row1)+30.0+95.0f)});
                    
                    
                }
                if (moveToMake.col1 > moveToMake.col2)
                {
                    
                    arrow.rotate(sf::degrees(0));
                    arrow.setPosition({static_cast<float> (50.0*(moveToMake.col1)+10.0-20.0f),static_cast<float>(60.0*(moveToMake.row1)+30.0-15.f)});
                }
                if (moveToMake.col2 > moveToMake.col1)
                {
                    arrow.rotate(sf::degrees(180));
                    
                    arrow.setPosition({static_cast<float> (50.0*(moveToMake.col1)+10.0+80.f),static_cast<float>(60.0*(moveToMake.row1)+30.0+75.f)});
                }
                
                
                
                
                for (int i = 0; i < cardsToUpdate.size() && wasAIMoveLegal; i++)
                {
                    //cout << "flag";
                    //gs.gameGrid.theGrid[cardsToUpdate[i].colLocation][cardsToUpdate[i].rowLocation].playerOwner = cardsToUpdate[i].playerOwner;
                    // the line of code above is not needed...processCaptures does it anyway.
                    
                    // this should make all the needed changes, in terms of changing player ownership based on capture
                    
                    // We only need to update the player owner circles.
                    
                    
                    
                    if (!RedTexture.loadFromFile("./RedCircle.png"))
                    {
                        //cout << "FILE LOAD ERROR";
                        exit(0);
                    }
                    
                    
                    if (!BlueTexture.loadFromFile("./BlueCircle.png"))
                    {
                        //cout << "FILE LOAD ERROR";
                        exit(0);
                    }
                    
                    
                    
                    if (!GreenTexture.loadFromFile("./GreenCircle.png"))
                    {
                        //cout << "FILE LOAD ERROR";
                        exit(0);
                    }
                    
                    if (!GrayTexture.loadFromFile("./GrayCircle.png"))
                    {
                        //cout << "FILE LOAD ERROR";
                        exit(0);
                    }
                    
                    sf::Sprite newRedSprite(RedTexture);
                    sf::Sprite newBlueSprite(BlueTexture);
                    sf::Sprite newGreenSprite(GreenTexture);
                    sf::Sprite newGraySprite(GrayTexture);
                    switch (cardsToUpdate[i].playerOwner)
                    {
                        case 1:
                            circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation] = newRedSprite;
                            break;
                        case 2:
                            circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation] = newBlueSprite;
                            break;
                        case 3:
                            circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation] = newGreenSprite;
                            break;
                        case 4:
                            circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation] = newGraySprite;
                            break;
                        default:
                            //cout << "ERROR, unhandled case.";
                            exit(0);
                    }
                    
                    
                    circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation].setPosition({static_cast<float>(50.0*cardsToUpdate[i].colLocation+5.0f) , static_cast<float>(60.0*cardsToUpdate[i].rowLocation+45.0f)});
                    circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation].setScale({1.0/11.0f,1.0/11.0f});
                    circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation].setColor(sf::Color::White);
                }
                // update board cards
                
                // pasted code from before.
                
                
                /*
                 swapTemp = boardCards[moveToMake.row1][moveToMake.col1];
                 boardCards[moveToMake.row1][moveToMake.col1] = boardCards[moveToMake.row2][moveToMake.col2];
                 boardCards[moveToMake.row1][moveToMake.col1].setPosition({static_cast<float> (50.0*(moveToMake.row1)+10.0f),static_cast<float>(60.0*(moveToMake.col1)+30.0f)});
                 boardCards[moveToMake.row1][moveToMake.col1].setScale({1.0/8.0f,1.0/9.0f});
                 boardCards[moveToMake.row1][moveToMake.col1].setColor(sf::Color::White);
                 
                 boardCards[moveToMake.row2][moveToMake.col2] = swapTemp;
                 boardCards[moveToMake.row2][moveToMake.col2].setPosition({static_cast<float> (50.0*(moveToMake.row2)+10.0f),static_cast<float>(60.0*(moveToMake.col2)+30.0f)});
                 boardCards[moveToMake.row2][moveToMake.col2].setScale({1.0/8.0f,1.0/9.0f});
                 boardCards[moveToMake.row2][moveToMake.col2].setColor(sf::Color::White);
                 
                 swapTemp = circleSprites[moveToMake.row1][moveToMake.col1];
                 circleSprites[moveToMake.row1][moveToMake.col1] = circleSprites[moveToMake.row2][moveToMake.col2];
                 circleSprites[moveToMake.row1][moveToMake.col1].setPosition({static_cast<float>(50.0*moveToMake.row1+5.0f) , static_cast<float>(60.0*moveToMake.col1+45.0f)});
                 circleSprites[moveToMake.row1][moveToMake.col1].setScale({1.0/11.0f,1.0/11.0f});
                 circleSprites[moveToMake.row1][moveToMake.col1].setColor(sf::Color::White);
                 
                 circleSprites[moveToMake.row2][moveToMake.col2] = swapTemp;
                 circleSprites[moveToMake.row2][moveToMake.col2].setPosition({static_cast<float>(50.0*moveToMake.row2+5.0f) , static_cast<float>(60.0*moveToMake.col2+45.0f)});
                 circleSprites[moveToMake.row2][moveToMake.col2].setScale({1.0/11.0f,1.0/11.0f});
                 circleSprites[moveToMake.row2][moveToMake.col2].setColor(sf::Color::White);
                 */
                
                // updated code, pasted from above
                //if (moveToMake.isMoveLegal(gs.gameGrid, gs.whoseTurn) == true)      // this should stop the segfault :).
                // don't ask if it's legal again, the move has already been applied. :)
                if (wasAIMoveLegal == true)
                {
                    
                    swapTemp = boardCards[moveToMake.row1][moveToMake.col1];
                    boardCards[moveToMake.row1][moveToMake.col1] = boardCards[moveToMake.row2][moveToMake.col2];
                    
                    // The line of commented out code below is from above, pasted for reference only.
                    // boardCards[i][j].setPosition({static_cast<float> (50.0*j+10.0f),static_cast<float>(60.0*i+30.0f)});
                    
                    boardCards[moveToMake.row1][moveToMake.col1].setPosition({static_cast<float>(50.0*(moveToMake.col1)+10.0f),static_cast<float> (60.0*(moveToMake.row1)+30.0f)});
                    boardCards[moveToMake.row1][moveToMake.col1].setScale({1.0/8.0f,1.0/9.0f});
                    boardCards[moveToMake.row1][moveToMake.col1].setColor(sf::Color::White);
                    
                    boardCards[moveToMake.row2][moveToMake.col2] = swapTemp;
                    boardCards[moveToMake.row2][moveToMake.col2].setPosition({static_cast<float>(50.0*(moveToMake.col2)+10.0f),static_cast<float> (60.0*(moveToMake.row2)+30.0f)});
                    boardCards[moveToMake.row2][moveToMake.col2].setScale({1.0/8.0f,1.0/9.0f});
                    boardCards[moveToMake.row2][moveToMake.col2].setColor(sf::Color::White);
                    
                    
                    // The line of code below is, too.
                    
                    // circleSprites[i][j].setPosition({static_cast<float>(50.0*j+5.0f) , static_cast<float>(60.0*i+45.0f)});
                    
                    // col actually does precede row in SetPosition statements
                    swapTemp = circleSprites[moveToMake.row1][moveToMake.col1];
                    circleSprites[moveToMake.row1][moveToMake.col1] = circleSprites[moveToMake.row2][moveToMake.col2];
                    circleSprites[moveToMake.row1][moveToMake.col1].setPosition({static_cast<float>(50.0*moveToMake.col1+5.0f), static_cast<float>(60.0*moveToMake.row1+45.0f)});
                    circleSprites[moveToMake.row1][moveToMake.col1].setScale({1.0/11.0f,1.0/11.0f});
                    circleSprites[moveToMake.row1][moveToMake.col1].setColor(sf::Color::White);
                    
                    circleSprites[moveToMake.row2][moveToMake.col2] = swapTemp;
                    circleSprites[moveToMake.row2][moveToMake.col2].setPosition({static_cast<float>(50.0*moveToMake.col2+5.0f), static_cast<float>(60.0*moveToMake.row2+45.0f)});
                    circleSprites[moveToMake.row2][moveToMake.col2].setScale({1.0/11.0f,1.0/11.0f});
                    circleSprites[moveToMake.row2][moveToMake.col2].setColor(sf::Color::White);
                }
                
                
                
                
                
                
                //sf::Sprite swapTemp = boardCards[moveToMake.col1][moveToMake.row1];
                //boardCards[moveToMake.col1][moveToMake.row1] = boardCards[moveToMake.col2][moveToMake.row2];
                //boardCards[moveToMake.col2][moveToMake.row2] = swapTemp;
                
                //swapTemp = boardCards[moveToMake.col1][moveToMake.row1];
                //circleSprites[moveToMake.col1][moveToMake.row1] = circleSprites[moveToMake.col2][moveToMake.row2];
                //circleSprites[moveToMake.col2][moveToMake.row2] = swapTemp;
                
                
                
                isACurrentPlayerCardHighlightedInBlue = false;
                isACurrentPlayerCardHighlightedInPink = false;  // at least just for a moment, turn off mouseover color
                squareSelected = false;
                selectedSquareCol = -1;
                selectedSquareRow = -1;
                //mouseOver = false;                              // this will persist because the AI turn lasts a while
                
                moveJustMade = true;
                gs.movementPointsTotal--;
                
            }
            
            
            
            
            if (moveJustMade && gs.players[gs.whoseTurn-1].isHuman == true)
            {
                vector <card> cardsToUpdate = gs.processCapturesAtEndOfTurn();
                
                
                
                cardJustCaptured = false;
                for (int i = 0; i < cardsToUpdate.size(); i++)
                {
                    cardJustCaptured = true;
                    textToDisplay += "\nPlayer ";
                    switch (gs.whoseTurn)
                    {
                        case 1:
                            textToDisplay += "Red";
                            break;
                        case 2:
                            textToDisplay += "Blue";
                            break;
                        case 3:
                            textToDisplay += "Green";
                            break;
                        case 4:
                            textToDisplay += "Gray";
                            break;
                        default:
                            //cout << "ERROR, unhandled case.";
                            exit(0);
                    }
                    textToDisplay += " has just captured the ";
                    switch(cardsToUpdate[i].rank)
                    {
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                        case 7:
                        case 8:
                        case 9:
                        case 10:
                            textToDisplay += to_string(cardsToUpdate[i].rank);
                            break;
                        case 11:
                            textToDisplay += "Jack";
                            break;
                        case 12:
                            textToDisplay += "Queen";
                            break;
                        case 13:
                            textToDisplay += "King";
                            break;
                        case 14:
                            textToDisplay += "Ace";
                            break;
                        default:
                            //cout << "ERROR, unhandled case.";
                            exit(0);
                    }
                    textToDisplay += " of ";
                    switch(cardsToUpdate[i].suit)
                    {
                        case 'c':
                            textToDisplay += "Clubs.\n";
                            break;
                        case 'd':
                            textToDisplay += "Diamonds.\n";
                            break;
                        case 'h':
                            textToDisplay += "Hearts.\n";
                            break;
                        case 's':
                            textToDisplay += "Spades.\n";
                            break;
                        default:
                            //cout << "ERROR, unhandled case.";
                            exit(0);
                    }
                }
                
                
                
                
                
                for (int i = 0; i < cardsToUpdate.size(); i++)
                {
                    //cout << "flag";
                    //gs.gameGrid.theGrid[cardsToUpdate[i].colLocation][cardsToUpdate[i].rowLocation].playerOwner = cardsToUpdate[i].playerOwner;
                    // the line of code above is not needed...processCaptures does it anyway.
                    
                    // this should make all the needed changes, in terms of changing player ownership based on capture
                    
                    // We only need to update the player owner circles.
                    
                    
                    
                    if (!RedTexture.loadFromFile("./RedCircle.png"))
                    {
                        //cout << "FILE LOAD ERROR";
                        exit(0);
                    }
                    
                    
                    if (!BlueTexture.loadFromFile("./BlueCircle.png"))
                    {
                        //cout << "FILE LOAD ERROR";
                        exit(0);
                    }
                    
                    
                    if (!GreenTexture.loadFromFile("./GreenCircle.png"))
                    {
                        //cout << "FILE LOAD ERROR";
                        exit(0);
                    }
                    
                    
                    if (!GrayTexture.loadFromFile("./GrayCircle.png"))
                    {
                        //cout << "FILE LOAD ERROR";
                        exit(0);
                    }
                    
                    
                    sf::Sprite newRedSprite(RedTexture);
                    sf::Sprite newBlueSprite(BlueTexture);
                    sf::Sprite newGreenSprite(GreenTexture);
                    sf::Sprite newGraySprite(GrayTexture);
                    switch (cardsToUpdate[i].playerOwner)
                    {
                        case 1:
                            circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation] = newRedSprite;
                            break;
                        case 2:
                            circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation] = newBlueSprite;
                            break;
                        case 3:
                            circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation] = newGreenSprite;
                            break;
                        case 4:
                            circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation] = newGraySprite;
                            break;
                        default:
                            //cout << "ERROR, unhandled case.";
                            exit(0);
                    }
                    
                    
                    
                    
                    
                    
                    
                    circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation].setPosition({static_cast<float>(50.0*cardsToUpdate[i].colLocation+5.0f) , static_cast<float>(60.0*cardsToUpdate[i].rowLocation+45.0f)});
                    circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation].setScale({1.0/11.0f,1.0/11.0f});
                    circleSprites[cardsToUpdate[i].rowLocation][cardsToUpdate[i].colLocation].setColor(sf::Color::White);
                    
                    
                    
                }
                moveJustMade = false;
            }
            
            if (gs.movementPointsTotal == 0)      // a turn has just ended
            {
                gs.whoseTurn++;
                gs.turnNum++;
                
                gs.addPointsBasedOnBaseSquares();
                
                
                if (gs.whoseTurn == 5)
                {
                    gs.whoseTurn = 1;
                }
                gs.movementPointsTotal = -1;
                
            }
            
            if (gs.movementPointsTotal <= -1)           // a new turn has just started...possibly after the previous code block ended a turn.
            {
                dice pairOfDice;
                gs.movementPointsTotal = pairOfDice.roll();
            }
            else
            {
                //  textToDisplay = "";     // Do this earlier, so the capture card text can be shown without resetting.
                textToDisplay += "Turn #" + to_string(gs.turnNum);
                textToDisplay += "\n\nIt is currently ";
                switch (gs.whoseTurn)
                {
                    case 1:
                        textToDisplay += "Red";
                        break;
                    case 2:
                        textToDisplay += "Blue";
                        break;
                    case 3:
                        textToDisplay += "Green";
                        break;
                    case 4:
                        textToDisplay += "Gray";
                        break;
                    default:
                        //cout << "ERROR, unhandled whoseturn";
                        exit(0);
                }
                textToDisplay += "'s turn.\nThe player has " + to_string(gs.movementPointsTotal) + " moves left to make before\nthat player's turn ends.\n\n";
                textToDisplay += "Scoreboard:\n\nRED:\t" + to_string(gs.players[0].numPoints) + "\nBLUE:\t" +
                to_string(gs.players[1].numPoints) + "\nGREEN:\t" + to_string(gs.players[2].numPoints) + "\nGRAY:\t"
                + to_string(gs.players[3].numPoints);
                
            }
            
            // Do this part last--overwrite the empty 4x4 "home base" squares.
            
            
            
            
            
            
            
            
            // SECTION 3:  GAME SCREEN DISPLAY UPDATE.
            
            
            window.clear(sf::Color(80,80,80));
            
            
            
            
            
            
            
            
            for (int i = 0; i < 12; i++)
            {
                for (int j = 0; j < 12; j++)
                {
                    if (isACurrentPlayerCardHighlightedInBlue && squareSelected)
                    {
                        boardCards[selectedSquareRow][selectedSquareCol].setColor(sf::Color(255,2,141));
                    }
                    if (isACurrentPlayerCardHighlightedInPink && !isACurrentPlayerCardHighlightedInBlue)
                    {
                        boardCards[mouseOverRow][mouseOverCol].setColor(sf::Color(1,101,255));
                    }
                    else if (isACurrentPlayerCardHighlightedInPink)
                    {
                        if (selectedSquareRow == mouseOverRow && selectedSquareCol == mouseOverCol)
                        {
                            ;           // do nothing; do not change the selected blue card to pink, even due to mouseover
                        }
                        else
                        {
                            boardCards[mouseOverRow][mouseOverCol].setColor(sf::Color(1,101,255));
                        }
                    }
                    if ((i != selectedSquareRow || j != selectedSquareCol) && (i != mouseOverRow || j != mouseOverCol))
                    {
                        boardCards[i][j].setColor(sf::Color::White);
                    }
                    else
                    {
                        if (!isACurrentPlayerCardHighlightedInBlue && !isACurrentPlayerCardHighlightedInPink)
                        {
                            boardCards[i][j].setColor(sf::Color::White);
                        }
                    }
                    
                    if (gs.gameGrid.theGrid[i][j].rank != 0 ||
                        (!(i < 4 && j < 4) &&
                         !(i > 7 && j < 4) &&
                         !(i < 4 && j > 7) &&
                         !(i > 7 & j > 7))
                       )
                    {
                        window.draw(boardCards[i][j]);
                    }
                    else
                    {
                        sf::Texture quickTex;
                        if (i < 4 && j < 4)
                        {
                            if (!quickTex.loadFromFile("./ClearRectangleRedHome.png"))
                            {
                                //cout << "FILE LOAD ERROR";
                                exit(0);
                            }
                        }
                        if (i > 7 && j < 4)
                        {
                            if (!quickTex.loadFromFile("./ClearRectangleGrayHome.png"))
                            {
                                //cout << "FILE LOAD ERROR";
                                exit(0);
                            }
                        }
                        if (i < 4 && j > 7)
                        {
                            if (!quickTex.loadFromFile("./ClearRectangleBlueHome.png"))
                            {
                                //cout << "FILE LOAD ERROR";
                                exit(0);
                            }
                        }
                        if (i > 7 && j > 7)
                        {
                            if (!quickTex.loadFromFile("./ClearRectangleGreenHome.png"))
                            {
                                //cout << "FILE LOAD ERROR";
                                exit(0);
                            }
                        }
                        sf::Sprite quickLastMinuteSprite(quickTex);
                        quickLastMinuteSprite.setPosition(boardCards[i][j].getPosition());
                        quickLastMinuteSprite.setScale(boardCards[i][j].getScale());
                        quickLastMinuteSprite.setColor(boardCards[i][j].getColor());
                        
                        window.draw(quickLastMinuteSprite);
                    }
                }
            }
            
            for (int i = 0; i < 12; i++)
            {
                for (int j = 0; j < 12; j++)
                {
                    window.draw(circleSprites[i][j]);
                }
            }
            
            sf::Font font("NK57 Monospace Cd Rg It.otf");
            sf::Text textString(font);
            textString.setString(textToDisplay);
            textString.setScale({2.0/3.0f,2.0/3.0f});
            textString.setPosition({750,100});
            textString.setFillColor(sf::Color(7,148,104));
            
            
            if (gs.players[gs.whoseTurn-1].isHuman == true && skipButtonInitializedFlag == true)
            {
                window.draw(skipButtonSprite);
            }
            
            
            window.draw(textString);
            if (AIskippingTurn)
            {
                this_thread::sleep_for(chrono::milliseconds(3000));
                AIskippingTurn = true;
            }
            
            //vector <sf::Sprite> flashingCircles;
            //vector <sf::Texture> flashcircletextures;
            
            
            
            /*if (cardJustCaptured > 0)
            {
                cardJustCaptured--;
                flashFlag = !flashFlag;
                for (int i = 0; i < 10; i++)
                {
                    if (flashFlag == true)
                    {
                        for (int j = 0; j < cardsToUpdate.size(); j++)
                        {
                            flashcircletextures.resize(flashcircletextures.size()+1);
                            if (!flashcircletextures[flashcircletextures.size()-1].loadFromFile("YellowCircle.png"))
                            {
                                cout << "ERROR, file wasn't loaded.";
                                exit(0);
                            }
                            flashingCircles.emplace_back(flashcircletextures[j]);
                            flashingCircles[flashingCircles.size()-1].setPosition({static_cast<float>(50.0*cardsToUpdate[j].colLocation+5.0f),
                                                        static_cast<float>(60.0*cardsToUpdate[j].rowLocation+45.0f)});
                            flashingCircles[flashingCircles.size()-1].setScale({1.0/11.0f,1.0/11.0f});
                        }
                    }
                }
            }*/
            /*if (flashFlag == true)
            {
                for (int i = 0; i < flashingCircles.size(); i++)
                {
                    window.draw(flashingCircles[i]);
                    cout << "good";
                    this_thread::sleep_for(chrono::milliseconds(100));
                }
            }*/
            
            if (drawMovementArrowAndPause == true)
            {
                window.draw(arrow);
                this_thread::sleep_for(chrono::milliseconds(500));
            }
            
            
            window.display();
            waitForUpdate = false;
            
            if (cardJustCaptured == true)
            {
                cardJustCaptured = false;
                this_thread::sleep_for(chrono::milliseconds(2000));
            }

            
        }
        else // if gameJustLoaded == 4
        {
            while (const std::optional event = window.pollEvent())
            {
                // "close requested" event: we close the window
                if (event->is<sf::Event::Closed>())
                    window.close();
                
                
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    clickOn = true;
                    
                }
                else
                {
                    clickOn = false;
                }
            }
            
            string theString;
            if (winners.size() == 1)
            {
                theString = "The game is finished.  The winner is:  ";
                switch(winners[0])
                {
                    case 1:
                        theString += "Red";
                        break;
                    case 2:
                        theString += "Blue";
                        break;
                    case 3:
                        theString += "Green";
                        break;
                    case 4:
                        theString += "Gray";
                        break;
                    default:
                        //cout << "ERROR, unhandled case.";
                        exit(0);
                }
                
                
                theString += ".  Congratulations!\n\n";
                
                
                bool wonBy1000Points = true;
                for (int i = 0; i < 4; i++)
                {
                    if (i+1 != winners[0] && gs.players[winners[0]-1].numPoints - gs.players[i].numPoints >= 1000)
                    {
                        ;
                    }
                    else
                    {
                        wonBy1000Points = false;
                    }
                }
                
                
                if (wonBy1000Points == true)        // I haven't really tested this code block, it's hard to get to.
                {
                    theString += "Additionally, the winning player did such a great job that he/she won\nthe 1000 points victory margin challenge, defeating all\nother players by 1000 points or more!  FANTASTIC WORK, you have mastered this game!\n\n";
                }
                
                
                
                theString += "Click anywhere to quit the game.";
                
                
                
            }
            else
            {
                theString = "The game is finished.  It is a tie.  The winners are:  ";
                for (int i = 0; i < winners.size(); i++)
                {
                    switch (winners[i])
                    {
                        case 1:
                            theString += "Red\n";
                            break;
                        case 2:
                            theString += "Blue\n";
                            break;
                        case 3:
                            theString += "Green\n";
                            break;
                        case 4:
                            theString += "Gray\n";
                            break;
                        default:
                            //cout << "ERROR, unhandled case.";
                            exit(0);
                    }
                }
                theString += ".  Congratulations!\n\nClick anywhere to quit the game.";
            }
        
            sf::Font font("NK57 Monospace Cd Rg It.otf");
            sf::Text finalText(font);
            finalText.setString(theString);
            finalText.setFillColor(sf::Color(0,255,0));
            finalText.setPosition({200,100});
            
            window.clear(sf::Color::Black);
            window.draw(finalText);
            
            window.display();
            
            
            if (clickOn)
            {
                window.close();
            }
            
            
        }
    }
    
    
    return 0;
}




/*
void updateBoardCards(gameState gs,vector < vector <sf::Sprite> > oldBoard, graphicsBundle & gb)
{
    //vector < vector <sf::Sprite > > toReturn;
    //vector < vector <sf::Texture > > finalTextures;
    // sf::Texture texture1;
     
    //gb.gridOfSquares.resize(0);
    gb.gridOfSquares.resize(12);
    //for (int i = 0; i < 12; i++)
    //{
    //    gb.gridOfSquares[i].clear();
    //    // It's declaring sf::Sprite instances when this vector is run through.
    //}
    //gb.texturesForGrid.resize(0);
    gb.texturesForGrid.resize(12);
    for (int i = 0; i < 12; i++)
    {
        gb.texturesForGrid[i].clear();
    }
    
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            string filename = getCardFileName(i, j, gs.gameGrid.theGrid);
            
            if (filename == "BlankOfBlank.png")
            {
                filename = "ClearRectangle.png";
            }
            gb.texturesForGrid[i].resize(gb.texturesForGrid[i].size()+1);
            if (gb.texturesForGrid[i][j].loadFromFile(filename))
            {
                ;
            }
            else
            {
                cout << "ERROR";
                exit(0);
            }

            //gb.texturesForGrid[i].push_back(texture1);

            //sf::Sprite sprite1(gb.texturesForGrid[i][j]);
            gb.gridOfSquares[i].emplace_back(gb.texturesForGrid[i][j]);
            gb.gridOfSquares[i][j].setPosition({static_cast<float> (50.0*i+10.0f),static_cast<float>(60.0*j+30.0f)});
            gb.gridOfSquares[i][j].setScale({1.0/9.0f,1.0/9.0f});



            //gb.gridOfSquares[i].push_back(sprite1);
            
        }
    }
    

    
    
    return;
}       // it doesn't update the circle sprites yet
*/

// Don't even use a function!

//void updateBoardCircles (gameState gs,vector < vector <sf::Sprite> > oldBoard, graphicsBundle & gb)
// Don't re-do the whole 2d vector...just detect the one sprite change and swap those two sprites.
//{
        // Google says:  Don't even *declare* the local texture variable.
    /*
    vector < vector <sf::Sprite > > toReturn;
    vector < vector <sf::Texture > > finalTextures;
    
    toReturn.resize(12); */
    //gb.gridOfSquares.resize(0);
    
    //gb.gridOfSquares.resize(12);      // this causes the class that stores all the data to have its data cleared out and rest
    //for (int i = 0; i < 12; i++)
    //{
    //    gb.gridOfSquares[i].clear();      // We can clear out the data here.
    //}
    //gb.texturesForGrid.resize(0);
    /*gb.texturesForGrid.resize(12);
    for (int i = 0; i < 12; i++)
    {
        gb.texturesForGrid[i].clear();
    }
    
    string filename;
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            switch (gs.gameGrid.theGrid[i][j].playerOwner)
            {
                case 0:
                    filename = "ClearCircle.png";
                    break;
                case 1:
                    filename = "RedCircle.png";
                    break;
                case 2:
                    filename = "BlueCircle.png";
                    break;
                case 3:
                    filename = "GreenCircle.png";
                    break;
                case 4:
                    filename = "GrayCircle.png";
                    break;
                default:
                    cout << "ERROR, unhandled case";
                    
            }
            
            gb.texturesForGrid[i].resize(gb.texturesForGrid[i].size()+1);
            if (gb.texturesForGrid[i][j].loadFromFile(filename))
            {
                ;
            }
            else
            {
                exit(0);
            }
            
            //gb.texturesForGrid[i].push_back(texture1);      // I think we are replacing the textures, which auto updates the sprites.
            // don't declare a Sprite like this.
            //sf::Sprite sprite1(gb.texturesForGrid[i][j]);
            
            gb.gridOfSquares[i].emplace_back(gb.texturesForGrid[i][j]);     // we *do* have to use emplace_back
            gb.gridOfSquares[i][j].setPosition({static_cast<float>(50.0*i+5.0f) , static_cast<float>(60.0*j+45.0f)});
            gb.gridOfSquares[i][j].setScale({1.0/9.0f,1.0/9.0f});
            
            //gb.gridOfSquares[i].push_back(sprite1);
            
        }
    }
    
    
    
    // gb.gridOfSquares = toReturn;
    // gb.texturesForGrid -- already set and handled by the preceding code
    

    
    return;
*/
    
//}



// THE KEY TO FIXING THIS:  JUST DON'T LET YOUR TEXTURES *BE IN A LOCAL SCOPE*.  DECLARE *ALL* OF YOUR TEXTURES AT THE *TOP* OF YOUR FILE.  DO
// *NOT* USE TEXTURES IN A FUNCTION, KEEP IT ALL IN MAIN.

// ALSO, DON'T LET YOUR SPRITES BE LOCAL EITHER!  IF YOU WANT TO DISPLAY THE SPRITE, THE SPRITE NEEDS TO "LIVE," TOO. :)



// I think what it's doing is:  Making moves and updating them.




// I think I figured out the issue.  The issue is that the blue AI player has been making moves, but I'm not seeing them, and then the game grid
// somehow isn't updated.  The moves are being made but not displayed.




// The big "show stopper" problem I was facing was, I wasn't waiting for the update after the game move was made.  Also, I tested the legality of
// the move *after* it had already been made and the grid was updated...so it thought I was trying to make the same move again.  Just a silly error.

// Now, the captures of cards isn't working.



// That was it!  I changed the value to make the AI "a little dumber"--j = i+1, I got that from ChatGPT--and it runs *much* faster!



//-----------THIS IS THE TO DO LIST FOR THE REST OF THE GAME---------


// 11-06, 8:15 p.m.:  The game is practically done! :)  I just need to add in the points, and the ability to choose which players are going to be human, and a
// nice introduction screen.  I should fix up the graphics, too; go for pretty, bright colors.
// I'm glad I was able to get over the scare...the answer was literally just that I was "trying to do the same thing twice"...I was trying to
// check if the move was legal after the computer already made it and the board had been updated.  It was no longer available as a move.

// Success!  Version 4.0 is right, 5.0 is not good, it had like 19 errors...this version is much better :).

// Work more tomorrow!  This might be a commercial flop.  At least it'll be a marketing learning experience...as long as itch.io lets me post it
// and charge $$ for it, I should "have the product up and available," and then the challenge will be to market it! #1 (DONE! :D) Also, fix the clicking issue
// for the human players, and #2 (DONE) make the computer moves more visible when they happen...add #3 some colors (DONE) and a #4 text bar (SKIPPED...I did the arrow) notification.  You could
// include #5 (DONE) a nice arrow.

// I caught a silly typo:  if (move1.isMoveLegal(game.gameGrid, game.whoseTurn)) ...should be, "if (move3.isMoveLegal...". :)

// Also:  #6 (DONE) The green player is moving to the wrong corner.  Probably, the whole corners thing needs to be re-done.

// Also:  #7 (SKIPPED) Aid readability of the cards by having a bigger version of each card that is impacted on the right-hand sidebar.  Have up to 3 cards...
// you'll have three cards when a capture is processed.  You can take your time to do the captures.  You might want to worry about multiple
// captures per turn.

// I'm *so glad* I got over the "huge roadblock."  :)  It's OK, maybe even preferable, if the AI is a little bit dumb.
    
// Also #8 (DONE) fix the availability of AI human players; allow the user to select which players will be human at the beginning of the turn.




// And, #9, give users the option to enter their own max screen resolution to fix problems if my program doesn't guess their resolution right.

// 11-7:  It's going great!  The faster geeksforgeeks Dijkstra program helped a lot.  #10:  (DONE, AI can skip, and I made the human player a separate issue in this list.)  Allow the AI player (and the human player) to
// skip its turn if no good moves are available (or whenever the AI/human player wants).

// 11-9:  #11 Fully debug AI.  (DONE. :D) #12 Get rid of extraneous couts in the console window.

// 11-10:  #13, (SKIPPED--too hard, and the machine isn't fast enough I think)  Add in *flashing colored circles* that oscillate between the old & new color when a card is captured.

// 11-12:  #14, (DONE) fix "best card to move home" function and re-incorporate moving home into the AI.  #15:  (DONE) Allow the AI player to skip its
//      turn if it can't find good moves.

// 11-12, 10:46 a.m.:  #16: (DONE)  Add a nice title-page screen, with a big photo of the 2 of diamonds with a blue circle on it.

// 11:27 a.m.:  #17:  (DONE) Re-include the different colored "home bases."  (12:13 p.m.) And:  Get the home bases coordinated with the correct
//      color player...upper left is red, upper right is blue, lower right is green, lower left is yellow. (SECOND PART, DONE.)

// 11-7 8:20 p.m.: (DONE--60% move home chance) Have the AI player choose a move that promotes capturing, rather than moving a card home, 3/5 of the time at least.
// Do two coinflips:  One to choose which kind of move, then, if you chose capturing, choose one or the other move (50-50).

// 11-12, 12:54 p.m.: #18 (SKIPPED), possibly add in graphics for dice to show the dice roll.

// 11-12, 9:11 p.m.:  #19: (DUPLICATE) Include a directions screen that also gives the credits for the game.  Discuss the fact that turn points
//      come from dice.  #20.(DONE)   Make it so that the first player to get 2000 points wins.  #21:  (DONE) Allow human player to skip turn.

// 11-13, 12:55 p.m.:  Next, do the credits, directions, and title page.  Put a gray-owned 2 of clubs as the illustration for the title
//      screen.





// 11-7, 10:37 a.m.:  Possible ways to speed this up:  Use a "min priority queue" in Dijkstra's algorithm, and, use an *adjacencny list* rather
// than a full matrix, i.e., list the edges in a list, that should take *much less space* than a full matrix, at least I think so.

// It looks like I am already using an adjacency list, i.e., an edges list.

// Try to implement the priority queue...it's worth it.


// 11-7, 12:47 p.m.:  Whoa!  It's so much faster with the geeksforgeeks Dijsktra code!  Awesome!
// The moves look dumber, though.


// 9,12,13,16,17,19,20,21:  Only 8 issues left to resolve. :).



// The AI is not working any more.  What did I change from the prototype version?  The Dijkstra implementation might have an issue, maybe
// I am still mis-using it.  I loaded the size of the vertices instead of the size of the edges one time, or something like that.






// 11-7-2025:  Resolved:  I need to code my own Dijkstra algorithm, and it needs to be faster.  I need to figure out Dijkstra.  It is OK
// if it takes a few weeks.  The Google Dijkstra code is defective, appparently.

// Update:  Just kidding.  I get Dijkstra conceptually well enough.  The GeeksForGeeks code worked well enough, the AI code was not // reliable.  That is a very good learning experience...you can trust human-generated published code that generates clicks and impressions on the internet, *not* AI generated code at this time.  The paradox might be...which coder, even if he/she could, would want to build highly sophisticated software to make him/herself obsolete?  It's not exactly a prisoner's dilemma!



// 11-7, 8:20 p.m.:  I think I actually get what it feels like to play the game.  It's about as fun as Minesweeper...pretty good :).


// 11-9, 1:04 p.m.:  I think sometimes the AI just gets torn between two options, and so that's why it moves the card back and forth.
// I think the AI is mostly fine, except I need to fix the corners moving, and, increase the odds of making a capture move as opposed to a
// card-move-to-scoring points move.

// 11-9, 3:21 p.m.:  Whew!  That was scary.  I finally fixed it...and *without* using the time library.  It's just, require ClickOn to be
// false when you run the loop to set it to be true. :). :D.  Next up:  Tackle #7.


// 3:57 p.m., 11-9:  The clicking is messed up again.  I'll fix it later.


// 3:21 p.m.:  Stick with version 7.  Don't spend any more time on Dijsktra...focus on the rest of the AI.  Look for bugs in "findclosesttwo..."
//   function.


// 4:36 p.m.:  I think the source of the recent glitch is improper use of OpenMP.  I checked on Google.


// 11-11, 4:52 P.M.OK, I removed all of the pragma code.  It doens't run much slower :(, but, it still has the error with the illegal AI move.
// Try asking Google if there's a problem with the FindClosestTwo... function.

// 5:01 p.m.:  Aha...this time the game was actually right, it was correct to say there were no more legal moves.  It could be that the AI is
// working perfectly! :).  Check again later.  If it passes 10 tests--stopping due to an illegal move, and it passes if there were no legal moves--
// then consider it working right :) :D.

// I think it's a problem with the "one card is adjacent" AI.

// This time the AI was correct; it was a *new player's turn*, you have to watch for that.

// I bet it works perfectly.  Make sure you know if the whoseturn variable just changed!  I'll check again later.

// 5:38 p.m.:  Test 1 passed.  It stopped when Gray had only one card left, and thus no ability to do captures.  Blue won!
// 5:43 p.m.:  Test 2 failed.  I took a screenshot.  Back to prototype.
// 6:36 p.m.:  Test it with *no* pragma omp things.
// 6:37 p.m.:  Test 1 passed.  Gray legitimately had no good moves left.  Test 2 passed, gray had no good moves.  Test 3 passed, red had no good moves.  Test 4 passed, blue had no good moves.  Test 5


// 6:58 p.m.:  I don't get it.  Test more later!

// 8:57 p.m.:  !!!  One of my evaluations was wrong.  Remember, the capture only works if cc1.rank+cc2.rank <= y.rank + 2!!!
// One of my evaluations was right, though.


// 10:05 p.m.:  I think I see the problem.  The "found moves" variable gets reset to 0 every time we go through the loop, but it's
//      nested for loops, so we need to preserve foundmoves's value when we repeat the loop...don't reset to 0.



// 10:39 p.m.:  I think the issue was, I was not checking the cc1 shortest paths to see if they were -1/infinity.
// 10:42 p.m.:  I think I might have fixed everything!! :D.
// Test 1 passed.  Do all other 9 tests tomorrow. :)
// 11-12, 8:44 a.m.:  Test 2 passed.  Red had no good moves.  (Also, I checked the code that previously didn't work right in int main,
//   and it worked fine this time. :)
// Test 3 passed.  Test 4 passed.  (Re-instated pragma omp for Dijkstra, and, fixed some code relating to testing for infinity.)  Test
//      5 passed.
// Sometimes it still goes "back and forth" with one card...but I think that's just "being torn between two equally good options."
// Test 6--failed.

// The failure:  If the two cards are already in capturing position at the beginning of the turn.  I.e., they should be able to capture
//  without moving at all.  It might be the other reason I mentioned before, though...test for infinity.
// Actually, I think the issue is probably pragma omp.  I put pragma omp back in...I'd better take it out, I don't really know what I'm
//      doing with that.
// Re-starting tests, with no pragma omp:

// Test 1:  Passed.  Test 2:  Passed.  Test 3:  Passed.  Test 4:  Passed.  Test 5:  Passed, it's actually correct.  I took a screenshot.
//      One card blocks the other useful card from moving, so it is correct to say there are no "good" legal moves.  Test 6:  Passed.
//      Test 7:  Passed.  Test 8:  Passed.  Test 9:  FAILED, 6 and 9 can take a King.  Gray had moves.
// 9:29 a.m.:  Restarting tests.  No pragma omp right now...if this test is successful, test with pragma omp reincluded.
// Test 1:  Failed.
// It's trying to move from an empty square.

// 9:41 a.m., restarting tests soon.

//10:05 a.m. .  Test 1 passed.  Some oscillating cards; just treat it as normal for now.  Test 2 passed.  Test 3 passed.  Test 4 passed.
//      Test 5 passed. Test 6 passed.  Test 7 passed. :).  Test 8, failed.

// Possible explanation for card oscillation:  It is trying to capture a card, in tandem with another card, but they are trying to land to
//      the same adjacent square to that card, because it's the only one.  When one card actually gets adjacent to the capturing card,
//      the card is blocked, and the opportunity disappears, causing the card that just became adjacent to be "freed up" and ready to go
//      after another capture opportunity, elsewhere on the board, and it starts moving towards that sometimes.

//10:42 a.m.:  I changed the sourcetotargetdistance function, I think using the nodeIDs vector was a mistake.  Test 1:  Passed.  Test 2:
//      Passed.  Test 3:  Passed.  Test 4:  Passed.  Test 5:  Passed.  Test 6:  Passed.  Test 7:  Passed.  Test 8:  Passed.  Test 9:
//      Passed.  Test 10:  Passed.  Great!  All 10 tests were a success.  I bet changing the "getshortestpathtotargetfromsource" function
//      change was critical, the nodeIDs vector probably didn't line up perfectly.  Now:  Do 10 tests with pragma omp restored.

// Test 1 pragma:  FAILED.  Yikes.  Let's turn pragma off then!

// Sometimes, the oscillation occurs due to a *capture*, i.e., the card captures another card, then turns back.
// On the card position oscillation:  Sometimes, a card moves adjacent to a card it can capture, but then that *blocks* the closest square
//      of the second "partner card" that was going to go capture it, thereby making the shortest path to that capture much longer.  Then,
//      there is a better opportunity, and the initially moving card might move back to its old square to pursue it.  The key is, note if
//      an oscillating card *might have been blocking* another card on the same team from capturing a target card.
// Yes, sometimes a card moving in a particular way might *block access* of another card to that card.
// 11:16 a.m.:  :( It failed later.
// Hmmm...the shortest path algorithm is coming up wrong.  How strange.

// 11:29 a.m..  Test battery round 2.  Do 25 tests this time.  Test 1:  Passed.
// 11:41 a.m.:  I found an extraneous index++ in one of my loops.  That was probably causing all the problems! :).
//      I'm going to restore the pragma code in Dijkstra, and do 25 tests.
// I will test it after lunch. :)  The code runs slower when you don't skip much of the loop, btw.  I might want to put pragma back in
//      several locations.
// Actually, just leave it.  One use of pragma is enough.


// 11-12, 12:15 p.m..  Test 1/25:  Passed.  Test 2:  Passed.  Test 3:  Passed.  Test 4:  Passed.  Test 5:  Passed.  Test 6:
//      Passed.  Test 7:  Passed.  Test 8:  Passed.  Test 9:  Passed.  Test 10:  Passed.  Test 11:  Passed.  Test 12:  Passed.
//      Test 13:  Passed.  (Sometimes the last move is a capture, but you didn't see the capture.)  Test 14:  Passed.  Test 15:
//      Passed.  Test 16:  Passed.  Test 17:  Passed.  Test 18:  Passed.  Test 19:  Passed.  Test 20:  Passed.  Test 21:  Passed.
//      Test 22:  Passed.  Test 23:  Passed.  Test 24:  Passed.  Test 25:  Passed.  All tests passed! :D.


// The phenomenon is:  Sometimes, a card rushing to its capture square, blocks the capture-partner, eliminating that opportunity and
//      causing a different priority for that card to draw it in a different direction.  It's not an AI bug.
// Sometimes, a card will block one of the "docking stations" for a card to be captured, so, the other "docking stations," i.e.,
//      adjacent squares need to be accessed by possibly the same pair of cards, but the target card is accessed at one or more
//      different places.





// 11-13, 12:24 p.m.  This code removed for now:

// Edition of the home base squares.
  /*  for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (gs.gameGrid.theGrid[i][j].rank == 0)
            {
                sf::Sprite tempSprite
                sf::Texture tempTex;
                if (i < 4 && j < 4)
                {
                    if (!tempTex.loadFromFile("ClearRectangleRedHome.png"))
                    {
                        cout << "ERROR, file failed to load.";
                        exit(0);
                    }
                    boardCardTextures[i][j].update(tempTex);
                }
                else if (i < 4 && j > 7)
                {
                    if (!tempTex.loadFromFile("ClearRectangleGrayHome.png"))
                    {
                        cout << "ERROR, file failed to load.";
                        exit(0);
                    }
                    tempSpriteupdate(tempTex);
                }
                else if (i > 7 && j < 4)
                {
                    if (!tempTex.loadFromFile("ClearRectangleBlueHome.png"))
                    {
                        cout << "ERROR, file failed to load.";
                        exit(0);
                    }
                    boardCardTextures[i][j].update(tempTex);
                }
                else if (i > 7 && j > 7)
                {
                    if (!tempTex.loadFromFile("ClearRectangleGreenHome.png"))
                    {
                        cout << "ERROR, file failed to load.";
                        exit(0);
                    }
                    boardCardTextures[i][j].update(tempTex);
                }
                else
                {
                    if (!tempTex.loadFromFile("ClearRectangle.png"))        // regular blank square
                    {
                        cout << "ERROR, file failed to load.";
                        exit(0);
                    }
                    boardCardTextures[i][j].update(tempTex);
                }
            }
        }
    }*/
// Update the 4x4 corner blank squares and the non-4x4 blank squares--load the right sprite.

// The problem with the code above is, a)  texture updating appears to be very slow--slower than creating a new sprite with a texture.
// Also, it doesn't make sense to update the texture of the existing sprite, because we are going to later *swap* it with the card that
// changes places with it by moving to the blank square.  What we need is a last-minute super-imposed new sprite that covers up the
// normal blank square sprite.  One good idea is, don't even create a vector...just do it on the fly in the "draw cards" for loop.




// 11-13-2025, 3:32 p.m.
// Remaining tasks:
// #9, give users the option to enter their own max screen resolution to fix problems if my program doesn't guess their resolution right.
// #12 Get rid of extraneous couts in the console window.
// #16:  (DONE) Add a nice title-page screen, with a big photo of the 2 of diamonds with a blue circle on it. + credits + how to play
//      directions + explanation of movement points coming from dice.
// #20.  (DONE) Make it so that the first player to get 2000 points wins.
// #21:  (DONE) Allow human player to skip turn.


// Schedule for completion:

// Do them in this order:  #16, #20, #21, #9 (rather hard), #12

// Goal:  Finish within the next 3 days.

// Friday:  #16 (DONE), #20 (DONE), #21 (DONE)
// Saturday:  #9
// Sunday:  #12 (easy at the end)

// Take Dr. Fowler's suggestion...beta test the game before you release it for money.


/* Text for directions, title screen and credits:
 
 TITLE/CREDITS SCREEN:
 
 Playing Cards Fight
 a game by Philip J. White
 Copyright 2025, Philip J. White
 All rights reserved.
 Please do not pirate this game.  It costs only $3.00 to purchase.
 
 (large image of gray-owned 2 of diamonds)
 
 Credits:
 
 Graphics.....Playing card graphics designed by Freepik.  Graphics used based on on-site written permission for use.  Check out freepik.com.
 C++ Programming....Philip J. White
 Game Development....Philip J. White
 Graphics......SFML (Simple and Fast Multimedia Library), check out https://www.sfml-dev.org
 Development Environment......Xcode and MacBook Air
 Other tools used............Open MP (for code parallelization)
 

 
 
 DIRECTIONS SCREEN:
 
 Playing Cards Fight is a game about capturing material--other players' cards--and bringing it home to home base to score points.  The players proceed from red, to blue, to green, to gray, making moves, one square at a time, horizontally and vertically.  Each card
     moves to an empty square.
 
 Of critical importance, whenever two cards of a particular rank are such that their ranks sum to a certain value and they are both adjacent to an opponent's card where the opponent's card is at least 2 greater than the aforementioned sum of two cards, then the opponent's card will be captured by the two capturing cards, and the capturing player will be able to move that card.  For example, if I am player Blue and I move the 4 of clubs and the 8 of diamonds, and both of these cards are owned by player Blue (ownership is portrayed based on a circle at the bottom left of the card), and I move both of those cards next to the King of Diamonds, owned by Player Gray, then I will capture the King of Diamonds, because 4 + 8 < 13 + 2.  (The rank of each card is, for 2-10, the number on the card, and, 11 for jack, 12 for queen, 13 for king, and 14 for ace.  The ace does not have a rank of 1 in this game.)
 
 The two main activities in the game are trying to capture opponent cards, as described above, and trying to move cards you control to your home base.  If you are a particular player with a certain color, your base is marked as the 4x4 corner in the board that you ought to move your cards to to score points.  If you have a card stationed at your base, whenever a player finishes moving and a new turn begins, you get to add points based on the rank of each card in your home base, so, e.g., if you have the Ace of Clubs and the King of Spades in your home base and those are the only cards controlled by you there, then you will get 14+13 = 27 points per turn.  The first player to score 2000 points wins the game.
 
 See if you can win 5 games in a row!
 
 The movement points are determined by simulated dice.  That is, die one can have as its outcome be one to six, and die two can also have its outcome be one to six.  The dice are summed by the computer and presented as the number of movement points each player has for his/her turn.
 
 Please enjoy the game!
 
 
 
 */


// 11-14-2025, 2:47 p.m.:  Woohoo!  The game is now ready for human testing! :).

// 11-14, 5:46 p.m.:  Later, add in recognition for the player if the player who wins wins by more than 1000 points.



// 11-14, 6:43 p.m.:  Feedback from Dana:  The game is fun.  It's fun to capture pieces.  The formula is hard to remember.  Include a directions pop-out that the person can click on if there's a need to review the rules.  Maybe also have a cheat sheet calculation on the sidebar.  Also, Dana suggests clarifying the dice thing...maybe having a graphic of the dice.  Also, I saw a mouse over glitch when Dana was playing the game...too many squares were moused over on Red's first turn.


// Left to do:

// Add directions pop out.  Add formula cheat sheet to the text string (easy).

/*
 
 
 MASTER LEFT-TO-DO-LIST AND WRITINGS:
 
 
1 - Include a screenshot that displays the full rules.  Maybe show a brief video of actual gameplay.  The idea is to prevent buyer’s remorse…the rules of the game don’t need to be secret.

Yes…add a screenshot video thing.  Would help to market the game.

2 - Also include a notification for when one player wins by 1000 points or more.

 (DONE)
 
 // Add directions pop out.  Add formula cheat sheet to the text string (easy).

 3, 4 - The formula is hard to remember.  Include a directions pop-out that the person can click on if there's a need to review the rules.  Maybe also have a cheat sheet calculation on the sidebar.
 
 DIRECTIONS (SKIPPED), FORMULA NOTICE (DONE)

 
 Allow the user to select their own resolution for the game. (SKIPPED)
 
 
 5 - Remove the extraneous couts.  (DONE)
 

 
 11-15-2025, 2:56 p.m.
 
 Wonderful news!  It looks like the window resizes itself automatically, so I won't need to worry about the resolution!! :D.

 
 2:57 p.m.
 
 Goal:  Be done by this upcoming Friday.  I.e., upload the game on Friday.

 
 3:29 p.m.
 
 Note:  The window.pollevent function is like the opposite of getch.  It stays with the while loop until the user stops doing things, I think.

 
 
 
 
 4:18 p.m.
 
 The game is done!  Do one play-through as a test.  Don't listen to Google AI on SFML.
 
 Make sure to do a video of play-through and a few screenshots.  Put the video up on YouTube.
 
I will probably plan to publish the game:  TOMORROW, Sunday, Nov 16, 2025 :).

 Oh, btw, I need to make a version for Windows, too.
 
 
 4:33 p.m.
 
 What if I just downloaded C++ on the computers here at Monarch, and tried to compile the code that way?
 
 Nah...it's probably doable on this computer.
 
 That is the challenge.
 
 */
