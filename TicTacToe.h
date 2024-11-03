#include <string>
using namespace std;

class TicTacToe{
public:

    string gamestate;
    bool isXturn;

    TicTacToe(): gamestate("123456789"), isXturn(true){};
    
    int checkWin() {
        if (gamestate[0] == gamestate[1] && gamestate[1] == gamestate[2] && gamestate[0] != 0)
            return gamestate[0] == 'X' ? 1 : 2;
        if (gamestate[3] == gamestate[4] && gamestate[4] == gamestate[5] && gamestate[3] != 0)
            return gamestate[3] == 'X' ? 1 : 2;
        if (gamestate[6] == gamestate[7] && gamestate[7] == gamestate[8] && gamestate[6] != 0)
            return gamestate[6] == 'X' ? 1 : 2;

        if (gamestate[0] == gamestate[3] && gamestate[3] == gamestate[6] && gamestate[0] != 0)
            return gamestate[0] == 'X' ? 1 : 2;
        if (gamestate[1] == gamestate[4] && gamestate[4] == gamestate[7] && gamestate[1] != 0)
            return gamestate[1] == 'X' ? 1 : 2;
        if (gamestate[2] == gamestate[5] && gamestate[5] == gamestate[8] && gamestate[2] != 0)
            return gamestate[2] == 'X' ? 1 : 2;

        if (gamestate[0] == gamestate[4] && gamestate[4] == gamestate[8] && gamestate[0] != 0)
            return gamestate[0] == 'X' ? 1 : 2;
        if (gamestate[2] == gamestate[4] && gamestate[4] == gamestate[6] && gamestate[2] != 0)
            return gamestate[2] == 'X' ? 1 : 2;

        // No hay ganador
        return 0;
    }


    int playTurn(int position){
        if(to_string(gamestate[position-1]) == to_string(position)){
            cout << position << endl;
            cout << gamestate[position-1] << endl;
            return -1;
        }
        gamestate[position-1] = (isXturn) ? 'X' : 'O';
        isXturn = !isXturn;
        return checkWin();
    };

    void setGame(string newGameState){
        gamestate = newGameState;
    }

    void printGame(){
        cout << " " << gamestate[0] << " | " << gamestate[1] << " | " << gamestate[2] << endl;
        cout << "---+---+---\n";
        cout << " " << gamestate[3] << " | " << gamestate[4] << " | " << gamestate[5] << endl;
        cout << "---+---+---\n";
        cout << " " << gamestate[6] << " | " << gamestate[7] << " | " << gamestate[8] << endl;
    }
};