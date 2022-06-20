#include "../include/MinesweeperBoard.hpp"

void MainMenu(){
    GameMode gameMode = GameMode::DEBUG;
    int sizex = 0;
    int sizey = 0;
    do{
        std::cout << "Please write board dimensions in x y format. Ex. 15 14 will create 15 of x and 14 of y\n";
        std::string input = fixInput();
        int inputlength = input.length();
        int ix = 0;
        sizex = -1;
        sizey = -1;

        for(; ix < inputlength;ix++){
            if(input[ix] == ' '){
                sizex = strtoint(input.substr(0,ix));
                break;
            }
        }
        for(int iy = ix + 1; iy < inputlength; iy++){
            if(input[iy] == ' ' || iy == inputlength-1){
                sizey = strtoint(input.substr(ix+1,iy));
                break;
            }
        }
    }while(sizex < 0 || sizey < 0);
    int diffSelect = 0;
    do{
        std::cout << "Please select difficulty\nType:\n0 for easy\n1 for normal\n2 for hard\n";
        std::string input = fixInput();
        diffSelect = strtoint(input);
    }while(diffSelect < 0 || (diffSelect > 2 && diffSelect != 69));

    switch(diffSelect){
        case 0:
            gameMode = GameMode::EASY;
            break;
        case 1:
            gameMode = GameMode::NORMAL;
            break;
        case 2:
            gameMode = GameMode::HARD;
            break;
        default:
            gameMode = GameMode::DEBUG;
            break;
    }

    MinesweeperBoard board{sizex, sizey, gameMode};
    MSBoardTextView view{board};
    MSTextController ctrl{board, view};

    ctrl.play();
}
