#pragma once
#include <fstream>
#include "player.h"
#include "generation.h"
class Pattern;

class Bot : public Player {
private :
    int thinkingDepth = {3};
    int candidateSize = {5};
    int generationId = {0};

    int negamax (Cell &side, Board &board, int depth, int alpha, int beta) const;
    std::vector<std::tuple<int, int, int> > getCandidates (Cell &side, Board &board) const; 
    std::tuple<int, int, int> minimax (int depth, Board &board, bool takeTurn, int alpha, int beta) const;
public :
    void maxDifficulty () {
        thinkingDepth = 6;
        candidateSize = 20;
    }
    int myOpenTwoWeight = {(int) 2e4};
    int myClosedThreeWeight = {(int) 1e5 / 2};
    int myOpenThreeWeight = {(int) 2e5};
    int myClosedBrokenThreeWeight = {(int) 1e5 / 2};
    int myOpenBrokenThreeWeight = {(int) 2e5};

    int opOpenTwoWeight = {(int) 1e3};
    int opClosedThreeWeight = {(int) 1e4};
    int opOpenThreeWeight = {(int) 1e5};
    int opClosedFourWeight = {(int) 1e5};   
    int opBrokenFourWeight = {(int) 1.1e5};
    int opClosedBrokenThreeWeight = {(int) 1e4};
    int opOpenBrokenThreeWeight = {(int) 1e5};

    Bot (int id) 
    : generationId {id} {
        std::ifstream fin("data/botGeneration" + std::to_string(id) + ".txt");
        if (!fin.is_open()) {
            return;
        }
        std::string tmp;
        fin >> tmp >> myOpenTwoWeight;
        fin >> tmp >> myClosedThreeWeight;
        fin >> tmp >> myOpenThreeWeight;
        fin >> tmp >> myClosedBrokenThreeWeight;
        fin >> tmp >> myOpenBrokenThreeWeight;
        fin >> tmp >> opOpenTwoWeight;
        fin >> tmp >> opClosedThreeWeight;
        fin >> tmp >> opOpenThreeWeight;
        fin >> tmp >> opClosedFourWeight;
        fin >> tmp >> opBrokenFourWeight;
        fin >> tmp >> opClosedBrokenThreeWeight;
        fin >> tmp >> opOpenBrokenThreeWeight;
        fin.close();
    }
    Bot (const Bot &anotherBot) = default;
    int getId () const {
        return generationId;
    }
    void increaseDepth () {
        // thinkingDepth += 2;
        // candidateSize -= 8;
    }
    int evaluateMySide (Cell &type, Board &board) const;
    int evaluate (Cell &myType, Board &board) const;
    virtual void makeMove (Board &board) const;
    void mutate (float mutateRate, float mutateRange);
    ~Bot () {        
        std::string fileName = "botGeneration" + std::to_string(generationId) + ".txt";
        std::ofstream fout("data/" + fileName);
        fout << "myOpenTwoWeight: " << myOpenTwoWeight << "\n";
        fout << "myClosedThreeWeight: " << myClosedThreeWeight << "\n";
        fout << "myOpenThreeWeight: " << myOpenThreeWeight << "\n";
        fout << "myClosedBrokenThreeWeight: " << myClosedBrokenThreeWeight << "\n";
        fout << "myOpenBrokenThreeWeight: " << myOpenBrokenThreeWeight << "\n";
        fout << "opOpenTwoWeight: " << opOpenTwoWeight << "\n";
        fout << "opClosedThreeWeight: " << opClosedThreeWeight << "\n";
        fout << "opOpenThreeWeight: " << opOpenThreeWeight << "\n";
        fout << "opClosedFourWeight: " << opClosedFourWeight << "\n";
        fout << "opBrokenFourWeight: " << opBrokenFourWeight << "\n";
        fout << "opClosedBrokenThreeWeight: " << opClosedBrokenThreeWeight << "\n";
        fout << "opOpenBrokenThreeWeight: " << opOpenBrokenThreeWeight << "\n";
        fout.close();
    }
};

std::vector<Bot> loader ();
