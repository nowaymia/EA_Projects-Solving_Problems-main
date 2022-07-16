// Inês Marçal 2019215917
// Noémia Gonçalves 2019219433
#include <iostream>
#include <array>
#include <vector>
#include <unordered_map>

using namespace std;

struct piece
{
    array<int, 4> piece;
    bool isOnBoard;
};

struct pieceSide
{
    vector<int> indexes;
    bool isVisited;
};

unordered_map<char, array<int, 4>> rotationIndex = {{'A', {0, 1, 2, 3}},
                                                    {'B', {3, 0, 1, 2}},
                                                    {'C', {2, 3, 0, 1}},
                                                    {'D', {1, 2, 3, 0}}};


// Struct ArrayHasher adapted from stackOverflow 
//Available at https://stackoverflow.com/questions/42701688/using-an-unordered-map-with-arrays-as-keys

struct ArrayHasher
{
    std::size_t operator()(const std::array<int, 2> &a) const
    {
        std::size_t h = 0;

        for (auto e : a)
        {
            h ^= std::hash<int>{}(e) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};
//-------------------------------------------------------------

unordered_map<array<int, 2>, pieceSide, ArrayHasher> organizedPairs;
array<array<array<int, 4>, 50>, 50> board;

array<piece, 2500> pieces;

int nPieces, nPiecesOnBoard, nLines, nColumns;
void printBoard()
{

    for (int i = 0; i < nLines; i++)
    {
        for (int j = 0; j < nColumns; j++)
        {
            cout << board[i][j][0] << " " << board[i][j][1];
            if (j != nColumns - 1)
            {
                cout << "  ";
            }
        }
        cout << "\n";
        for (int j = 0; j < nColumns; j++)
        {
            cout << board[i][j][3] << " " << board[i][j][2];
            if (j != nColumns - 1)
            {
                cout << "  ";
            }
        }
        cout << "\n";
        if (i != nLines - 1)
        {
            cout << "\n";
        }
    }
}

bool puzzle(int x, int y)
{
    vector<int> possiblePiecesIndex;
    
    // Base Case
    if (nPieces == nPiecesOnBoard)
        return true;

    
    //Increase rows
    if (y >= nColumns)
    {
        x++;
        y = 0;
    }

    //Middle Pieces
    if (x > 0 && y > 0){
        possiblePiecesIndex = organizedPairs[{board[x][y - 1][2], board[x][y - 1][1]}].indexes;
    }else{
        //First Row
        if(x == 0){
            possiblePiecesIndex = organizedPairs[{board[x][y - 1][2], board[x][y - 1][1]}].indexes;
        //First Column
        }else{
            possiblePiecesIndex = organizedPairs[{board[x - 1][y][3], board[x - 1][y][2]}].indexes;
        }   
    }

    
    for (int i = 0; i < int(possiblePiecesIndex.size()); i++)
        {
            int currentPieceIndex = possiblePiecesIndex[i];

            if (pieces[currentPieceIndex].isOnBoard)
            {
                continue;
            }
          
            for (auto j : rotationIndex)
            {
                bool canFit = false;

                if (x == 0){
                    canFit = board[x][y - 1][1] == pieces[possiblePiecesIndex[i]].piece[j.second[0]] && 
                            board[x][y - 1][2] == pieces[possiblePiecesIndex[i]].piece[j.second[3]];
                }else{
                    if (y == 0){
                        canFit = board[x - 1][y][3] == pieces[possiblePiecesIndex[i]].piece[j.second[0]] && 
                                board[x - 1][y][2] == pieces[possiblePiecesIndex[i]].piece[j.second[1]];
                    }else{
                        canFit = board[x][y - 1][1] == pieces[currentPieceIndex].piece[j.second[0]] &&
                                board[x][y - 1][2] == pieces[currentPieceIndex].piece[j.second[3]] &&
                                board[x - 1][y][3] == pieces[currentPieceIndex].piece[j.second[0]] &&
                                board[x - 1][y][2] == pieces[currentPieceIndex].piece[j.second[1]];
                    }
                }

                if (canFit){
                    board[x][y][0] = pieces[currentPieceIndex].piece[j.second[0]];
                    board[x][y][1] = pieces[currentPieceIndex].piece[j.second[1]];
                    board[x][y][2] = pieces[currentPieceIndex].piece[j.second[2]];
                    board[x][y][3] = pieces[currentPieceIndex].piece[j.second[3]];
                  
                    pieces[currentPieceIndex].isOnBoard = true;
                 
                    nPiecesOnBoard++;
                    if (puzzle(x, y + 1))
                    {
                        return true;
                    }
                    nPiecesOnBoard--;
                    pieces[currentPieceIndex].isOnBoard = false;
                }

               
            }
        }
        
        return false;

}


int main()
{
    int nTestCase;
    int nOdd;

    
    cin >> nTestCase;

    for (int i = 0; i < nTestCase; i++)
    {
        
        board = {{{}}};
        pieces = {{}};
        
        nPiecesOnBoard = 1;
        cin >> nPieces >> nLines >> nColumns;
        nOdd = 0;

        organizedPairs.clear();
        for (int j = 0; j < nPieces; j++)
        {
            
            for (int k = 0; k < 4; k++)
            {
                cin >> pieces[j].piece[k];
            }
            for (int k = 0; k < 4; k++)
            {
              
                if (organizedPairs.find({pieces[j].piece[k], pieces[j].piece[(k + 1) % 4]}) != organizedPairs.end())
                {
                    organizedPairs.at({pieces[j].piece[k], pieces[j].piece[(k + 1) % 4]}).indexes.push_back(j);
                }
                else
                {
                
                    organizedPairs.insert({{pieces[j].piece[k], pieces[j].piece[(k + 1) % 4]}, pieceSide()});
                    organizedPairs.at({pieces[j].piece[k], pieces[j].piece[(k + 1) % 4]}).indexes.push_back(j);
                }
            }
        }
     
        board[0][0] = pieces[0].piece;
        pieces[0].isOnBoard = 1;

        //Check if number of pair match with it's reverse (1,2) matches with (2,1)  
        for (auto j : organizedPairs)
        {

            if (j.second.isVisited)
                continue;

            //Equal numbers match with themselves
            if (j.first[0] == j.first[1])
            {
                nOdd += int(j.second.indexes.size()) % 2;
                continue;
            }

            
            auto complementIt = organizedPairs.find({j.first[1], j.first[0]});
          
            if (complementIt != organizedPairs.end())
            {
                complementIt->second.isVisited = 1;
                nOdd += abs(int(j.second.indexes.size()) - int(complementIt->second.indexes.size()));
            }
            else
            {
                
                nOdd += int(j.second.indexes.size());
            }
        }

        //Check if the number pair with no matches is higher than the board perimeter 
        //puzzle it's impossible
        if (nOdd > 2 * nLines + 2 * nColumns)
        {
            cout << "impossible puzzle!\n";
            continue;
        }

        if (puzzle(0, 1))
        {
            printBoard();
        }
        else
        {
            cout << "impossible puzzle!\n";
        }
    }

    return 0;
}