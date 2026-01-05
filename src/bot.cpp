#include "bot.h"
#include <vector>
#include <tuple>

const int inf = 1e9;

int Bot::evaluate (Cell &myType, Board &board) const {
    auto someoneWins = board.checkWinner();
    if (someoneWins != Cell::Empty) {
        return someoneWins == myType ? inf : -inf;
    }
    Cell opType = (myType == Cell::X) ? Cell::O : Cell::X;
    Pattern myPattern = board.getPattern(myType);
    Pattern opPattern = board.getPattern(opType);
    if (myPattern.closedFour || myPattern.openFour || myPattern.brokenFour) return inf - 1;
    if (opPattern.openFour) return -inf + 1;
    // if (myPattern.openThree && (!opPattern.closedFour) && (!opPattern.brokenFour)) return inf - 1;
    // if (opPattern.closedFour + opPattern.openThree >= 2) return -inf + 1;
    int myScore = 0;
    myScore += myPattern.openTwo * myOpenTwoWeight;
    myScore += myPattern.closedThree * myClosedThreeWeight;
    myScore += myPattern.openThree * myOpenThreeWeight;
    myScore += myPattern.closedBrokenThree * myClosedBrokenThreeWeight;
    myScore += myPattern.openBrokenThree * myOpenBrokenThreeWeight;
    int opScore = 0;
    opScore += opPattern.openTwo * opOpenTwoWeight;
    opScore += opPattern.openThree * opOpenThreeWeight;
    opScore += opPattern.closedThree * opClosedThreeWeight;
    opScore += opPattern.closedFour * opClosedFourWeight;
    opScore += opPattern.brokenFour * opBrokenFourWeight;
    opScore += opPattern.closedBrokenThree * opClosedBrokenThreeWeight;
    opScore += opPattern.openBrokenThree * opOpenBrokenThreeWeight;
    return myScore - opScore;
}

void Bot::mutate (float mutateRate, float mutateRange) {
    generationId = ++generationCounter;
    if (randomFloat(0.0f, 1.0f) < mutateRate) {
        myOpenTwoWeight += (int)(randomFloat(-mutateRange, mutateRange) * myOpenTwoWeight);
    }
    if (randomFloat(0.0f, 1.0f) < mutateRate) {
        myClosedThreeWeight += (int)(randomFloat(-mutateRange, mutateRange) * myClosedThreeWeight);
    }
    if (randomFloat(0.0f, 1.0f) < mutateRate) {
        myOpenThreeWeight += (int)(randomFloat(-mutateRange, mutateRange) * myOpenThreeWeight);
    }
    if (randomFloat(0.0f, 1.0f) < mutateRate) {
        myClosedBrokenThreeWeight += (int)(randomFloat(-mutateRange, mutateRange) * myClosedBrokenThreeWeight);
    }
    if (randomFloat(0.0f, 1.0f) < mutateRate) {
        myOpenBrokenThreeWeight += (int)(randomFloat(-mutateRange, mutateRange) * myOpenBrokenThreeWeight);
    }
    if (randomFloat(0.0f, 1.0f) < mutateRate) {
        opOpenTwoWeight += (int)(randomFloat(-mutateRange, mutateRange) * opOpenTwoWeight);
    }
    if (randomFloat(0.0f, 1.0f) < mutateRate) {
        opClosedThreeWeight += (int)(randomFloat(-mutateRange, mutateRange) * opClosedThreeWeight);
    }
    if (randomFloat(0.0f, 1.0f) < mutateRate) {
        opOpenThreeWeight += (int)(randomFloat(-mutateRange, mutateRange) * opOpenThreeWeight);
    }
    if (randomFloat(0.0f, 1.0f) < mutateRate) {
        opClosedFourWeight += (int)(randomFloat(-mutateRange, mutateRange) * opClosedFourWeight);
    }
    if (randomFloat(0.0f, 1.0f) < mutateRate) {
        opBrokenFourWeight += (int)(randomFloat(-mutateRange, mutateRange) * opBrokenFourWeight);
    }
    if (randomFloat(0.0f, 1.0f) < mutateRate) {
        opClosedBrokenThreeWeight += (int)(randomFloat(-mutateRange, mutateRange) * opClosedBrokenThreeWeight);
    }
    if (randomFloat(0.0f, 1.0f) < mutateRate) {
        opOpenBrokenThreeWeight += (int)(randomFloat(-mutateRange, mutateRange) * opOpenBrokenThreeWeight);
    }
}

