#include <basics.h>
#include <debug.h>
#include <minitui.h>
#include <mainscr.h>

tui_widget *reg_mainscr(tui_point head, tui_point tail, const char *msg) {
  auto mainscr_0 = new mainscr(tui_rect(head, tail));
  mainscr_0->set_global_formatter(
    tui_formatter(TUI_NJU_PURPLE, TUI_WHITE_V)
  );

  mainscr_0->set_central_message(
    msg,
    tui_formatter(TUI_NJU_PURPLE, TUI_WHITE_V)
  );

  root->create_widget(mainscr_0);
  return mainscr_0;
}

int main(int argc, char *argv[]) {
  
  tui_init(argc, argv);
  ansi_title("NJU Universalis");

  auto mainscr_0 = reg_mainscr(
    (tui_point) {0, 0},
    (tui_point) {SCR_HEIGHT - 1, SCR_WIDTH - 1},
    "Hello, Nanjing University!"
  );

  mainscr_0->create_widget(
    tui_proxy_factory::create<tui_ncanvas>(
      tui_rect(
        tui_point(scr_size.x - 5, scr_size.y / 2), 
        global_rect.tail
      ),
      {
        "kbd_move",
        "border", 
        "title"
      }
    )
  );

  mainscr_0->create_widget(
    tui_proxy_factory::create<tui_textbox>(
      tui_rect(
        global_rect.head,
        global_rect.head + tui_point(6, 40)
      ),
      {
        "reset",
        tui_fmt("quit %d", 'q'),
        "border", 
        "title"
      },
      "storage/meowmeow.txt",
      tui_formatter()
    )
  );


  int ret_value = tui_exec();
  if (ret_value != 0)
    Warn("tui_exec() returns with a non-zero value %d", ret_value);
  
  return ret_value;
}
