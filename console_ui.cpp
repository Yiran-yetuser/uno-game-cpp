// console_ui.cpp - 基于 src/ 现有类的控制台四方位 UNO
// 布局：AI一号在左，AI二号在上，AI三号在右，玩家在下
// 编译: cd build && cmake .. && cmake --build . --target uno-console
//
// 编码策略：使用系统默认 ANSI 代码页（中文 Windows = GBK）
// 所有非 ASCII 字符（中文 + box-drawing）都用 \xHH 十六进制转义

#include <algorithm>
#include <chrono>
#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

#include "ai_player.h"
#include "card.h"
#include "deck.h"
#include "player.h"

using namespace std;

// ========== 控制台常量 ==========
const int CON_W = 100;
const int CON_H = 34;
const int C_RST = 7;
const int C_RED = 12;
const int C_YELLOW = 14;
const int C_GREEN = 10;
const int C_BLUE = 11;
const int C_WHITE = 15;
const int C_GRAY = 8;
const int C_CYAN = 11;
const int C_PINK = 13;

HANDLE hCon;

// ========== 控制台工具 ==========
void setC(int c) { SetConsoleTextAttribute(hCon, c); }
void resetC() { SetConsoleTextAttribute(hCon, C_RST); }

void gotoxy(int x, int y) {
    if (x < 0) x = 0; if (y < 0) y = 0;
    if (x >= CON_W) x = CON_W - 1;
    if (y >= CON_H) y = CON_H - 1;
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hCon, pos);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO ci = {1, FALSE};
    SetConsoleCursorInfo(hCon, &ci);
}
void showCursor() {
    CONSOLE_CURSOR_INFO ci = {1, TRUE};
    SetConsoleCursorInfo(hCon, &ci);
}
void cls() { system("cls"); }
void sleepMs(int ms) { this_thread::sleep_for(chrono::milliseconds(ms)); }

void printAt(int x, int y, const string &s, int color = C_WHITE) {
    gotoxy(x, y);
    setC(color);
    cout << s;
    resetC();
}

string repeatStr(const string &s, int n) {
    string r;
    for (int i = 0; i < n; i++) r += s;
    return r;
}

// ========== 颜色映射 ==========
int colorCode(Color c) {
    switch (c) {
    case Color::Red:    return C_RED;
    case Color::Yellow: return C_YELLOW;
    case Color::Green:  return C_GREEN;
    case Color::Blue:   return C_BLUE;
    default:            return C_WHITE;
    }
}
string colorNameShort(Color c) {
    switch (c) {
    case Color::Red:    return "R";
    case Color::Yellow: return "Y";
    case Color::Green:  return "G";
    case Color::Blue:   return "B";
    default:            return "W";
    }
}

// ========== 绘图组件（用 \xHH 避免乱码）==========
//
// 全部框线字符都用 hex 字节写出 UTF-8 序列。
// GBK 引擎不会读 UTF-8 多字节，那只能走另一条路：
// 改用 ANSI（GBK）控制台支持的"半角" ASCII 框线
// =+, -, |, * < 这些字符。完全规避编码问题。

// 顶部/底部横线
static const string LINE_H  = "==========================================================";
static const string LINE_HL = "----------------------------------------------------------";

// 框线字符
static const string C_TL = "+";
static const string C_TR = "+";
static const string C_BL = "+";
static const string C_BR = "+";
static const string C_H  = "-";
static const string C_V  = "|";
static const string C_X  = "+";

// box drawing heavy corners
static const string H_TL = "+";
static const string H_TR = "+";
static const string H_BL = "+";
static const string H_BR = "+";
static const string H_H  = "=";
static const string H_V  = "||";

// 小卡牌背面  +--+
//              |..|
//              +--+
void drawMiniBack(int x, int y) {
    setC(C_GRAY);
    gotoxy(x, y);     cout << "+--+";
    gotoxy(x, y + 1); cout << "|..|";
    gotoxy(x, y + 2); cout << "+--+";
    resetC();
}

