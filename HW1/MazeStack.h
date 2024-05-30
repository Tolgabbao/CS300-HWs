#ifndef MAZESTACK_H
#define MAZESTACK_H

// Author: Tolga Toker 32639
// CS300 Homework 1

#include <string>
#include <fstream>
#include <iostream>
#include <stack>

using namespace std;

// Randgen
#include <chrono>

class RandGen {
public:
    RandGen() {
        auto currentTime = std::chrono::system_clock::now();
        auto duration = currentTime.time_since_epoch();
        unsigned seed = static_cast<unsigned>(duration.count());
        srand(seed);
    }

    int RandInt(int min, int max) {
        return rand() % (max - min + 1) + min;
    }
};

template<class Object>
struct Cell{
        Object x, y;                   
        Cell *up, *right, *down, *left;
        int walls[4] = {1, 1, 1, 1};
        bool visited, visited_pathfinder;

        Cell(Object x, Object y, bool visited = false, bool visited_pathfinder = false,
                 Cell* up = nullptr, Cell* right = nullptr, Cell* down = nullptr, Cell* left = nullptr)
                        : x(x), y(y), visited(visited), visited_pathfinder(visited_pathfinder),
                            right(right), left(left), up(up), down(down) {}
};

template <class Object>
struct path{
    Cell<Object>* pathCell; // an element of the cell stack
    path* next; // next element of the stack

    // constructor
    path(Cell<Object>* theElement, path* n = nullptr )
            :next(n), pathCell(theElement)
            {};
};


template <class Object>
class LinkedStack {
public:
    // constructor
    LinkedStack(int row, int col) {
        maxX = col;
        maxY = row;
        cellCount = 1;
        mazeHead = mazeBuilder(); // build maze
        topOfStack = new path<Object>(mazeHead, nullptr); // assign the head of it
        pathfinder = nullptr; // do not start pathfinder yet
    }
    // destructor
    ~LinkedStack() { // delete the maze and empyt the stack and delete the pathfinder
        delete pathfinder; // pathfinder will be nullptr in file writing method of path
        pathfinder = nullptr;
        deleteMaze();
        makeEmpty();
    }


    // functions to build the maze
    void goUp(Cell<Object>* & pointer){
        pointer = pointer->up;
    }
    void goDown(Cell<Object>* & pointer){
        pointer = pointer->down;
    }
    void goRight(Cell<Object>* & pointer){
        pointer = pointer->right;
    }
    void goLeft(Cell<Object>* & pointer){
        pointer = pointer->left;
    }
    Cell<Object>* buildRow(Cell<Object>* pointer, int row, int col) {
        if (col == 0) {
            for (int i = 0; i < row; i++) {
                pointer->up = new Cell<Object>(col, i + 1, false, false, nullptr, nullptr, nullptr);
                pointer->up->down = pointer;
                goUp(pointer);
            }
            for (int i = 0; i < row; i++)
                goDown(pointer);
        } else {
            for (int i = 0; i < col; i++) {
                pointer->right = new Cell<Object>(i + 1, row, false, false, nullptr, nullptr, nullptr);
                pointer->right->left = pointer;
                goRight(pointer);
            }
            for (int i = 0; i < col; i++)
                goLeft(pointer);
        }
        return pointer;
    }
    Cell<Object>* connectRows(Cell<Object>* top, Cell<Object>* below) {
        for (int i = 0; i < maxX; i++) {
            top->down = below;
            below->up = top;
            if (top->right != nullptr && below->right != nullptr) {
                top = top->right;
                below = below->right;
            }
        }
        for (int i = 0; i < maxX - 1; i++)
            top = top->left;
        return top;
    }
    Cell<Object>* mazeBuilder() {
        mazeHead = new Cell<Object>(0, 0, true, false, nullptr, nullptr, nullptr);
        if (maxY == 1) {
            mazeHead = buildRow(mazeHead, 0, maxX - 1);
        } else if (maxX == 1) {
            mazeHead = buildRow(mazeHead, maxY - 1, 0);
        } else {
            Cell<Object>* temp = mazeHead;
            temp = buildRow(temp, 0, maxX - 1);
            for (int i = 0; i < maxY - 1; i++) {
                Cell<Object>* follower = temp;
                temp = new Cell<Object>(0, i + 1);
                temp = buildRow(temp, i + 1, maxX - 1);
                temp = connectRows(temp, follower);
                follower = follower->up;
            }
        }
        return mazeHead;
    }

