#include "../include/MinesweeperBoard.hpp"

int chartoint(char &c){
    int x = (int)c;
    if(c == '\000') return 0;
    if(c >= (char)(48) && c <= (char)(57)){
        x = x - 48;
        return x;
    }else return -1;
}

int power(int x, int y){
    int buffer = x;
    if(y ==0)return 1;
    for(int i = 1; i < y; i++){
        x *= x;
    }
    return buffer;
}

int strtoint(std::string strin){
    int str_len = strin.length();
    int buffer = 0;
    for(int i =str_len - 1; i >= 0; i--){
        int k = power(10,i);
        int ln = str_len - 1 - i;
        if(chartoint(strin[ln]) == -1) return -1;
        buffer += chartoint(strin[ln])*k;
    }
    return buffer;
}

int count_numbers ( int num) {
    if(num == 0) return 1;
   int count =0;
   while (num !=0) {
      count++;
      num/=10;
   }
   return count;
}

std::string fixInput(){
    std::string stdbuffer = "";
    std::cout << ":";
    std::getline(std::cin,stdbuffer);
    return stdbuffer;
}

void clear_terminal(){
    #ifdef linux
        system("clear");
    #endif // linux
    #ifdef _WIN32
        system("cls");
    #endif
}

MinesweeperBoard::MinesweeperBoard(int width_in, int height_in, GameMode mode){
	// setting up size
	MinesweeperBoard::width = width_in;
	MinesweeperBoard::height = height_in;

	//setting up game state
	MinesweeperBoard::gameState = GameState::RUNNING;

	//set up game mode
	MinesweeperBoard::game_mode = mode;

	//setting 0 in field struct, initializing all fields
	for(int x = 0; x < width_in; x++){
		std::vector<Field> vector_y;
		for(int y = 0; y < height_in; y++){
            Field buffer;
			buffer.hasMine = 0;
			buffer.hasFlag = 0;
			buffer.isRevealed = 0;
            vector_y.push_back(buffer);
		}
		MinesweeperBoard::board.push_back(vector_y);
	}

	//Game modes
	//initialize variables and seed the rand
	int random_x = 0;
	int random_y = 0;
	int number_of_fields = width_in * height_in;
	int number_of_mines = 0;
//	srand (time(NULL)); //deterministic sequence of the numbers, possible to predict position of mines (maybe)

	//set difficulty
	double difficulty = 0; //0-1
	switch(mode){
		case GameMode::EASY:
			difficulty = 0.1;
			break;
		case GameMode::NORMAL:
			difficulty = 0.2;
			break;
		case GameMode::HARD:
			difficulty = 0.3;
			break;
        case GameMode::DEBUG:
            break;
	}

	//setting up the mines
	if(MinesweeperBoard::game_mode == GameMode::DEBUG){
		//running in debug mode
		for(int x = 0; x < MinesweeperBoard::width; x++){
			for(int y = 0; y < MinesweeperBoard::height; y++){
				if(x==y){
                        MinesweeperBoard::board[x][y].hasMine = 1;
                        number_of_mines++;
				}else
				if(x == 0){
                        MinesweeperBoard::board[x][y].hasMine = 1;
                        number_of_mines++;
                }else
				if((y == 0) && (x%2 == 0)){
                        MinesweeperBoard::board[x][y].hasMine = 1;
                        number_of_mines++;
                }
			}
		}
        debug_display();
	}
	else{
		number_of_mines = number_of_fields * difficulty;
		for(int i = 0; i < number_of_mines; ){
			random_x = rand() % MinesweeperBoard::width;
			random_y = rand() % MinesweeperBoard::height;
			if(MinesweeperBoard::board[random_x][random_y].hasMine == 0){
				MinesweeperBoard::board[random_x][random_y].hasMine = 1;
				i++;
			}
		}
	}
	MinesweeperBoard::number_of_mines = number_of_mines;
	MinesweeperBoard::markedMines = 0;
	MinesweeperBoard::countAllFlags = 0;
}

//display board debug
void MinesweeperBoard::debug_display() const{
	for(int x = 0; x < MinesweeperBoard::width; x++){
		for(int y = 0; y < MinesweeperBoard::height; y++){
			if(MinesweeperBoard::board[x][y].hasMine) std::cout << "[M";
			else std::cout << "[.";
			if(MinesweeperBoard::board[x][y].isRevealed) std::cout << "o";
			else std::cout << ".";
			if(MinesweeperBoard::board[x][y].hasFlag) std::cout << "f]";
			else std::cout << ".]";
		}
		std::cout << "\n";
	}
}