std::vector<std::tuple<int, int, int> > Bot::getCandidates (Cell &side, Board &board) const {
    Cell otherSide = side == Cell::X ? Cell::O : Cell::X;
    std::vector<std::tuple<int, int, int> > moveCandidates;
    for (int row = 0; row < size; row++) for (int col = 0; col < size; col++) {
        if (board.getCell(row, col) != Cell::Empty) continue;
        bool adjNotEmpty = false;
        int range = 1j;
        for (int x = row - range; x <= row + range; x++) for (int y = col - range; y <= col + range; y++) {
            // if (abs(x - row) + abs(y - col) > range) continue;
            if (board.inBoard(x, y) && board.getCell(x, y) != Cell::Empty) {
                adjNotEmpty = true;
                break;
            }
        }
        if (adjNotEmpty == false) continue;
        board.setCell(row, col, side);
        int score = -evaluate(otherSide, board);
        moveCandidates.emplace_back(score, row, col);
        board.rollBack();
    }
    std::sort(moveCandidates.begin(), moveCandidates.end());
    reverse(moveCandidates.begin(), moveCandidates.end());
    while (moveCandidates.size() > candidateSize) {
        moveCandidates.pop_back();
    }
    return moveCandidates;
}

int Bot::negamax (Cell &side, Board &board, int depth, int alpha = -1e9, int beta = 1e9) const {
    int curScore = evaluate(side, board);
    if (abs(curScore) >= inf / 10) {
        return curScore;
    }
    if (board.isDraw()) {
        return 0;
    }
    if (depth == thinkingDepth) {
        return curScore;
    }
    Cell otherSide = side == Cell::X ? Cell::O : Cell::X;
    auto moveCandidates = getCandidates(side, board);
    int bestValue = -inf;
    for (auto &[score, x, y] : moveCandidates) {
        board.setCell(x, y, side);
        bestValue = std::max(bestValue, -negamax(otherSide, board, depth + 1, -beta, -alpha));
        board.rollBack();
        alpha = std::max(alpha, bestValue);
        if (alpha >= beta) break;
    }
    return bestValue;
}

void Bot::makeMove (Board &board) const {
    Board nBoard = board;
    Pattern pattern = nBoard.getPattern(getMyType());
    // pattern.print();
    Cell myType = getMyType();
    Cell opType = getOpType();
    auto moveCandidates = getCandidates(myType, nBoard);
    // std::cout << evaluate(myType, board) << "\n";
    int best = -inf - 1, traceX = -1, traceY = -1;
    for (auto &[score, x, y] : moveCandidates) {
        nBoard.setCell(x, y, myType);
        if (nBoard.checkWinner() == myType) {
            // std::cout << "!!!\n";
            Player::makeMove(board, x, y);
            return;
        }
        int tmp = -negamax(opType, nBoard, 1);
        
        if (best < tmp) {
            best = tmp;
            traceX = x;
            traceY = y;
        }
        nBoard.rollBack();
    }
    assert(traceX != -1);
    Player::makeMove(board, traceX, traceY);
}

std::vector<Bot> loader () {
    std::ifstream fin("data/hallOfFame.txt");
    fin >> generationCounter;
    int botCnt; fin >> botCnt;
    std::vector<int> idxList(botCnt);
    for (auto &idx : idxList) {
        fin >> idx;
    }
    for (auto idx : idxList) {
        std::cout << idx << "\n";
    }
    fin.close();
    std::vector<Bot> bots;
    for (auto idx : idxList) {
        std::string fileName = "botGeneration" + std::to_string(idx) + ".txt";
        std::ifstream fin("data/" + fileName);
        Bot bot(idx);
        std::string tmp;
        fin >> tmp >> bot.myOpenTwoWeight;
        fin >> tmp >> bot.myClosedThreeWeight;
        fin >> tmp >> bot.myOpenThreeWeight;
        fin >> tmp >> bot.myClosedBrokenThreeWeight;
        fin >> tmp >> bot.myOpenBrokenThreeWeight;
        fin >> tmp >> bot.opOpenTwoWeight;
        fin >> tmp >> bot.opClosedThreeWeight;
        fin >> tmp >> bot.opOpenThreeWeight;
        fin >> tmp >> bot.opClosedFourWeight;
        fin >> tmp >> bot.opBrokenFourWeight;
        fin >> tmp >> bot.opClosedBrokenThreeWeight;
        fin >> tmp >> bot.opOpenBrokenThreeWeight;
        fin.close();
        bots.push_back(bot);   
    }
    return bots;
}