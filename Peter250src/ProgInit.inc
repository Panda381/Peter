
/////////////////////////////////////////////////////////////////////////////
// inicializaèní tabulka funkcí

const INITFUNC InitTabFunc[] = {
	PR_COMMAND | PR_ICON,	PR_ICON,	PR_ONE | PR_SETPAR,				"empty",				// IDF_NONE
	PR_COMMAND,				PR_COMMAND,	0,								"group",				// IDF_GROUP
	PR_COMMAND,				0,			0,								"if",					// IDF_IF
	0,						PR_BOOL,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"if_cond",				// IDF_IF_COND
	0,						PR_COMMAND,	PR_NOMOVE,						"then",					// IDF_IF_YES
	0,						PR_COMMAND,	PR_NOMOVE,						"else",					// IDF_IF_NO
	PR_COMMAND,				0,			0,								"while",				// IDF_WHILE
	0,						PR_BOOL,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"while_cond",			// IDF_WHILE_COND
	0,						PR_COMMAND,	PR_NOMOVE,						"while_body",			// IDF_WHILE_BODY
	PR_COMMAND,				0,			0,								"break",				// IDF_WHILE_BREAK
	PR_DIGIT,				0,			0,								"minus",				// IDF_MINUS
	PR_COMMAND,				0,			0,								"case",					// IDF_CASE
	0,						PR_ALLDATA,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"case_expr",			// IDF_CASE_SUM
	0,						PR_COMMAND,	0,								"case_on",				// IDF_CASE_ON
	0,						PR_ALLDATA,	PR_ONE | PR_SETPAR,				"case_equ",				// IDF_CASE_EQU
	0,						PR_COMMAND,	PR_NOMOVE,						"default",				// IDF_CASE_DEF
	PR_EXPRES,				PR_DIGIT,	0,								"num",					// IDF_NUM
	PR_EXPRES | PR_DIGIT,	0,			0,								"0",					// IDF_0
	PR_EXPRES | PR_DIGIT,	0,			0,								"1",					// IDF_1
	PR_EXPRES | PR_DIGIT,	0,			0,								"2",					// IDF_2
	PR_EXPRES | PR_DIGIT,	0,			0,								"3",					// IDF_3
	PR_EXPRES | PR_DIGIT,	0,			0,								"4",					// IDF_4
	PR_EXPRES | PR_DIGIT,	0,			0,								"5",					// IDF_5
	PR_EXPRES | PR_DIGIT,	0,			0,								"6",					// IDF_6
	PR_EXPRES | PR_DIGIT,	0,			0,								"7",					// IDF_7
	PR_EXPRES | PR_DIGIT,	0,			0,								"8",					// IDF_8
	PR_EXPRES | PR_DIGIT,	0,			0,								"9",					// IDF_9
	PR_DIGIT,				0,			0,								".",					// IDF_DOT
	PR_DIGIT,				0,			0,								"e",					// IDF_EXP
	PR_BOOL,				PR_EXPRES,	PR_SETPAR,						"=",					// IDF_EQU
	PR_BOOL,				PR_EXPRES,	PR_SETPAR,						"<>",					// IDF_NEQU
	PR_BOOL,				PR_EXPRES,	PR_SETPAR,						">=",					// IDF_EQGR
	PR_BOOL,				PR_EXPRES,	PR_SETPAR,						"<=",					// IDF_EQLT
	PR_BOOL,				PR_EXPRES,	PR_SETPAR,						">",					// IDF_GR
	PR_BOOL,				PR_EXPRES,	PR_SETPAR,						"<",					// IDF_LT
	PR_BOOL,				PR_BOOL,	PR_SETPAR,						"and",					// IDF_AND
	PR_BOOL,				PR_BOOL,	PR_SETPAR,						"or",					// IDF_OR
	PR_BOOL,				PR_BOOL,	PR_SETPAR,						"xor",					// IDF_XOR
	PR_BOOL,				PR_BOOL,	PR_ONE | PR_SETPAR,				"not",					// IDF_NOT
	PR_EXPRES,				PR_EXPRES,	PR_SETPAR,						"+",					// IDF_ADD
	PR_EXPRES,				PR_EXPRES,	PR_SETPAR,						"-",					// IDF_SUB
	PR_EXPRES,				PR_EXPRES,	PR_SETPAR,						"*",					// IDF_MUL
	PR_EXPRES,				PR_EXPRES,	PR_SETPAR,						"/",					// IDF_DIV
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"real",					// IDF_REAL
	PR_COMMAND | PR_ICON,	PR_ICON,	PR_ONE | PR_SETPAR,				"picture",				// IDF_ICON
	PR_COMMAND | PR_SOUND,	PR_SOUND,	PR_ONE | PR_SETPAR,				"sound",				// IDF_SND
	PR_COMMAND | PR_MUSIC,	PR_MUSIC,	PR_ONE | PR_SETPAR,				"music",				// IDF_MUS
	PR_COMMAND | PR_ALLDATA,0,			0,								"function",				// IDF_FNC
	PR_COMMAND | PR_MAP,	PR_MAP,		PR_ONE | PR_SETPAR,				"map",					// IDF_MAP
	PR_COMMAND | PR_SPRITE,	PR_SPRITE,	PR_ONE | PR_SETPAR,				"sprite",				// IDF_SPRITE
	0,						0,			PR_NOMOVE,						"operand_extra",		// IDF_OPER_EXTRA
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR,				"text",					// IDF_TEXT
	0,						0,			PR_NOMOVE,						"windows",				// IDF_WINDOWS
	0,						PR_ALLDATA,	PR_NOMOVE,						"iparam",				// IDF_PAR
	0,						PR_ALLDATA,	PR_ONE | PR_NOMOVE,				"oparam",				// IDF_OUT
	0,						0,			PR_NOMOVE,						"map_extra",			// IDF_MAP_EXTRA
	0,						PR_ALLDATA,	0,								"list",					// IDF_LIST
	0,						0,			PR_NOMOVE,						"operand",				// IDF_OPER
	PR_COMMAND,				0,			0,								"step",					// IDF_KROK
	PR_COMMAND,				0,			0,								"left",					// IDF_VLEVO
	PR_COMMAND,				0,			0,								"right",				// IDF_VPRAVO
	PR_COMMAND,				0,			0,								"back",					// IDF_VZAD
	0,						0,			PR_NOMOVE,						"peter",				// IDF_PETRIK
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"logic",				// IDF_BOOL
	PR_TEXT,				0,			0,								"text_const",			// IDF_TEXT_CONST
	PR_BOOL,				0,			0,								"true",					// IDF_TRUE
	PR_BOOL,				0,			0,								"false",				// IDF_FALSE
	0,						0,			PR_NOMOVE,						"pet2",					// IDF_PETRA
	PR_COMMAND,				0,			0,								"pet2_step",			// IDF_PETRA_KROK
	PR_COMMAND,				0,			0,								"pet2_left",			// IDF_PETRA_VLEVO
	PR_COMMAND,				0,			0,								"pet2_right",			// IDF_PETRA_VPRAVO
	PR_COMMAND,				0,			0,								"pet2_back",			// IDF_PETRA_VZAD
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"quick",				// IDF_RYCHLE
	PR_COMMAND,				0,			0,								"exit",					// IDF_EXIT
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"pet2_quick",			// IDF_PETRA_RYCHLE
	0,						0,			PR_NOMOVE,						"soundmusic_group",		// IDF_SOUNDMUSIC_GROUP
	PR_COMMAND | PR_ICON,	PR_ICON,	PR_ONE | PR_SETPAR,				"take",					// IDF_PREDMET
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"turn",					// IDF_SMER
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"posx",					// IDF_POZICEX
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"posy",					// IDF_POZICEY
	PR_COMMAND | PR_ICON,	PR_ICON,	PR_ONE | PR_SETPAR,				"pet2_take",			// IDF_PETRA_PREDMET
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"pet2_turn",			// IDF_PETRA_SMER
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"pet2_posx",			// IDF_PETRA_POZICEX
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"pet2_posy",			// IDF_PETRA_POZICEY
	0,						0,			PR_NOMOVE,						"const",				// IDF_CONST
	0,						0,			PR_NOMOVE,						"math",					// IDF_VYPOC
	0,						0,			PR_NOMOVE,						"functions",			// IDF_FUNC
	0,						0,			PR_NOMOVE,						"compare",				// IDF_POROVNANI
	0,						0,			PR_NOMOVE,						"log_oper",				// IDF_LOG_OPER
	PR_EXPRES,				PR_EXPRES,	PR_SETPAR,						"mod",					// IDF_MOD
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"sin",					// IDF_SIN
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"cos",					// IDF_COS
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"tg",					// IDF_TG
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"cotg",					// IDF_COTG
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"arcsin",				// IDF_ARCSIN
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"arccos",				// IDF_ARCCOS
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"arctg",				// IDF_ARCTG
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"arccotg",				// IDF_ARCCOTG
	PR_EXPRES,				0,			0,								"pi",					// IDF_PI
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"integer",				// IDF_INTEGER
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"frac",					// IDF_FRAC
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"abs",					// IDF_ABS
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"sqrt",					// IDF_SQRT
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"fact",					// IDF_FACT
	PR_EXPRES,				PR_EXPRES,	PR_ONE,							"random",				// IDF_RAND
	PR_EXPRES,				0,			0,								"eul",					// IDF_EUL
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"degrad",				// IDF_DEGRAD
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"raddeg",				// IDF_RADDEG
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"log",					// IDF_LOG
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"ln",					// IDF_LN
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"nexp",					// IDF_PEXP
	PR_EXPRES,				PR_EXPRES,	PR_SETPAR,						"power",				// IDF_MOCN
	0,						0,			PR_NOMOVE,						"angle",				// IDF_UHEL
	PR_EXPRES,				0,			0,								"deg0",					// IDF_UHEL0
	PR_EXPRES,				0,			0,								"deg45",				// IDF_UHEL45
	PR_EXPRES,				0,			0,								"deg90",				// IDF_UHEL90
	PR_EXPRES,				0,			0,								"deg135",				// IDF_UHEL135
	PR_EXPRES,				0,			0,								"deg180",				// IDF_UHEL180
	PR_EXPRES,				0,			0,								"deg225",				// IDF_UHEL225
	PR_EXPRES,				0,			0,								"deg270",				// IDF_UHEL270
	PR_EXPRES,				0,			0,								"deg315",				// IDF_UHEL315
	0,						0,			PR_NOMOVE,						"command",				// IDF_COMMAND
	PR_COMMAND,				PR_EXPRES,	PR_ONE,							"wait",					// IDF_WAIT
	0,						0,			PR_NOMOVE,						"key",					// IDF_KLAV
	PR_BOOL,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"key_test",				// IDF_KLAV_TEST
	PR_EXPRES,				0,			0,								"key_vkey",				// IDF_KLAV_VKEY
	PR_TEXT,				0,			0,								"key_char",				// IDF_KLAV_CHAR
	0,						0,			PR_NOMOVE,						"key_vk",				// IDF_KLAV_VK
	0,						0,			PR_NOMOVE,						"key_fx",				// IDF_KLAV_FX
	0,						0,			PR_NOMOVE,						"key_let",				// IDF_KLAV_PISM
	0,						0,			PR_NOMOVE,						"key_num",				// IDF_KLAV_NUM
	0,						0,			PR_NOMOVE,						"key_misc",				// IDF_KLAV_RUZNE
	0,						0,			PR_NOMOVE,						"key_ctrl",				// IDF_KLAV_RID
	PR_COMMAND,				0,			PR_LITE,						"transparent",			// IDF_TRANSPARENT
	PR_EXPRES,				0,			0,								"vk_f1",				// IDF_F1
	PR_EXPRES,				0,			0,								"vk_f2",				// IDF_F2
	PR_EXPRES,				0,			0,								"vk_f3",				// IDF_F3
	PR_EXPRES,				0,			0,								"vk_f4",				// IDF_F4
	PR_EXPRES,				0,			0,								"vk_f5",				// IDF_F5
	PR_EXPRES,				0,			0,								"vk_f6",				// IDF_F6
	PR_EXPRES,				0,			0,								"vk_f7",				// IDF_F7
	PR_EXPRES,				0,			0,								"vk_f8",				// IDF_F8
	PR_EXPRES,				0,			0,								"vk_f9",				// IDF_F9
	PR_EXPRES,				0,			0,								"vk_f10",				// IDF_F10
	PR_EXPRES,				0,			0,								"vk_f11",				// IDF_F11
	PR_EXPRES,				0,			0,								"vk_f12",				// IDF_F12
	PR_EXPRES,				0,			0,								"vk_a",					// IDF_A
	PR_EXPRES,				0,			0,								"vk_b",					// IDF_B
	PR_EXPRES,				0,			0,								"vk_c",					// IDF_C
	PR_EXPRES,				0,			0,								"vk_d",					// IDF_D
	PR_EXPRES,				0,			0,								"vk_e",					// IDF_E
	PR_EXPRES,				0,			0,								"vk_f",					// IDF_F
	PR_EXPRES,				0,			0,								"vk_g",					// IDF_G
	PR_EXPRES,				0,			0,								"vk_h",					// IDF_H
	PR_EXPRES,				0,			0,								"vk_i",					// IDF_I
	PR_EXPRES,				0,			0,								"vk_j",					// IDF_J
	PR_EXPRES,				0,			0,								"vk_k",					// IDF_K
	PR_EXPRES,				0,			0,								"vk_l",					// IDF_L
	PR_EXPRES,				0,			0,								"vk_m",					// IDF_M
	PR_EXPRES,				0,			0,								"vk_n",					// IDF_N
	PR_EXPRES,				0,			0,								"vk_o",					// IDF_O
	PR_EXPRES,				0,			0,								"vk_p",					// IDF_P
	PR_EXPRES,				0,			0,								"vk_q",					// IDF_Q
	PR_EXPRES,				0,			0,								"vk_r",					// IDF_R
	PR_EXPRES,				0,			0,								"vk_s",					// IDF_S
	PR_EXPRES,				0,			0,								"vk_t",					// IDF_T
	PR_EXPRES,				0,			0,								"vk_u",					// IDF_U
	PR_EXPRES,				0,			0,								"vk_v",					// IDF_V
	PR_EXPRES,				0,			0,								"vk_w",					// IDF_W
	PR_EXPRES,				0,			0,								"vk_x",					// IDF_X
	PR_EXPRES,				0,			0,								"vk_y",					// IDF_Y
	PR_EXPRES,				0,			0,								"vk_z",					// IDF_Z
	PR_EXPRES,				0,			0,								"vk_num0",				// IDF_VK_NUM0
	PR_EXPRES,				0,			0,								"vk_num1",				// IDF_VK_NUM1
	PR_EXPRES,				0,			0,								"vk_num2",				// IDF_VK_NUM2
	PR_EXPRES,				0,			0,								"vk_num3",				// IDF_VK_NUM3
	PR_EXPRES,				0,			0,								"vk_num4",				// IDF_VK_NUM4
	PR_EXPRES,				0,			0,								"vk_num5",				// IDF_VK_NUM5
	PR_EXPRES,				0,			0,								"vk_num6",				// IDF_VK_NUM6
	PR_EXPRES,				0,			0,								"vk_num7",				// IDF_VK_NUM7
	PR_EXPRES,				0,			0,								"vk_num8",				// IDF_VK_NUM8
	PR_EXPRES,				0,			0,								"vk_num9",				// IDF_VK_NUM9
	PR_EXPRES,				0,			0,								"vk_num.",				// IDF_VK_NUMTECKA
	PR_EXPRES,				0,			0,								"vk_num+",				// IDF_VK_NUMPLUS
	PR_EXPRES,				0,			0,								"vk_num-",				// IDF_VK_NUMMINUS
	PR_EXPRES,				0,			0,								"vk_num*",				// IDF_VK_NUMKRAT
	PR_EXPRES,				0,			0,								"vk_num/",				// IDF_VK_NUMDELENO
	PR_EXPRES,				0,			0,								"vk_numenter",			// IDF_VK_NUMENTER
	PR_EXPRES,				0,			0,								"vk_numlock",			// IDF_VK_NUMLOCK
	PR_EXPRES,				0,			0,								"vk_1",					// IDF_VK_1
	PR_EXPRES,				0,			0,								"vk_2",					// IDF_VK_2
	PR_EXPRES,				0,			0,								"vk_3",					// IDF_VK_3
	PR_EXPRES,				0,			0,								"vk_4",					// IDF_VK_4
	PR_EXPRES,				0,			0,								"vk_5",					// IDF_VK_5
	PR_EXPRES,				0,			0,								"vk_6",					// IDF_VK_6
	PR_EXPRES,				0,			0,								"vk_7",					// IDF_VK_7
	PR_EXPRES,				0,			0,								"vk_8",					// IDF_VK_8
	PR_EXPRES,				0,			0,								"vk_9",					// IDF_VK_9
	PR_EXPRES,				0,			0,								"vk_0",					// IDF_VK_0
	PR_EXPRES,				0,			0,								"vk_`",					// IDF_VK_LCARKA
	PR_EXPRES,				0,			0,								"vk_-",					// IDF_VK_POMLCKA
	PR_EXPRES,				0,			0,								"vk_=",					// IDF_VK_ROVNITKO
	PR_EXPRES,				0,			0,								"vk_\\",				// IDF_VK_ZLOMITKO
	PR_EXPRES,				0,			0,								"vk_[",					// IDF_VK_LLOMENA
	PR_EXPRES,				0,			0,								"vk_]",					// IDF_VK_RLOMENA
	PR_EXPRES,				0,			0,								"vk_;",					// IDF_VK_STREDNIK
	PR_EXPRES,				0,			0,								"vk_'",					// IDF_VK_RCARKA
	PR_EXPRES,				0,			0,								"vk_,",					// IDF_VK_CARKA
	PR_EXPRES,				0,			0,								"vk_.",					// IDF_VK_TECKA
	PR_EXPRES,				0,			0,								"vk_/",					// IDF_VK_LOMITKO
	PR_EXPRES,				0,			0,								"vk_esc",				// IDF_VK_ESC
	PR_EXPRES,				0,			0,								"vk_tab",				// IDF_VK_TAB
	PR_EXPRES,				0,			0,								"vk_capslock",			// IDF_VK_CAPSLOCK
	PR_EXPRES,				0,			0,								"vk_win",				// IDF_VK_WIN
	PR_EXPRES,				0,			0,								"vk_bs",				// IDF_VK_BS
	PR_EXPRES,				0,			0,								"vk_enter",				// IDF_VK_ENTER
	PR_EXPRES,				0,			0,								"vk_menu",				// IDF_VK_MENU
	PR_EXPRES,				0,			0,								"vk_left",				// IDF_VK_LEFT
	PR_EXPRES,				0,			0,								"vk_right",				// IDF_VK_RIGHT
	PR_EXPRES,				0,			0,								"vk_up",				// IDF_VK_UP
	PR_EXPRES,				0,			0,								"vk_down",				// IDF_VK_DOWN
	PR_EXPRES,				0,			0,								"vk_insert",			// IDF_VK_INSERT
	PR_EXPRES,				0,			0,								"vk_delete",			// IDF_VK_DELETE
	PR_EXPRES,				0,			0,								"vk_home",				// IDF_VK_HOME
	PR_EXPRES,				0,			0,								"vk_end",				// IDF_VK_END
	PR_EXPRES,				0,			0,								"vk_pageup",			// IDF_VK_PAGEUP
	PR_EXPRES,				0,			0,								"vk_pagedown",			// IDF_VK_PAGEDOWN
	PR_EXPRES,				0,			0,								"vk_print",				// IDF_VK_PRINT
	PR_EXPRES,				0,			0,								"vk_scroll",			// IDF_VK_SCROLL
	PR_EXPRES,				0,			0,								"vk_pause",				// IDF_VK_PAUSE
	PR_EXPRES,				0,			0,								"vk_shift",				// IDF_VK_SHIFT
	PR_EXPRES,				0,			0,								"vk_ctrl",				// IDF_VK_CTRL
	PR_EXPRES,				0,			0,								"vk_alt",				// IDF_VK_ALT
	PR_EXPRES,				0,			0,								"vk_space",				// IDF_VK_SPACE
	0,						0,			PR_NOMOVE,						"mouse",				// IDF_MOUSE
	PR_BOOL,				0,			0,								"mouse_l",				// IDF_MOUSE_L
	PR_BOOL,				0,			0,								"mouse_r",				// IDF_MOUSE_R
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"mouse_x",				// IDF_MOUSE_X
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"mouse_y",				// IDF_MOUSE_Y
	PR_BOOL,				PR_TEXT,	PR_SETPAR,						"equ_text",				// IDF_EQU_TEXT
	PR_BOOL,				PR_ICON,	PR_SETPAR,						"equ_pic",				// IDF_EQU_ICO
	0,						0,			PR_NOMOVE,						"sprite_extra",			// IDF_SPRITE_EXTRA
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"window_frame",			// IDF_WINDOW_FRAME
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"window_caption",		// IDF_WINDOW_CAPTION
	PR_TEXT,				0,			0,								"command_line",			// IDF_COMMAND_LINE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"return_code",			// IDF_RETURN_CODE
	PR_COMMAND | PR_PIC,	PR_PIC,		PR_ONE | PR_SETPAR,				"background",			// IDF_PIC
	PR_COMMAND,				0,			0,								"repeat",				// IDF_REPEAT
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"repeat_num",			// IDF_REPEAT_N
	PR_ICON,				0,			0,								"bounds",				// IDF_OKRAJ
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_INTERN | PR_SETPAR, "sprite_x",				// IDF_SPRITE_X
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_INTERN | PR_SETPAR,	"sprite_y",				// IDF_SPRITE_Y
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_INTERN | PR_SETPAR,	"sprite_dir",			// IDF_SPRITE_SMER
	PR_BOOL,				0,			0,								"sprite_move",			// IDF_SPRITE_MOVE
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_INTERN | PR_SETPAR,	"sprite_vis",			// IDF_SPRITE_VISIBLE
	PR_EXPRES,				PR_DIGIT,	PR_NOMOVE,						"list_size",			// IDF_LIST_SIZE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"list_index",			// IDF_LIST_INDEX
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"list_auto",			// IDF_LIST_AUTO
	0,						0,			PR_NOMOVE,						"extra",				// IDF_PETER_EXTRA
	0,						0,			PR_NOMOVE,						"pet2_extra",			// IDF_PETRA_EXTRA
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"switch_n1",			// IDF_PETER_SWCN1
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"pet2_switch_n1",		// IDF_PETRA_SWCN1
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"switch_n2",			// IDF_PETER_SWCN2
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"pet2_switch_n2",		// IDF_PETRA_SWCN2
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"switch_n3",			// IDF_PETER_SWCN3
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"pet2_switch_n3",		// IDF_PETRA_SWCN3
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"switch_1",				// IDF_PETER_SWC1
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"pet2_switch_1",		// IDF_PETRA_SWC1
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"switch_2",				// IDF_PETER_SWC2
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"pet2_switch_2",		// IDF_PETRA_SWC2
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"switch_3",				// IDF_PETER_SWC3
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"pet2_switch_3",		// IDF_PETRA_SWC3
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"switch_4",				// IDF_PETER_SWC4
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"pet2_switch_4",		// IDF_PETRA_SWC4
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"switch_5",				// IDF_PETER_SWC5
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"pet2_switch_5",		// IDF_PETRA_SWC5
	0,						0,			PR_NOMOVE,						"text_group",			// IDF_TEXT_GROUP
	PR_TEXT,				0,			0,								"text_left",			// IDF_TEXT_LEFT
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"text_left_len",		// IDF_TEXT_LEFT_LEN
	PR_TEXT,				0,			0,								"text_right",			// IDF_TEXT_RIGHT
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"text_right_pos",		// IDF_TEXT_RIGHT_POS
	PR_TEXT,				0,			0,								"text_mid",				// IDF_TEXT_MID
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"text_mid_pos",			// IDF_TEXT_MID_POS
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"text_mid_len",			// IDF_TEXT_MID_LEN
	PR_EXPRES,				PR_TEXT,	PR_ONE | PR_SETPAR,				"text_len",				// IDF_TEXT_LEN
	PR_TEXT,				PR_TEXT,	PR_SETPAR,						"text_add",				// IDF_TEXT_ADD
	PR_EXPRES,				PR_TEXT,	PR_ONE | PR_SETPAR,				"text_scan",			// IDF_TEXT_SCAN
	PR_TEXT,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"text_format",			// IDF_TEXT_FORMAT
	PR_BOOL,				PR_TEXT,	PR_SETPAR,						"text_gt",				// IDF_TEXT_GT
	PR_TEXT,				PR_TEXT,	PR_ONE | PR_SETPAR,				"text_lower",			// IDF_TEXT_LOWER
	PR_TEXT,				PR_TEXT,	PR_ONE | PR_SETPAR,				"text_upper",			// IDF_TEXT_UPPER
	PR_EXPRES,				0,			0,								"text_find",			// IDF_TEXT_FIND
	0,						PR_TEXT,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"text_input",			// IDF_TEXT_INPUT
	0,						PR_TEXT,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"text_find_sub",		// IDF_TEXT_FIND_SUB
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR,				"status_line",			// IDF_STATUS_LINE
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR,				"caption",				// IDF_CAPTION
	PR_EXPRES,				PR_TEXT,	PR_ONE | PR_SETPAR,				"text_ord",				// IDF_ORD
	PR_TEXT,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"text_char",			// IDF_CHAR
	0,						0,			PR_NOMOVE,						"sound_group",			// IDF_SOUND_GROUP
	PR_COMMAND,				0,			0,								"sound_play",			// IDF_SOUND_PLAY
	PR_COMMAND,				PR_SOUND,	PR_ONE,							"sound_stop",			// IDF_SOUND_STOP
	PR_EXPRES,				0,			0,								"color",				// IDF_COLOR
	0,						0,			PR_NOMOVE,						"graph_group",			// IDF_GRAPH_GROUP
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"graph_color",			// IDF_GRAPH_COLOR
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"graph_width",			// IDF_GRAPH_WIDTH
	PR_COMMAND,				0,			0,								"point",				// IDF_POINT
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"point_x",				// IDF_POINT_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"point_y",				// IDF_POINT_Y
	PR_COMMAND,				0,			0,								"line",					// IDF_LINE
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"line_x1",				// IDF_LINE_X1
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"line_y1",				// IDF_LINE_Y1
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"line_x2",				// IDF_LINE_X2
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"line_y2",				// IDF_LINE_Y2
	PR_COMMAND,				0,			0,								"box",					// IDF_BOX
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"box_x1",				// IDF_BOX_X1
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"box_y1",				// IDF_BOX_Y1
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"box_x2",				// IDF_BOX_X2
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"box_y2",				// IDF_BOX_Y2
	PR_COMMAND,				0,			0,								"box_fill",				// IDF_BOX_FILL
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"box_fill_x1",			// IDF_BOX_FILL_X1
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"box_fill_y1",			// IDF_BOX_FILL_Y1
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"box_fill_x2",			// IDF_BOX_FILL_X2
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"box_fill_y2",			// IDF_BOX_FILL_Y2
	PR_COMMAND,				0,			0,								"fill",					// IDF_FILL
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"fill_x",				// IDF_FILL_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"fill_y",				// IDF_FILL_Y
	PR_COMMAND,				0,			0,								"circle",				// IDF_CIRCLE
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"circle_x",				// IDF_CIRCLE_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"circle_y",				// IDF_CIRCLE_Y
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"circle_r",				// IDF_CIRCLE_R
	PR_COMMAND,				0,			0,								"circle_fill",			// IDF_CIRCLE_FILL
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"circle_fill_x",		// IDF_CIRCLE_FILL_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"circle_fill_y",		// IDF_CIRCLE_FILL_Y
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"circle_fill_r",		// IDF_CIRCLE_FILL_R
	PR_COMMAND,				0,			0,								"triangle",				// IDF_TRIANGLE
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"triangle_x1",			// IDF_TRIANGLE_X1
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"triangle_y1",			// IDF_TRIANGLE_Y1
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"triangle_x2",			// IDF_TRIANGLE_X2
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"triangle_y2",			// IDF_TRIANGLE_Y2
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"triangle_x3",			// IDF_TRIANGLE_X3
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"triangle_y3",			// IDF_TRIANGLE_Y3
	PR_COMMAND,				0,			0,								"sphere",				// IDF_SPHERE
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"sphere_x",				// IDF_SPHERE_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"sphere_y",				// IDF_SPHERE_Y
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"sphere_r",				// IDF_SPHERE_R
	PR_EXPRES,				0,			0,								"color_level",			// IDF_COLOR_LEVEL
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR, "color_level_n",		// IDF_COLOR_LEVEL_N
	PR_COMMAND | PR_EXPRES,	0,			0,								"key_vkey_wait",		// IDF_KLAV_VKEY_WAIT
	PR_COMMAND | PR_TEXT,	0,			0,								"key_char_wait",		// IDF_KLAV_CHAR_WAIT
	PR_EXPRES,				0,			0,								"vk_nokey",				// IDF_VK_NOKEY
	PR_EXPRES,				0,			0,								"vk_win_r",				// IDF_VK_WIN2
	PR_EXPRES,				0,			0,								"vk_shift_r",			// IDF_VK_SHIFTR
	PR_EXPRES,				0,			0,								"vk_ctrl_r",			// IDF_VK_CTRLR
	PR_EXPRES,				0,			0,								"vk_alt_r",				// IDF_VK_ALTR
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"status_num_lock",		// IDF_STATUS_NUMLOCK
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"status_caps_lock",		// IDF_STATUS_CAPSLOCK
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"status_scroll_lock",	// IDF_STATUS_SCROLLLOCK
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"status_insert",		// IDF_STATUS_INSERT
	PR_BOOL,				0,			0,								"mouse_l_click",		// IDF_MOUSE_LCLICK
	PR_BOOL,				0,			0,								"mouse_r_click",		// IDF_MOUSE_RCLICK
	PR_BOOL,				0,			0,								"mouse_l_dclick",		// IDF_MOUSE_LDCLICK
	PR_BOOL,				0,			0,								"mouse_r_dclick",		// IDF_MOUSE_RDCLICK
	PR_BOOL,				0,			0,								"mouse_region",			// IDF_MOUSE_REGION
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"mouse_region_x",		// IDF_MOUSE_REGION_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"mouse_region_y",		// IDF_MOUSE_REGION_Y
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"mouse_region_w",		// IDF_MOUSE_REGION_W
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"mouse_region_h",		// IDF_MOUSE_REGION_H
	PR_COMMAND,				0,			0,								"textout",				// IDF_GRAPH_TEXT
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"textout_x",			// IDF_GRAPH_TEXT_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"textout_y",			// IDF_GRAPH_TEXT_Y
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"textout_angle",		// IDF_GRAPH_TEXT_ANGLE
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"textout_h",			// IDF_GRAPH_TEXT_H
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"textout_w",			// IDF_GRAPH_TEXT_W
	0,						PR_BOOL,	PR_ONE | PR_NOMOVE,				"textout_bold",			// IDF_GRAPH_TEXT_BOLD
	0,						PR_BOOL,	PR_ONE | PR_NOMOVE,				"textout_italic",		// IDF_GRAPH_TEXT_ITALIC
	0,						PR_BOOL,	PR_ONE | PR_NOMOVE,				"textout_underline",	// IDF_GRAPH_TEXT_UNDERLINE
	0,						PR_BOOL,	PR_ONE | PR_NOMOVE,				"textout_strikeout",	// IDF_GRAPH_TEXT_STRIKEOUT
	0,						PR_BOOL,	PR_ONE | PR_NOMOVE,				"textout_serif",		// IDF_GRAPH_TEXT_SERIF
	0,						PR_BOOL,	PR_ONE | PR_NOMOVE,				"textout_courier",		// IDF_GRAPH_TEXT_COURIER
	0,						PR_TEXT,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"textout_text",			// IDF_GRAPH_TEXT_TEXT
	PR_EXPRES,				PR_TEXT,	PR_ONE | PR_SETPAR,				"textline_num",			// IDF_TEXTLINE_NUM
	PR_TEXT,				0,			0,								"textline_get",			// IDF_TEXTLINE_GET
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"textline_index",		// IDF_TEXTLINE_INDEX
	PR_BOOL,				PR_SOUND,	PR_ONE,							"sound_test",			// IDF_SOUND_TEST
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"sound_play_loop",		// IDF_SOUND_PLAY_LOOP
	PR_COMMAND | PR_ICON,	PR_ICON,	PR_ONE | PR_SETPAR,				"take_pos",				// IDF_PREDMET_POZ
	PR_COMMAND | PR_ICON,	PR_ICON,	PR_ONE | PR_SETPAR,				"pet2_take_pos",		// IDF_PETRA_PREDMET_POZ
	PR_COMMAND,				0,			0,								"background_disp",		// IDF_PIC_DISP
	0,						PR_PIC,		PR_ONE | PR_NOMOVE | PR_SETPAR,	"background_back",		// IDF_PIC_BACK
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"background_x",			// IDF_PIC_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"background_y",			// IDF_PIC_Y
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"background_block_x",	// IDF_PIC_BLOCK_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"background_block_y",	// IDF_PIC_BLOCK_Y
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"background_block_w",	// IDF_PIC_BLOCK_W
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"background_block_h",	// IDF_PIC_BLOCK_H
	PR_PIC,					0,			0,								"background_get",		// IDF_PIC_GET
	PR_ICON,				0,			0,								"icon_get",				// IDF_ICON_GET
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"language",				// IDF_LANGUAGE
	0,						PR_SOUND,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"sound_play_sound",		// IDF_SOUND_PLAY_SOUND
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"sound_play_volume",	// IDF_SOUND_PLAY_VOL
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"sound_play_balance",	// IDF_SOUND_PLAY_BAL
	0,						PR_BOOL,	PR_ONE | PR_NOMOVE,				"sound_play_stop",		// IDF_SOUND_PLAY_STOP
	PR_ALL,					PR_ALL,		0,								"comment",				// IDF_COMMENT
	0,						0,			PR_NOMOVE,						"sound_functions",		// IDF_SOUND_FUNCTIONS
	PR_COMMAND,				0,			0,								"mouse_cursor",			// IDF_MOUSE_CURSOR
	0,						PR_ICON,	PR_ONE | PR_NOMOVE,				"mouse_cursor_pic",		// IDF_MOUSE_CURSOR_PIC
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"mouse_cursor_x",		// IDF_MOUSE_CURSOR_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"mouse_cursor_y",		// IDF_MOUSE_CURSOR_Y
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"mouse_cursor_w",		// IDF_MOUSE_CURSOR_W
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"mouse_cursor_h",		// IDF_MOUSE_CURSOR_H
	PR_COMMAND,				PR_MUSIC,	PR_ONE | PR_SETPAR,				"music_play",			// IDF_MUSIC_PLAY
	PR_COMMAND,				PR_MUSIC,	PR_ONE | PR_SETPAR,				"music_loop",			// IDF_MUSIC_LOOP
	PR_COMMAND,				0,			0,								"music_stop",			// IDF_MUSIC_STOP
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"music_pause",			// IDF_MUSIC_PAUSE
	PR_BOOL,				0,			0,								"music_test",			// IDF_MUSIC_TEST
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"music_position",		// IDF_MUSIC_POS
	PR_EXPRES,				0,			PR_LITE,						"music_length",			// IDF_MUSIC_LEN
	PR_EXPRES,				0,			0,								"color_synth",			// IDF_COLOR_SYNTH
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"color_synth_red",		// IDF_COLOR_SYNTH_RED
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"color_synth_green",	// IDF_COLOR_SYNTH_GREEN
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"color_synth_blue",		// IDF_COLOR_SYNTH_BLUE
	PR_EXPRES,				0,			0,								"get_point",			// IDF_GET_POINT
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"get_point_x",			// IDF_GET_POINT_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"get_point_y",			// IDF_GET_POINT_Y
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"sqr",					// IDF_SQR
	0,						0,			PR_NOMOVE,						"music_group",			// IDF_MUSIC_GROUP
	0,						0,			PR_NOMOVE,						"cd_group",				// IDF_CD_GROUP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_LITE,				"cd_play",				// IDF_CD_PLAY
	PR_COMMAND,				0,			PR_LITE,						"cd_stop",				// IDF_CD_STOP
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"cd_pause",				// IDF_CD_PAUSE
	PR_BOOL,				0,			PR_LITE,						"cd_test",				// IDF_CD_TEST
	PR_EXPRES,				0,			PR_LITE,						"cd_length",			// IDF_CD_LENGTH
	PR_EXPRES,				0,			PR_LITE,						"cd_tracks",			// IDF_CD_TRACKS
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"cd_pos",				// IDF_CD_POS
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"cd_track",				// IDF_CD_TRACK
	PR_EXPRES,				0,			PR_LITE,						"cd_track_length",		// IDF_CD_TRACK_LENGTH
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"cd_track_pos",			// IDF_CD_TRACK_POS
	0,						0,			PR_NOMOVE,						"file_group",			// IDF_FILE_GROUP
	0,						0,			PR_NOMOVE,						"file_file_group",		// IDF_FILE_FILEGROUP
	PR_COMMAND | PR_BOOL,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_PARPAR | PR_LITE,	"file_file_new",		// IDF_FILE_FILENEW
	PR_COMMAND | PR_BOOL,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_PARPAR | PR_LITE,	"file_dir_new",			// IDF_FILE_DIRNEW
	PR_COMMAND | PR_BOOL,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_PARPAR | PR_LITE,	"file_delete",			// IDF_FILE_DELETE
	PR_BOOL,				PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"file_test",			// IDF_FILE_TEST
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"file_active",			// IDF_FILE_ACT
	PR_EXPRES,				0,			PR_LITE,						"file_disk_size",		// IDF_FILE_DISKSIZE
	PR_EXPRES,				0,			PR_LITE,						"file_disk_free",		// IDF_FILE_DISKFREE
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"file_disk_label",		// IDF_FILE_DISKLABEL
	PR_EXPRES,				0,			PR_LITE,						"file_disk_serial",		// IDF_FILE_DISKSERIAL
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"file_read",			// IDF_FILE_READ
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"file_write",			// IDF_FILE_WRITE
	PR_COMMAND | PR_BOOL,	0,			PR_LITE,						"file_copy",			// IDF_FILE_COPY
	PR_COMMAND | PR_BOOL,	0,			PR_LITE,						"file_move",			// IDF_FILE_MOVE
	0,						0,			PR_NOMOVE,						"file_disk_group",		// IDF_FILE_DISKGROUP
	0,						0,			PR_NOMOVE,						"file_attrib",			// IDF_FILE_ATTRIB
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"file_ro",				// IDF_FILE_RO
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"file_hid",				// IDF_FILE_HID
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"file_sys",				// IDF_FILE_SYS
	PR_BOOL,				0,			PR_LITE,						"file_dir",				// IDF_FILE_DIR
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"file_arc",				// IDF_FILE_ARC
	PR_BOOL,				0,			PR_LITE,						"file_temp",			// IDF_FILE_TMP
	PR_BOOL,				0,			PR_LITE,						"file_comp",			// IDF_FILE_CMP
	PR_BOOL,				0,			PR_LITE,						"file_off",				// IDF_FILE_OFF
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_create",			// IDF_FILE_CREATE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_open",			// IDF_FILE_OPEN
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_modi",			// IDF_FILE_MODI
	0,						0,			PR_NOMOVE,						"file_data",			// IDF_FILE_DATA
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_bytes",			// IDF_FILE_BYTES
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_byteu",			// IDF_FILE_BYTEU
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_words",			// IDF_FILE_WORDS
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_wordu",			// IDF_FILE_WORDU
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_dwords",			// IDF_FILE_DWORDS
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_dwordu",			// IDF_FILE_DWORDU
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_float",			// IDF_FILE_FLOAT
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_double",			// IDF_FILE_DOUBLE
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"file_text0",			// IDF_FILE_TEXT0
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"file_textcr",			// IDF_FILE_TEXTCR
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"file_textn",			// IDF_FILE_TEXTN
	PR_COMMAND | PR_ICON,	PR_ICON,	PR_ONE | PR_SETPAR | PR_LITE,	"file_icon",			// IDF_FILE_ICON
	PR_COMMAND | PR_PIC,	PR_PIC,		PR_ONE | PR_SETPAR | PR_LITE,	"file_picture",			// IDF_FILE_PICTURE
	PR_COMMAND | PR_SOUND,	PR_SOUND,	PR_ONE | PR_SETPAR | PR_LITE,	"file_sound",			// IDF_FILE_SOUND
	PR_COMMAND | PR_MUSIC,	PR_MUSIC,	PR_ONE | PR_SETPAR | PR_LITE,	"file_music",			// IDF_FILE_MUSIC
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_readoff",			// IDF_FILE_READOFF
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_writeoff",		// IDF_FILE_WRITEOFF
	0,						0,			PR_NOMOVE,						"datetime_group",		// IDF_DATETIME_GROUP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_current",		// IDF_DATETIME_CURRENT
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_utc",			// IDF_DATETIME_UTC
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_local",		// IDF_DATETIME_LOCAL
	PR_EXPRES,				0,			0,								"datetime_comp",		// IDF_DATETIME_COMP
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"datetime_comp_year",	// IDF_DATETIME_COMP_YEAR
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"datetime_comp_month",	// IDF_DATETIME_COMP_MONTH
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"datetime_comp_day",	// IDF_DATETIME_COMP_DAY
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"datetime_comp_hour",	// IDF_DATETIME_COMP_HOUR
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"datetime_comp_min",	// IDF_DATETIME_COMP_MINUTE
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"datetime_comp_sec",	// IDF_DATETIME_COMP_SECONDE
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"datetime_comp_msec",	// IDF_DATETIME_COMP_MSECONDE
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_get_year",	// IDF_DATETIME_GET_YEAR
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_get_month",	// IDF_DATETIME_GET_MONTH
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_get_day",		// IDF_DATETIME_GET_DAY
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_get_week",	// IDF_DATETIME_GET_WEEK
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_get_hour",	// IDF_DATETIME_GET_HOUR
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_get_min",		// IDF_DATETIME_GET_MINUTE
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_get_sec",		// IDF_DATETIME_GET_SECONDE
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_get_msec",	// IDF_DATETIME_GET_MSECONDE
	PR_TEXT,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_date_eur",	// IDF_DATETIME_DATEEUR
	PR_TEXT,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_date_usa",	// IDF_DATETIME_DATEUSA
	PR_TEXT,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_date_jap",	// IDF_DATETIME_DATEJAP
	PR_TEXT,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_time_24",		// IDF_DATETIME_TIME24
	PR_TEXT,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"datetime_time_12",		// IDF_DATETIME_TIME12
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_size",			// IDF_FILE_SIZE
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"file_error",			// IDF_FILE_ERROR
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_textn_len",		// IDF_FILE_TEXTN_LEN
	PR_EXPRES,				0,			PR_LITE,						"file_disk_type",		// IDF_FILE_DISK_TYPE
	PR_TEXT,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"text_format2",			// IDF_TEXT_FORMAT2
	PR_TEXT,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"text_formath",			// IDF_TEXT_FORMATH
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_3bytes",			// IDF_FILE_3BYTES
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"file_3byteu",			// IDF_FILE_3BYTEU
	0,						0,			PR_NOMOVE,						"dialog_group",			// IDF_DIALOG_GROUP
	PR_COMMAND | PR_EXPRES,	0,			PR_PARPAR,						"dialog_box",			// IDF_DIALOG_BOX
	0,						PR_TEXT,	PR_ONE | PR_NOMOVE,				"dialog_box_text",		// IDF_DIALOG_BOX_TEXT
	0,						PR_TEXT,	PR_ONE | PR_NOMOVE,				"dialog_box_capt",		// IDF_DIALOG_BOX_CAPTION
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"dialog_box_icon",		// IDF_DIALOG_BOX_ICON
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"dialog_box_but",		// IDF_DIALOG_BOX_BUTTON
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"dialog_box_def",		// IDF_DIALOG_BOX_DEFAULT
	PR_TEXT,				0,			0,								"text_crlf",			// IDF_TEXT_CRLF
	PR_TEXT,				0,			PR_PARPAR,						"dialog_text",			// IDF_DIALOG_TEXT
	0,						PR_TEXT,	PR_ONE | PR_NOMOVE,				"dialog_text1",			// IDF_DIALOG_TEXT1
	0,						PR_TEXT,	PR_ONE | PR_NOMOVE,				"dialog_text2",			// IDF_DIALOG_TEXT2
	0,						PR_TEXT,	PR_ONE | PR_NOMOVE,				"dialog_text3",			// IDF_DIALOG_TEXT3
	0,						PR_TEXT,	PR_ONE | PR_NOMOVE,				"dialog_text_par",		// IDF_DIALOG_TEXT_PAR
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"cd_eject",				// IDF_CD_EJECT
	PR_EXPRES,				PR_SOUND,	PR_ONE | PR_SETPAR | PR_LITE,	"sound_bit",			// IDF_SOUND_BIT
	PR_EXPRES,				PR_SOUND,	PR_ONE | PR_SETPAR | PR_LITE,	"sound_freq",			// IDF_SOUND_FREQ
	PR_BOOL,				PR_SOUND,	PR_ONE | PR_SETPAR | PR_LITE,	"sound_stereo",			// IDF_SOUND_STEREO
	PR_EXPRES,				PR_SOUND,	PR_ONE | PR_SETPAR | PR_LITE,	"sound_length",			// IDF_SOUND_LENGTH
	PR_SOUND,				PR_SOUND,	PR_ONE | PR_SETPAR | PR_LITE,	"sound_conv_8bit",		// IDF_SOUND_CONV_8BIT
	PR_SOUND,				PR_SOUND,	PR_ONE | PR_SETPAR | PR_LITE,	"sound_conv_16bit",		// IDF_SOUND_CONV_16BIT
	PR_SOUND,				PR_SOUND,	PR_ONE | PR_SETPAR | PR_LITE,	"sound_conv_stereo",	// IDF_SOUND_CONV_STEREO
	PR_SOUND,				PR_SOUND,	PR_ONE | PR_SETPAR | PR_LITE,	"sound_conv_mono",		// IDF_SOUND_CONV_MONO
	PR_SOUND,				PR_SOUND,	PR_ONE | PR_SETPAR | PR_LITE,	"sound_conv_11025",		// IDF_SOUND_CONV_11025
	PR_SOUND,				PR_SOUND,	PR_ONE | PR_SETPAR | PR_LITE,	"sound_conv_22050",		// IDF_SOUND_CONV_22050
	PR_SOUND,				PR_SOUND,	PR_ONE | PR_SETPAR | PR_LITE,	"sound_conv_44100",		// IDF_SOUND_CONV_44100
	PR_SOUND,				PR_SOUND,	PR_SETPAR | PR_LITE,			"sound_add",			// IDF_SOUND_ADD
	PR_SOUND,				0,			PR_LITE,						"sound_speed",			// IDF_SOUND_SPEED
	0,						PR_SOUND,	PR_ONE | PR_NOMOVE | PR_SETPAR | PR_LITE,	"sound_input",			// IDF_SOUND_INPUT
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR | PR_LITE,	"sound_speed_k",		// IDF_SOUND_SPEED_K
	PR_SOUND,				0,			PR_LITE,						"tongen",				// IDF_TONGEN
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR | PR_LITE,	"tongen_f",				// IDF_TONGEN_F
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR | PR_LITE,	"tongen_t",				// IDF_TONGEN_T
	0,						0,			PR_NOMOVE,						"map_group",			// IDF_MAP_GROUP
	PR_EXPRES,				PR_MAP,		PR_ONE,							"map_width",			// IDF_MAP_WIDTH
	PR_EXPRES,				PR_MAP,		PR_ONE,							"map_height",			// IDF_MAP_HEIGHT
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"map_x",				// IDF_MAP_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"map_y",				// IDF_MAP_Y
	0,						PR_MAP,		PR_ONE | PR_NOMOVE | PR_SETPAR,	"map_input",			// IDF_MAP_INPUT
	PR_ICON,				0,			0,								"map_icon",				// IDF_MAP_ICON
	PR_EXPRES,				0,			0,								"map_num1",				// IDF_MAP_NUM1
	PR_EXPRES,				0,			0,								"map_num2",				// IDF_MAP_NUM2
	PR_EXPRES,				0,			0,								"map_num3",				// IDF_MAP_NUM3
	PR_BOOL,				0,			0,								"map_swc1",				// IDF_MAP_SWC1
	PR_BOOL,				0,			0,								"map_swc2",				// IDF_MAP_SWC2
	PR_BOOL,				0,			0,								"map_swc3",				// IDF_MAP_SWC3
	PR_BOOL,				0,			0,								"map_swc4",				// IDF_MAP_SWC4
	PR_BOOL,				0,			0,								"map_swc5",				// IDF_MAP_SWC5
	PR_MAP,					0,			0,								"map_copy",				// IDF_MAP_COPY
	0,						PR_MAP,		PR_ONE | PR_NOMOVE | PR_SETPAR,	"map_copy_input",		// IDF_MAP_COPY_INPUT
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"map_copy_x",			// IDF_MAP_COPY_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"map_copy_y",			// IDF_MAP_COPY_Y
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"map_copy_w",			// IDF_MAP_COPY_W
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"map_copy_h",			// IDF_MAP_COPY_H
	0,						0,			PR_NOMOVE,						"console_group",		// IDF_CONSOLE_GROUP
	PR_COMMAND,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"sprite_shift_x",		// IDF_SPRITE_SHIFT_X
	PR_COMMAND,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"sprite_shift_y",		// IDF_SPRITE_SHIFT_Y
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"sprite_win_x1",		// IDF_SPRITE_WIN_X1
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"sprite_win_y1",		// IDF_SPRITE_WIN_Y1
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"sprite_win_x2",		// IDF_SPRITE_WIN_X2
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"sprite_win_y2",		// IDF_SPRITE_WIN_Y2
	PR_COMMAND,				0,			0,								"console_on",			// IDF_CONSOLE_ON
	PR_COMMAND,				PR_TEXT,	PR_ONE | PR_SETPAR,				"console_out",			// IDF_CONSOLE_OUT
	PR_TEXT,				0,			0,								"console_in",			// IDF_CONSOLE_IN
	PR_COMMAND,				PR_TEXT,	PR_ONE | PR_SETPAR,				"console_err",			// IDF_CONSOLE_ERR
	PR_TEXT,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"command_par",			// IDF_COMMAND_PAR
	PR_TEXT,				0,			0,								"exec_name",			// IDF_EXEC_NAME
	PR_COMMAND,				PR_TEXT,	PR_ONE | PR_SETPAR | PR_PARPAR | PR_LITE,	"exec",					// IDF_EXEC
	PR_COMMAND | PR_EXPRES,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_PARPAR | PR_LITE,	"exec_wait",			// IDF_EXEC_WAIT
	0,						0,			PR_NOMOVE,						"joystick_group",		// IDF_JOYSTICK_GROUP
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_LITE,				"joystick_x",			// IDF_JOYSTICK_X
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_LITE,				"joystick_y",			// IDF_JOYSTICK_Y
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_LITE,				"joystick_z",			// IDF_JOYSTICK_Z
	PR_BOOL,				PR_EXPRES,	PR_ONE | PR_LITE,				"joystick_1",			// IDF_JOYSTICK_1
	PR_BOOL,				PR_EXPRES,	PR_ONE | PR_LITE,				"joystick_2",			// IDF_JOYSTICK_2
	PR_BOOL,				PR_EXPRES,	PR_ONE | PR_LITE,				"joystick_3",			// IDF_JOYSTICK_3
	PR_BOOL,				PR_EXPRES,	PR_ONE | PR_LITE,				"joystick_4",			// IDF_JOYSTICK_4
	0,						0,			PR_NOMOVE,						"windownew_group",		// IDF_WINDOWNEW_GROUP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"window_id",			// IDF_WINDOW_ID
	PR_COMMAND,				PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"window_del",			// IDF_WINDOW_DEL
	PR_EXPRES,				0,			PR_LITE,						"window_new",			// IDF_WINDOW_NEW
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_LITE,				"window_x",				// IDF_WINDOW_X
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_LITE,				"window_y",				// IDF_WINDOW_Y
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_LITE,				"window_w",				// IDF_WINDOW_W
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_LITE,				"window_h",				// IDF_WINDOW_H
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"window_text",			// IDF_WINDOW_TEXT
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"window_visible",		// IDF_WINDOW_VISIBLE
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"window_disable",		// IDF_WINDOW_DISABLE
	PR_EXPRES,				0,			PR_LITE,						"button_new",			// IDF_BUTTON_NEW
	PR_BOOL,				0,			PR_LITE,						"button_click",			// IDF_BUTTON_CLICK
	PR_EXPRES,				0,			PR_LITE,						"buttoncheck_new",		// IDF_BUTTONCHECK_NEW
	PR_EXPRES,				0,			PR_LITE,						"edit_new",				// IDF_EDIT_NEW
	PR_EXPRES,				0,			PR_LITE,						"buttonradio_new",		// IDF_BUTTONRADIO_NEW
	0,						0,			PR_NOMOVE,						"windowpar_group",		// IDF_WINDOWPAR_GROUP
	PR_EXPRES,				0,			PR_LITE,						"memo_new",				// IDF_MEMO_NEW
	0,						0,			PR_NOMOVE,						"wincontrol_group",		// IDF_WINCONTROL_GROUP
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"checkbox_on",			// IDF_CHECKBOX_ON
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"focus",				// IDF_FOCUS
	PR_EXPRES,				0,			PR_LITE,						"defbutton_new",		// IDF_BUTTON_NEW
	PR_EXPRES,				0,			PR_LITE,						"buttonicon_new",		// IDF_BUTTONICON_NEW
	PR_EXPRES,				0,			PR_LITE,						"groupbox_new",			// IDF_GROUPBOX_NEW
	PR_EXPRES,				0,			PR_LITE,						"listbox_new",			// IDF_LISTBOX_NEW
	PR_EXPRES,				0,			PR_LITE,						"wintext_new",			// IDF_WINTEXT_NEW
	PR_EXPRES,				0,			PR_LITE,						"combobox_new",			// IDF_COMBOBOX_NEW
	PR_EXPRES,				0,			PR_LITE,						"comboedit_new",		// IDF_COMBOEDIT_NEW
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"fontbold",				// IDF_FONTBOLD
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"fontfixed",			// IDF_FONTFIXED
	PR_EXPRES,				0,			PR_LITE,						"buttonradio_new2",		// IDF_BUTTONRADIO_NEW2
	0,						0,			PR_NOMOVE,						"windowlist_group",		// IDF_WINDOWLIST_GROUP
	PR_COMMAND,				PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"listbox_add",			// IDF_LISTBOX_ADD
	PR_COMMAND,				0,			PR_LITE,						"listbox_clear",		// IDF_LISTBOX_CLEAR
	PR_EXPRES,				0,			PR_LITE,						"listedit_new",			// IDF_LISTEDIT_NEW
	PR_EXPRES,				0,			PR_LITE,						"button3_new",			// IDF_BUTTON3_NEW
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"button3_on",			// IDF_BUTTON3_ON
	PR_EXPRES,				0,			PR_LITE,						"windowro_new",			// IDF_WINDOWRO_NEW
	PR_COMMAND | PR_ICON,	PR_ICON,	PR_ONE | PR_SETPAR | PR_LITE,	"window_icon",			// IDF_WINDOW_ICON
	PR_EXPRES,				0,			PR_LITE,						"winicon_new",			// IDF_WINICON_NEW
	PR_COMMAND | PR_SPRITE,	PR_SPRITE,	PR_ONE | PR_SETPAR | PR_LITE,	"file_sprite",			// IDF_FILE_SPRITE
	PR_EXPRES,				0,			0,								"pi2",					// IDF_2PI
	PR_EXPRES,				0,			0,								"bod32",				// IDF_BOD32
	PR_EXPRES,				0,			0,								"-1",					// IDF_M1
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_INTERN | PR_SETPAR,	"sprite_phase",			// IDF_SPRITE_FAZE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_INTERN | PR_SETPAR,	"sprite_status",		// IDF_SPRITE_STATUS
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE,							"fullscreen",			// IDF_FULLSCREEN
	PR_COMMAND,				0,			0,								"key_flush",			// IDF_KLAV_FLUSH
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"sound_position",		// IDF_SOUND_POS
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"sound_pause",			// IDF_SOUND_PAUSE
	PR_TEXT,				PR_TEXT,	PR_ONE | PR_LITE,				"file_list",			// IDF_FILE_LIST
	PR_BOOL,				PR_MAP,		PR_SETPAR,						"equ_map",				// IDF_EQU_MAP
	PR_BOOL,				PR_PIC,		PR_SETPAR,						"equ_background",		// IDF_EQU_PIC
	PR_BOOL,				PR_SPRITE,	PR_SETPAR,						"equ_sprite",			// IDF_EQU_SPR
	PR_BOOL,				PR_SOUND,	PR_SETPAR,						"equ_sound",			// IDF_EQU_SND
	PR_BOOL,				PR_MUSIC,	PR_SETPAR,						"equ_music",			// IDF_EQU_MUS
	0,						0,			PR_NOMOVE,						"net_group",			// IDF_NET_GROUP
	0,						0,			PR_NOMOVE,						"directplay_group",		// IDF_DIRECTPLAY_GROUP
	PR_TEXT,				0,			PR_LITE,						"connect_list",			// IDF_CONNECT_LIST
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"connect_act",			// IDF_CONNECT_ACT
	PR_TEXT,				0,			PR_LITE,						"game_list",			// IDF_GAME_LIST
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"game_act",				// IDF_GAME_ACT
	PR_TEXT,				0,			PR_LITE,						"player_list",			// IDF_PLAYER_LIST
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"player_act",			// IDF_PLAYER_ACT
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"game_n1",				// IDF_GAME_N1
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"game_n2",				// IDF_GAME_N2
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"game_n3",				// IDF_GAME_N3
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"game_n4",				// IDF_GAME_N4
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"game_n5",				// IDF_GAME_N5
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"game_n6",				// IDF_GAME_N6
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"game_n7",				// IDF_GAME_N7
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"game_n8",				// IDF_GAME_N8
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"game_l1",				// IDF_GAME_L1
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"game_l2",				// IDF_GAME_L2
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"game_l3",				// IDF_GAME_L3
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"game_l4",				// IDF_GAME_L4
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"game_l5",				// IDF_GAME_L5
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"game_l6",				// IDF_GAME_L6
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"game_l7",				// IDF_GAME_L7
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"game_l8",				// IDF_GAME_L8
	0,						0,			PR_NOMOVE,						"game_group",			// IDF_GAME_GROUP
	PR_BOOL,				0,			PR_LITE,						"game_host",			// IDF_GAME_HOST
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"player_max",			// IDF_PLAYER_MAX
	PR_EXPRES,				0,			PR_LITE,						"player_inx",			// IDF_PLAYER_INX
	0,						0,			PR_NOMOVE,						"game_data_group",		// IDF_GAME_DATA_GROUP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_LITE,				"game_packet",			// IDF_GAME_PACKET
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"game_datan",			// IDF_GAME_DATAN
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"game_datat",			// IDF_GAME_DATAT
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"game_datal",			// IDF_GAME_DATAL
	PR_TEXT,				0,			0,								"game_ip",				// IDF_GAME_IP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"game_datab",			// IDF_GAME_DATAB
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"inc",					// IDF_INC
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"dec",					// IDF_DEC
	PR_EXPRES,				0,			PR_LITE,						"wintext_new_center",	// IDF_WINTEXT_NEW_CENTER
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"button_esc",			// IDF_BUTTON_ESC
	PR_EXPRES,				PR_PIC,		PR_ONE | PR_SETPAR,				"picture_width",		// IDF_PICTURE_WIDTH
	PR_EXPRES,				PR_PIC,		PR_ONE | PR_SETPAR,				"picture_height",		// IDF_PICTURE_HEIGHT
	PR_PIC,					PR_PIC,		PR_ONE | PR_SETPAR,				"picture_xflip",		// IDF_PICTURE_XFLIP
	PR_PIC,					PR_PIC,		PR_ONE | PR_SETPAR,				"picture_yflip",		// IDF_PICTURE_YFLIP
	0,						0,			PR_NOMOVE,						"picture_group",		// IDF_PICTURE_GROUP
	PR_PIC,					0,			0,								"picture_zoom",			// IDF_PICTURE_ZOOM
	0,						PR_PIC,		PR_ONE | PR_NOMOVE | PR_SETPAR,	"picture_zoom_pic",		// IDF_PICTURE_ZOOM_PIC
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"picture_zoom_width",	// IDF_PICTURE_ZOOM_WIDTH
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"picture_zoom_height",	// IDF_PICTURE_ZOOM_HEIGHT
	0,						PR_BOOL,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"picture_zoom_int",		// IDF_PICTURE_ZOOM_INT
	0,						PR_BOOL,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"picture_zoom_dith",	// IDF_PICTURE_ZOOM_DITH
	0,						0,			PR_NOMOVE,						"graphic_group",		// IDF_GRAPHIC_GROUP
	PR_EXPRES,				0,			PR_LITE,						"winpic_new",			// IDF_WINPIC_NEW
	PR_COMMAND | PR_PIC,	PR_PIC,		PR_ONE | PR_SETPAR | PR_LITE,	"window_pic",			// IDF_WINDOW_PIC
	PR_EXPRES,				0,			PR_LITE,						"richedit_new",			// IDF_RICHEDIT_NEW
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"fontheight",			// IDF_FONTHEIGHT
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"fontitalic",			// IDF_FONTITALIC
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"fontunder",			// IDF_FONTUNDER
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"fontstrike",			// IDF_FONTSTRIKE
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_LITE,				"fontserif",			// IDF_FONTSERIF
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"fontwidth",			// IDF_FONTWIDTH
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"fontangle",			// IDF_FONTANGLE
	PR_EXPRES,				0,			PR_LITE,						"wintext_new_right",	// IDF_WINTEXT_NEW_RIGHT
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"fontcolor",			// IDF_FONTCOL
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"fontbackground",		// IDF_FONTBACK
	0,						0,			PR_NOMOVE,						"font_group",			// IDF_FONTGROUP
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"sound_play_speed",		// IDF_SOUND_PLAY_SPEED
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"sound_distinguish",	// IDF_SOUND_DISTINGUISH
	PR_TEXT,				0,			PR_PARPAR | PR_LITE,			"dialog_open",			// IDF_DIALOG_OPEN
	PR_TEXT,				0,			PR_PARPAR | PR_LITE,			"dialog_save",			// IDF_DIALOG_SAVE
	PR_TEXT,				0,			PR_LITE,						"dialog_find",			// IDF_DIALOG_FIND
	PR_TEXT,				0,			PR_LITE,						"dialog_color",			// IDF_DIALOG_COLOR
	0,						PR_TEXT,	PR_ONE | PR_NOMOVE | PR_LITE,	"dialog_file_ext",		// IDF_DIALOG_EXT
	0,						PR_TEXT,	PR_ONE | PR_NOMOVE | PR_LITE,	"dialog_file_name",		// IDF_DIALOG_NAME
	0,						PR_TEXT,	PR_ONE | PR_NOMOVE | PR_LITE,	"dialog_file_files",	// IDF_DIALOG_FILES
	PR_PIC,					0,			0,								"picture_level",		// IDF_PICTURE_LEVEL
	0,						PR_PIC,		PR_ONE | PR_NOMOVE,				"picture_level_pic",	// IDF_PICTURE_LEVEL_PIC
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"picture_level_n",		// IDF_PICTURE_LEVEL_N
	PR_PIC,					0,			0,								"picture_rotate",		// IDF_PICTURE_ROTATE
	0,						PR_PIC,		PR_ONE | PR_NOMOVE,				"picture_rotate_pic",	// IDF_PICTURE_ROTATE_PIC
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"picture_rotate_angle",	// IDF_PICTURE_ROTATE_ANGLE
	PR_SOUND,				PR_SOUND,	PR_ONE | PR_SETPAR,				"sound_conv_pcm",		// IDF_SOUND_CONV_PCM
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"file_text_unicode",	// IDF_FILE_TEXT_UNICODE
	PR_PIC,					PR_MAP,		PR_ONE | PR_SETPAR,				"map_picture",			// IDF_MAP_PICTURE
	PR_PIC,					PR_ICON,	PR_ONE | PR_SETPAR,				"icon_picture",			// IDF_ICON_PICTURE
	0,						0,			PR_NOMOVE,						"bit_group",			// IDF_BIT_GROUP
	PR_EXPRES,				PR_EXPRES,	PR_SETPAR,						"bit_and",				// IDF_BIT_AND
	PR_EXPRES,				PR_EXPRES,	PR_SETPAR,						"bit_or",				// IDF_BIT_OR
	PR_EXPRES,				PR_EXPRES,	PR_SETPAR,						"bit_xor",				// IDF_BIT_XOR
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"bit_not",				// IDF_BIT_NOT
	PR_EXPRES,				0,			0,								"bit_max",				// IDF_BIT_MAX
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"bit_left",				// IDF_BIT_LEFT
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"bit_right",			// IDF_BIT_RIGHT
	PR_BOOL,				PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"mouse_item",			// IDF_MOUSE_ITEM
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"window_w_int",			// IDF_WINDOW_W_INT
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"window_h_int",			// IDF_WINDOW_H_INT
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"codepage",				// IDF_CODEPAGE
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"user_font",			// IDF_USERFONT
	PR_TEXT,				0,			PR_LITE,						"enum_fonts",			// IDF_ENUMFONTS
	PR_MAP,					0,			0,								"map_new",				// IDF_MAP_NEW
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"map_new_w",			// IDF_MAP_NEW_W
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"map_new_h",			// IDF_MAP_NEW_H
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"window_top",			// IDF_WINDOW_TOP
	0,						0,			PR_NOMOVE,						"examples_cdrom",		// IDF_VZOR2
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"file_alias",			// IDF_ALIAS
	0,						0,			PR_NOMOVE,						"com_group",			// IDF_COM_GROUP
	PR_EXPRES,				PR_TEXT,	PR_ONE | PR_SETPAR,				"com_open",				// IDF_COM_OPEN
	PR_COMMAND,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"com_close",			// IDF_COM_CLOSE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"com_id",				// IDF_COM_ID
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"com_num",				// IDF_COM_NUM
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR,				"com_text",				// IDF_COM_TEXT
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"com_baud",				// IDF_COM_BAUD
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"com_stop",				// IDF_COM_STOP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"com_bits",				// IDF_COM_BITS
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"com_parit",			// IDF_COM_PARIT
	PR_BOOL,				0,			0,								"com_in",				// IDF_COM_IN
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"com_send",				// IDF_COM_SEND
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"com_receive",			// IDF_COM_RECEIVE
	PR_PIC,					0,			0,								"picture_mask",			// IDF_PICTURE_MASK
	0,						PR_PIC,		PR_ONE | PR_NOMOVE | PR_SETPAR,	"picture_mask_pic",		// IDF_PICTURE_MASK_PIC
	0,						PR_PIC,		PR_ONE | PR_NOMOVE | PR_SETPAR,	"picture_mask_mask",	// IDF_PICTURE_MASK_MASK
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"graph_area_width",		// IDF_GRAPH_AREA_WIDTH
	PR_EXPRES,				0,			PR_LITE,						"richmemo_new",			// IDF_RICHMEMO_NEW
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"window_resizable",		// IDF_WINDOW_RESIZABLE
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"window_may_close",		// IDF_WINDOW_MAY_CLOSE
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"window_maxim",			// IDF_WINDOW_MAXIM
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"window_minim",			// IDF_WINDOW_MINIM
	PR_EXPRES,				0,			PR_LITE,						"progress_new",			// IDF_PROGRESS_NEW
	PR_EXPRES,				0,			PR_LITE,						"htrackbar_new",		// IDF_HTRACKBAR_NEW
	PR_EXPRES,				0,			PR_LITE,						"vtrackbar_new",		// IDF_VTRACKBAR_NEW
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"window_pos",			// IDF_WINDOW_POS
	0,						0,			PR_NOMOVE,						"mix_group",			// IDF_MIX_GROUP
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"mix_device",			// IDF_MIX_DEVICE
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"mix_line_dst",			// IDF_MIX_LINE_DST
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"mix_line_src",			// IDF_MIX_LINE_SRC
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"mix_line_type",		// IDF_MIX_LINE_TYPE
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"mix_control",			// IDF_MIX_CONTROL
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"mix_control_type",		// IDF_MIX_CONTROL_TYPE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"mix_control_value",	// IDF_MIX_CONTROL_VAL
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"mix_control_list",		// IDF_MIX_CONTROL_LIST
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"mix_channel",			// IDF_MIX_CHANNEL
	0,						0,			PR_NOMOVE,						"d3d_group",			// IDF_DIRECT3D_GROUP
	PR_COMMAND,				0,			PR_PARPAR,						"d3d_view",				// IDF_DIRECT3D_VIEW
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_view_x",			// IDF_DIRECT3D_VIEW_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_view_y",			// IDF_DIRECT3D_VIEW_Y
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_view_w",			// IDF_DIRECT3D_VIEW_W
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_view_h",			// IDF_DIRECT3D_VIEW_H
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR,				"videomodes",			// IDF_DIRECT3D_MODE_LIST
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"dialog_graphic",		// IDF_DIALOG_GRAPHIC
	0,						0,			PR_NOMOVE,						"d3d_setting_group",	// IDF_DIRECT3D_SETTING_GROUP
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_wireframe",		// IDF_DIRECT3D_WIREFRAME
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_lighton",			// IDF_DIRECT3D_LIGHTON
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_shades",			// IDF_DIRECT3D_SHADES
	PR_COMMAND,				0,			0,								"d3d_invert",			// IDF_DIRECT3D_INVERT
	0,						0,			PR_NOMOVE,						"d3d_objects_group",	// IDF_DIRECT3D_OBJECTS_GROUP
	PR_EXPRES,				0,			0,								"d3d_wall",				// IDF_DIRECT3D_WALL
	PR_EXPRES,				0,			0,								"d3d_cube",				// IDF_DIRECT3D_CUBE
	PR_EXPRES,				0,			0,								"d3d_sphere",			// IDF_DIRECT3D_SPHERE
	PR_EXPRES,				0,			0,								"d3d_cylinder",			// IDF_DIRECT3D_CYLINDER
	PR_EXPRES,				0,			0,								"d3d_cone",				// IDF_DIRECT3D_CONE
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_clone",			// IDF_DIRECT3D_CLONE
	PR_EXPRES,				PR_TEXT,	PR_ONE | PR_SETPAR,				"d3d_text",				// IDF_DIRECT3D_TEXT
	PR_EXPRES,				PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"d3d_file",				// IDF_DIRECT3D_FILE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_object_id",		// IDF_DIRECT3D_OBJECT_ID
	PR_COMMAND,				PR_EXPRES,	PR_SETPAR,						"d3d_object_delete",	// IDF_DIRECT3D_OBJECT_DELETE
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_visible",			// IDF_DIRECT3D_VISIBLE
	PR_COMMAND,				PR_EXPRES,	PR_SETPAR,						"d3d_link",				// IDF_DIRECT3D_LINK
	PR_COMMAND,				PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"d3d_save",				// IDF_DIRECT3D_SAVE	 // zatím zrušeno
	0,						0,			PR_NOMOVE,						"d3d_trans_group",		// IDF_DIRECT3D_TRANS_GROUP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_scale_x",			// IDF_DIRECT3D_SCALE_X
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_scale_y",			// IDF_DIRECT3D_SCALE_Y
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_scale_z",			// IDF_DIRECT3D_SCALE_Z
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_rotate_x",			// IDF_DIRECT3D_ROTATE_X
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_rotate_y",			// IDF_DIRECT3D_ROTATE_Y
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_rotate_z",			// IDF_DIRECT3D_ROTATE_Z
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_trans_x",			// IDF_DIRECT3D_TRANS_X
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_trans_y",			// IDF_DIRECT3D_TRANS_Y
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_trans_z",			// IDF_DIRECT3D_TRANS_Z
	0,						0,			PR_NOMOVE,						"d3d_material_group",	// IDF_DIRECT3D_MAT_GROUP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_color",			// IDF_DIRECT3D_COLOR
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_emissive",			// IDF_DIRECT3D_EMISSIVE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_power",			// IDF_DIRECT3D_POWER
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_specular",			// IDF_DIRECT3D_SPECULAR
	PR_COMMAND | PR_PIC,	PR_PIC,		PR_ONE | PR_SETPAR,				"d3d_texture",			// IDF_DIRECT3D_TEXTURE
	PR_COMMAND,				0,			PR_PARPAR,						"d3d_textmap",			// IDF_DIRECT3D_TEXTMAP
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_textmap_type",		// IDF_DIRECT3D_TEXTMAP_TYPE
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_textmap_ox",		// IDF_DIRECT3D_TEXTMAP_OX
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_textmap_oy",		// IDF_DIRECT3D_TEXTMAP_OY
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_textmap_oz",		// IDF_DIRECT3D_TEXTMAP_OZ
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_textmap_rx",		// IDF_DIRECT3D_TEXTMAP_RX
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_textmap_ry",		// IDF_DIRECT3D_TEXTMAP_RY
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_textmap_rz",		// IDF_DIRECT3D_TEXTMAP_RZ
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_transparent",		// IDF_DIRECT3D_TRANSPARENT
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_alpharef",			// IDF_DIRECT3D_ALPHAREF
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_textmap_method",	// IDF_DIRECT3D_TEXTMAP_METHOD
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_textmap_ou",		// IDF_DIRECT3D_TEXTMAP_OU
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_textmap_ov",		// IDF_DIRECT3D_TEXTMAP_OV
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_textmap_su",		// IDF_DIRECT3D_TEXTMAP_SU
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_textmap_sv",		// IDF_DIRECT3D_TEXTMAP_SV
	0,						0,			PR_NOMOVE,						"d3d_light_group",		// IDF_DIRECT3D_LIGHT_GROUP
	PR_EXPRES,				0,			0,								"d3d_light",			// IDF_DIRECT3D_LIGHT
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_light_color",		// IDF_DIRECT3D_LIGHT_COLOR
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_light_type",		// IDF_DIRECT3D_LIGHT_TYPE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_light_umbra",		// IDF_DIRECT3D_LIGHT_UMBRA
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_light_penumbra",	// IDF_DIRECT3D_LIGHT_PENUMBRA
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_light_range",		// IDF_DIRECT3D_LIGHT_RANGE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_light_catten",		// IDF_DIRECT3D_LIGHT_CATTEN
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_light_latten",		// IDF_DIRECT3D_LIGHT_LATTEN
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_light_qatten",		// IDF_DIRECT3D_LIGHT_QATTEN
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_projection",		// IDF_DIRECT3D_PROJECTION
	0,						0,			PR_NOMOVE,						"d3d_scene_group",		// IDF_DIRECT3D_SCENE_GROUP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_scene_color",		// IDF_DIRECT3D_SCENE_COLOR
	PR_COMMAND | PR_PIC,	PR_PIC,		PR_ONE | PR_SETPAR,				"d3d_scene_texture",	// IDF_DIRECT3D_SCENE_TEXTURE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_fog_color",		// IDF_DIRECT3D_FOG_COLOR
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_fog_mode",			// IDF_DIRECT3D_FOG_MODE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_fog_start",		// IDF_DIRECT3D_FOG_START
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_fog_end",			// IDF_DIRECT3D_FOG_END
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_fog_density",		// IDF_DIRECT3D_FOG_DENSITY
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_view_front",		// IDF_DIRECT3D_VIEW_FRONT
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_view_back",		// IDF_DIRECT3D_VIEW_BACK
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_upper",			// IDF_DIRECT3D_UPPER
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_lower",			// IDF_DIRECT3D_LOWER
	PR_EXPRES,				0,			PR_PARPAR | PR_LITE,			"d3d_pick",				// IDF_DIRECT3D_PICK  // zatím zrušeno
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_LITE,	"d3d_pick_x",			// IDF_DIRECT3D_PICK_X  // zatím zrušeno
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_LITE,	"d3d_pick_y",			// IDF_DIRECT3D_PICK_Y  // zatím zrušeno
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_minfilter",		// IDF_DIRECT3D_MINFILTER
	PR_EXPRES,				PR_PIC,		PR_ONE | PR_SETPAR,				"d3d_2dobject",			// IDF_DIRECT3D_2DOBJECT
	PR_EXPRES,				PR_PIC,		PR_ONE | PR_SETPAR,				"d3d_terrain",			// IDF_DIRECT3D_TERRAIN
	PR_EXPRES,				0,			PR_PARPAR,						"d3d_level",			// IDF_DIRECT3D_LEVEL
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_level_x",			// IDF_DIRECT3D_LEVEL_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"d3d_level_y",			// IDF_DIRECT3D_LEVEL_Y
	PR_EXPRES,				0,			0,								"d3d_empty",			// IDF_DIRECT3D_EMPTY
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_splits",			// IDF_DIRECT3D_SPLITS
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_order",			// IDF_DIRECT3D_ORDER
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_driver",			// IDF_DIRECT3D_DRIVER
	PR_EXPRES,				PR_TEXT,	PR_ONE | PR_SETPAR,				"d3d_rotary",			// IDF_DIRECT3D_ROTARY
	PR_EXPRES,				PR_MAP,		PR_ONE | PR_SETPAR,				"d3d_terrain_map",		// IDF_DIRECT3D_TERRAIN_MAP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_ambiref",			// IDF_DIRECT3D_AMBIREF
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"active",				// IDF_ACTIVE
	PR_EXPRES,				0,			PR_PARPAR,						"polar_radius",			// IDF_POLAR_RADIUS
	PR_EXPRES,				0,			PR_PARPAR,						"polar_angle",			// IDF_POLAR_ANGLE
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"polar_x",				// IDF_POLAR_X
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"polar_y",				// IDF_POLAR_Y
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"file_logic",			// IDF_FILE_LOGIC
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_smooth",			// IDF_DIRECT3D_SMOOTH
	PR_EXPRES,				0,			0,								"precise_timer",		// IDF_PRECISE_TIMER
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_ambient",			// IDF_DIRECT3D_AMBIENT
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_magfilter",		// IDF_DIRECT3D_MAGFILTER
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_mipfilter",		// IDF_DIRECT3D_MIPFILTER
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_owireframe",		// IDF_DIRECT3D_OWIREFRAME
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_olighton",			// IDF_DIRECT3D_OLIGHTON
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_oshades",			// IDF_DIRECT3D_OSHADES
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_ominfilter",		// IDF_DIRECT3D_OMINFILTER
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_omagfilter",		// IDF_DIRECT3D_OMAGFILTER
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_omipfilter",		// IDF_DIRECT3D_OMIPFILTER
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_oculling",			// IDF_DIRECT3D_OCULLING
	0,						0,			PR_NOMOVE,						"d3d_new_group",		// IDF_DIRECT3D_NEW_GROUP
	PR_COMMAND,				0,			0,								"d3d_norm",				// IDF_DIRECT3D_NORM
	PR_EXPRES,				0,			0,								"d3d_circle",			// IDF_DIRECT3D_CIRCLE
	PR_EXPRES,				0,			0,								"d3d_triangle",			// IDF_DIRECT3D_TRIANGLE
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_torus",			// IDF_DIRECT3D_TORUS
	PR_EXPRES,				PR_PIC,		PR_ONE | PR_SETPAR,				"d3d_2dstatic",			// IDF_DIRECT3D_2DSTATIC
	PR_EXPRES,				0,			0,								"d3d_hemisphere",		// IDF_DIRECT3D_HEMISPHERE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_interface",		// IDF_DIRECT3D_INTERFACE
	PR_EXPRES,				PR_EXPRES,	PR_ONE,							"d3d_elapsed",			// IDF_DIRECT3D_ELAPSED
	PR_EXPRES,				0,			0,								"d3d_speed",			// IDF_DIRECT3D_SPEED
	PR_EXPRES,				0,			0,								"d3d_free",				// IDF_DIRECT3D_FREE
	PR_EXPRES,				PR_PIC,		PR_ONE | PR_SETPAR,				"d3d_picture",			// IDF_DIRECT3D_PICTURE
	0,						0,			PR_NOMOVE,						"d3d_text_group",		// IDF_DIRECT3D_TEXT_GROUP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_stage",			// IDF_DIRECT3D_STAGE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_colorop",			// IDF_DIRECT3D_COLOROP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_colorarg1",		// IDF_DIRECT3D_COLORARG1
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_colorarg2",		// IDF_DIRECT3D_COLORARG2
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_alphaop",			// IDF_DIRECT3D_ALPHAOP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_alphaarg1",		// IDF_DIRECT3D_ALPHAARG1
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_alphaarg2",		// IDF_DIRECT3D_ALPHAARG2
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_addressu",			// IDF_DIRECT3D_ADDRESSU
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_addressv",			// IDF_DIRECT3D_ADDRESSV
//	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_border",			// IDF_DIRECT3D_BORDER
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_bias",				// IDF_DIRECT3D_BIAS
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_cone2",			// IDF_DIRECT3D_CONE2
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_tfactor",			// IDF_DIRECT3D_TFACTOR
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE,				"color_synth_alpha",	// IDF_COLOR_SYNTH_ALPHA
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_mipmaps",			// IDF_DIRECT3D_MIPMAPS
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_zgroup",			// IDF_DIRECT3D_ZGROUP
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_zsort",			// IDF_DIRECT3D_ZSORT
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR,				"d3d_zwrite",			// IDF_DIRECT3D_ZWRITE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_ztest",			// IDF_DIRECT3D_ZTEST
	PR_COMMAND,				PR_EXPRES,	PR_ONE,							"d3d_normals",			// IDF_DIRECT3D_NORMALS
	PR_COMMAND,				0,			0,								"d3d_render",			// IDF_DIRECT3D_RENDER
	PR_TEXT,				0,			0,								"text_block",			// IDF_TEXT_BLOCK
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"text_block_begline",	// IDF_TEXT_BLOCK_BEGLINE
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"text_block_begpos",	// IDF_TEXT_BLOCK_BEGPOS
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"text_block_endline",	// IDF_TEXT_BLOCK_ENDLINE
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"text_block_endpos",	// IDF_TEXT_BLOCK_ENDPOS
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"window_pos2",			// IDF_WINDOW_POS2
	PR_EXPRES,				0,			PR_LITE,						"buttoncheck_new2",		// IDF_BUTTONCHECK_NEW2
	PR_EXPRES,				PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"table_new",			// IDF_TABLE_NEW
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"table_width",			// IDF_TABLE_WIDTH
	PR_COMMAND | PR_EXPRES, PR_EXPRES,	PR_ONE | PR_SETPAR,				"shadow_range",			// IDF_SHADOW_RANGE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"table_align",			// IDF_TABLE_ALIGN
	PR_EXPRES,				0,			PR_LITE,						"hscrollbar_new",		// IDF_HSCROLLBAR_NEW
	PR_EXPRES,				0,			PR_LITE,						"vscrollbar_new",		// IDF_VSCROLLBAR_NEW
	PR_PIC,					0,			0,								"picture_xcol",			// IDF_PICTURE_XCOL
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"picture_xcol1",		// IDF_PICTURE_XCOL1
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"picture_xcol2",		// IDF_PICTURE_XCOL2
	PR_PIC,					0,			0,								"picture_scol",			// IDF_PICTURE_SCOL
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"picture_scol1",	// IDF_PICTURE_SCOL1
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR,	"picture_scol2",	// IDF_PICTURE_SCOL2
	PR_COMMAND | PR_BOOL,	PR_PIC,		PR_ONE | PR_SETPAR | PR_PARPAR | PR_LITE,	"print_picture",	// IDF_PRINT_PICTURE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_lod_object",		// IDF_DIRECT3D_LOD_OBJECT
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_lod_distance",		// IDF_DIRECT3D_LOD_DISTANCE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_morph_model",		// IDF_DIRECT3D_MORPH_MODEL
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"d3d_morph_level",		// IDF_DIRECT3D_MORPH_LEVEL
	PR_EXPRES,				PR_PIC,		PR_ONE | PR_SETPAR,				"d3d_lensflare",		// IDF_DIRECT3D_LENSFLARE
	0,						0,			PR_NOMOVE,						"dde_group",			// IDF_DDE_GROUP
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"dde_app",				// IDF_DDE_APP
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"dde_topic",			// IDF_DDE_TOPIC
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"dde_item",				// IDF_DDE_ITEM
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"dde_server",			// IDF_DDE_SERVER
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"dde_type",				// IDF_DDE_TYPE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"dde_number",			// IDF_DDE_NUMBER
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"dde_bool",				// IDF_DDE_BOOL
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"dde_text",				// IDF_DDE_TEXT
	PR_COMMAND | PR_ICON,	PR_ICON,	PR_ONE | PR_SETPAR | PR_LITE,	"dde_icon",				// IDF_DDE_ICON
	PR_COMMAND | PR_PIC,	PR_PIC,		PR_ONE | PR_SETPAR | PR_LITE,	"dde_picture",			// IDF_DDE_PICTURE
	PR_COMMAND | PR_SOUND,	PR_SOUND,	PR_ONE | PR_SETPAR | PR_LITE,	"dde_sound",			// IDF_DDE_SOUND
	PR_COMMAND | PR_MUSIC,	PR_MUSIC,	PR_ONE | PR_SETPAR | PR_LITE,	"dde_music",			// IDF_DDE_MUSIC
	PR_COMMAND | PR_SPRITE,	PR_SPRITE,	PR_ONE | PR_SETPAR | PR_LITE,	"dde_sprite",			// IDF_DDE_SPRITE
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"dde_execute",			// IDF_DDE_EXECUTE
	0,						0,			PR_NOMOVE,						"dll_group",			// IDF_DLL_GROUP
	PR_EXPRES,				PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"dll_load",				// IDF_DLL_LOAD
	PR_COMMAND,				PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"dll_free",				// IDF_DLL_FREE
	PR_EXPRES,				0,			PR_LITE,						"dll_import",			// IDF_DLL_IMPORT
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_SETPAR | PR_LITE,	"dll_import_lib",	// IDF_DLL_IMPORT_LIB
	0,						PR_TEXT,	PR_ONE | PR_NOMOVE | PR_SETPAR | PR_LITE,	"dll_import_name",	// IDF_DLL_IMPORT_NAME
	0,						PR_EXPRES,	PR_ONE | PR_NOMOVE | PR_LITE,	"dll_import_par",		// IDF_DLL_IMPORT_PAR
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_PARPAR | PR_SETPAR | PR_LITE,	"dll_exec",	// IDF_DLL_EXEC
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_new",			// IDF_MEMORY_NEW
	PR_COMMAND,				PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_free",			// IDF_MEMORY_FREE
	PR_EXPRES,				0,			PR_LITE,						"memory_resize",		// IDF_MEMORY_RESIZE
	0,						PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE | PR_NOMOVE,	"memory_resize_adr",	// IDF_MEMORY_RESIZE_ADR
	0,						PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE | PR_NOMOVE,	"memory_resize_new",	// IDF_MEMORY_RESIZE_NEW
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_read",			// IDF_MEMORY_READ
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_write",			// IDF_MEMORY_WRITE
	PR_COMMAND,				PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_copy",			// IDF_MEMORY_COPY
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_compare",		// IDF_MEMORY_COMPARE
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_bytes",			// IDF_MEMORY_BYTES
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_byteu",			// IDF_MEMORY_BYTEU
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_words",			// IDF_MEMORY_WORDS
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_wordu",			// IDF_MEMORY_WORDU
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_3bytes",		// IDF_MEMORY_3BYTES
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_3byteu",		// IDF_MEMORY_3BYTEU
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_dwords",		// IDF_MEMORY_DWORDS
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_dwordu",		// IDF_MEMORY_DWORDU
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_float",			// IDF_MEMORY_FLOAT
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_double",		// IDF_MEMORY_DOUBLE
	PR_COMMAND | PR_BOOL,	PR_BOOL,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_bool",			// IDF_MEMORY_BOOL
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_text0",			// IDF_MEMORY_TEXT0
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_text0p",		// IDF_MEMORY_TEXT0P
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_textn",			// IDF_MEMORY_TEXTN
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_textn_len",		// IDF_MEMORY_TEXTN_LEN
	PR_COMMAND,				PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_load",			// IDF_MEMORY_LOAD
	PR_COMMAND,				PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_save",			// IDF_MEMORY_SAVE
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"window_handle",		// IDF_WINDOW_HANDLE
	0,						PR_BOOL,	PR_ONE | PR_NOMOVE | PR_LITE,	"dll_import_c",			// IDF_DLL_IMPORT_C
	PR_EXPRES,				0,			PR_LITE,						"tabs_new",				// IDF_TABS_NEW
	0,						0,			PR_NOMOVE,						"ini_group",			// IDF_INI_GROUP
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"ini_file",				// IDF_INI_FILE
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR,				"ini_section",			// IDF_INI_SECTION
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR,				"ini_value",			// IDF_INI_VALUE
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_PARPAR,				"ini_string",			// IDF_INI_STRING
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_PARPAR,				"ini_num",				// IDF_INI_NUM
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"reg_key",				// IDF_REG_KEY
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR | PR_LITE,	"reg_subkey",			// IDF_REG_SUBKEY
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR,				"reg_value",			// IDF_REG_VALUE
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_PARPAR,				"reg_string",			// IDF_REG_STRING
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_PARPAR,				"reg_num",				// IDF_REG_NUM
	PR_EXPRES,				0,			PR_LITE,						"memory_3d",			// IDF_MEMORY_3D
	0,						PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_3d_vn",			// IDF_MEMORY_3D_VN
	0,						PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_3d_vert",		// IDF_MEMORY_3D_VERT
	0,						PR_EXPRES,	PR_ONE | PR_LITE,				"memory_3d_norm",		// IDF_MEMORY_3D_NORM
	0,						PR_EXPRES,	PR_ONE | PR_LITE,				"memory_3d_text",		// IDF_MEMORY_3D_TEXT
	0,						PR_EXPRES,	PR_ONE | PR_LITE,				"memory_3d_col",		// IDF_MEMORY_3D_COL
	0,						PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_3d_fn",			// IDF_MEMORY_3D_FN
	0,						PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"memory_3d_face",		// IDF_MEMORY_3D_FACE
	PR_COMMAND,				0,			PR_LITE,						"d3d_texture_file",		// IDF_DIRECT3D_TEXTURE_FILE
	PR_EXPRES,				PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"d3d_memory",			// IDF_DIRECT3D_MEMORY
	PR_EXPRES,				PR_EXPRES,	PR_ONE,							"d3d_child",			// IDF_DIRECT3D_CHILD
	0,						0,			PR_NOMOVE,						"udp_group",			// IDF_UDP_GROUP
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR,				"udp_send_ip",			// IDF_UDP_SEND_IP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"udp_send_port",		// IDF_UDP_SEND_PORT
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"udp_rec_size",			// IDF_UDP_REC_SIZE
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR,				"udp_rec_ip",			// IDF_UDP_REC_IP
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR,				"udp_rec_port",			// IDF_UDP_REC_PORT
	PR_COMMAND | PR_EXPRES,	PR_EXPRES,	PR_ONE | PR_SETPAR | PR_LITE,	"udp_mem",				// IDF_UDP_MEM
	PR_COMMAND | PR_TEXT,	PR_TEXT,	PR_ONE | PR_SETPAR,				"udp_text",				// IDF_UDP_TEXT
};