// 直接用 Card::toString() 返回的中文
// src/ 已改为 GBK 编码，MSVC/MinGW 用 GBK 编译
void drawPlayerCard(int x, int y, const Card &c, int idx, bool canPlay) {
    int cc = canPlay ? colorCode(c.color) : C_GRAY;
    string txt = c.toString();
    // GBK 中文字符占 2 字节（控制台列宽 1），所以按字符数截断
    if (txt.length() > 6) txt = txt.substr(0, 6);

    setC(cc);
    gotoxy(x, y);     cout << "+" << repeatStr("-", 8) << "+";

    gotoxy(x, y + 1);
    int pad = 8 - (int)txt.size();
    int leftPad = pad / 2;
    cout << "| " << repeatStr(" ", leftPad);
    setC(canPlay ? colorCode(c.color) : C_GRAY);
    cout << txt;
    setC(cc);
    cout << repeatStr(" ", pad - leftPad) << " |";

    gotoxy(x, y + 2); cout << "+" << repeatStr("-", 8) << "+";
    resetC();

    if (canPlay) {
        printAt(x + 2, y + 3, "[" + to_string(idx) + "]", C_GREEN);
    } else {
        printAt(x + 2, y + 3, " " + to_string(idx) + " ", C_GRAY);
    }
}

