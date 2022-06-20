#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <cstdlib>

void MainMenu();

int chartoint(char &c);
int power(int x, int y);
int strtoint(std::string strin);
int count_numbers ( int num);
std::string fixInput();
void clear_terminal();

enum class GameMode
{
    DEBUG,
    EASY,
    NORMAL,
    HARD
};

enum class GameState
{
    RUNNING,
    FINISHED_WIN,
    FINISHED_LOSS
};

struct Field
{
    bool hasMine;
    bool hasFlag;
    bool isRevealed;
};

class MinesweeperBoard{
public:
    MinesweeperBoard(int width_in, int height_in, GameMode mode);
    void debug_display() const;

    int getBoardWidth() const;
    int getBoardHeight() const;
    int getMineCount() const;

	bool insideOfBoard(int row, int col) const;
	void toggleBit(bool &bit);
	bool isRevealed(int row, int col) const;
    bool hasFlag(int row, int col) const;

    bool hasMine(int row, int col) const;

	int countMines(int row, int col) const;

	char getFieldInfo(int row, int col) const;

    void toggleFlag(int row, int col);
    void revealField(int row, int col);

    GameState getGameState() const;

    int getMineCountView() const;

private:
	GameMode game_mode;
	GameState gameState;
    std::vector<std::vector<Field>> board;
    int width;
    int height;
	int number_of_mines;

    int markedMines;
    int countAllFlags;
};

class MSBoardTextView{
    private:

    public:
        void display();
        MinesweeperBoard &board;
};

class MSTextController{
    private:
        bool checkAdj(int i, int j);
    public:
        void play();
        MinesweeperBoard &board;
        MSBoardTextView &view;


};