/////////////////////////////////////////////////////////////////////////////
// inicializaèní tabulka okna struktur

const INITTREE InitTabStr[] = {

	0,			IDF_PETRIK,
		1,			IDF_KROK,
		1,			IDF_VLEVO,
		1,			IDF_VPRAVO,
		1,			IDF_VZAD,
		1,			IDF_RYCHLE,
		1,			IDF_PREDMET,
		1,			IDF_PETER_EXTRA,
			2,			IDF_POZICEX,
			2,			IDF_POZICEY,
			2,			IDF_SMER,
			2,			IDF_PREDMET_POZ,
			2,			IDF_PETER_SWCN1,
			2,			IDF_PETER_SWCN2,
			2,			IDF_PETER_SWCN3,
			2,			IDF_PETER_SWC1,
			2,			IDF_PETER_SWC2,
			2,			IDF_PETER_SWC3,
			2,			IDF_PETER_SWC4,
			2,			IDF_PETER_SWC5,

	0,			IDF_PETRA,
		1,			IDF_PETRA_KROK,
		1,			IDF_PETRA_VLEVO,
		1,			IDF_PETRA_VPRAVO,
		1,			IDF_PETRA_VZAD,
		1,			IDF_PETRA_RYCHLE,
		1,			IDF_PETRA_PREDMET,
		1,			IDF_PETRA_EXTRA,
			2,			IDF_PETRA_POZICEX,
			2,			IDF_PETRA_POZICEY,
			2,			IDF_PETRA_SMER,
			2,			IDF_PETRA_PREDMET_POZ,
			2,			IDF_PETRA_SWCN1,
			2,			IDF_PETRA_SWCN2,
			2,			IDF_PETRA_SWCN3,
			2,			IDF_PETRA_SWC1,
			2,			IDF_PETRA_SWC2,
			2,			IDF_PETRA_SWC3,
			2,			IDF_PETRA_SWC4,
			2,			IDF_PETRA_SWC5,

	0,			IDF_OPER,
		1,			IDF_IF,
			2,			IDF_IF_COND,
			2,			IDF_IF_YES,
			2,			IDF_IF_NO,
		1,			IDF_WHILE,
			2,			IDF_WHILE_COND,
			2,			IDF_WHILE_BODY,
		1,			IDF_REPEAT,
			2,			IDF_REPEAT_N,
			2,			IDF_WHILE_BODY,
		1,			IDF_WHILE_BREAK,
		1,			IDF_CASE,
			2,			IDF_CASE_SUM,
			2,			IDF_CASE_ON,
				3,			IDF_CASE_EQU,
			2,			IDF_CASE_DEF,
		1,			IDF_WAIT,
		1,			IDF_GROUP,
		1,			IDF_LIST,
			2,			IDF_LIST_SIZE,
			2,			IDF_LIST_INDEX,
			2,			IDF_LIST_AUTO,
		1,			IDF_EXIT,
		1,			IDF_COMMENT,
		1,			IDF_OPER_EXTRA,
			2,			IDF_ACTIVE,
			2,			IDF_PRECISE_TIMER,
			2,			IDF_LANGUAGE,
			2,			IDF_USERFONT,
			2,			IDF_ENUMFONTS,
			2,			IDF_CODEPAGE,
			2,			IDF_COMMAND_PAR,
			2,			IDF_EXEC_NAME,
		1,			IDF_DLL_GROUP,
			2,			IDF_DLL_LOAD,
			2,			IDF_DLL_FREE,
			2,			IDF_DLL_IMPORT,
				3,			IDF_DLL_IMPORT_LIB,
				3,			IDF_DLL_IMPORT_NAME,
				3,			IDF_DLL_IMPORT_PAR,
				3,			IDF_DLL_IMPORT_C,
			2,			IDF_DLL_EXEC,
			2,			IDF_MEMORY_NEW,
			2,			IDF_MEMORY_FREE,
			2,			IDF_MEMORY_RESIZE,
				3,			IDF_MEMORY_RESIZE_ADR,
				3,			IDF_MEMORY_RESIZE_NEW,
			2,			IDF_MEMORY_READ,
			2,			IDF_MEMORY_WRITE,
			2,			IDF_MEMORY_COPY,
			2,			IDF_MEMORY_COMPARE,
			2,			IDF_MEMORY_BYTES,
			2,			IDF_MEMORY_BYTEU,
			2,			IDF_MEMORY_WORDS,
			2,			IDF_MEMORY_WORDU,
			2,			IDF_MEMORY_3BYTES,
			2,			IDF_MEMORY_3BYTEU,
			2,			IDF_MEMORY_DWORDS,
			2,			IDF_MEMORY_DWORDU,
			2,			IDF_MEMORY_FLOAT,
			2,			IDF_MEMORY_DOUBLE,
			2,			IDF_MEMORY_BOOL,
			2,			IDF_MEMORY_TEXT0,
			2,			IDF_MEMORY_TEXT0P,
			2,			IDF_MEMORY_TEXTN,
			2,			IDF_MEMORY_TEXTN_LEN,
			2,			IDF_MEMORY_LOAD,
			2,			IDF_MEMORY_SAVE,
			2,			IDF_WINDOW_HANDLE,
			2,			IDF_MEMORY_3D,
				3,			IDF_MEMORY_3D_VN,
				3,			IDF_MEMORY_3D_VERT,
				3,			IDF_MEMORY_3D_NORM,
				3,			IDF_MEMORY_3D_TEXT,
				3,			IDF_MEMORY_3D_COL,
				3,			IDF_MEMORY_3D_FN,
				3,			IDF_MEMORY_3D_FACE,
			2,			IDF_DIRECT3D_MEMORY,
		1,			IDF_INI_GROUP,
			2,			IDF_INI_FILE,
			2,			IDF_INI_SECTION,
			2,			IDF_INI_VALUE,
			2,			IDF_INI_STRING,
			2,			IDF_INI_NUM,
			2,			IDF_REG_KEY,
			2,			IDF_REG_SUBKEY,
			2,			IDF_REG_VALUE,
			2,			IDF_REG_STRING,
			2,			IDF_REG_NUM,

	0,			IDF_CONST,
		1,			IDF_NUM,
			2,			IDF_0,
			2,			IDF_1,
			2,			IDF_2,
			2,			IDF_3,
			2,			IDF_4,
			2,			IDF_5,
			2,			IDF_6,
			2,			IDF_7,
			2,			IDF_8,
			2,			IDF_9,
			2,			IDF_DOT,
			2,			IDF_MINUS,
			2,			IDF_EXP,
		1,			IDF_VYPOC,
			2,			IDF_ADD,
			2,			IDF_SUB,
			2,			IDF_MUL,
			2,			IDF_DIV,
			2,			IDF_MOD,
			2,			IDF_MOCN,
		1,			IDF_FUNC,
			2,			IDF_RAND,
			2,			IDF_SQR,
			2,			IDF_SQRT,
			2,			IDF_ABS,
			2,			IDF_INTEGER,
			2,			IDF_FRAC,
			2,			IDF_INC,
			2,			IDF_DEC,
			2,			IDF_SIN,
			2,			IDF_COS,
			2,			IDF_TG,
			2,			IDF_COTG,
			2,			IDF_ARCSIN,
			2,			IDF_ARCCOS,
			2,			IDF_ARCTG,
			2,			IDF_ARCCOTG,
			2,			IDF_DEGRAD,
			2,			IDF_RADDEG,
			2,			IDF_LOG,
			2,			IDF_LN,
			2,			IDF_PEXP,
			2,			IDF_FACT,
			2,			IDF_POLAR_RADIUS,
				3,			IDF_POLAR_X,
				3,			IDF_POLAR_Y,
			2,			IDF_POLAR_ANGLE,
				3,			IDF_POLAR_X,
				3,			IDF_POLAR_Y,
		1,			IDF_POROVNANI,
			2,			IDF_EQU,
			2,			IDF_NEQU,
			2,			IDF_GR,
			2,			IDF_LT,
			2,			IDF_EQGR,
			2,			IDF_EQLT,
		1,			IDF_LOG_OPER,
			2,			IDF_TRUE,
			2,			IDF_FALSE,
			2,			IDF_AND,
			2,			IDF_OR,
			2,			IDF_XOR,
			2,			IDF_NOT,
		1,			IDF_BIT_GROUP,
			2,			IDF_BIT_AND,
			2,			IDF_BIT_OR,
			2,			IDF_BIT_XOR,
			2,			IDF_BIT_NOT,
			2,			IDF_BIT_MAX,
			2,			IDF_BIT_LEFT,
			2,			IDF_BIT_RIGHT,
		1,			IDF_UHEL,
			2,			IDF_UHEL0,
			2,			IDF_UHEL45,
			2,			IDF_UHEL90,
			2,			IDF_UHEL135,
			2,			IDF_UHEL180,
			2,			IDF_UHEL225,
			2,			IDF_UHEL270,
			2,			IDF_UHEL315,
		1,			IDF_M1,
		1,			IDF_PI,
		1,			IDF_2PI,
		1,			IDF_EUL,
		1,			IDF_BOD32,

	0,			IDF_COMMAND,
		1,			IDF_KLAV,
			2,			IDF_KLAV_VK,
				3,			IDF_VK_NOKEY,
				3,			IDF_KLAV_FX,
					4,			IDF_F1,
					4,			IDF_F2,
					4,			IDF_F3,
					4,			IDF_F4,
					4,			IDF_F5,
					4,			IDF_F6,
					4,			IDF_F7,
					4,			IDF_F8,
					4,			IDF_F9,
					4,			IDF_F10,
					4,			IDF_F11,
					4,			IDF_F12,
				3,			IDF_KLAV_PISM,
					4,			IDF_A,
					4,			IDF_B,
					4,			IDF_C,
					4,			IDF_D,
					4,			IDF_E,
					4,			IDF_F,
					4,			IDF_G,
					4,			IDF_H,
					4,			IDF_I,
					4,			IDF_J,
					4,			IDF_K,
					4,			IDF_L,
					4,			IDF_M,
					4,			IDF_N,
					4,			IDF_O,
					4,			IDF_P,
					4,			IDF_Q,
					4,			IDF_R,
					4,			IDF_S,
					4,			IDF_T,
					4,			IDF_U,
					4,			IDF_V,
					4,			IDF_W,
					4,			IDF_X,
					4,			IDF_Y,
					4,			IDF_Z,
				3,			IDF_KLAV_NUM,
					4,			IDF_VK_NUM0,
					4,			IDF_VK_NUM1,
					4,			IDF_VK_NUM2,
					4,			IDF_VK_NUM3,
					4,			IDF_VK_NUM4,
					4,			IDF_VK_NUM5,
					4,			IDF_VK_NUM6,
					4,			IDF_VK_NUM7,
					4,			IDF_VK_NUM8,
					4,			IDF_VK_NUM9,
					4,			IDF_VK_NUMTECKA,
					4,			IDF_VK_NUMPLUS,
					4,			IDF_VK_NUMMINUS,
					4,			IDF_VK_NUMKRAT,
					4,			IDF_VK_NUMDELENO,
					4,			IDF_VK_NUMENTER,
					4,			IDF_VK_NUMLOCK,
				3,			IDF_KLAV_RUZNE,
					4,			IDF_VK_SPACE,
					4,			IDF_VK_1,
					4,			IDF_VK_2,
					4,			IDF_VK_3,
					4,			IDF_VK_4,
					4,			IDF_VK_5,
					4,			IDF_VK_6,
					4,			IDF_VK_7,
					4,			IDF_VK_8,
					4,			IDF_VK_9,
					4,			IDF_VK_0,
					4,			IDF_VK_LCARKA,
					4,			IDF_VK_POMLCKA,
					4,			IDF_VK_ROVNITKO,
					4,			IDF_VK_ZLOMITKO,
					4,			IDF_VK_LLOMENA,
					4,			IDF_VK_RLOMENA,
					4,			IDF_VK_STREDNIK,
					4,			IDF_VK_RCARKA,
					4,			IDF_VK_CARKA,
					4,			IDF_VK_TECKA,
					4,			IDF_VK_LOMITKO,
				3,			IDF_KLAV_RID,
					4,			IDF_VK_ESC,
					4,			IDF_VK_TAB,
					4,			IDF_VK_CAPSLOCK,
					4,			IDF_VK_WIN,
					4,			IDF_VK_WINR,
					4,			IDF_VK_BS,
					4,			IDF_VK_ENTER,
					4,			IDF_VK_MENU,
					4,			IDF_VK_LEFT,
					4,			IDF_VK_RIGHT,
					4,			IDF_VK_UP,
					4,			IDF_VK_DOWN,
					4,			IDF_VK_INSERT,
					4,			IDF_VK_DELETE,
					4,			IDF_VK_HOME,
					4,			IDF_VK_END,
					4,			IDF_VK_PAGEUP,
					4,			IDF_VK_PAGEDOWN,
					4,			IDF_VK_PRINT,
					4,			IDF_VK_SCROLL,
					4,			IDF_VK_PAUSE,
					4,			IDF_VK_SHIFT,
					4,			IDF_VK_SHIFTR,
					4,			IDF_VK_CTRL,
					4,			IDF_VK_CTRLR,
					4,			IDF_VK_ALT,
					4,			IDF_VK_ALTR,
			2,			IDF_KLAV_CHAR_WAIT,
			2,			IDF_KLAV_CHAR,
			2,			IDF_KLAV_VKEY_WAIT,
			2,			IDF_KLAV_VKEY,
			2,			IDF_KLAV_TEST,
			2,			IDF_KLAV_FLUSH,
			2,			IDF_STATUS_NUMLOCK,
			2,			IDF_STATUS_CAPSLOCK,
			2,			IDF_STATUS_SCROLLLOCK,
			2,			IDF_STATUS_INSERT,
		1,			IDF_MOUSE,
			2,			IDF_MOUSE_X,
			2,			IDF_MOUSE_Y,
			2,			IDF_MOUSE_L,
			2,			IDF_MOUSE_R,
			2,			IDF_MOUSE_LCLICK,
			2,			IDF_MOUSE_RCLICK,
			2,			IDF_MOUSE_LDCLICK,
			2,			IDF_MOUSE_RDCLICK,
			2,			IDF_MOUSE_REGION,
				3,			IDF_MOUSE_REGION_X,
				3,			IDF_MOUSE_REGION_Y,
				3,			IDF_MOUSE_REGION_W,
				3,			IDF_MOUSE_REGION_H,
			2,			IDF_MOUSE_CURSOR,
				3,			IDF_MOUSE_CURSOR_PIC,
				3,			IDF_MOUSE_CURSOR_X,
				3,			IDF_MOUSE_CURSOR_Y,
				3,			IDF_MOUSE_CURSOR_W,
				3,			IDF_MOUSE_CURSOR_H,
		1,			IDF_JOYSTICK_GROUP,
			2,			IDF_JOYSTICK_X,
			2,			IDF_JOYSTICK_Y,
			2,			IDF_JOYSTICK_Z,
			2,			IDF_JOYSTICK_1,
			2,			IDF_JOYSTICK_2,
			2,			IDF_JOYSTICK_3,
			2,			IDF_JOYSTICK_4,
		1,			IDF_DIALOG_GROUP,
			2,			IDF_CAPTION,
NOREFER |		3,			IDF_TEXT_CONST,
			2,			IDF_STATUS_LINE,
NOREFER |		3,			IDF_TEXT_CONST,
			2,			IDF_DIALOG_BOX,
				3,			IDF_DIALOG_BOX_CAPTION,
NOREFER |			4,			IDF_TEXT_CONST,
				3,			IDF_DIALOG_BOX_TEXT,
NOREFER |			4,			IDF_TEXT_ADD,
NOREFER |				5,			IDF_TEXT_CONST,
NOREFER |				5,			IDF_TEXT_CRLF,
NOREFER |				5,			IDF_TEXT_CONST,
NOREFER |				5,			IDF_TEXT_CRLF,
NOREFER |				5,			IDF_TEXT_CONST,
NOREFER |				5,			IDF_TEXT_CRLF,
				3,			IDF_DIALOG_BOX_ICON,
				3,			IDF_DIALOG_BOX_BUTTON,
				3,			IDF_DIALOG_BOX_DEFAULT,
			2,			IDF_DIALOG_TEXT,
				3,			IDF_DIALOG_BOX_CAPTION,
NOREFER |			4,			IDF_TEXT_CONST,
				3,			IDF_DIALOG_TEXT1,
NOREFER |			4,			IDF_TEXT_CONST,
				3,			IDF_DIALOG_TEXT2,
NOREFER |			4,			IDF_TEXT_CONST,
				3,			IDF_DIALOG_TEXT3,
NOREFER |			4,			IDF_TEXT_CONST,
				3,			IDF_DIALOG_TEXT_PAR,
NOREFER |			4,			IDF_TEXT_CONST,
			2,			IDF_DIALOG_OPEN,
				3,			IDF_DIALOG_BOX_CAPTION,
NOREFER |			4,			IDF_TEXT_CONST,
				3,			IDF_DIALOG_FILES,
NOREFER |			4,			IDF_TEXT_CONST,
				3,			IDF_DIALOG_EXT,
NOREFER |			4,			IDF_TEXT_CONST,
				3,			IDF_DIALOG_NAME,
NOREFER |			4,			IDF_TEXT_CONST,
			2,			IDF_DIALOG_SAVE,
				3,			IDF_DIALOG_BOX_CAPTION,
NOREFER |			4,			IDF_TEXT_CONST,
				3,			IDF_DIALOG_FILES,
NOREFER |			4,			IDF_TEXT_CONST,
				3,			IDF_DIALOG_EXT,
NOREFER |			4,			IDF_TEXT_CONST,
				3,			IDF_DIALOG_NAME,
NOREFER |			4,			IDF_TEXT_CONST,
//			2,			IDF_DIALOG_FIND,
//			2,			IDF_DIALOG_COLOR,
			2,			IDF_WINDOW_DEL,
			2,			IDF_WINDOW_ID,
			2,			IDF_DIALOG_GRAPHIC,
			2,			IDF_WINDOWPAR_GROUP,
				3,			IDF_WINDOW_X,
				3,			IDF_WINDOW_Y,
				3,			IDF_WINDOW_W,
				3,			IDF_WINDOW_H,
				3,			IDF_WINDOW_VISIBLE,
				3,			IDF_WINDOW_DISABLE,
				3,			IDF_WINDOW_TEXT,
				3,			IDF_WINDOW_ICON,
				3,			IDF_WINDOW_PIC,
				3,			IDF_BUTTON_CLICK,
				3,			IDF_FOCUS,
				3,			IDF_BUTTON_ESC,
				3,			IDF_WINDOW_W_INT,
				3,			IDF_WINDOW_H_INT,
				3,			IDF_MOUSE_ITEM,
				3,			IDF_WINDOW_POS,
				3,			IDF_WINDOW_POS2,
				3,			IDF_FONTGROUP,
					4,			IDF_FONTBOLD,
					4,			IDF_FONTITALIC,
					4,			IDF_FONTUNDER,
					4,			IDF_FONTSTRIKE,
					4,			IDF_FONTFIXED,
					4,			IDF_FONTSERIF,
					4,			IDF_FONTHEIGHT,
					4,			IDF_FONTWIDTH,
					4,			IDF_FONTANGLE,
					4,			IDF_FONTCOL,
					4,			IDF_FONTBACK,
			2,			IDF_WINDOWS,
				3,			IDF_WINDOW_NEW,
				3,			IDF_WINDOWRO_NEW,
				3,			IDF_WINDOW_FRAME,
				3,			IDF_WINDOW_CAPTION,
				3,			IDF_WINDOW_TOP,
				3,			IDF_WINDOW_RESIZABLE,
				3,			IDF_WINDOW_MAY_CLOSE,
				3,			IDF_WINDOW_MAXIM,
				3,			IDF_WINDOW_MINIM,
			2,			IDF_WINCONTROL_GROUP,
				3,			IDF_BUTTON_NEW,
				3,			IDF_DEFBUTTON_NEW,
				3,			IDF_BUTTONICON_NEW,
				3,			IDF_BUTTONCHECK_NEW2,
				3,			IDF_BUTTONCHECK_NEW,
				3,			IDF_BUTTON3_NEW,
				3,			IDF_BUTTONRADIO_NEW2,
				3,			IDF_BUTTONRADIO_NEW,
				3,			IDF_GROUPBOX_NEW,
				3,			IDF_CHECKBOX_ON,
				3,			IDF_BUTTON3_ON,
				3,			IDF_HTRACKBAR_NEW,
				3,			IDF_VTRACKBAR_NEW,
				3,			IDF_HSCROLLBAR_NEW,
				3,			IDF_VSCROLLBAR_NEW,
			2,			IDF_WINDOWNEW_GROUP,
				3,			IDF_EDIT_NEW,
				3,			IDF_MEMO_NEW,
				3,			IDF_RICHMEMO_NEW,
				3,			IDF_RICHEDIT_NEW,
				3,			IDF_WINTEXT_NEW,
				3,			IDF_WINTEXT_NEW_CENTER,
				3,			IDF_WINTEXT_NEW_RIGHT,
				3,			IDF_WINICON_NEW,
				3,			IDF_WINPIC_NEW,
				3,			IDF_PROGRESS_NEW,
			2,			IDF_WINDOWLIST_GROUP,
				3,			IDF_LISTBOX_NEW,
				3,			IDF_LISTEDIT_NEW,
				3,			IDF_COMBOBOX_NEW,
				3,			IDF_COMBOEDIT_NEW,
				3,			IDF_TABS_NEW,
				3,			IDF_LISTBOX_ADD,
				3,			IDF_LISTBOX_CLEAR,
				3,			IDF_TABLE_NEW,
NOREFER |			4,			IDF_TEXT_ADD,
NOREFER |				5,			IDF_TEXT_CONST,
NOREFER |				5,			IDF_TEXT_CRLF,
NOREFER |				5,			IDF_TEXT_CONST,
NOREFER |				5,			IDF_TEXT_CRLF,
				3,			IDF_TABLE_WIDTH,
				3,			IDF_TABLE_ALIGN,

	0,			IDF_TEXT_GROUP,
		1,			IDF_TEXT_CONST,
		1,			IDF_TEXT_CRLF,
		1,			IDF_TEXT_ADD,
		1,			IDF_EQU_TEXT,
		1,			IDF_TEXT_GT,
		1,			IDF_TEXT_LEFT,
			2,			IDF_TEXT_LEFT_LEN,
			2,			IDF_TEXT_INPUT,
		1,			IDF_TEXT_RIGHT,
			2,			IDF_TEXT_RIGHT_POS,
			2,			IDF_TEXT_INPUT,
		1,			IDF_TEXT_MID,
			2,			IDF_TEXT_MID_POS,
			2,			IDF_TEXT_MID_LEN,
			2,			IDF_TEXT_INPUT,
		1,			IDF_TEXT_LEN,
		1,			IDF_TEXT_SCAN,
		1,			IDF_TEXT_FORMAT,
		1,			IDF_TEXT_FORMAT2,
		1,			IDF_TEXT_FORMATH,
		1,			IDF_ORD,
		1,			IDF_CHAR,
		1,			IDF_TEXT_LOWER,
		1,			IDF_TEXT_UPPER,
		1,			IDF_TEXT_FIND,
			2,			IDF_TEXT_FIND_SUB,
			2,			IDF_TEXT_INPUT,
		1,			IDF_TEXTLINE_NUM,
		1,			IDF_TEXTLINE_GET,
			2,			IDF_TEXTLINE_INDEX,
			2,			IDF_TEXT_INPUT,
		1,			IDF_TEXT_BLOCK,
			2,			IDF_TEXT_BLOCK_BEGLINE,
			2,			IDF_TEXT_BLOCK_BEGPOS,
			2,			IDF_TEXT_BLOCK_ENDLINE,
			2,			IDF_TEXT_BLOCK_ENDPOS,
			2,			IDF_TEXT_INPUT,

	0,			IDF_GRAPH_GROUP,
		1,			IDF_GRAPHIC_GROUP,
			2,			IDF_POINT,
				3,			IDF_GRAPH_COLOR,
NOREFER |			4,			IDF_COLOR,
				3,			IDF_GRAPH_WIDTH,
				3,			IDF_POINT_X,
				3,			IDF_POINT_Y,
			2,			IDF_LINE,
				3,			IDF_GRAPH_COLOR,
NOREFER |			4,			IDF_COLOR,
				3,			IDF_GRAPH_WIDTH,
				3,			IDF_LINE_X1,
				3,			IDF_LINE_Y1,
				3,			IDF_LINE_X2,
				3,			IDF_LINE_Y2,
			2,			IDF_BOX,
				3,			IDF_GRAPH_COLOR,
NOREFER |			4,			IDF_COLOR,
				3,			IDF_GRAPH_WIDTH,
				3,			IDF_BOX_X1,
				3,			IDF_BOX_Y1,
				3,			IDF_BOX_X2,
				3,			IDF_BOX_Y2,
			2,			IDF_BOX_FILL,
				3,			IDF_GRAPH_COLOR,
NOREFER |			4,			IDF_COLOR,
				3,			IDF_BOX_FILL_X1,
				3,			IDF_BOX_FILL_Y1,
				3,			IDF_BOX_FILL_X2,
				3,			IDF_BOX_FILL_Y2,
			2,			IDF_FILL,
				3,			IDF_GRAPH_COLOR,
NOREFER |			4,			IDF_COLOR,
				3,			IDF_FILL_X,
				3,			IDF_FILL_Y,
			2,			IDF_CIRCLE,
				3,			IDF_GRAPH_COLOR,
NOREFER |			4,			IDF_COLOR,
				3,			IDF_GRAPH_WIDTH,
				3,			IDF_CIRCLE_X,
				3,			IDF_CIRCLE_Y,
				3,			IDF_CIRCLE_R,
			2,			IDF_CIRCLE_FILL,
				3,			IDF_GRAPH_COLOR,
NOREFER |			4,			IDF_COLOR,
				3,			IDF_CIRCLE_FILL_X,
				3,			IDF_CIRCLE_FILL_Y,
				3,			IDF_CIRCLE_FILL_R,
			2,			IDF_SPHERE,
				3,			IDF_GRAPH_COLOR,
NOREFER |			4,			IDF_COLOR,
				3,			IDF_SPHERE_X,
				3,			IDF_SPHERE_Y,
				3,			IDF_SPHERE_R,
			2,			IDF_TRIANGLE,
				3,			IDF_GRAPH_COLOR,
NOREFER |			4,			IDF_COLOR,
				3,			IDF_TRIANGLE_X1,
				3,			IDF_TRIANGLE_Y1,
				3,			IDF_TRIANGLE_X2,
				3,			IDF_TRIANGLE_Y2,
				3,			IDF_TRIANGLE_X3,
				3,			IDF_TRIANGLE_Y3,
			2,			IDF_GRAPH_TEXT,
				3,			IDF_GRAPH_TEXT_TEXT,
NOREFER |			4,			IDF_TEXT_CONST,
				3,			IDF_GRAPH_COLOR,
NOREFER |			4,			IDF_COLOR,
				3,			IDF_GRAPH_TEXT_X,
				3,			IDF_GRAPH_TEXT_Y,
				3,			IDF_GRAPH_TEXT_ANGLE,
				3,			IDF_GRAPH_TEXT_H,
				3,			IDF_GRAPH_TEXT_W,
				3,			IDF_GRAPH_TEXT_BOLD,
				3,			IDF_GRAPH_TEXT_ITALIC,
				3,			IDF_GRAPH_TEXT_UNDERLINE,
				3,			IDF_GRAPH_TEXT_STRIKEOUT,
				3,			IDF_GRAPH_TEXT_SERIF,
				3,			IDF_GRAPH_TEXT_COURIER,
			2,			IDF_GRAPH_AREA_WIDTH,
		1,			IDF_PICTURE_GROUP,
			2,			IDF_PIC_DISP,
				3,			IDF_PIC_BACK,
				3,			IDF_PIC_X,
				3,			IDF_PIC_Y,
				3,			IDF_PIC_BLOCK_X,
				3,			IDF_PIC_BLOCK_Y,
				3,			IDF_PIC_BLOCK_W,
				3,			IDF_PIC_BLOCK_H,
			2,			IDF_PIC_GET,
				3,			IDF_PIC_BLOCK_X,
				3,			IDF_PIC_BLOCK_Y,
				3,			IDF_PIC_BLOCK_W,
				3,			IDF_PIC_BLOCK_H,
			2,			IDF_TRANSPARENT,
			2,			IDF_EQU_PIC,
			2,			IDF_PICTURE_WIDTH,
			2,			IDF_PICTURE_HEIGHT,
			2,			IDF_PICTURE_XFLIP,
			2,			IDF_PICTURE_YFLIP,
			2,			IDF_PICTURE_ZOOM,
				3,			IDF_PICTURE_ZOOM_PIC,
				3,			IDF_PICTURE_ZOOM_WIDTH,
				3,			IDF_PICTURE_ZOOM_HEIGHT,
				3,			IDF_PICTURE_ZOOM_INT,
				3,			IDF_PICTURE_ZOOM_DITH,
			2,			IDF_PICTURE_LEVEL,
				3,			IDF_PICTURE_LEVEL_PIC,
				3,			IDF_PICTURE_LEVEL_N,
				3,			IDF_PICTURE_ZOOM_DITH,
			2,			IDF_PICTURE_ROTATE,
				3,			IDF_PICTURE_ROTATE_PIC,
				3,			IDF_PICTURE_ROTATE_ANGLE,
				3,			IDF_PICTURE_ZOOM_INT,
				3,			IDF_PICTURE_ZOOM_DITH,
			2,			IDF_PICTURE_MASK,
				3,			IDF_PICTURE_MASK_PIC,
				3,			IDF_PICTURE_MASK_MASK,
			2,			IDF_PICTURE_XCOL,
				3,			IDF_PICTURE_MASK_PIC,
				3,			IDF_PICTURE_XCOL1,
				3,			IDF_PICTURE_XCOL2,
			2,			IDF_PICTURE_SCOL,
				3,			IDF_PICTURE_MASK_PIC,
				3,			IDF_PICTURE_SCOL1,
				3,			IDF_PICTURE_SCOL2,
			2,			IDF_MAP_PICTURE,
			2,			IDF_ICON_PICTURE,
			2,			IDF_PRINT_PICTURE,
		1,			IDF_DIRECT3D_GROUP,
			2,			IDF_DIRECT3D_MODE_LIST,
			2,			IDF_DIRECT3D_VIEW,
				3,			IDF_DIRECT3D_VIEW_X,
				3,			IDF_DIRECT3D_VIEW_Y,
				3,			IDF_DIRECT3D_VIEW_W,
				3,			IDF_DIRECT3D_VIEW_H,
			2,			IDF_DIRECT3D_OBJECT_ID,
			2,			IDF_DIRECT3D_ELAPSED,
			2,			IDF_DIRECT3D_SETTING_GROUP,
				3,			IDF_DIRECT3D_INTERFACE,
				3,			IDF_DIRECT3D_DRIVER,
				3,			IDF_DIRECT3D_FREE,
				3,			IDF_DIRECT3D_SPEED,
				3,			IDF_DIRECT3D_WIREFRAME,
				3,			IDF_DIRECT3D_LIGHTON,
				3,			IDF_DIRECT3D_SHADES,
				3,			IDF_DIRECT3D_MAGFILTER,
				3,			IDF_DIRECT3D_MINFILTER,
				3,			IDF_DIRECT3D_MIPFILTER,
				3,			IDF_DIRECT3D_OWIREFRAME,
				3,			IDF_DIRECT3D_OLIGHTON,
				3,			IDF_DIRECT3D_OSHADES,
				3,			IDF_DIRECT3D_OCULLING,
			2,			IDF_DIRECT3D_SCENE_GROUP,
				3,			IDF_DIRECT3D_SCENE_COLOR,
				3,			IDF_DIRECT3D_SCENE_TEXTURE,
				3,			IDF_DIRECT3D_FOG_COLOR,
				3,			IDF_DIRECT3D_FOG_MODE,
				3,			IDF_DIRECT3D_FOG_START,
				3,			IDF_DIRECT3D_FOG_END,
				3,			IDF_DIRECT3D_FOG_DENSITY,
				3,			IDF_DIRECT3D_VIEW_FRONT,
				3,			IDF_DIRECT3D_VIEW_BACK,
				3,			IDF_DIRECT3D_PROJECTION,
				3,			IDF_DIRECT3D_RENDER,
//				3,			IDF_DIRECT3D_PICK,	 // zatím zrušeno
//					4,			IDF_DIRECT3D_PICK_X,  // zatím zrušeno
//					4,			IDF_DIRECT3D_PICK_Y,  // zatím zrušeno
			2,			IDF_DIRECT3D_NEW_GROUP,
				3,			IDF_DIRECT3D_EMPTY,
				3,			IDF_DIRECT3D_WALL,
				3,			IDF_DIRECT3D_CIRCLE,
				3,			IDF_DIRECT3D_TRIANGLE,
				3,			IDF_DIRECT3D_CUBE,
				3,			IDF_DIRECT3D_SPHERE,
				3,			IDF_DIRECT3D_HEMISPHERE,
				3,			IDF_DIRECT3D_CYLINDER,
				3,			IDF_DIRECT3D_CONE,
				3,			IDF_DIRECT3D_CONE2,
				3,			IDF_DIRECT3D_TORUS,
				3,			IDF_DIRECT3D_ROTARY,
				3,			IDF_DIRECT3D_2DOBJECT,
				3,			IDF_DIRECT3D_2DSTATIC,
				3,			IDF_DIRECT3D_PICTURE,
				3,			IDF_DIRECT3D_LENSFLARE,
				3,			IDF_DIRECT3D_TERRAIN,
				3,			IDF_DIRECT3D_TERRAIN_MAP,
				3,			IDF_DIRECT3D_TEXT,
				3,			IDF_DIRECT3D_FILE,
			2,			IDF_DIRECT3D_OBJECTS_GROUP,
				3,			IDF_DIRECT3D_VISIBLE,
				3,			IDF_DIRECT3D_OBJECT_DELETE,
				3,			IDF_DIRECT3D_CLONE,
				3,			IDF_DIRECT3D_SPLITS,
				3,			IDF_DIRECT3D_UPPER,
				3,			IDF_DIRECT3D_LOWER,
				3,			IDF_SHADOW_RANGE,
				3,			IDF_DIRECT3D_LINK,
				3,			IDF_DIRECT3D_CHILD,
				3,			IDF_DIRECT3D_INVERT,
				3,			IDF_DIRECT3D_NORM,
				3,			IDF_DIRECT3D_NORMALS,
//				3,			IDF_DIRECT3D_SAVE,	 // zatím zrušeno
				3,			IDF_DIRECT3D_LEVEL,
					4,			IDF_DIRECT3D_LEVEL_X,
					4,			IDF_DIRECT3D_LEVEL_Y,
				3,			IDF_DIRECT3D_ZGROUP,
				3,			IDF_DIRECT3D_ZSORT,
				3,			IDF_DIRECT3D_ZWRITE,
				3,			IDF_DIRECT3D_ZTEST,
				3,			IDF_DIRECT3D_LOD_OBJECT,
				3,			IDF_DIRECT3D_LOD_DISTANCE,
				3,			IDF_DIRECT3D_MORPH_MODEL,
				3,			IDF_DIRECT3D_MORPH_LEVEL,
			2,			IDF_DIRECT3D_TRANS_GROUP,
				3,			IDF_DIRECT3D_SCALEX,
				3,			IDF_DIRECT3D_SCALEY,
				3,			IDF_DIRECT3D_SCALEZ,
				3,			IDF_DIRECT3D_ROTATEX,
				3,			IDF_DIRECT3D_ROTATEY,
				3,			IDF_DIRECT3D_ROTATEZ,
				3,			IDF_DIRECT3D_TRANSX,
				3,			IDF_DIRECT3D_TRANSY,
				3,			IDF_DIRECT3D_TRANSZ,
				3,			IDF_DIRECT3D_ORDER,
			2,			IDF_DIRECT3D_MAT_GROUP,
				3,			IDF_DIRECT3D_COLOR,
				3,			IDF_DIRECT3D_AMBIREF,
				3,			IDF_DIRECT3D_EMISSIVE,
				3,			IDF_DIRECT3D_POWER,
				3,			IDF_DIRECT3D_SPECULAR,
				3,			IDF_DIRECT3D_ALPHAREF,
				3,			IDF_DIRECT3D_TRANSPARENT,
			2,			IDF_DIRECT3D_TEXT_GROUP,
				3,			IDF_DIRECT3D_STAGE,
				3,			IDF_DIRECT3D_TEXTURE,
				3,			IDF_DIRECT3D_SMOOTH,
				3,			IDF_DIRECT3D_TEXTURE_FILE,
				3,			IDF_DIRECT3D_OMAGFILTER,
				3,			IDF_DIRECT3D_OMINFILTER,
				3,			IDF_DIRECT3D_OMIPFILTER,
				3,			IDF_DIRECT3D_MIPMAPS,
				3,			IDF_DIRECT3D_BIAS,
				3,			IDF_DIRECT3D_TEXTMAP,
					4,			IDF_DIRECT3D_TEXTMAP_METHOD,
					4,			IDF_DIRECT3D_TEXTMAP_TYPE,
					4,			IDF_DIRECT3D_TEXTMAP_OX,
					4,			IDF_DIRECT3D_TEXTMAP_OY,
					4,			IDF_DIRECT3D_TEXTMAP_OZ,
					4,			IDF_DIRECT3D_TEXTMAP_RZ,
					4,			IDF_DIRECT3D_TEXTMAP_RX,
					4,			IDF_DIRECT3D_TEXTMAP_RY,
					4,			IDF_DIRECT3D_TEXTMAP_OU,
					4,			IDF_DIRECT3D_TEXTMAP_OV,
					4,			IDF_DIRECT3D_TEXTMAP_SU,
					4,			IDF_DIRECT3D_TEXTMAP_SV,
				3,			IDF_DIRECT3D_ADDRESSU,
				3,			IDF_DIRECT3D_ADDRESSV,
//				3,			IDF_DIRECT3D_BORDER,
				3,			IDF_DIRECT3D_COLOROP,
				3,			IDF_DIRECT3D_COLORARG1,
				3,			IDF_DIRECT3D_COLORARG2,
				3,			IDF_DIRECT3D_ALPHAOP,
				3,			IDF_DIRECT3D_ALPHAARG1,
				3,			IDF_DIRECT3D_ALPHAARG2,
				3,			IDF_DIRECT3D_TFACTOR,
			2,			IDF_DIRECT3D_LIGHT_GROUP,
				3,			IDF_DIRECT3D_LIGHT,
				3,			IDF_DIRECT3D_LIGHT_COLOR,
				3,			IDF_DIRECT3D_LIGHT_TYPE,
				3,			IDF_DIRECT3D_LIGHT_UMBRA,
				3,			IDF_DIRECT3D_LIGHT_PENUMBRA,
				3,			IDF_DIRECT3D_LIGHT_RANGE,
				3,			IDF_DIRECT3D_LIGHT_CATTEN,
				3,			IDF_DIRECT3D_LIGHT_LATTEN,
				3,			IDF_DIRECT3D_LIGHT_QATTEN,
				3,			IDF_DIRECT3D_AMBIENT,
		1,			IDF_COLOR,
		1,			IDF_GET_POINT,
			2,			IDF_GET_POINT_X,
			2,			IDF_GET_POINT_Y,
		1,			IDF_COLOR_LEVEL,
			2,			IDF_GRAPH_COLOR,
			2,			IDF_COLOR_LEVEL_N,
		1,			IDF_COLOR_SYNTH,
			2,			IDF_COLOR_SYNTH_RED,
			2,			IDF_COLOR_SYNTH_GREEN,
			2,			IDF_COLOR_SYNTH_BLUE,
			2,			IDF_COLOR_SYNTH_ALPHA,
		1,			IDF_ICON_GET,
			2,			IDF_PIC_BLOCK_X,
			2,			IDF_PIC_BLOCK_Y,

	0,			IDF_MAP_GROUP,
		1,			IDF_OKRAJ,
		1,			IDF_EQU_ICO,
		1,			IDF_MAP_WIDTH,
		1,			IDF_MAP_HEIGHT,
		1,			IDF_FULLSCREEN,
		1,			IDF_MAP_EXTRA,
			2,			IDF_EQU_MAP,
			2,			IDF_MAP_NEW,
				3,			IDF_MAP_NEW_W,
				3,			IDF_MAP_NEW_H,
			2,			IDF_MAP_ICON,
				3,			IDF_MAP_INPUT,
				3,			IDF_MAP_X,
				3,			IDF_MAP_Y,
			2,			IDF_MAP_SWC1,
				3,			IDF_MAP_INPUT,
				3,			IDF_MAP_X,
				3,			IDF_MAP_Y,
			2,			IDF_MAP_SWC2,
				3,			IDF_MAP_INPUT,
				3,			IDF_MAP_X,
				3,			IDF_MAP_Y,
			2,			IDF_MAP_SWC3,
				3,			IDF_MAP_INPUT,
				3,			IDF_MAP_X,
				3,			IDF_MAP_Y,
			2,			IDF_MAP_SWC4,
				3,			IDF_MAP_INPUT,
				3,			IDF_MAP_X,
				3,			IDF_MAP_Y,
			2,			IDF_MAP_SWC5,
				3,			IDF_MAP_INPUT,
				3,			IDF_MAP_X,
				3,			IDF_MAP_Y,
			2,			IDF_MAP_NUM1,
				3,			IDF_MAP_INPUT,
				3,			IDF_MAP_X,
				3,			IDF_MAP_Y,
			2,			IDF_MAP_NUM2,
				3,			IDF_MAP_INPUT,
				3,			IDF_MAP_X,
				3,			IDF_MAP_Y,
			2,			IDF_MAP_NUM3,
				3,			IDF_MAP_INPUT,
				3,			IDF_MAP_X,
				3,			IDF_MAP_Y,
			2,			IDF_MAP_COPY,
				3,			IDF_MAP_INPUT,
				3,			IDF_MAP_X,
				3,			IDF_MAP_Y,
				3,			IDF_MAP_COPY_INPUT,
				3,			IDF_MAP_COPY_X,
				3,			IDF_MAP_COPY_Y,
				3,			IDF_MAP_COPY_W,
				3,			IDF_MAP_COPY_H,
		1,			IDF_SPRITE_EXTRA,
			2,			IDF_EQU_SPR,
			2,			IDF_SPRITE_SHIFT_X,
			2,			IDF_SPRITE_SHIFT_Y,
			2,			IDF_SPRITE_WIN_X1,
			2,			IDF_SPRITE_WIN_Y1,
			2,			IDF_SPRITE_WIN_X2,
			2,			IDF_SPRITE_WIN_Y2,

	0,			IDF_SOUNDMUSIC_GROUP,
		1,			IDF_SOUND_GROUP,
			2,			IDF_SOUND_PLAY,
				3,			IDF_SOUND_PLAY_SOUND,
				3,			IDF_SOUND_PLAY_VOL,
				3,			IDF_SOUND_PLAY_BAL,
				3,			IDF_SOUND_PLAY_SPEED,
				3,			IDF_SOUND_PLAY_LOOP,
				3,			IDF_SOUND_PLAY_STOP,
			2,			IDF_SOUND_TEST,
			2,			IDF_SOUND_STOP,
			2,			IDF_SOUND_POS,
			2,			IDF_SOUND_PAUSE,
			2,			IDF_SOUND_ADD,
			2,			IDF_EQU_SND,
			2,			IDF_SOUND_DISTINGUISH,
			2,			IDF_SOUND_FUNCTIONS,
				3,			IDF_SOUND_BIT,
				3,			IDF_SOUND_FREQ,
				3,			IDF_SOUND_STEREO,
				3,			IDF_SOUND_LENGTH,
				3,			IDF_SOUND_CONV_8BIT,
				3,			IDF_SOUND_CONV_16BIT,
				3,			IDF_SOUND_CONV_STEREO,
				3,			IDF_SOUND_CONV_MONO,
				3,			IDF_SOUND_CONV_11025,
				3,			IDF_SOUND_CONV_22050,
				3,			IDF_SOUND_CONV_44100,
				3,			IDF_SOUND_CONV_PCM,
				3,			IDF_SOUND_SPEED,
					4,			IDF_SOUND_INPUT,
					4,			IDF_SOUND_SPEED_K,
				3,			IDF_TONGEN,
					4,			IDF_TONGEN_F,
					4,			IDF_TONGEN_T,
		1,			IDF_MUSIC_GROUP,
			2,			IDF_MUSIC_PLAY,
			2,			IDF_MUSIC_LOOP,
			2,			IDF_MUSIC_STOP,
			2,			IDF_MUSIC_PAUSE,
			2,			IDF_MUSIC_TEST,
			2,			IDF_MUSIC_POS,
			2,			IDF_MUSIC_LEN,
			2,			IDF_EQU_MUS,
		1,			IDF_CD_GROUP,
			2,			IDF_CD_PLAY,
			2,			IDF_CD_STOP,
			2,			IDF_CD_PAUSE,
			2,			IDF_CD_TEST,
			2,			IDF_CD_EJECT,
			2,			IDF_CD_LENGTH,
			2,			IDF_CD_TRACKS,
			2,			IDF_CD_POS,
			2,			IDF_CD_TRACK,
			2,			IDF_CD_TRACK_LENGTH,
			2,			IDF_CD_TRACK_POS,
		1,			IDF_MIX_GROUP,
			2,			IDF_MIX_DEVICE,
			2,			IDF_MIX_LINE_DST,
			2,			IDF_MIX_LINE_SRC,
			2,			IDF_MIX_LINE_TYPE,
			2,			IDF_MIX_CONTROL,
			2,			IDF_MIX_CONTROL_TYPE,
			2,			IDF_MIX_CHANNEL,
			2,			IDF_MIX_CONTROL_LIST,
			2,			IDF_MIX_CONTROL_VAL,
	0,			IDF_DATETIME_GROUP,
		1,			IDF_DATETIME_CURRENT,
		1,			IDF_DATETIME_UTC,
		1,			IDF_DATETIME_LOCAL,
		1,			IDF_DATETIME_COMP,
			2,			IDF_DATETIME_COMP_YEAR,
			2,			IDF_DATETIME_COMP_MONTH,
			2,			IDF_DATETIME_COMP_DAY,
			2,			IDF_DATETIME_COMP_HOUR,
			2,			IDF_DATETIME_COMP_MINUTE,
			2,			IDF_DATETIME_COMP_SECONDE,
			2,			IDF_DATETIME_COMP_MSECONDE,
		1,			IDF_DATETIME_GET_YEAR,
		1,			IDF_DATETIME_GET_MONTH,
		1,			IDF_DATETIME_GET_DAY,
		1,			IDF_DATETIME_GET_WEEK,
		1,			IDF_DATETIME_GET_HOUR,
		1,			IDF_DATETIME_GET_MINUTE,
		1,			IDF_DATETIME_GET_SECONDE,
		1,			IDF_DATETIME_GET_MSECONDE,
		1,			IDF_DATETIME_DATEEUR,
		1,			IDF_DATETIME_DATEUSA,
		1,			IDF_DATETIME_DATEJAP,
		1,			IDF_DATETIME_TIME24,
		1,			IDF_DATETIME_TIME12,

	0,			IDF_CONSOLE_GROUP,
		1,			IDF_CONSOLE_ON,
		1,			IDF_CONSOLE_OUT,
NOREFER |	2,			IDF_TEXT_ADD,
NOREFER |		3,			IDF_TEXT_CONST,
NOREFER |		3,			IDF_TEXT_CRLF,
		1,			IDF_CONSOLE_ERR,
NOREFER |	2,			IDF_TEXT_ADD,
NOREFER |		3,			IDF_TEXT_CONST,
NOREFER |		3,			IDF_TEXT_CRLF,
		1,			IDF_CONSOLE_IN,

	0,			IDF_FILE_GROUP,
		1,			IDF_FILE_ACT,
		1,			IDF_FILE_READ,
NOREFER |	2,			IDF_TEXT_CONST,
		1,			IDF_FILE_WRITE,
NOREFER |	2,			IDF_TEXT_CONST,
		1,			IDF_FILE_ERROR,
		1,			IDF_EXEC,
NOREFER |	2,			IDF_TEXT_CONST,
		1,			IDF_EXEC_WAIT,
NOREFER |	2,			IDF_TEXT_CONST,
		1,			IDF_FILE_FILEGROUP,
			2,			IDF_FILE_FILENEW,
NOREFER |		3,			IDF_TEXT_CONST,
			2,			IDF_FILE_DIRNEW,
NOREFER |		3,			IDF_TEXT_CONST,
			2,			IDF_FILE_DELETE,
NOREFER |		3,			IDF_TEXT_CONST,
			2,			IDF_FILE_TEST,
NOREFER |		3,			IDF_TEXT_CONST,
			2,			IDF_FILE_COPY,
			2,			IDF_FILE_MOVE,
			2,			IDF_FILE_LIST,
		1,			IDF_FILE_DATA,
			2,			IDF_FILE_READOFF,
			2,			IDF_FILE_WRITEOFF,
			2,			IDF_FILE_BYTES,
			2,			IDF_FILE_BYTEU,
			2,			IDF_FILE_WORDS,
			2,			IDF_FILE_WORDU,
			2,			IDF_FILE_3BYTES,
			2,			IDF_FILE_3BYTEU,
			2,			IDF_FILE_DWORDS,
			2,			IDF_FILE_DWORDU,
			2,			IDF_FILE_FLOAT,
			2,			IDF_FILE_DOUBLE,
			2,			IDF_FILE_LOGIC,
			2,			IDF_FILE_TEXT0,
			2,			IDF_FILE_TEXTCR,
			2,			IDF_FILE_TEXTN,
			2,			IDF_FILE_TEXTN_LEN,
			2,			IDF_FILE_TEXT_UNICODE,
			2,			IDF_FILE_ICON,
			2,			IDF_FILE_PICTURE,
			2,			IDF_FILE_SOUND,
			2,			IDF_FILE_MUSIC,
			2,			IDF_FILE_SPRITE,
		1,			IDF_FILE_ATTRIB,
			2,			IDF_FILE_SIZE,
			2,			IDF_FILE_RO,
			2,			IDF_FILE_ARC,
			2,			IDF_FILE_HID,
			2,			IDF_FILE_SYS,
			2,			IDF_FILE_DIR,
			2,			IDF_FILE_TMP,
			2,			IDF_FILE_CMP,
			2,			IDF_FILE_OFF,
			2,			IDF_FILE_CREATE,
			2,			IDF_FILE_OPEN,
			2,			IDF_FILE_MODI,
			2,			IDF_ALIAS,
		1,			IDF_FILE_DISKGROUP,
			2,			IDF_FILE_DISK_TYPE,
			2,			IDF_FILE_DISKSIZE,
			2,			IDF_FILE_DISKFREE,
			2,			IDF_FILE_DISKLABEL,
			2,			IDF_FILE_DISKSERIAL,

// !!! po pøidání prvku do skupiny sítì aktualizovat rozlišení pro MINI zavadìè !!!
	0,			IDF_NET_GROUP,
		1,			IDF_COM_GROUP,
			2,			IDF_COM_OPEN,
			2,			IDF_COM_CLOSE,
			2,			IDF_COM_ID,
			2,			IDF_COM_BAUD,
			2,			IDF_COM_STOP,
			2,			IDF_COM_BITS,
			2,			IDF_COM_PARIT,
			2,			IDF_COM_IN,
			2,			IDF_COM_SEND,
			2,			IDF_COM_RECEIVE,
			2,			IDF_COM_NUM,
			2,			IDF_COM_TEXT,
		1,			IDF_DIRECTPLAY_GROUP,
			2,			IDF_CONNECT_LIST,
			2,			IDF_CONNECT_ACT,
			2,			IDF_GAME_IP,
			2,			IDF_GAME_GROUP,
				3,			IDF_GAME_LIST,
				3,			IDF_GAME_ACT,
				3,			IDF_GAME_HOST,
				3,			IDF_PLAYER_MAX,
				3,			IDF_GAME_N1,
				3,			IDF_GAME_N2,
				3,			IDF_GAME_N3,
				3,			IDF_GAME_N4,
				3,			IDF_GAME_N5,
				3,			IDF_GAME_N6,
				3,			IDF_GAME_N7,
				3,			IDF_GAME_N8,
				3,			IDF_GAME_L1,
				3,			IDF_GAME_L2,
				3,			IDF_GAME_L3,
				3,			IDF_GAME_L4,
				3,			IDF_GAME_L5,
				3,			IDF_GAME_L6,
				3,			IDF_GAME_L7,
				3,			IDF_GAME_L8,
			2,			IDF_PLAYER_LIST,
			2,			IDF_PLAYER_ACT,
			2,			IDF_PLAYER_INX,
			2,			IDF_GAME_DATA_GROUP,
				3,			IDF_GAME_PACKET,
				3,			IDF_GAME_DATAN,
				3,			IDF_GAME_DATAB,
				3,			IDF_GAME_DATAT,
				3,			IDF_GAME_DATAL,
		1,			IDF_UDP_GROUP,
			2,			IDF_UDP_SEND_IP,
			2,			IDF_UDP_SEND_PORT,
			2,			IDF_UDP_REC_SIZE,
			2,			IDF_UDP_REC_IP,
			2,			IDF_UDP_REC_PORT,
			2,			IDF_UDP_MEM,
			2,			IDF_UDP_TEXT,
//		1,			IDF_DDE_GROUP,
//			2,			IDF_DDE_APP,
//			2,			IDF_DDE_TOPIC,
//			2,			IDF_DDE_ITEM,
//			2,			IDF_DDE_SERVER,
//			2,			IDF_DDE_TYPE,
//			2,			IDF_DDE_NUMBER,
//			2,			IDF_DDE_BOOL,
//			2,			IDF_DDE_TEXT,
//			2,			IDF_DDE_ICON,
//			2,			IDF_DDE_PICTURE,
//			2,			IDF_DDE_SOUND,
//			2,			IDF_DDE_MUSIC,
//			2,			IDF_DDE_SPRITE,
//			2,			IDF_DDE_EXECUTE,
};

/////////////////////////////////////////////////////////////////////////////
// inicializaèní tabulka okna tøíd

//#define CLASSFNCINDEX		0		// index FNC v bufferu tøíd
//#define CLASSICONINDEX	4		// index ICON v bufferu tøíd
//#define CLASSMAPINDEX		5		// index MAP v bufferu tøíd
//#define CLASSPICINDEX		6		// index BACK v bufferu tøíd
//#define CLASSSPRITEINDEX	9		// index SPRITE v bufferu tøíd

const INITTREE InitTabCls[] = {
	0,			IDF_FNC,
	0,			IDF_REAL,
	0,			IDF_BOOL,
	0,			IDF_TEXT,
	0,			IDF_ICON,
	0,			IDF_MAP,
	0,			IDF_PIC,
	0,			IDF_SND,
	0,			IDF_MUS,
	0,			IDF_SPRITE,
};

