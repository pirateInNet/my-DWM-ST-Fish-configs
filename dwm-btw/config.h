#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const int refreshrate = 28;
static const unsigned int borderpx = 2;
static const unsigned int snap = 32;
static const int showbar = 1;
static const int topbar = 1;
static const char *fonts[] = {"JetBrainsMono Nerd:size=10"};
static const char dmenufont[] = "monospace:size=10";
static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#eeeeee";
static const char col_cyan[] = "#555555";
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
    [SchemeSel] = {col_gray4, col_cyan, col_cyan},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
    /* class      instance    title       tags mask     isfloating   monitor */
    {"Gimp", NULL, NULL, 0, 1, -1},
    {"Firefox", NULL, NULL, 1 << 1, 0, -1},
};

/* layout(s) */
static const float mfact = 1.2;
static const int nmaster = 1;
static const int resizehints = 1;
static const int lockfullscreen = 1;

static const Layout layouts[] = {
    {"[]=", tile},    /* default: tiling */
    {"><>", NULL},    /* floating */
    {"[M]", monocle}, /* monocle */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                        \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                         \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},          \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

#define SHCMD(cmd)                                       \
  {                                                      \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
  }

/* commands */
static char dmenumon[2] = "0";
static const char *dmenucmd[] = {
    "dmenu_run", "-m",      dmenumon, "-fn",    dmenufont, "-nb",     col_gray1,
    "-nf",       col_gray3, "-sb",    col_cyan, "-sf",     col_gray4, NULL};
static const char *termcmd[] = {"st", NULL};
static const char *roficmd[] = {
    "rofi", "-show", "drun", "-theme", "~/.config/rofi/config.rasi", NULL};
static const char *filemanager[] = {"thunar", NULL};
static const char *browser[] = {"firefox", NULL};
static const char *screenshot[] = {"maim", "-s", "/tmp/screenshot.png",
                                   NULL}; /* selection screenshot */
static const char *scrfull[] = {"maim", "/tmp/screenshot.png",
                                NULL}; /* full screenshot */
static const char *wifi[] = {"st", "-e", "nmtui", NULL};
/*to change between AR & FR*/
static const char *kblayout[] = {"/bin/sh", "-c", "~/.dwm/kblayout.sh", NULL};

static const Key keys[] = {
    /* --- Manjaro i3 style keybindings --- */

    /* Alt+space          -> change language*/
    {Mod1Mask, XK_space, spawn, {.v = kblayout}},

    /* Super+c        ->  tclock */
    {MODKEY, XK_c, spawn,
     SHCMD("st -f monospace-12 -e tty-clock -c -t -C 7 -s -b -B -u -f "
           "%A-%w-%d/%B%m/%Y...%z")},

    /* Super+Enter        → terminal */
    {MODKEY, XK_Return, spawn, {.v = termcmd}},

    /* Super+w            > nmtui */
    {MODKEY, XK_w, spawn, {.v = wifi}},

    /* Super+d            → rofi (like i3's $mod+d for dmenu) */
    {MODKEY, XK_d, spawn, {.v = roficmd}},

    /* Super+Shift+d      → dmenu */
    {MODKEY | ShiftMask, XK_d, spawn, {.v = dmenucmd}},

    /* Super+Shift+q      → kill focused window (i3: $mod+Shift+q) */
    {MODKEY | ShiftMask, XK_q, killclient, {0}},

    /* Super+Shift+e      → quit DWM (i3: $mod+Shift+e) */
    {MODKEY | ShiftMask, XK_e, quit, {0}},

    /* Super+f            → toggle fullscreen/monocle layout */
    {MODKEY, XK_f, setlayout, {.v = &layouts[2]}},

    /* Super+Shift+Space  → toggle floating (i3: $mod+Shift+Space) */
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},

    /* Super+Space        → toggle layout */
    {MODKEY, XK_space, setlayout, {0}},

    /* Super+b            → toggle bar */
    {MODKEY, XK_b, togglebar, {0}},

    /* Focus navigation (i3: $mod+j/k/h/l or arrow keys) */
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_Left, focusstack, {.i = -1}},
    {MODKEY, XK_Right, focusstack, {.i = +1}},

    /* Resize master area (i3: $mod+h/l) */
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},

    /* Master window count */
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY | ShiftMask, XK_i, incnmaster, {.i = -1}},

    /* Layouts */
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}}, /* tiling */
    {MODKEY, XK_g, setlayout, {.v = &layouts[1]}}, /* floating */
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}}, /* monocle */

    /* Zoom / swap with master (i3: $mod+Shift+Return) */
    {MODKEY | ShiftMask, XK_Return, zoom, {0}},

    /* Make window visible on ALL tags              agrave instead 0 because
       azerty*/
    {MODKEY | ShiftMask, XK_agrave, tag, {.ui = ~0}},

    /* View ALL tags at once */
    {MODKEY, XK_agrave, view, {.ui = ~0}},

    /* Multi-monitor */
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},

    /* Apps (Manjaro i3 style) */
    {MODKEY, XK_F1, spawn, {.v = filemanager}},
    {MODKEY, XK_F2, spawn, {.v = browser}},

    /* Screenshot (i3: Print key) */
    {0, XK_Print, spawn, {.v = scrfull}},
    {MODKEY, XK_Print, spawn, {.v = screenshot}}, /* selection */

    /* Volume (Manjaro i3 uses XF86 keys) */
    {0, XF86XK_AudioRaiseVolume, spawn,
     SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%")},
    {0, XF86XK_AudioLowerVolume, spawn,
     SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%")},
    {0, XF86XK_AudioMute, spawn,
     SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle")},

    /* Brightness */
    {0, XF86XK_MonBrightnessUp, spawn, SHCMD("brightnessctl set +10%")},
    {0, XF86XK_MonBrightnessDown, spawn, SHCMD("brightnessctl set 10%-")},

    /* Lock screen (i3: $mod+F1 or $mod+l ... using $mod+x here to avoid
       conflict) */
    {MODKEY, XK_x, spawn, SHCMD("slock")},

    /* Tags 1-9 — AZERTY layout */
    TAGKEYS(XK_ampersand, 0)  /* & = 1 */
    TAGKEYS(XK_eacute, 1)     /* é = 2 */
    TAGKEYS(XK_quotedbl, 2)   /* " = 3 */
    TAGKEYS(XK_apostrophe, 3) /* ' = 4 */
    TAGKEYS(XK_parenleft, 4)  /* ( = 5 */
    TAGKEYS(XK_minus, 5)      /* - = 6 */
    TAGKEYS(XK_egrave, 6)     /* è = 7 */
    TAGKEYS(XK_underscore, 7) /* _ = 8 */
    TAGKEYS(XK_ccedilla, 8)   /* ç = 9 */
};

/* button definitions */
static const Button buttons[] = {
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