int MinesweeperBoard::getBoardWidth() const{
	return MinesweeperBoard::width;
}

int MinesweeperBoard::getBoardHeight() const{
	return MinesweeperBoard::height;
}

int MinesweeperBoard::getMineCount() const{
    int mines = MinesweeperBoard::number_of_mines;
    mines = mines - MinesweeperBoard::countAllFlags;
	return mines;
}

//	field check
//is inside
//	field revealed
// 	mine - end the game - write x
// 	no mine
//			no mine in vicinity - space
// 		mines in vicinity - number
// flag
// not revealed and no flag

//check if coords are out of bounds
bool MinesweeperBoard::insideOfBoard(int row, int col) const{
    int boardw = getBoardWidth();
    int boardh = getBoardHeight();
	if(row >= 0 && row < boardw && col >= 0 && col < boardh) return true;
	else return false;
}

bool MinesweeperBoard::isRevealed(int row, int col) const{
	return MinesweeperBoard::board[row][col].isRevealed;
}
bool MinesweeperBoard::hasFlag(int row, int col) const{
	if(MinesweeperBoard::insideOfBoard(row, col) == 0)  return false;
	if(MinesweeperBoard::isRevealed(row, col)) return false;
	return MinesweeperBoard::board[row][col].hasFlag;
}
int MinesweeperBoard::countMines(int row, int col) const{
    if(MinesweeperBoard::board[row][col].hasMine) return -1;
	int mines = 0;
	for(int x = -1; x < 2 ; x++){
		for(int y = -1; y < 2; y++){
            if(MinesweeperBoard::insideOfBoard(row+x, col+y)){
                if(MinesweeperBoard::board[row+x][col+y].hasMine) mines++;
            }
		}
	}
	return mines;
}

bool MinesweeperBoard::hasMine(int row, int col) const{
    if(MinesweeperBoard::board[row][col].hasMine) return 1;
    else return 0;
}

char MinesweeperBoard::getFieldInfo(int row, int col) const{
	if(MinesweeperBoard::insideOfBoard(row, col)){
		if(MinesweeperBoard::isRevealed(row,col)){
            if(countMines(row,col) == -1) return 'x';
			if(countMines(row,col) > 0) return (char)((int)'0' + countMines(row,col));
			if(countMines(row,col) == 0) return ' ';
			return 'x';
		}else{
			if(hasFlag(row,col)) return 'F';
			else{
                return '_';
			}
		}
	} return '#';
}

void MinesweeperBoard::toggleBit(bool &bit){
	if(bit) bit = 0;
	else bit = 1;
}

void MinesweeperBoard::toggleFlag(int row, int col){
	if(insideOfBoard(row, col) && MinesweeperBoard::countAllFlags <= MinesweeperBoard::number_of_mines){
		if(isRevealed(row, col) == 0){
			MinesweeperBoard::toggleBit(MinesweeperBoard::board[row][col].hasFlag);
			if(MinesweeperBoard::board[row][col].hasFlag){
                MinesweeperBoard::countAllFlags++;
                if(MinesweeperBoard::board[row][col].hasMine) MinesweeperBoard::markedMines++;
			}
            if(!(MinesweeperBoard::board[row][col].hasFlag)){
                MinesweeperBoard::countAllFlags--;
                if(MinesweeperBoard::board[row][col].hasMine) MinesweeperBoard::markedMines--;
			}
		}
	}
	if(MinesweeperBoard::markedMines == MinesweeperBoard::number_of_mines) gameState = GameState::FINISHED_WIN;
}
void MinesweeperBoard::revealField(int row, int col){
	if(insideOfBoard(row, col)){
		if(isRevealed(row, col) == 0){
			if(hasFlag(row,col) == 0){
				if(getGameState() == GameState::RUNNING){
					if(countMines(row, col) == -1){
						board[row][col].isRevealed = 1;
						gameState = GameState::FINISHED_LOSS;
					}else{
						board[row][col].isRevealed = 1;
                        if(getFieldInfo(row,col) == ' '){
                            for(int i = -1; i < 2; i++){
                                for(int j = -1; j < 2; j++){
                                    if(i!=j && i != j*(-1)){
                                        if(insideOfBoard(row +i,col+j) && isRevealed(row +i,col+j) == 0 && MinesweeperBoard::hasFlag(row +i,col+j) == 0 && countMines(row +i,col+j) != -1)revealField(row +i,col+j);
                                    }
                                }
                            }
                        }
				}
				}
			}
		}
	}
}

