// minitui.h 中包含了 C++ 常用库和所有 minitui 提供的对象 & 接口
#include <minitui.h>

// 程序主函数，需要接受命令行提供的参数
int main(int argc, char *argv[]) {

  // 调用 minitui 的初始化接口处理命令行参数并进行必要的初始化
  tui_init(argc, argv);

  // 设置你喜欢的窗口标题
  ansi_title("NJU Universalis");

  // 启动主事件循环
  int ret_value = tui_exec();
  // 处理异常返回值
  if (ret_value != 0)
    Warn("tui_exec() returns with a non-zero value %d", ret_value);

  return ret_value;
}