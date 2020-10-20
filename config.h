#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int gappih    = 6;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 6;       /* vert inner gap between windows */
static const unsigned int gappoh    = 6;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 6;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "FiraCode QiHei NF:size=10" };
static const char dmenufont[]       = "SF Mono:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_border[]        = "#42A5F5";

/* solarized colors http://ethanschoonover.com/solarized */
static const char s_base03[]        = "#002b36";
static const char s_base02[]        = "#073642";
static const char s_base01[]        = "#586e75";
static const char s_base00[]        = "#657b83";
static const char s_base0[]         = "#839496";
static const char s_base1[]         = "#93a1a1";
static const char s_base2[]         = "#eee8d5";
static const char s_base3[]         = "#fdf6e3";

static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3] = {
    /*               fg         bg         border   */
  /* orig */
    [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
    [SchemeSel] = {col_gray4, col_cyan, col_border},
    [SchemeHid] = {col_cyan, col_gray1, col_border},
    /* dark */
    /* [SchemeNorm] = {s_base0, s_base03, s_base2}, */
    /* [SchemeSel] = {s_base0, s_base02, s_base2}, */
    /* light */
    /* [SchemeNorm] { s_base00, s_base3, s_base02 }, */
    /* [SchemeSel] { s_base00, s_base2, s_base02 } */
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };
/* static const char *tags[] = { "\uf120", "\uf7ae", "\uf121", "\uf04b", "\ue62e", "\uf251", "\ue727", "\uf537", "\uf684" }; */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
  // { "Wine",     "tim.exe",  NULL,       0,            1,           -1 }
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "Tile",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *roficmd[] = { "rofi", "-combi-modi", "window,drun,run,ssh", "-show", "combi", "-modi", "combi", NULL };
static const char *termcmd[]  = { "st", NULL };

static const char *upvol[]   = { "/home/cycleke/scripts/vol-up.sh",  NULL };
static const char *downvol[] = { "/home/cycleke/scripts/vol-down.sh",  NULL };
static const char *mutevol[] = { "/home/cycleke/scripts/vol-toggle.sh",  NULL };

static const char *wpcmd[]  = { "/home/cycleke/scripts/wp-change.sh", NULL };
static const char *sktogglecmd[]  = { "/home/cycleke/scripts/sck-tog.sh", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "80x24", NULL };

static const char *lockcmd[] = { "/home/cycleke/scripts/lock.sh", NULL };
static const char *suspendcmd[]  = { "/home/cycleke/scripts/suspend.sh", NULL };

static const char *screenshotcmd[] = { "flameshot", "gui", NULL };

static Key keys[] = {
	/* modifier            key                      function        argument */
	{ MODKEY,              XK_d,                    spawn,          {.v = roficmd } },
	{ MODKEY,              XK_Return,               spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,    XK_l,                    spawn,          {.v = lockcmd } },
	{ MODKEY|ShiftMask,    XK_p,                    spawn,          {.v = suspendcmd } },
	{ MODKEY|ControlMask,  XK_s,                    spawn,          {.v = sktogglecmd } },
	{ 0,                   XF86XK_AudioMute,        spawn,          {.v = mutevol } },
	{ 0,                   XF86XK_AudioLowerVolume, spawn,          {.v = downvol } },
	{ 0,                   XF86XK_AudioRaiseVolume, spawn,          {.v = upvol   } },
	{ MODKEY,              XK_bracketleft,          spawn,          {.v = downvol } },
	{ MODKEY,              XK_bracketright,         spawn,          {.v = upvol   } },
	{ MODKEY,              XK_backslash,            spawn,          {.v = mutevol } },
	{ MODKEY,              XK_b,                    spawn,          {.v = wpcmd } },
	{ 0,                   XK_Print,                spawn,          {.v = screenshotcmd } },
	{ MODKEY|ShiftMask,    XK_j,                    rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_k,                    rotatestack,    {.i = -1 } },
	{ MODKEY,              XK_j,                    focusstack,     {.i = +1 } },
	{ MODKEY,              XK_k,                    focusstack,     {.i = -1 } },
	{ MODKEY,              XK_u,                    viewtoleft,     {0} },
	{ MODKEY,              XK_i,                    viewtoright,    {0} },
	{ MODKEY|ShiftMask,    XK_u,                    tagtoleft,      {0} },
	{ MODKEY|ShiftMask,    XK_i,                    tagtoright,     {0} },
	{ MODKEY|ControlMask,  XK_bracketleft,          incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,  XK_bracketright,         incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_bracketleft,          setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,    XK_bracketright,         setmfact,       {.f = +0.05} },
	{ MODKEY,              XK_h,                    hidewin,        {0} },
	{ MODKEY|ShiftMask,    XK_h,                    restorewin,     {0} },
	{ MODKEY,              XK_o,                    hideotherwins,  {0}},
	{ MODKEY|ShiftMask,    XK_o,                    restoreotherwins, {0}},
	{ MODKEY|ShiftMask,    XK_Return,               zoom,           {0} },
	{ MODKEY,              XK_Tab,                  view,           {0} },
	{ MODKEY|ShiftMask,    XK_q,                    killclient,     {0} },
	{ MODKEY,              XK_t,                    setlayout,      {.v = &layouts[0]} },
	{ MODKEY,              XK_m,                    setlayout,      {.v = &layouts[2]} },
	{ MODKEY,              XK_f,                    fullscreen,     {0} },
	{ MODKEY,              XK_space,                setlayout,      {0} },
	{ MODKEY|ShiftMask,    XK_space,                togglefloating, {0} },
	{ MODKEY,              XK_apostrophe,           togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,              XK_0,                    view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,    XK_0,                    tag,            {.ui = ~0 } },
	{ MODKEY,              XK_comma,                focusmon,       {.i = -1 } },
	{ MODKEY,              XK_period,               focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_comma,                tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_period,               tagmon,         {.i = +1 } },
	TAGKEYS(               XK_1,                      0)
	TAGKEYS(               XK_2,                      1)
	TAGKEYS(               XK_3,                      2)
	TAGKEYS(               XK_4,                      3)
	TAGKEYS(               XK_5,                      4)
	TAGKEYS(               XK_6,                      5)
	TAGKEYS(               XK_7,                      6)
	TAGKEYS(               XK_8,                      7)
	TAGKEYS(               XK_9,                      8)
	{ MODKEY|ControlMask,  XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