GameState MinesweeperBoard::getGameState() const{
	if(MinesweeperBoard::gameState == GameState::RUNNING) return GameState::RUNNING;
	if(MinesweeperBoard::gameState == GameState::FINISHED_WIN) return GameState::FINISHED_WIN;
	if(MinesweeperBoard::gameState == GameState::FINISHED_LOSS) return GameState::FINISHED_LOSS;
	else return GameState::FINISHED_LOSS;
}

void MSBoardTextView::display(){
    int boardh = board.getBoardHeight(); // y
    int boardw = board.getBoardWidth(); // x
    for(int x = -1; x < boardw; x++){
        if(x>=0)std::cout << x;
        else std::cout << " ";

        int spaces = count_numbers(boardw) - count_numbers(x) + 1;
        for(int i = 0; i < spaces; i++){
            std::cout << " ";
        }
        int spacesymax = 0;
        for(int y = 0; y < boardh; y++){
            if(y >=0){
                if(y>0){
                    spaces = count_numbers(boardh) - count_numbers(y);
                    for(int i = 0; i < spaces; i++){
                        std::cout << " ";
                    }
                    if(spaces>spacesymax) spacesymax = spaces;
                }
                if(x>=0){std::cout << board.getFieldInfo(x,y);
                    int spaces = count_numbers(y+1);
                    for(int i = 0; i < spaces; i++){
                        std::cout << " ";
                    }
                }else{
                    std::cout << y << " ";
                    if(y == boardh -1) std::cout << "X";
                }
            }
        }
        for(int i = 0; i < spacesymax+1; i++){
        std::cout << "\n";
        }
    }
    std::cout << "Y" << std::endl;
}

bool MSTextController::checkAdj(int i, int j){
    bool close = 0;
    for(int k = -1; k < 2; k++){
        for(int l = -1; l < 2; l++){
            if(board.getFieldInfo(j+l,i+k) == ' ') close = 1;
        }
    }
    return close;
}

void MSTextController::play(){
    view.display();
    std::cout << "\nHello!\n";
    bool userPlaying = 1;
    while(board.getGameState() == GameState::RUNNING && userPlaying){
        char action = 'm';
        int x = -1,y = -1;
        std::string input = "";


        do{
        //this is needed for some reason, cin does not return error with multople things
            std::cout << "\nMines:" << board.getMineCount() << "\nType h then enter for help\n";
            input = fixInput();
            action = input[0];
        }while(input.length() <= 1 && action != 'q' && action != 'h');


        if(action != 'q' && action != 'h'){
            int inputlength = input.length();
            int ix = 0;
            input = input.substr(2,inputlength-1);
            inputlength = input.length();
            for(; ix < inputlength;ix++){
                if(input[ix] == ' '){
                    x = strtoint(input.substr(0,ix));
                    break;
                }
            }
            for(int iy = ix + 1; iy < inputlength; iy++){
                if(input[iy] == ' ' || iy == inputlength-1){
                    y = strtoint(input.substr(ix+1,iy));
                    break;
                }
            }
            clear_terminal();
        }

        if((x == -1 || y == -1 || x >= board.getBoardWidth() || y >= board.getBoardHeight()) && action != 'q' && action != 'h') std::cout << "Please enter valid coordinates\n";
        else{
            // row = y col = x
            switch(action){
                case 'r':
                    board.revealField(y,x);
                    break;
                case 'm':
                    board.toggleFlag(y,x);
                    break;
                case 'q':
                    userPlaying = 0;
                    break;
                default:
                    std::cout << "Please enter valid command.\nAvailable commands:\nr x y - reveal on a field in x, y coordiates\nm x y - plant a flag on a field in x, y coordiates\nTo remove a flag type m x y where x,y is coordiantes of flag you wish to remove\nq - if you wish to exit the game\n\n";
                    break;
                    //help messege
            }
        }
        if(action != 'q' && action != 'h')view.display();
        //if(board.getGame)std::cout << board.countMines(1,0) << "\n";
        //std::cout << board.countMines(2,9) << "\n";
        //std::cout << board.countMines(2,1) << "\n";
        //std::cout << board.countMines(6,7) << "\n";

    }
    std::cout << "\n";
	if(board.getGameState() == GameState::FINISHED_WIN) std::cout << "YOU WON!\n";
	if(board.getGameState() == GameState::FINISHED_LOSS) std::cout << "YOU LOST, try again!\n";
}

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
