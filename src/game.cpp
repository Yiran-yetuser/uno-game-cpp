#include "game.h"
#include <iostream>
#include <limits>
#include <random>

Game::Game()
    : currentPlayerIdx(0), direction(1), drawStack(0)
{
    // 1人类 3AI
    players.push_back(new Player("玩家(你)"));
    players.push_back(new AIPlayer("AI一号"));
    players.push_back(new AIPlayer("AI二号"));
    players.push_back(new AIPlayer("AI三号"));

    initDeal();
    // 翻开局第一张，必须是数字牌
    do {
        topCard = deck.draw();
    } while (topCard.type != CardType::Number);
    currentColor = topCard.color;

    // 随机决定先手
    randomFirstPlayer();
}

Game::~Game()
{
    for (auto p : players) delete p;
}

void Game::initDeal()
{
    // 每人7张
    for (int r = 0; r < 7; ++r)
    {
        for (auto p : players)
            p->drawCard(deck.draw());
    }
}

void Game::randomFirstPlayer()
{
    std::mt19937 rng((unsigned)time(nullptr));
    std::uniform_int_distribution<> dist(0, 3);
    currentPlayerIdx = dist(rng);
}

bool Game::playerHasColor(Player* p, Color target) const
{
    for (auto& c : p->getHand())
    {
        if (c.color == target)
            return true;
    }
    return false;
}

bool Game::playerHasDrawTwo(Player* p) const
{
    for (auto& c : p->getHand())
    {
        if (c.type == CardType::DrawTwo)
            return true;
    }
    return false;
}

bool Game::canPlayCard(Player* p, const Card& play) const
{
    // 万能、万能+4随时可出
    if (play.type == CardType::Wild || play.type == CardType::WildDrawFour)
        return true;

    // 有叠加罚牌时
    if (drawStack > 0)
    {
        // 已有+4叠加：只能出万能+4，+2不能叠+4
        if (drawStack >= 4)
        {
            return play.type == CardType::WildDrawFour;
        }
        // 只有+2叠加：可继续叠+2
        else
        {
            return play.type == CardType::DrawTwo;
        }
    }

    // 无叠加罚牌，正常匹配规则
    if (play.color == currentColor)
        return true;
    if (play.type == topCard.type)
        return true;
    if (play.type == CardType::Number && topCard.type == CardType::Number && play.number == topCard.number)
        return true;
    return false;
}

void Game::nextPlayer()
{
    currentPlayerIdx += direction;
    if (currentPlayerIdx >= (int)players.size())
        currentPlayerIdx = 0;
    if (currentPlayerIdx < 0)
        currentPlayerIdx = (int)players.size() - 1;
}

Card Game::getTopCard() const { return topCard; }
Color Game::getCurrentColor() const { return currentColor; }
void Game::setTopCard(const Card& c) { topCard = c; }
void Game::setCurrentColor(Color c) { currentColor = c; }
Player* Game::getCurrentPlayer() { return players[currentPlayerIdx]; }

void Game::executeEffect(const Card& played)
{
    switch (played.type)
    {
    case CardType::Skip:
        // 跳过：下一位直接无法行动
        nextPlayer();
        std::cout << "触发跳过，下一位玩家本轮不能行动！\n";
        break;
    case CardType::Reverse:
        direction *= -1;
        std::cout << "出牌顺序反转！\n";
        break;
    case CardType::DrawTwo:
        drawStack += 2;
        std::cout << "+2叠加，当前待罚牌数量：" << drawStack << "\n";
        break;
    case CardType::WildDrawFour:
        drawStack += 4;
        std::cout << "万能+4叠加，当前待罚牌数量：" << drawStack << "\n";
        break;
    default: break;
    }
}

bool Game::checkWin() const
{
    for (auto p : players)
    {
        if (p->getHandCount() == 0)
            return true;
    }
    return false;
}

