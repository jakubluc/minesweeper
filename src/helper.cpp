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
