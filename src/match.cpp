#include "match.h"
#include "asset_path.h"


const int lineThickness = 3;
const int lineCnt = 15;
const int cellSize = 50;
const int gridSize = cellSize * (lineCnt + 1) + lineThickness * lineCnt;

const float radius = (float) cellSize * 0.35;
sf::Font font;


double pre = 0;
double isReady (double delayTime) {
    double cur = (double) clock() / CLOCKS_PER_SEC;
    if (cur - pre >= delayTime) {
        pre = cur;
        return true; 
    } 
    return false;
}

void loadFont () {
    if (!font.openFromFile(getAssetPath("Jetbrain.ttf"))) {
        std::cout << "Cannot load font\n";
    }
}

bool draw (sf::RenderWindow &window, Board &board, Bot &playerThisTurn, Bot &playerNextTurn) { // return whether the game is over or not 
    std::string _playerThisTurnName = playerThisTurn.getMyType() == Cell::X ? "Black" : "White";
    window.setTitle(_playerThisTurnName + "'s thinking...");
    
    sf::Text text({font, "", 18});
    text.setFillColor(sf::Color::Black);

    window.clear(sf::Color(244, 164, 96, 255));

    std::string genName1 = "Bot Generation " + std::to_string(playerThisTurn.getId());
    std::string genName2 = "Bot Generation " + std::to_string(playerNextTurn.getId());
    if (playerThisTurn.getMyType() == Cell::O) {
        std::swap(genName1, genName2);
    }
    sf::Text genText1({font, genName1, 30});
    genText1.setFillColor(sf::Color(51, 153, 255));
    genText1.setOutlineColor(sf::Color::Black);
    genText1.setOutlineThickness(3);
    genText1.setPosition({825, 20});
    sf::Text middle ({font, "vs", 30});
    middle.setFillColor(sf::Color::White);
    middle.setOutlineColor(sf::Color::Black);
    middle.setOutlineThickness(3);
    middle.setPosition({950, 50});
    window.draw(middle);
    window.draw(genText1);
    sf::Text genText2({font, genName2, 30});
    genText2.setFillColor(sf::Color{255, 51, 51});
    genText2.setOutlineColor(sf::Color::Black);
    genText2.setOutlineThickness(3);
    genText2.setPosition({825, 80});
    window.draw(genText2);


    sf::RectangleShape rollBackButton ({150, 50});
    rollBackButton.setFillColor(sf::Color::Green);
    rollBackButton.setPosition({850, 150});
    window.draw(rollBackButton);
    sf::Text rollBackText({font, "Move Back", 20});
    rollBackText.setFillColor(sf::Color::Black);
    rollBackText.setPosition({870, 165});
    window.draw(rollBackText);

    sf::RectangleShape horizontalLine({gridSize - cellSize * 2, lineThickness});
    sf::RectangleShape verticalLine({lineThickness, gridSize - cellSize * 2});
    horizontalLine.setFillColor(sf::Color::Black);
    verticalLine.setFillColor(sf::Color::Black);
    for (int i = 0; i < lineCnt; i++) {
        verticalLine.setPosition({(float)cellSize * (i + 1) + lineThickness * i, cellSize});
        horizontalLine.setPosition({cellSize, (float)cellSize * (i + 1) + lineThickness * i});
        window.draw(horizontalLine);
        window.draw(verticalLine);
        text.setString(char(int('a') + i));
        text.setPosition({(float)cellSize + (cellSize + lineThickness) * i - 4, 10});
        window.draw(text);
        int num = 15 - i;
        text.setString(char(int('0') + num % 10));
        text.setPosition({20, (float)cellSize + (cellSize + lineThickness) * i - 10});
        window.draw(text);
        if (num >= 10) {
            text.setString("1");
            text.setPosition({10, (float)cellSize + (cellSize + lineThickness) * i - 10});
            window.draw(text);
        }
    }
    for (int i = 0; i < lineCnt; i++) for (int j = 0; j < lineCnt; j++) {
        const int size = 7;
        sf::RectangleShape dot({size, size});
        dot.setFillColor(sf::Color::Black);
        float x = (float) cellSize + i * (cellSize + lineThickness) + (lineThickness + 1) / 2;
        float y = (float) cellSize + j * (cellSize + lineThickness) + (lineThickness + 1) / 2;
        dot.setPosition({x - size / 2 - 1, y - size / 2 - 1});
        window.draw(dot);
    }
    for (int i = 0; i < lineCnt; i++) for (int j = 0; j < lineCnt; j++) {
        if (board.getCell(i, j) == Cell::Empty) {
            continue;
        }
        sf::Color color = board.getCell(i, j) == Cell::X ? sf::Color::Black : sf::Color::White;
        float x = (float) cellSize + i * (cellSize + lineThickness) + (lineThickness + 1) / 2 - radius;
        float y = (float) cellSize + j * (cellSize + lineThickness) + (lineThickness + 1) / 2 - radius;
        sf::CircleShape circle;
        circle.setRadius(radius);
        circle.setPosition({x, y});
        circle.setFillColor(color);
        window.draw(circle);
    }

    auto [x, y] = board.getLastMove();
    if (board.inBoard(x, y)) {
        float xPos = (float) cellSize + x * (cellSize + lineThickness) + (lineThickness + 1) / 2;
        float yPos = (float) cellSize + y * (cellSize + lineThickness) + (lineThickness + 1) / 2;
        sf::CircleShape highlight;
        highlight.setRadius(radius / 2);
        highlight.setPosition({xPos - radius / 2, yPos - radius / 2});
        highlight.setFillColor(sf::Color::Red);
        window.draw(highlight);
    }

    auto tmp = board.checkWinner();
    std::string playerThisTurnName = "Black";
    std::string playerNextTurnName = "White";
    if (playerThisTurn.getMyType() == Cell::O) {
        std::swap(playerThisTurnName, playerNextTurnName);
    }
    // float scorePlayerThisTurn = playerThisTurn.evaluate(1, board);
    // float scorePlayerNextTurn = playerNextTurn.evaluate(0, board);
    // sf::Text scoreText({font, playerThisTurnName + "'s score: " + std::to_string(scorePlayerThisTurn), 15});
    // scoreText.setFillColor(sf::Color::Black);
    // scoreText.setPosition({850, 40});
    // window.draw(scoreText);
    // sf::Text _scoreText({font, playerNextTurnName + "'s score: " + std::to_string(scorePlayerNextTurn), 15});
    // _scoreText.setFillColor(sf::Color::Black);
    // _scoreText.setPosition({850, 80});
    // window.draw(_scoreText);
    if (tmp == Cell::Empty && !board.isDraw()) return false;
    if (board.isDraw()) {
        sf::RectangleShape cover ({(float) gridSize + 300, (float) gridSize});
        cover.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(cover);
        sf::Text _text({font, "DRAW !!!", 100});
        _text.setFillColor(sf::Color::Yellow);
        _text.setPosition({250, 350});
        window.draw(_text); 
        return true;
    }
    std::string winner = tmp == Cell::X ? "BLACK" : "WHITE";
    sf::RectangleShape cover ({(float) gridSize + 400, (float) gridSize});
    cover.setFillColor(sf::Color(0, 0, 0, 150));
    // window.draw(cover);
    sf::Text _text({font, winner + " WINS !!!", 100});
    _text.setFillColor(sf::Color::Yellow);
    _text.setPosition({120, 350});
    window.draw(_text); 
    return true;
}

