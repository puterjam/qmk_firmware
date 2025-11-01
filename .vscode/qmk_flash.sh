#!/bin/bash
# QMK Flash (use saved configuration from update_clangd)
# 烧录固件，使用 update_clangd 保存的配置

set -e

# 颜色输出
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# 获取脚本所在目录的父目录（项目根目录）
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

cd "$PROJECT_ROOT"

# 读取保存的配置
CONFIG_FILE="$PROJECT_ROOT/.vscode/.last_config"

if [ ! -f "$CONFIG_FILE" ]; then
    echo -e "${RED}Error: No saved configuration found${NC}"
    echo -e "${YELLOW}Please run ${GREEN}update_clangd${YELLOW} first to set the keyboard and keymap${NC}"
    exit 1
fi

# 加载配置
source "$CONFIG_FILE"

if [ -z "$KEYBOARD" ] || [ -z "$KEYMAP" ]; then
    echo -e "${RED}Error: Invalid saved configuration${NC}"
    echo -e "${YELLOW}Please run ${GREEN}update_clangd${YELLOW} again${NC}"
    exit 1
fi

echo -e "${BLUE}Flashing keyboard firmware:${NC}"
echo -e "  Keyboard: ${GREEN}${KEYBOARD}${NC}"
echo -e "  Keymap:   ${GREEN}${KEYMAP}${NC}"
echo ""

# 提示用户进入 bootloader 模式
echo -e "${YELLOW}Please put your keyboard into bootloader mode now...${NC}"
echo -e "${YELLOW}(Usually by pressing the reset button or using a key combination)${NC}"
echo ""

# 烧录固件
echo -e "${BLUE}Flashing...${NC}"
qmk flash -kb "$KEYBOARD" -km "$KEYMAP"

echo ""
echo -e "${GREEN}✓ Flash complete!${NC}"