    // destuctor
    void makeEmpty() { //empty the stack and delete topOfStack
        while (topOfStack != nullptr) {
            path<Object>* temp = topOfStack;
            topOfStack = topOfStack->next;
            delete temp;
            temp = nullptr;
        }
        delete topOfStack;
        topOfStack = nullptr;
    }
    void deleteMaze(){ //deletes the maze and deletes the mazeHead
        if (mazeHead != nullptr) {
            Cell<Object>* iter = mazeHead;
            while (iter->up != nullptr)
                goUp(iter);

            for (int i = 0; i < maxY; i++) {
                while (iter->right != nullptr)
                    goRight(iter);
                for (int j = 0; j < maxX; j++) {
                    if (iter->up != nullptr)
                        iter->up = nullptr;
                    if (iter->right != nullptr)
                        iter->right = nullptr;
                    if (iter->down != nullptr) {
                        if (iter->left == nullptr) {
                            iter = iter->down;
                            iter->up->down = nullptr;
                            delete iter->up;
                            iter->up = nullptr;
                        }
                        else
                            iter->down = nullptr;
                    }

                    if (iter->left != nullptr) {
                        iter = iter->left;
                        iter->right->left = nullptr;
                        delete iter->right;
                        iter->right = nullptr;
                    }
                }
            }
            delete mazeHead;
            mazeHead = nullptr;
        }
    }
    void restart() { //restarts the maze for new maze creations
        makeEmpty();
        deleteMaze();
        mazeHead = mazeBuilder();
        topOfStack = new path<Object>(mazeHead, nullptr);
        cellCount = 1;
    }


    // checks stack
    bool isDeadEndStack() { //checks the dead end situations for topOfStack
        Cell<Object>* Cell = topOfStack->pathCell;
        bool isDeadEnd = false;

        // Check corner conditions
        if ((Cell->up == nullptr && Cell->right == nullptr && Cell->down != nullptr && Cell->down->visited == true && Cell->left != nullptr && Cell->left->visited == true) ||
            (Cell->up == nullptr && Cell->left == nullptr && Cell->down != nullptr && Cell->down->visited == true && Cell->right != nullptr && Cell->right->visited == true) ||
            (Cell->down == nullptr && Cell->left == nullptr && Cell->up != nullptr && Cell->up->visited == true && Cell->right != nullptr && Cell->right->visited == true) ||
            (Cell->down == nullptr && Cell->right == nullptr && Cell->up != nullptr && Cell->up->visited == true && Cell->left != nullptr && Cell->left->visited == true))
        {
            isDeadEnd = true;
        }

        // Check side conditions
        else if ((Cell->up == nullptr && Cell->right != nullptr && Cell->right->visited == true && Cell->down != nullptr && Cell->down->visited == true && Cell->left != nullptr && Cell->left->visited == true) ||
                 (Cell->right == nullptr && Cell->up != nullptr && Cell->up->visited == true && Cell->down != nullptr && Cell->down->visited == true && Cell->left != nullptr && Cell->left->visited == true) ||
                 (Cell->down == nullptr && Cell->up != nullptr && Cell->up->visited == true && Cell->right != nullptr && Cell->right->visited == true && Cell->left != nullptr && Cell->left->visited == true) ||
                 (Cell->left == nullptr && Cell->up != nullptr && Cell->up->visited == true && Cell->down != nullptr && Cell->down->visited == true && Cell->right != nullptr && Cell->right->visited == true))
        {
            isDeadEnd = true;
        }

        // Check center condition
        else if (Cell->left != nullptr && Cell->left->visited == true && Cell->up != nullptr && Cell->up->visited == true && Cell->down != nullptr && Cell->down->visited == true && Cell->right != nullptr && Cell->right->visited == true)
        {
            isDeadEnd = true;
        }

        return isDeadEnd;
    }
    bool isEmpty(path<Object>* pathCell){
        return pathCell == nullptr;
    }
    bool isFinished(){ // is all the maze created
        return cellCount == (maxX * maxY);
    }
    bool shouldStackGoBack() { // checks if the current Cell is proper to move for a new Cell
        return  (topOfStack->pathCell->up == nullptr || topOfStack->pathCell->up->visited) &&
                (topOfStack->pathCell->right == nullptr || topOfStack->pathCell->right->visited) &&
                (topOfStack->pathCell->down == nullptr || topOfStack->pathCell->down->visited) &&
                (topOfStack->pathCell->left == nullptr || topOfStack->pathCell->left->visited);
    }