void Match::humanVsBot (Bot bot) {
    loadFont();
    sf::RenderWindow window(sf::VideoMode({gridSize + 400, gridSize}), "SFML 3 window");
    Board board;
    if (bot.getMyType() == Cell::X) {
        board.setCell(15 / 2, 15 / 2, Cell::X);
    }
    Bot player(0);
    player.setType(bot.getOpType());
    bool isOver = false;
    bool botTurn = false;
    // std::cerr << (bot.getMyType() == Cell::O) << "\n";
    while (window.isOpen()) {
        isOver = draw(window, board, player, bot);
        if (!isOver && botTurn && isReady(0.0)) {
            bot.makeMove(board);
            isOver = (board.checkWinner() != Cell::Empty || board.isDraw());
            botTurn = false;
        }
        while (const auto event = window.pollEvent()) {
            if (event -> is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto * mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed -> button == sf::Mouse::Button::Left) {
                    int mX = mousePressed -> position.x;
                    int mY = mousePressed -> position.y;
                    if (isOver || botTurn) continue;
                    if (850 <= mX && mX <= 850 + 150 && 150 <= mY && mY <= 150 + 50) {
                        board.rollBack();
                        if (!board.rollBack()) board.setCell(15 / 2, 15 / 2, bot.getMyType());
                        isOver = false;
                    }
                    int cellX = -1, cellY = -1;
                    for (int i = 0; i < lineCnt; i++) for (int j = 0; j < lineCnt; j++) {
                        float x = (float) cellSize + i * (cellSize + lineThickness) + (lineThickness + 1) / 2;
                        float y = (float) cellSize + j * (cellSize + lineThickness) + (lineThickness + 1) / 2;
                        double distance = sqrt((x - mX) * (x - mX) + (y - mY) * (y - mY));
                        if (distance <= radius) {
                            cellX = i;
                            cellY = j;
                            break;
                        }
                    }
                    if (cellX == -1) continue;
                    if (board.getCell(cellX, cellY) != Cell::Empty) {
                        continue;
                    }
                    if (!botTurn) {
                        player.Player::makeMove(board, cellX, cellY);
                        // draw(window, board, player, bot);
                        isOver = (board.checkWinner() != Cell::Empty || board.isDraw());
                        botTurn = true;    
                    }
                }
            }
        }   
        window.clear();
        draw(window, board, player, bot);
        window.display();
        if (isOver) {
            while (true) {
                if (isReady(2)) {
                    window.close();
                    return;
                }
            }
        }
    }
}


