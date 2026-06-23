# 🎴 UNO 纸牌游戏

<div align="center">

一个用C++实现的 UNO 纸牌游戏，采用 HTML5 + CSS3 + JavaScript 技术，支持 1v3 对战，使用真实 UNO 卡牌图片，开箱即用。

[![Release](https://img.shields.io/badge/release-v1.0.0-green)](https://github.com/YOUR_USERNAME/uno-game/releases)
[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)
[![HTML5](https://img.shields.io/badge/tech-C++%2FHTML5%2FCSS3%2FJS-orange)](uno-game.html)

</div>

---

## ✨ 功能特性

| 特性 | 说明 |
|------|------|
| 🎮 **完整 UNO 规则** | 颜色匹配、数字匹配、功能牌、万能牌、罚牌叠加 |
| 🤖 **智能 AI** | 3 个 AI 对手，基于评分策略出牌，支持罚牌叠加 |
| 🎨 **真实卡牌图片** | 使用官方 UNO 风格卡牌图片，视觉还原度高 |
| 🔊 **音效系统** | Web Audio API 生成 8 种音效，可开关 |
| 📐 **四方位布局** | 玩家坐下方，AI 分坐左/上/右，方位清晰 |
| 🎯 **单文件即用** | 下载 `uno-game-standalone.html`，双击浏览器打开即玩 |

---

## 🚀 快速开始

### 方式一：直接下载（推荐）

1. 进入 [Releases](https://github.com/YOUR_USERNAME/uno-game/releases) 页面
2. 下载 `uno_game-cpp_v1.0.0.html`
3. 用浏览器打开即可游玩

### 方式二：克隆仓库

```bash
git clone https://github.com/YOUR_USERNAME/uno-game.git
cd uno-game
# 用浏览器打开 uno-game.html
```

---

## 🎮 游戏规则

### 基本规则
- 出牌需与弃牌堆顶牌**颜色相同**或**数字/功能相同**，或出**万能牌**
- 无法出牌时必须**抽牌**
- 手牌剩 **1 张**时喊 **UNO！**

### 功能牌

| 卡牌 | 效果 |
|------|------|
| 🚫 **跳过 (Skip)** | 下家被跳过 |
| 🔄 **反转 (Reverse)** | 出牌方向反转 |
| ➕2 **+2 (Draw Two)** | 下家抽 2 张牌并被跳过 |
| 🌈 **万能 (Wild)** | 指定下一种颜色 |
| ➕4 **万能+4 (Wild Draw Four)** | 指定颜色 + 下家抽 4 张 |

### 罚牌叠加
- 被 +2 / +4 时，可以再出 +2 / +4 **叠加**，直到有人无法叠加而抽走全部叠加数量的牌

---

## 📁 项目结构

```
uno-game-cpp/
├── uno-game.html                # 开发版（需 Cards/ 文件夹）
├── uno-game-standalone.html    # 发布版（单文件，含所有图片）
├── UNO_GAME_DOCS.md            # 技术文档
├── Cards/                      # 卡牌图片（开发版使用）
├── include/                    # C++ 头文件（原始后端）
└── src/                        # C++ 源码（原始后端）
```

---

## 🛠️ 技术栈

- **前端**：纯 HTML5 + CSS3 + JavaScript（无框架依赖）
- **音效**：Web Audio API（无需外部音频文件）
- **图片**：UNO 官方风格卡牌图片（JPEG）
- **构建**：Python 脚本嵌入图片生成单文件版本

---

## 📝 开发记录

- 四方位玩家布局（玩家下、AI 左/上/右）
- 方向指示器（右上角，顺时针/逆时针）

详见 [UNO_GAME_DOCS.md](UNO_GAME_DOCS.md)

---

## 📄 License

MIT License — 自由使用、修改和分发。

---

<div align="center">
  <sub>用 ❤️ 和 C++ / JavaScript 制作</sub>
</div>