    // STACK movement
    void popStack(){ //remove an element from stack
        if (!isEmpty(topOfStack)){
            path<Object>* temp = topOfStack->next;
            topOfStack = nullptr;
            topOfStack = temp;
        }
    }
    Cell<Object>* getCell(string way, path<Object>* subject){
        if (way == "UP")
            return subject->pathCell->up;
        else if (way == "RIGHT")
            return subject->pathCell->right;
        else if(way == "DOWN")
            return subject->pathCell->down;
        else
            return subject->pathCell->left;
    }
    void visit(string way){
        if (way == "UP")
            topOfStack->pathCell->up->visited = true;
        else if (way == "RIGHT")
            topOfStack->pathCell->right->visited = true;
        else if(way == "DOWN")
            topOfStack->pathCell->down->visited = true;
        else
            topOfStack->pathCell->left->visited = true;
    }
    void knockDown(string way){ // based on the way, it knock downs wall in the given direction
        if (way == "UP"){
            topOfStack->pathCell->walls[0] = 0; // knocks down the up wall of the current Cell
            topOfStack->pathCell->up->walls[2] = 0; //knocks down the down wall of the up Cell
        }

        else if (way == "RIGHT"){
            topOfStack->pathCell->walls[1] = 0; // knocks down the right wall of the current Cell
            topOfStack->pathCell->right->walls[3] = 0; // knocks down the left wall of the right Cell
        }

        else if(way == "DOWN"){
            topOfStack->pathCell->walls[2] = 0; // knocks down the down wall of the current Cell
            topOfStack->pathCell->down->walls[0] = 0; //knocks down the up wall of the down Cell
        }

        else{
            topOfStack->pathCell->walls[3] = 0; // knocks down the left wall of the current Cell
            topOfStack->pathCell->left->walls[1] = 0; //knocks down the right wall of the left Cell
        }
    }
    void pushStack(string way){ //puts an element to the stack
        visit(way);
        knockDown(way);
        cellCount++;
        path<Object>* temp = topOfStack;
        Cell<Object>* newCell = getCell(way, topOfStack); //gets a Cell in the given direction
        path<Object>* newPath = new path<Object> (newCell, temp);
        topOfStack = newPath;
    }

    //randomizer for chosing the way
    string randomDirection(){ //choosing a way for the topOfStack
        string availableWays = "0123";

        if (topOfStack->pathCell->up == nullptr || topOfStack->pathCell->up->visited)
            availableWays = availableWays.substr(1);
        if (topOfStack->pathCell->right == nullptr || topOfStack->pathCell->right->visited)
            availableWays = availableWays.substr(0, availableWays.find('1')) + availableWays.substr(availableWays.find('1') + 1);
        if (topOfStack->pathCell->down == nullptr || topOfStack->pathCell->down->visited)
            availableWays = availableWays.substr(0, availableWays.find('2')) + availableWays.substr(availableWays.find('2') + 1);
        if (topOfStack->pathCell->left == nullptr || topOfStack->pathCell->left->visited)
            availableWays = availableWays.substr(0, availableWays.find('3')) + availableWays.substr(availableWays.find('3') + 1);

        RandGen random; //random generator
        int wall = random.RandInt(0, availableWays.length() - 1);  //chooses a random wall for current Cell
                                                                            // and from availableWays
        if (availableWays[wall] == '0')
            return "UP";

        else if (availableWays[wall] == '1')
            return "RIGHT";

        else if (availableWays[wall] == '2')
            return "DOWN";

        else
            return "LEFT";
    }

