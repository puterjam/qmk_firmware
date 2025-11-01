#!/bin/bash
# Update clangd configuration (auto-detect from current keymap.c)
# 仅更新 clangd 配置，包括 compile_commands.json 和 .clangd 文件
# 会引用之前编译生成的 layout.h

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

# 尝试从参数获取配置
KEYBOARD=""
KEYMAP=""
FILE_PATH=""

# 参数解析
if [[ $# -eq 1 ]]; then
    # 单个参数：可能是文件路径
    if [[ "$1" == *"keymap.c"* ]]; then
        FILE_PATH="$1"
    fi
elif [[ $# -eq 2 ]]; then
    # 两个参数：KEYBOARD KEYMAP
    KEYBOARD="$1"
    KEYMAP="$2"
fi

# 如果提供了文件路径，从路径中提取 keyboard 和 keymap
if [ -n "$FILE_PATH" ]; then
    # 转换为相对于项目根目录的路径
    FILE_PATH="${FILE_PATH#$PROJECT_ROOT/}"

    echo -e "${BLUE}Using file: ${GREEN}${FILE_PATH}${NC}"

    # 从路径中提取 keyboard 和 keymap
    if [[ ! "$FILE_PATH" =~ keyboards/([^/]+(/[^/]+)*)/keymaps/([^/]+)/keymap\.c ]]; then
        echo -e "${RED}Error: File path does not match expected pattern${NC}"
        echo -e "${YELLOW}Expected: keyboards/KEYBOARD/keymaps/KEYMAP/keymap.c${NC}"
        echo -e "${YELLOW}Got: $FILE_PATH${NC}"
        exit 1
    fi

    KEYBOARD="${BASH_REMATCH[1]}"
    KEYMAP="${BASH_REMATCH[3]}"
fi

# 如果还没有 keyboard 和 keymap，尝试自动检测
if [ -z "$KEYBOARD" ] || [ -z "$KEYMAP" ]; then
    echo -e "${YELLOW}Auto-detecting keyboard configuration from recent keymap.c...${NC}"

    # 查找最近修改的 keymap.c 文件（排除 .build 目录）
    FILE_PATH=$(find keyboards -name "keymap.c" -not -path "*/.build/*" -type f -exec stat -f "%m %N" {} \; 2>/dev/null | sort -rn | head -1 | cut -d' ' -f2-)

    if [ -z "$FILE_PATH" ]; then
        echo -e "${RED}Error: Could not find any keymap.c file${NC}"
        echo -e "${YELLOW}Usage:${NC}"
        echo -e "${YELLOW}  $0 [FILE_PATH]${NC}"
        echo -e "${YELLOW}  $0 [KEYBOARD] [KEYMAP]${NC}"
        echo -e "${YELLOW}Example:${NC}"
        echo -e "${YELLOW}  $0 keyboards/keychron/q1/q1_ansi_stm32l432_ec11/keymaps/geekrgb/keymap.c${NC}"
        echo -e "${YELLOW}  $0 keychron/q1/q1_ansi_stm32l432_ec11 geekrgb${NC}"
        exit 1
    fi

    # 从路径中提取 keyboard 和 keymap
    if [[ ! "$FILE_PATH" =~ keyboards/([^/]+(/[^/]+)*)/keymaps/([^/]+)/keymap\.c ]]; then
        echo -e "${RED}Error: File path does not match expected pattern${NC}"
        echo -e "${YELLOW}Got: $FILE_PATH${NC}"
        exit 1
    fi

    KEYBOARD="${BASH_REMATCH[1]}"
    KEYMAP="${BASH_REMATCH[3]}"

    echo -e "${BLUE}Detected from: ${GREEN}${FILE_PATH}${NC}"
fi

echo -e "${BLUE}Updating clangd configuration for:${NC}"
echo -e "  Keyboard: ${GREEN}${KEYBOARD}${NC}"
echo -e "  Keymap:   ${GREEN}${KEYMAP}${NC}"
echo ""

# 1. 生成 compile_commands.json
echo -e "${BLUE}[1/2] Generating compile_commands.json...${NC}"
qmk generate-compilation-database -kb "$KEYBOARD" -km "$KEYMAP"
echo ""

# 2. 更新 .clangd 配置文件，添加对已编译生成的 layout.h 的引用
echo -e "${BLUE}[2/2] Updating .clangd configuration...${NC}"

# 将 keyboard 路径中的 / 替换为 _ 以匹配 .build 目录结构
BUILD_DIR_NAME=$(echo "$KEYBOARD" | tr '/' '_')
LAYOUT_H_DIR=".build/obj_${BUILD_DIR_NAME}_${KEYMAP}/src"

# 创建或更新 .clangd 文件
cat > "$PROJECT_ROOT/.clangd" << EOF
CompileFlags:
  Add:
    - "-I\${workspaceFolder}"
    - "-I\${workspaceFolder}/quantum"
    - "-I\${workspaceFolder}/tmk_core"
    - "-I\${workspaceFolder}/${LAYOUT_H_DIR}"
  Remove:
    - "-mno-thumb-interwork"
    - "-fno-dwarf-directory-asm"
EOF

echo -e "  Layout directory: ${GREEN}${LAYOUT_H_DIR}${NC}"
echo ""

# 检查 layouts.h 是否存在
if [ -f "$PROJECT_ROOT/${LAYOUT_H_DIR}/layouts.h" ]; then
    echo -e "${GREEN}✓ layouts.h found at ${LAYOUT_H_DIR}/layouts.h${NC}"
else
    echo -e "${YELLOW}⚠ Warning: layouts.h not found at ${LAYOUT_H_DIR}/layouts.h${NC}"
    echo -e "${YELLOW}  Run compile first: .vscode/qmk_compile.sh${NC}"
fi
echo ""

# 保存配置供 compile 和 flash 使用
cat > "$PROJECT_ROOT/.vscode/.last_config" << EOF
KEYBOARD=$KEYBOARD
KEYMAP=$KEYMAP
EOF

echo -e "${GREEN}✓ Clangd configuration updated successfully.${NC}"
echo -e "  Configuration saved for compile and flash commands"
echo -e "${BLUE}Note: You may need to reload VSCode window to apply changes.${NC}"
echo -e "  Press ${GREEN}Cmd+Shift+P${NC} and type ${GREEN}'Reload Window'${NC}"