void drawFullScreen(
    const vector<Player *> &players,
    int curTurn,
    bool hasDrawn,
    int direction,
    int drawStack,
    const Card &topCard,
    Color currentColor)
{
    cls();
    int midX = CON_W / 2;

    // ===== 标题栏 =====
    setC(C_YELLOW);
    gotoxy(0, 0); cout << repeatStr(H_H, CON_W / 2);
    gotoxy(midX - 8, 0); cout << " * UNO 控制台 * ";
    resetC();

    string dirStr = (direction == 1) ? "顺时针 ?" : "逆时针 ?";
    printAt(CON_W - 18, 0, "方向: " + dirStr, C_CYAN);

    // ===== 上方: AI二号 =====
    printAt(midX - 6, 2, "【AI二号】", C_PINK);
    printAt(midX - 7, 3, "剩余: " + to_string(players[2]->getHandCount()) + " 张", C_GRAY);
    int ai2Cnt = min(players[2]->getHandCount(), 14);
    int ai2X = midX - (ai2Cnt * 4) / 2;
    for (int i = 0; i < ai2Cnt; i++) {
        drawMiniBack(ai2X + i * 4, 4);
    }

    // ===== 左方: AI一号 =====
    printAt(1, 6, "【AI一号】", C_PINK);
    printAt(1, 7, "剩余: " + to_string(players[1]->getHandCount()) + " 张", C_GRAY);
    int ai1Cnt = min(players[1]->getHandCount(), 10);
    for (int i = 0; i < ai1Cnt; i++) {
        drawMiniBack(1, 9 + i * 3);
    }

    // ===== 右方: AI三号 =====
    printAt(CON_W - 12, 6, "【AI三号】", C_PINK);
    printAt(CON_W - 12, 7, "剩余: " + to_string(players[3]->getHandCount()) + " 张", C_GRAY);
    int ai3Cnt = min(players[3]->getHandCount(), 10);
    for (int i = 0; i < ai3Cnt; i++) {
        drawMiniBack(CON_W - 5, 9 + i * 3);
    }

    // ===== 中央: 牌堆 =====
    int cx = midX - 12, cy = 9;

    setC(C_CYAN);
    gotoxy(cx, cy);     cout << "+===========+";
    gotoxy(cx, cy + 1); cout << "|  抽牌堆   |";
    gotoxy(cx, cy + 2); cout << "|  ~~~~~~   |";
    gotoxy(cx, cy + 3); cout << "|  ~~~~~~   |";
    gotoxy(cx, cy + 4); cout << "+===========+";
    resetC();

    int dx = cx + 15;
    int tc = colorCode(topCard.color);
    setC(tc);
    gotoxy(dx, cy);     cout << "+===========+";
    gotoxy(dx, cy + 1); cout << "| ";

    string topTxt = topCard.toString();
    setC(tc); cout << topTxt; resetC();
    int fill = max(0, 9 - (int)topTxt.size());
    cout << repeatStr(" ", fill);
    setC(tc); cout << " |";

    // 颜色标签：颜色全称（红/黄/绿/蓝）
    string colorName;
    switch (currentColor) {
    case Color::Red:    colorName = "红色"; break;
    case Color::Yellow: colorName = "黄色"; break;
    case Color::Green:  colorName = "绿色"; break;
    case Color::Blue:   colorName = "蓝色"; break;
    default:            colorName = "无色"; break;
    }
    gotoxy(dx, cy + 2);
    cout << "| 颜色:";
    setC(colorCode(currentColor));
    cout << colorName;
    resetC();
    int fill2 = max(0, 4 - (int)colorName.size());
    setC(tc);
    cout << repeatStr(" ", fill2) << " |";
    gotoxy(dx, cy + 3);
    cout << "+===========+";
    resetC();

    if (drawStack > 0) {
        setC(C_RED);
        gotoxy(cx, cy + 6);
        cout << "!! 罚牌叠加: +" << drawStack << " 张 !!";
        resetC();
    }

    // ===== 当前回合提示 =====
    if (curTurn == 0) {
        printAt(midX - 6, 18, ">> 你的回合 <<", C_GREEN);
    } else {
        string name = players[curTurn]->getName();
        if (name == "You") name = "你";
        else if (name == "AI-1") name = "AI一号";
        else if (name == "AI-2") name = "AI二号";
        else if (name == "AI-3") name = "AI三号";
        printAt(midX - 10, 18, "> " + name + " 正在出牌...", C_PINK);
    }

    // ===== 下方: 玩家手牌 =====
    Player *human = players[0];
    printAt(2, 20, "你的手牌 (共 " + to_string(human->getHandCount()) + " 张):", C_WHITE);

    auto &hand = human->getHand();
    int cardW = 10;
    int startX = 2;
    int cardsPerRow = (CON_W - startX) / cardW;

    for (int i = 0; i < (int)hand.size(); i++) {
        int row = i / cardsPerRow;
        int col = i % cardsPerRow;
        int x = startX + col * cardW;
        int y = 21 + row * 5;
        if (y >= CON_H - 5) break;

        bool cp = false;
        if (drawStack > 0) {
            if (drawStack >= 4)
                cp = (hand[i].type == CardType::WildDrawFour);
            else
                cp = (hand[i].type == CardType::DrawTwo || hand[i].type == CardType::WildDrawFour);
        } else {
            if (hand[i].type == CardType::Wild || hand[i].type == CardType::WildDrawFour)
                cp = true;
            else if (hand[i].color == currentColor)
                cp = true;
            else if (hand[i].type == topCard.type && hand[i].type != CardType::Number)
                cp = true;
            else if (hand[i].type == CardType::Number && topCard.type == CardType::Number && hand[i].number == topCard.number)
                cp = true;
        }
        drawPlayerCard(x, y, hand[i], i, cp);
    }

    // ===== 底部操作栏 =====
    int barY = CON_H - 5;
    setC(C_CYAN);
    gotoxy(2, barY); cout << repeatStr("-", CON_W - 4); resetC();

    printAt(2, barY + 1, "操作:", C_WHITE);
    printAt(8, barY + 1, "数字=出牌", C_GREEN);
    printAt(18, barY + 1, "d=抽牌", C_YELLOW);
    printAt(28, barY + 1, "p=结束", C_CYAN);
    printAt(38, barY + 1, "q=退出", C_RED);

    if (drawStack == 0 && hasDrawn) {
        printAt(2, barY + 2, "本回合已抽过牌, 请出牌或按 p 结束回合", C_RED);
    } else if (drawStack > 0) {
        printAt(2, barY + 2, "必须抽 " + to_string(drawStack) + " 张罚牌! (按 d 抽牌)", C_RED);
    }

    printAt(2, CON_H - 1, ">", C_YELLOW);
}

// ========== 弹窗 ==========
void toast(const string &msg, int color = C_WHITE, int wait = 800) {
    int len = (int)msg.length() + 4;
    int x = max(0, (CON_W - len) / 2);
    int y = CON_H / 2 - 1;
    setC(color);
    gotoxy(x, y);     cout << "+" << repeatStr("=", len - 2) << "+";
    gotoxy(x, y + 1); cout << "| " << msg << " |";
    gotoxy(x, y + 2); cout << "+" << repeatStr("=", len - 2) << "+";
    resetC();
    sleepMs(wait);
}