    // write to file
    void mazeFileWriter(string fileName){
        ofstream outputFile;
        outputFile.open(fileName);
        if (outputFile.is_open()){
            Cell<Object>* iterator = mazeHead;
            outputFile << to_string(maxY) + " " + to_string(maxX) + "\n"; //writes the size of maze
            for (int j = 0; j <= maxY - 1; j++){
                for (int i = 0; i <= maxX - 1; i++){
                    //takes the elements of the iterator
                    string x = to_string(iterator->x),
                        y = to_string(iterator->y), 
                        up = to_string(iterator->walls[0]),
                        right = to_string(iterator->walls[1]), 
                        down = to_string(iterator->walls[2]), 
                        left = to_string(iterator->walls[3]);
                    outputFile <<"x=" + x + " y=" + y + " l=" + left + " r=" + right + " u=" + up + " d=" + down + "\n"; //puts the elements into txt file
                    if (iterator->right != nullptr)
                        goRight(iterator);
                }
                for (int i = 0; i <= maxX - 1; i++){ //go beginning of the row
                    if (iterator->left != nullptr)
                        goLeft(iterator);
                }

                if (iterator->up != nullptr) //and a step up of iterator
                    goUp(iterator);
            }

        }
    }

    // pathfinder
    void pathWriter(int mazeID, Object enterX, Object enterY, Object exitX, Object exitY){
        string fileName = "maze_" + to_string(mazeID) + "_path_" + to_string(enterX) + "_" + to_string(enterY) + "_" + to_string(exitX) + "_" + to_string(exitY) + ".txt";
        ofstream outputFile;
        outputFile.open(fileName);
        // reverses the pathfinder stack
        path<Object>* real = nullptr;
        while (pathfinder != nullptr){
            if (real == nullptr){
                real = new path<Object> (pathfinder->pathCell, nullptr);
            }else{
                path<Object>* temp = new path<Object> (pathfinder->pathCell, real);
                real = temp;
            }
            popPath(); //it makes the pathfinder nullptr
        }

        if (outputFile.is_open()){
            while (real != nullptr){
                outputFile << to_string(real->pathCell->x) + " " + to_string(real->pathCell->y) + "\n";
                real = real->next;
            }
        }
    }

    void popPath(){ //pathfinder steps back
        //removes a found path from path stack
        if (!isEmpty(pathfinder)){
            path<Object>* temp = pathfinder->next;
            delete pathfinder;
            pathfinder = nullptr;
            pathfinder = temp;
        }
    }
    void cellModifier(string line, Cell<Object>* & modify){ //reads the line and determines the important values for pathfinder and modifies the given Cell
        //takes the important values from line
        Object x = stoi(line.substr(line.find("x=") + 2, line.find(" ")));
        line = line.substr(line.find(" ") + 1);
        Object y = stoi(line.substr(line.find("y=") + 2, line.find(" ")));
        line = line.substr(line.find(" ") + 1);
        string left = line.substr(line.find("l=") + 2, line.find(" ") - line.find("l=") - 2);
        line = line.substr(line.find(" ") + 1);
        string right = line.substr(line.find("r=") + 2, line.find(" ") - line.find("r=") - 2);
        line = line.substr(line.find(" ") + 1);
        string up = line.substr(line.find("u=") + 2, line.find(" ") - line.find("u=") - 2);
        line = line.substr(line.find(" ") + 1);
        string down = line.substr(line.find("d=") + 2, line.find(" ") - line.find("d=") - 2);
        modify->x = x;
        modify->y = y;

        //modifying the given Cell
        modify->walls[0] = up == "1" ? 1 : 0;
        modify->walls[1] = right == "1" ? 1 : 0;
        modify->walls[2] = down == "1" ? 1 : 0;
        modify->walls[3] = left == "1" ? 1 : 0;
    }
    void readMaze(string mazeName){ //reads the maze from a given input file
        ifstream file(mazeName);
        string line;
        int first = 0, row = 1, col = 1;
        mazeHead = mazeBuilder(); //calls it again reinitialize the maze
        Cell<Object>* iter = mazeHead;
        while(getline(file, line)){ //takes the lines from txt file
            if (first++ == 0) continue; //skip first line
            cellModifier(line, iter); //modifies the Cell
            if (iter->right != nullptr) goRight(iter); //step for next Cell
            if (++col == maxX){ //checks the end of row by checking column number
                getline(file, line);
                cellModifier(line, iter);
                while(iter->left != nullptr) goLeft(iter); //go to beginning
                if (iter->up != nullptr) goUp(iter); //and go up for new row
                col = 1;
                row++; //increment row
            }
        }
    }
    void startPathfinder(string mazeName, Object enterX, Object enterY) {
        readMaze(mazeName); //initializes the maze by reading maze from file
        Cell<Object>* initial = nullptr; //temporary starting point iterator
        if (enterY == 0 && enterX == 0) {
            initial = mazeHead;
            initial->visited_pathfinder = true;

        } else if (enterY == 0 && enterX != 0) {
            Cell<Object> *temp = mazeHead;
            for (int i = 0; i < enterX ; i++)
                goRight(temp);
            initial = temp;
            initial->visited_pathfinder = true;

        } else if (enterY != 0 && enterX == 0) {
            Cell<Object> *temp = mazeHead;
            for (int i = 0; i < enterY ; i++)
                goUp(temp);
            initial = temp;
            initial->visited_pathfinder = true;

        } else if (enterX != 0 && enterY != 0){
            Cell<Object> *temp = mazeHead;
            for(int i = 0; i < enterX; i++)
                goRight(temp);
            for (int i = 0; i < enterY; i++)
                goUp(temp);
            initial = temp;
            initial->visited_pathfinder = true;
        }
        pathfinder = new path<Object>(initial, nullptr); // assign the pathfinder to starting point
    }