Cell Match::botVsBot (Bot playerThisTurn, Bot playerNextTurn) { 
    loadFont();
    if (playerThisTurn.getMyType() == Cell::O) {
        std::swap(playerThisTurn, playerNextTurn);
    }
    playerNextTurn.increaseDepth();
    sf::RenderWindow window(sf::VideoMode({gridSize + 400, gridSize}), "SFML 3 window");
    Board board;
    std::vector<std::pair<int, int> > openingMoves = {
        {7, 7}, {7, 8}, {8, 7}, {8, 8}
    };
    int idx = randomInt(0, (int) openingMoves.size() - 1);
    board.setCell(openingMoves[idx].first, openingMoves[idx].second, playerThisTurn.getMyType());
    std::swap(playerThisTurn, playerNextTurn);
    bool isOver = false;
    while (window.isOpen()) {
        draw(window, board, playerThisTurn, playerNextTurn);
        while (const auto event = window.pollEvent()) {
            if (event -> is<sf::Event::Closed>()) {
                window.close();
            }
        }   
        if (!isOver && isReady(0.0)) {
            playerThisTurn.makeMove(board);
            std::swap(playerThisTurn, playerNextTurn);
            isOver = (board.checkWinner() != Cell::Empty || board.isDraw());
        }
        window.clear();
        draw(window, board, playerThisTurn, playerNextTurn);
        window.display();
        if (isOver) {
            while (true) {
                if (isReady(0.5)) {
                    window.close();
                    return board.checkWinner();
                }
            }
        }
    }
    return Cell::Empty;
}

void Match::humanVsHuman () {
    loadFont();
    sf::RenderWindow window(sf::VideoMode({gridSize + 300, gridSize}), "SFML 3 window");
    Board board;
    Bot playerThisTurn(1), playerNextTurn(2);
    playerThisTurn.setType(Cell::X);
    playerNextTurn.setType(Cell::O);
    while (window.isOpen()) {
        draw(window, board, playerThisTurn, playerNextTurn);
        while (const auto event = window.pollEvent()) {
            bool isOver = draw(window, board, playerThisTurn, playerNextTurn);
            if (event -> is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto * mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed -> button == sf::Mouse::Button::Left) {
                    int mX = mousePressed -> position.x;
                    int mY = mousePressed -> position.y;
                    if (850 <= mX && mX <= 850 + 150 && 150 <= mY && mY <= 150 + 50) {
                        if (board.rollBack()) {
                            std::swap(playerThisTurn, playerNextTurn);
                            continue;
                        }
                    }
                    if (isOver) continue;
                    int cellX = -1, cellY = -1;
                    for (int i = 0; i < lineCnt; i++) for (int j = 0; j < lineCnt; j++) {
                        float x = (float) cellSize + i * (cellSize + lineThickness) + (lineThickness + 1) / 2;
                        float y = (float) cellSize + j * (cellSize + lineThickness) + (lineThickness + 1) / 2;
                        double distance = sqrt((x - mX) * (x - mX) + (y - mY) * (y - mY));
                        if (distance <= radius) {
                            cellX = i;
                            cellY = j;
                            break;
                        }
                    }
                    // std::cerr << cellX << " " << cellY << "\n";
                    if (cellX == -1) continue;
                    if (board.getCell(cellX, cellY) != Cell::Empty) {
                        continue;
                    }
                    playerThisTurn.Player::makeMove(board, cellX, cellY);
                    std::swap(playerThisTurn, playerNextTurn);
                }
            }
        }   
        window.display();
    }
}