#include <SFML/Graphics.hpp>
#include "player.h"
#include "board.h"
#include "cell.h"
#include "bot.h"
#include <optional>
#include <iostream>
#include <cassert>
#include <iostream>
#include <random> 
#include "random.h"
#include "generation.h"
#include <ctime>
#include "match.h"


void training(int cycles) {
    Match match;
    std::vector<Bot> hallOfFame = loader();
    std::vector<Bot> baselineBot = {Bot(1665), Bot(1530), Bot(1437), Bot(721), Bot(648), Bot(481)};
    std::vector<Bot> newGeneration;
    const int newBotCnt = 40;
    const int hallOfFameSize = 10;
    const int hallOfFameMatches = 2;
    const int randomBotMatches = 3;
    for (int i = 0; i < cycles; i++) {
        std::cout << "Training cycle " << i + 1 << " / " << cycles << "\n";
        newGeneration = hallOfFame; 
        for (int i = 0; i < newBotCnt; i++) {
            int idx = randomInt(0, (int)hallOfFame.size() - 1);
            Bot newBot = hallOfFame[idx];
            newBot.mutate(0.08f, 0.08f); 
            newGeneration.push_back(newBot);
        }
        std::vector<Bot> opponentsPool = baselineBot;
        for (int i = 0; i < hallOfFame.size() && i < hallOfFameMatches; i++) {
            opponentsPool.push_back(hallOfFame[i]);
        }
        for (int i = 0; i < randomBotMatches; i++) {
            int idx = randomInt(0, (int)newGeneration.size() - 1);
            opponentsPool.push_back(newGeneration[idx]);
        }
        std::cout << opponentsPool.size() << " opponents in pool.\n"; 
        auto fitness = [&] (Bot &bot) {
            int score = 0;
            for (auto &opponent : opponentsPool) {
                int delta = 0;
                bot.setType(Cell::X);
                opponent.setType(Cell::O);
                Cell winner = match.botVsBot(bot, opponent);
                if (winner == Cell::X) delta += 1;
                else if (winner == Cell::Empty) delta += 0;
                else delta--;
                // std::cerr << "Match completed between Bot " << bot.getId() << " and Bot " << opponent.getId() << "\n";
                // std::cerr << (winner == Cell::X) << "\n";
                // std::cerr << "Scores: Bot " << bot.getId() << " = " << score << "\n";
                bot.setType(Cell::O);
                opponent.setType(Cell::X);
                winner = match.botVsBot(bot, opponent);
                if (winner == Cell::O) delta += 1;
                else if (winner == Cell::Empty) delta += 0;
                else delta--;

                if (delta > 0) score += 3;
                else if (delta == 0) score += 1;
                // std::cerr << "Match completed between Bot " << bot.getId() << " and Bot " << opponent.getId() << "\n";
                // std::cerr << (winner == Cell::X) << "\n";
                // std::cerr << "Scores: Bot " << bot.getId() << " = " << score << "\n";
            }
            return score;
        };
        // std::reverse(newGeneration.begin(), newGeneration.end());
        std::vector<std::pair<int, int>> scoredBots;
        for (int i = 0; i < newGeneration.size(); i++) {
            std::cout << "Evaluating Bot Generation " << newGeneration[i].getId() << "\n";
            int tmp = fitness(newGeneration[i]);
            std::cout << "Score: " << tmp << "\n";
            scoredBots.emplace_back(-tmp, i);
        }
        std::sort(scoredBots.begin(), scoredBots.end());
        hallOfFame.clear();
        for (int i = 0; i < hallOfFameSize && i < scoredBots.size(); i++) {
            hallOfFame.push_back(newGeneration[scoredBots[i].second]);
        }
        std::ofstream fout ("data/hallOfFame.txt");
        fout << generationCounter << "\n";
        fout << hallOfFame.size() << "\n";
        for (auto &bot : hallOfFame) {
            fout << bot.getId() << "\n";
        }
        fout.close();
    }
}

int main() {
    // std::cerr << "Please select mode:\n";
    // std::cerr << "1. Human vs Human\n";
    // std::cerr << "2. Human vs Bot\n";
    // std::cerr << "3. Training\n";
    int mode = 2; 
    // std::cin >> mode;
    
    if (mode == 1) {
        Match match;
        match.humanVsHuman();
        return 0;
    } else if (mode == 2) {
        int genId = 1875;
        // std::cerr << "Please enter Bot type (X goes first / O goes second) [X/O]: ";
        // char botType; std::cin >> botType;
        Bot bot(genId);
        Match match;
        bot.setType(Cell::X);
        bot.maxDifficulty();
        match.humanVsBot(bot);
        return 0;
    } else if (mode == 3) {
        std::cerr << "Please enter number of training cycles: ";
        int cycles; std::cin >> cycles;
        training(cycles);
        return 0;
    }
    // match.botVsBot(bot1, bot2);
}
