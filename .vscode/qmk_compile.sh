#!/bin/bash
# QMK Compile (use saved configuration from update_clangd)
# 编译键盘固件，使用 update_clangd 保存的配置

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
    echo -e "${YELLOW}Usage: ${GREEN}.vscode/update_clangd.sh${NC}"
    exit 1
fi

# 加载配置
source "$CONFIG_FILE"

if [ -z "$KEYBOARD" ] || [ -z "$KEYMAP" ]; then
    echo -e "${RED}Error: Invalid saved configuration${NC}"
    echo -e "${YELLOW}Please run ${GREEN}update_clangd${YELLOW} again${NC}"
    exit 1
fi

echo -e "${BLUE}Compiling keyboard firmware:${NC}"
echo -e "  Keyboard: ${GREEN}${KEYBOARD}${NC}"
echo -e "  Keymap:   ${GREEN}${KEYMAP}${NC}"
echo ""

# 编译固件
echo -e "${BLUE}Compiling...${NC}"
qmk compile -kb "$KEYBOARD" -km "$KEYMAP"

# 检查编译结果
BUILD_DIR_NAME=$(echo "$KEYBOARD" | tr '/' '_')
FIRMWARE_NAME="${BUILD_DIR_NAME}_${KEYMAP}"

# 查找生成的固件文件
FIRMWARE_FILE=""
for ext in bin hex uf2; do
    if [ -f "${FIRMWARE_NAME}.${ext}" ]; then
        FIRMWARE_FILE="${FIRMWARE_NAME}.${ext}"
        break
    fi
done

echo ""
if [ -n "$FIRMWARE_FILE" ]; then
    echo -e "${GREEN}✓ Compilation successful!${NC}"
    echo -e "  Firmware: ${GREEN}${FIRMWARE_FILE}${NC}"

    # 保存最后编译的配置，供 flash 脚本使用
    cat > "$PROJECT_ROOT/.vscode/.last_compile" << EOF
KEYBOARD=$KEYBOARD
KEYMAP=$KEYMAP
FIRMWARE_FILE=$FIRMWARE_FILE
EOF
    echo -e "  (Saved for qmk_flash.sh)"
else
    echo -e "${YELLOW}⚠ Warning: Could not find firmware file${NC}"
fi

echo ""
echo -e "${BLUE}Tip: Use ${GREEN}.vscode/qmk_flash.sh${BLUE} to flash this firmware${NC}"
