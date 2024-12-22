#ifndef MINITUI_H__
#define MINITUI_H__

#include "../../common/include/basics.h"
#include "../../common/include/ansi.h"
#include "../../common/include/debug.h"
#include "../../common/include/tinyfmt.h"
#include "../../common/include/box.h"

#include "geometry.h"
#include "keycode.h"
#include "init.h"
#include "event.h"
#include "ui.h"
#include "widget.h"
#include "widgets/msgbox.h"
#include "widgets/dummy.h"
#include "widgets/canvas.h"
#include "widgets/ncanvas.h"
#include "widgets/textbox.h"
#include "timer.h"
#include "widget_proxy.h"
#include "proxies/title_proxy.h"
#include "proxies/border_proxy.h"
#include "proxies/quit_proxy.h"
#include "proxies/reset_proxy.h"
#include "proxies/kbd_move_proxy.h"
#include "proxy_factory.h"

#endif