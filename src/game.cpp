#include "game.h"
#include <iostream>
#include <limits>

Game::Game() : currentPlayerIdx(0), direction(1)
{
    // 1���࣬3AI
    players.push_back(new Player("���(��)"));
    players.push_back(new AIPlayer("AIһ��"));
    players.push_back(new AIPlayer("AI����"));
    players.push_back(new AIPlayer("AI����"));
    initDeal();
    // ���ֵ�һ�ų����ƣ�����������+4��
    do {
        topCard = deck.draw();
    } while (topCard.type == CardType::WildDrawFour);
}

Game::~Game()
{
    for (auto p : players) delete p;
}

void Game::initDeal()
{
    // ÿ��7��
    for (int r = 0; r < 7; ++r)
    {
        for (auto p : players)
        {
            p->drawCard(deck.draw());
        }
    }
}

void Game::nextPlayer()
{
    currentPlayerIdx += direction;
    if (currentPlayerIdx >= (int)players.size())
        currentPlayerIdx = 0;
    if (currentPlayerIdx < 0)
        currentPlayerIdx = (int)players.size() - 1;
}

Card Game::getTopCard() const
{
    return topCard;
}

void Game::setTopCard(const Card& c)
{
    topCard = c;
}

Player* Game::getCurrentPlayer()
{
    return players[currentPlayerIdx];
}

void Game::executeCardEffect(const Card& played)
{
    switch (played.type)
    {
    case CardType::Skip:
        nextPlayer();
        break;
    case CardType::Reverse:
        direction *= -1;
        break;
    case CardType::DrawTwo:
    {
        nextPlayer();
        Player* victim = getCurrentPlayer();
        victim->drawCard(deck.draw());
        victim->drawCard(deck.draw());
        break;
    }
    case CardType::WildDrawFour:
    {
        nextPlayer();
        Player* victim = getCurrentPlayer();
        for (int i = 0; i < 4; ++i)
            victim->drawCard(deck.draw());
        break;
    }
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
    std::cout << "===== UNO ����̨��Ϸ��ʼ =====\n";
    while (!checkWin())
    {
        Player* cur = getCurrentPlayer();
        std::cout << "\n------------------------------------\n";
        std::cout << "��ǰ�����ƣ�" << topCard.toString() << "\n";
        std::cout << "�ֵ���" << cur->getName() << "\n";

        // ��������߼�
        if (cur->getName() == "���(��)")
        {
            std::cout << "������ƣ�\n";
            cur->showHand();
            std::cout << "������������������±���ƣ�-1 ��һ����\n";
            int op;
            while (!(std::cin >> op))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "�������֣��������룺";
            }
            if (op == -1)
            {
                Card newCard = deck.draw();
                cur->drawCard(newCard);
                std::cout << "��鵽��" << newCard.toString() << "\n";
            }
            else
            {
                auto& hand = cur->getHand();
                if (op < 0 || op >= (int)hand.size() || !hand[op].canPlayOn(topCard))
                {
                    std::cout << "�޷���������ƣ������غ�\n";
                }
                else
                {
                    Card out = cur->playCard(op);
                    std::cout << "������" << out.toString() << "\n";
                    setTopCard(out);
                    executeCardEffect(out);
                    if (cur->getHandCount() == 1)
                        std::cout << "�㺰UNO��\n";
                }
            }
        }
        else
        {
            // AI�߼�
            AIPlayer* ai = dynamic_cast<AIPlayer*>(cur);
            int idx = ai->chooseCard(topCard);
            if (idx != -1)
            {
                Card out = ai->playCard(idx);
                std::cout << ai->getName() << "�����" << out.toString() << "\n";
                setTopCard(out);
                executeCardEffect(out);
                if (ai->getHandCount() == 1)
                    std::cout << ai->getName() << "��UNO��\n";
            }
            else
            {
                Card draw = deck.draw();
                ai->drawCard(draw);
                std::cout << ai->getName() << "���ƿɳ�����һ��\n";
            }
        }
        nextPlayer();
    }
    // ����ʤ����
    for (auto p : players)
    {
        if (p->getHandCount() == 0)
        {
            std::cout << "\n��Ϸ������ʤ���ߣ�" << p->getName() << "\n";
            break;
        }
    }
}