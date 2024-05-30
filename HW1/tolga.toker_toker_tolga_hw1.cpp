// Author: Tolga Toker 32639
// CS300 Homework 1

#include <iostream>
#include "MazeStack.h"

using namespace std;

int main() {
    //take the number of mazes and the size of mazes from the user
    int numberOfMazeK, rowsOfMazeM, colsOfMazeN;
    cout << "Enter the number of mazes: ";
    cin >> numberOfMazeK; cout << endl;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> rowsOfMazeM >> colsOfMazeN;
    cout << endl;

    LinkedStack<int> stack(rowsOfMazeM, colsOfMazeN);
    
    // produce mazes
    for (int i = 0; i < numberOfMazeK; i++){
        while (!stack.isFinished()){ 
            if (!stack.isDeadEndStack()){ 
                string way = stack.randomDirection(); 
                stack.pushStack(way);
            }
            else{
                while (stack.shouldStackGoBack())
                    stack.popStack(); 
            }
        }
        stack.mazeFileWriter("maze_"+ to_string(i+1) + ".txt");
        stack.restart();
    }

    int mazeID, enterX, enterY, exitX, exitY;
    cout << "All mazes are generated." << endl << endl;
    cout << "Enter a maze ID between 1 to " + to_string(numberOfMazeK) + " inclusive to find a path: ";
    cin >> mazeID;
    cout << endl;
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> enterX >> enterY;
    cout << endl;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> exitX >> exitY;
    cout << endl;

    // find the path
    stack.startPathfinder("maze_"+ to_string(mazeID) + ".txt", enterX, enterY);
    while (!stack.isEndpoint(exitX, exitY)){
        if (!stack.isDeadEndPath()){
            string way = stack.choseWay();
            if (way != "NO WAY")
                stack.moveOnce(way);
        }
        else{
            while (stack.isDeadEndPath())
                stack.popPath();
        }
    }
    // write the path to the file
    stack.pathWriter(mazeID, enterX, enterY,exitX, exitY);
    return 0;
}