void Game::gameLoop()
{
    std::cout << "===== UNO游戏开始 =====" << std::endl;
    while (!checkWin())
    {
        Player* cur = getCurrentPlayer();
        std::cout << "\n================================" << std::endl;
        std::cout << "场上有效颜色：";
        switch (currentColor)
        {
        case Color::Red: std::cout << "红色"; break;
        case Color::Yellow: std::cout << "黄色"; break;
        case Color::Green: std::cout << "绿色"; break;
        case Color::Blue: std::cout << "蓝色"; break;
        default: std::cout << "万能";
        }
        std::cout << "\n场上底牌：" << topCard.toString() << std::endl;
        if (drawStack > 0)
            std::cout << "待叠加罚牌总数：" << drawStack << std::endl;

        // 打印所有AI剩余手牌数量
        std::cout << "\n各AI手牌剩余：" << std::endl;
        for (int i = 0; i < (int)players.size(); i++)
        {
            if (players[i]->getName() != "玩家(你)")
            {
                std::cout << players[i]->getName() << "：" << players[i]->getHandCount() << "张" << std::endl;
            }
        }
        std::cout << "轮到：" << cur->getName() << std::endl;

        // 有叠加罚牌，必须处理抽牌
        if (drawStack > 0)
        {
            bool canStack = false;
            if (drawStack < 4)
                canStack = playerHasDrawTwo(cur);
            // +4叠加只能用万能+4叠
            if (drawStack >= 4)
            {
                for (auto& c : cur->getHand())
                {
                    if (c.type == CardType::WildDrawFour)
                    {
                        canStack = true;
                        break;
                    }
                }
            }

            if (!canStack)
            {
                std::cout << "无法叠加，抽取" << drawStack << "张牌，本轮跳过！" << std::endl;
                for (int i = 0; i < drawStack; i++)
                    cur->drawCard(deck.draw());
                drawStack = 0;
                nextPlayer();
                continue;
            }
        }

        // 人类玩家回合
        if (cur->getName() == "玩家(你)")
        {
            while (true)
            {
                std::cout << "\n你的手牌列表：" << std::endl;
                cur->showHand();
                std::cout << "请输入手牌序号出牌；输入-1抽一张牌：";
                int op;
                if (!(std::cin >> op))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "输入非法数字，请重新输入！\n";
                    continue;
                }

                if (op == -1)
                {
                    Card newCard = deck.draw();
                    cur->drawCard(newCard);
                    std::cout << "你抽到：" << newCard.toString() << "，回合结束" << std::endl;
                    break;
                }

                auto& hand = cur->getHand();
                if (op < 0 || op >= (int)hand.size())
                {
                    std::cout << "序号不存在，请重新输入！\n";
                    continue;
                }

                if (!canPlayCard(cur, hand[op]))
                {
                    std::cout << "这张牌不符合出牌规则，不能打出，请重新选择！\n";
                    continue;
                }

                // 合法出牌
                Card out = cur->playCard(op);
                std::cout << "你打出：" << out.toString() << std::endl;
                setTopCard(out);

                // 万能牌/万能+4手动选颜色
                if (out.type == CardType::Wild || out.type == CardType::WildDrawFour)
                {
                    int sel;
                    std::cout << "请选择下一轮颜色：1红 2黄 3绿 4蓝" << std::endl;
                    std::cin >> sel;
                    switch (sel)
                    {
                    case 1: setCurrentColor(Color::Red); break;
                    case 2: setCurrentColor(Color::Yellow); break;
                    case 3: setCurrentColor(Color::Green); break;
                    case 4: setCurrentColor(Color::Blue); break;
                    default: std::cout << "输入错误，默认红色"; setCurrentColor(Color::Red);
                    }
                }
                else
                {
                    setCurrentColor(out.color);
                }

                executeEffect(out);
                if (cur->getHandCount() == 1)
                    std::cout << "你只剩一张牌，记得喊UNO！" << std::endl;
                break;
            }
        }
        else
        {
            // AI回合
            AIPlayer* ai = dynamic_cast<AIPlayer*>(cur);
            int playIdx = -1;
            // 寻找第一张合法可出牌
            for (int i = 0; i < (int)ai->getHand().size(); i++)
            {
                if (canPlayCard(ai, ai->getHand()[i]))
                {
                    playIdx = i;
                    break;
                }
            }

            if (playIdx == -1)
            {
                Card draw = deck.draw();
                ai->drawCard(draw);
                std::cout << ai->getName() << "无牌可出，抽一张牌" << std::endl;
            }
            else
            {
                Card out = ai->playCard(playIdx);
                std::cout << ai->getName() << "打出：" << out.toString() << std::endl;
                setTopCard(out);

                // AI万能牌根据手牌最多的颜色选择
                if (out.type == CardType::Wild || out.type == CardType::WildDrawFour)
                {
                    int cntR = 0, cntY = 0, cntG = 0, cntB = 0;
                    for (auto& c : ai->getHand())
                    {
                        switch (c.color)
                        {
                        case Color::Red: cntR++; break;
                        case Color::Yellow: cntY++; break;
                        case Color::Green: cntG++; break;
                        case Color::Blue: cntB++; break;
                        default: break;
                        }
                    }
                    Color best = Color::Red;
                    int maxCnt = cntR;
                    if (cntY > maxCnt) { maxCnt = cntY; best = Color::Yellow; }
                    if (cntG > maxCnt) { maxCnt = cntG; best = Color::Green; }
                    if (cntB > maxCnt) { maxCnt = cntB; best = Color::Blue; }
                    setCurrentColor(best);
                    std::cout << ai->getName() << "选择新颜色" << std::endl;
                }
                else
                {
                    setCurrentColor(out.color);
                }
                executeEffect(out);
                if (ai->getHandCount() == 1)
                    std::cout << ai->getName() << "喊UNO！" << std::endl;
            }
        }
        nextPlayer();
    }

    // 结算胜利者
    std::cout << "\n===== 游戏结束 =====" << std::endl;
    for (auto p : players)
    {
        if (p->getHandCount() == 0)
        {
            std::cout << "胜利者：" << p->getName() << std::endl;
            break;
        }
    }
}