Color pickColor() {
    cls();
    printAt(CON_W / 2 - 8, CON_H / 2 - 3, "请选择颜色:", C_YELLOW);
    printAt(CON_W / 2 - 8, CON_H / 2 - 1, "  1 =", C_WHITE);
    setC(C_RED); cout << "红色"; resetC();
    printAt(CON_W / 2 - 8, CON_H / 2,     "  2 =", C_WHITE);
    setC(C_YELLOW); cout << "黄色"; resetC();
    printAt(CON_W / 2 - 8, CON_H / 2 + 1, "  3 =", C_WHITE);
    setC(C_GREEN);  cout << "绿色";  resetC();
    printAt(CON_W / 2 - 8, CON_H / 2 + 2, "  4 =", C_WHITE);
    setC(C_BLUE);   cout << "蓝色";   resetC();
    printAt(CON_W / 2 - 8, CON_H / 2 + 4, "请输入 [1-4]: ", C_WHITE);

    showCursor();
    while (true) {
        char ch = _getch();
        if (ch >= '1' && ch <= '4') {
            Color cols[] = {Color::Red, Color::Yellow, Color::Green, Color::Blue};
            hideCursor();
            return cols[ch - '1'];
        }
    }
}

bool canPlayCard(const Card &play, const Card &topCard, Color currentColor, int drawStack) {
    if (drawStack > 0) {
        if (drawStack >= 4) return play.type == CardType::WildDrawFour;
        else return play.type == CardType::DrawTwo || play.type == CardType::WildDrawFour;
    }
    if (play.type == CardType::Wild || play.type == CardType::WildDrawFour) return true;
    if (play.color == currentColor) return true;
    if (play.type == topCard.type && play.type != CardType::Number) return true;
    if (play.type == CardType::Number && topCard.type == CardType::Number && play.number == topCard.number) return true;
    return false;
}

bool playerHasType(Player *p, CardType t) {
    for (auto &c : p->getHand())
        if (c.type == t) return true;
    return false;
}

string readLine() {
    string result;
    while (true) {
        int ch = _getch();
        if (ch == 13 || ch == 10) { cout << endl; break; }
        else if (ch == 8 || ch == 127) {
            if (!result.empty()) { result.pop_back(); cout << "\b \b"; }
        }
        else if (ch >= 32 && ch < 127) { result += (char)ch; cout << (char)ch; }
    }
    return result;
}

