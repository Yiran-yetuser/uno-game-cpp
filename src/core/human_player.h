// TODO: 实现HumanPlayer类 - 人类玩家，支持手动选择卡牌
#pragma once
#include "player.h"

// 人类玩家类，继承自Player基类
class HumanPlayer : public Player
{
public:
    explicit HumanPlayer(std::string playerName);

    // 重写选牌逻辑（对接控制台输入，后续图形界面仅修改此处交互逻辑）
    int chooseCard(const Card& topDiscard) override;

    // 辅助接口：设置选中卡牌下标（对应你文档里的 setSelectedCard）
    void setSelectedCard(int idx);

private:
    int selectedIdx = -1; // 记录当前选中的手牌下标
};