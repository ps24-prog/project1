#include <minitui.h>

tui_textbox::tui_textbox(
  tui_rect area, 
  const char *path, 
  tui_formatter formatter
) : tui_canvas(area) {
  // area.log_rect();
  sprintf(name, "Text: %s", path);
  FILE *fp = fopen(path, "rb");
  if (!fp) {
    Error("Cannot open file %s", path);
  }
  int length = 0;
  fseek(fp, 0, SEEK_END);
  length = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char buffer[1024];
  fread(buffer, 1, length, fp);
  buffer[length] = '\0';
  Debug("%s\n", buffer);
  print_content(tui_point(0, 0), false, formatter, buffer);
  fclose(fp);
}

tui_textbox::~tui_textbox() {

}