int main() {
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hIn  = GetStdHandle(STD_INPUT_HANDLE);

    // src/ 已改为 GBK 编码，设置控制台代码页为 GBK
    SetConsoleOutputCP(936);
    SetConsoleCP(936);

    // 设置窗口大小
    COORD bufferSize = {(SHORT)CON_W, (SHORT)CON_H};
    SetConsoleScreenBufferSize(hCon, bufferSize);
    SMALL_RECT windowSize = {0, 0, (SHORT)(CON_W - 1), (SHORT)(CON_H - 1)};
    SetConsoleWindowInfo(hCon, TRUE, &windowSize);

    hideCursor();
    srand((unsigned)time(nullptr));

    // ---- 初始化 ----
    Deck deck;
    vector<Player *> players;
    players.push_back(new Player("You"));
    players.push_back(new AIPlayer("AI-1"));
    players.push_back(new AIPlayer("AI-2"));
    players.push_back(new AIPlayer("AI-3"));

    for (int r = 0; r < 7; ++r)
        for (auto p : players)
            p->drawCard(deck.draw());

    Card topCard = deck.draw();
    int retry = 0;
    while (topCard.type != CardType::Number && retry < 50) {
        deck.addDiscard(topCard);
        topCard = deck.draw();
        ++retry;
    }
    Color currentColor;
    if (topCard.type == CardType::Wild || topCard.type == CardType::WildDrawFour)
        currentColor = (Color)(rand() % 4);
    else
        currentColor = topCard.color;
    deck.addDiscard(topCard);

    int currentPlayerIdx = rand() % 4;
    int direction = 1;
    int drawStack = 0;
    bool gameOver = false;
    bool hasDrawnThisTurn = false;

    auto advanceTurn = [&]() {
        currentPlayerIdx += direction;
        if (currentPlayerIdx >= 4) currentPlayerIdx = 0;
        if (currentPlayerIdx < 0)  currentPlayerIdx = 3;
        hasDrawnThisTurn = false;
    };

    auto executeEffect = [&](const Card &played) {
        string pname = players[currentPlayerIdx]->getName();
        if (pname == "You") pname = "你";
        else if (pname == "AI-1") pname = "AI一号";
        else if (pname == "AI-2") pname = "AI二号";
        else if (pname == "AI-3") pname = "AI三号";
        switch (played.type) {
        case CardType::Skip:
            toast(pname + " 被跳过!", C_RED, 800); break;
        case CardType::Reverse:
            direction *= -1;
            toast("方向反转!", C_CYAN, 800); break;
        case CardType::DrawTwo:
            drawStack += 2;
            toast("+2 罚牌叠加!", C_RED, 800); break;
        case CardType::WildDrawFour:
            drawStack += 4;
            toast("+4 罚牌叠加!", C_RED, 800); break;
        default: break;
        }
    };

    if (currentPlayerIdx != 0) {
        drawFullScreen(players, currentPlayerIdx, hasDrawnThisTurn, direction, drawStack, topCard, currentColor);
        toast("AI 先出牌, 按任意键继续...", C_GRAY, 500);
        _getch();
    }

    while (!gameOver) {
        Player *cur = players[currentPlayerIdx];
        drawFullScreen(players, currentPlayerIdx, hasDrawnThisTurn, direction, drawStack, topCard, currentColor);

        if (currentPlayerIdx != 0) {
            sleepMs(900);

            if (drawStack > 0) {
                bool canStack = (drawStack >= 4)
                                    ? playerHasType(cur, CardType::WildDrawFour)
                                    : playerHasType(cur, CardType::DrawTwo);
                if (!canStack) {
                    string n = cur->getName();
                    if (n == "AI-1") n = "AI一号";
                    else if (n == "AI-2") n = "AI二号";
                    else if (n == "AI-3") n = "AI三号";
                    toast(n + " 被罚抽 " + to_string(drawStack) + " 张!", C_RED, 1000);
                    for (int i = 0; i < drawStack; i++)
                        cur->drawCard(deck.draw());
                    drawStack = 0;
                    advanceTurn();
                    continue;
                }
            }

            AIPlayer *ai = dynamic_cast<AIPlayer *>(cur);
            int playIdx = -1;
            auto &aiHand = ai->getHand();
            for (int i = 0; i < (int)aiHand.size(); i++) {
                if (canPlayCard(aiHand[i], topCard, currentColor, drawStack)) { playIdx = i; break; }
            }

            if (playIdx == -1) {
                cur->drawCard(deck.draw());
                string n = cur->getName();
                if (n == "AI-1") n = "AI一号";
                else if (n == "AI-2") n = "AI二号";
                else if (n == "AI-3") n = "AI三号";
                toast(n + " 抽了一张牌", C_YELLOW, 700);
            } else {
                Card played = cur->playCard(playIdx);
                deck.addDiscard(played);
                topCard = played;

                if (played.type == CardType::Wild || played.type == CardType::WildDrawFour) {
                    int cnt[4] = {0, 0, 0, 0};
                    for (auto &c : ai->getHand()) {
                        switch (c.color) {
                        case Color::Red:    cnt[0]++; break;
                        case Color::Yellow: cnt[1]++; break;
                        case Color::Green:  cnt[2]++; break;
                        case Color::Blue:   cnt[3]++; break;
                        default: break;
                        }
                    }
                    int best = 0;
                    for (int i = 1; i < 4; i++) if (cnt[i] > cnt[best]) best = i;
                    Color cols[] = {Color::Red, Color::Yellow, Color::Green, Color::Blue};
                    currentColor = cols[best];
                    string cname; switch (currentColor) {
                        case Color::Red: cname = "红"; break;
                        case Color::Yellow: cname = "黄"; break;
                        case Color::Green: cname = "绿"; break;
                        case Color::Blue: cname = "蓝"; break;
                        default: cname = "?"; break;
                    }
                    toast(ai->getName() + " 选了 " + cname + "色", colorCode(currentColor), 700);
                } else {
                    currentColor = played.color;
                }

                executeEffect(played);
                if (cur->getHandCount() == 1) {
                    string n = ai->getName();
                    if (n == "AI-1") n = "AI一号";
                    else if (n == "AI-2") n = "AI二号";
                    else if (n == "AI-3") n = "AI三号";
                    toast(n + " 喊 UNO!", C_YELLOW, 700);
                }
                if (cur->getHandCount() == 0) { gameOver = true; break; }
            }

            advanceTurn();
            continue;
        }

        // 人类回合
        showCursor();
        gotoxy(3, CON_H - 1);
        setC(C_WHITE); cout << "> "; resetC();
        string input = readLine();
        hideCursor();

        if (!input.empty()) {
            size_t s = input.find_first_not_of(" \t");
            if (s == string::npos) input = "";
            else {
                size_t e = input.find_last_not_of(" \t");
                input = input.substr(s, e - s + 1);
            }
        }
        if (input.empty()) continue;

        if (input == "q" || input == "Q") {
            toast("再见!", C_YELLOW, 600);
            break;
        }
        if (input == "d" || input == "D") {
            if (drawStack == 0 && hasDrawnThisTurn) {
                toast("本回合已抽过牌!", C_RED, 700);
                continue;
            }
            if (drawStack > 0) {
                toast("正在抽 " + to_string(drawStack) + " 张罚牌...", C_RED, 1000);
                for (int i = 0; i < drawStack; i++)
                    cur->drawCard(deck.draw());
                drawStack = 0;
                advanceTurn();
                continue;
            }
            cur->drawCard(deck.draw());
            hasDrawnThisTurn = true;
            toast("抽了 1 张牌", C_YELLOW, 600);
            continue;
        }
        if (input == "p" || input == "P") {
            if (hasDrawnThisTurn && drawStack == 0) { advanceTurn(); continue; }
            toast("还不能结束, 请出牌或抽牌", C_RED, 600);
            continue;
        }

        try {
            int idx = stoi(input);
            auto &hand = cur->getHand();
            if (idx < 0 || idx >= (int)hand.size()) {
                toast("无效的编号: " + to_string(idx), C_RED, 600); continue;
            }
            if (!canPlayCard(hand[idx], topCard, currentColor, drawStack)) {
                toast("这张牌不能出!", C_RED, 600); continue;
            }

            Card played = cur->playCard(idx);
            deck.addDiscard(played);
            topCard = played;

            if (played.type == CardType::Wild || played.type == CardType::WildDrawFour)
                currentColor = pickColor();
            else
                currentColor = played.color;

            executeEffect(played);
            if (cur->getHandCount() == 1) toast("UNO! 你只剩一张牌了!", C_YELLOW, 800);
            if (cur->getHandCount() == 0) { gameOver = true; break; }

            advanceTurn();
        } catch (...) {
            toast("输入无效, 请输入数字/d/p/q", C_RED, 600);
        }
    }

    cls();
    int midX = CON_W / 2;
    setC(C_YELLOW);
    gotoxy(midX - 8, CON_H / 2 - 2);
    cout << "====== 游戏结束 ======";
    resetC();

    for (auto p : players) {
        if (p->getHandCount() == 0) {
            gotoxy(midX - 12, CON_H / 2);
            string n = p->getName();
            if (n == "You") n = "你";
            else if (n == "AI-1") n = "AI一号";
            else if (n == "AI-2") n = "AI二号";
            else if (n == "AI-3") n = "AI三号";
            if (p->getName() == "You") {
                setC(C_GREEN); cout << ">>>> 你赢了! <<<<<";
            } else {
                setC(C_RED); cout << n << " 赢了!";
            }
            resetC();
            break;
        }
    }

    gotoxy(midX - 10, CON_H / 2 + 2);
    setC(C_GRAY); cout << "按任意键退出..."; resetC();
    showCursor();
    _getch();

    for (auto p : players) delete p;
    return 0;
}
