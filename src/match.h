#include "bot.h"
#include "board.h"
#include <SFML/Graphics.hpp>

class Match {
    public : 
    Cell botVsBot (Bot bot1, Bot bot2);
    void humanVsHuman ();
    void humanVsBot (Bot bot);
};