    // PATHFINDER movement
    bool isDeadEndPath() {
        int available = 0;
        if (pathfinder->pathCell->walls[0] != 1 && !pathfinder->pathCell->up->visited_pathfinder)
            available++;
        if (pathfinder->pathCell->walls[1] != 1 && !pathfinder->pathCell->right->visited_pathfinder)
            available++;
        if (pathfinder->pathCell->walls[2] != 1 && !pathfinder->pathCell->down->visited_pathfinder)
            available++;
        if (pathfinder->pathCell->walls[3] != 1 && !pathfinder->pathCell->left->visited_pathfinder)
            available++;

        return available == 0;

    }
    void moveOnce(string way){ // push method for the pathfinder same functionality with topOfStack push method
        path<Object>* temp = pathfinder;
        Cell<Object>* newCell = getCell(way, pathfinder);
        path<Object>* newPath = new path<Object> (newCell, temp);
        pathfinder = newPath;
        pathfinder->pathCell->visited_pathfinder = true;
    }

    bool isEndpoint(Object exitX,Object exitY){ // is pathfinder on the exit point
        return pathfinder->pathCell->x == exitX && pathfinder->pathCell->y == exitY;
    }

    string choseWay(){// for the current position of the pathfinder checks the available ways and returns it
                    // same functonality with choseWayStack()
        string availableWays = "0123";
        if (pathfinder->pathCell->up == nullptr || pathfinder->pathCell->walls[0] == 1 || pathfinder->pathCell->up->visited_pathfinder)
            availableWays = availableWays.substr(1);
        if (pathfinder->pathCell->right == nullptr || pathfinder->pathCell->walls[1] == 1 || pathfinder->pathCell->right->visited_pathfinder)
            availableWays = availableWays.substr(0, availableWays.find('1')) + availableWays.substr(availableWays.find('1') + 1);
        if (pathfinder->pathCell->down == nullptr || pathfinder->pathCell->walls[2] == 1 || pathfinder->pathCell->down->visited_pathfinder)
            availableWays = availableWays.substr(0, availableWays.find('2')) + availableWays.substr(availableWays.find('2') + 1);
        if (pathfinder->pathCell->left == nullptr || pathfinder->pathCell->walls[3] == 1 || pathfinder->pathCell->left->visited_pathfinder)
            availableWays = availableWays.substr(0, availableWays.find('3')) + availableWays.substr(availableWays.find('3') + 1);

        RandGen random;
        int wall = random.RandInt(0, availableWays.length() - 1);

        return (availableWays[wall] == '0') ? "UP" :
               (availableWays[wall] == '1') ? "RIGHT" :
               (availableWays[wall] == '2') ? "DOWN" :
               (availableWays[wall] == '3') ? "LEFT" :
                                              "NO WAY";
    }

private:
    path<Object>* topOfStack; // last element in stack
    path<Object>* pathfinder; // pathfinder for finding the path for given entry and exit
    Cell<Object>* mazeHead; // left bottom point of maze (0,0)
    int cellCount; // number of cells in the maze visited by the stack
    int maxX; // max column number
    int maxY; // max row number
};


#endif