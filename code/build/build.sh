# ...existing code...
#!/usr/bin/env bash
set -e

# 获取项目根和 build 目录
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$(cd "$(dirname "$0")" && pwd)"

# 如果环境变量已设置，就使用设置的结果，否则配置为脚本中的值
GENERATOR="${GENERATOR:-MinGW Makefiles}"
CMAKE_CMD="${CMAKE_CMD:-cmake}"
MAKE_CMD="${MAKE_CMD:-make}"
TOOLCHAIN_FILE_ARG=""

if [ -n "${TOOLCHAIN_FILE:-}" ]; then
  TOOLCHAIN_FILE_ARG="-DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_FILE}"
fi
echo "$GENERATOR"
echo "$CMAKE_CMD"
echo "$MAKE_CMD"
echo "$TOOLCHAIN_FILE"

usage() {
  echo "Usage: $0 {config|build|clean|rebuild}"
  echo "  GENERATOR, CMAKE_CMD, MAKE_CMD, TOOLCHAIN_FILE 可以通过环境变量指定"
  exit 1
}

cmd="${1:-build}"

case "$cmd" in
  config)
    echo "cmake configure -> generator: $GENERATOR"
    "$CMAKE_CMD" -G "$GENERATOR" -S "$ROOT_DIR" -B "$BUILD_DIR" $TOOLCHAIN_FILE_ARG
    ;;

  build)
    echo "configure then build"
    "$CMAKE_CMD" -G "$GENERATOR" -S "$ROOT_DIR" -B "$BUILD_DIR" $TOOLCHAIN_FILE_ARG
    "$MAKE_CMD" -C "$BUILD_DIR"
    ;;

  clean)
    echo "clean build files"
    rm -f "$BUILD_DIR/CMakeCache.txt"
    rm -rf "$BUILD_DIR/CMakeFiles"
    rm -f "$BUILD_DIR/Makefile" "$BUILD_DIR/build.ninja" "$BUILD_DIR/Project.sln" "$BUILD_DIR/Project.vcxproj" || true
    # 尝试调用 make clean（若可用）
    if [ -f "$BUILD_DIR/Makefile" ] || [ -d "$BUILD_DIR/CMakeFiles" ]; then
      "$MAKE_CMD" -C "$BUILD_DIR" clean || true
    fi
    ;;

  rebuild)
    "$0" clean
    "$0" build
    ;;

  *)
    usage
    ;;
esac
# ...existing code...