#include "input/cursesinput.h"
#include "input/keycodes.h"

Input* Input::instance = nullptr;

CursesInput::CursesInput() {
  cbreak();
  noecho();
}

bool CursesInput::_is_key_pressed(int32_t keycode) {
  halfdelay(1);
  int32_t ch = getch();
  bool value;
  if ( ch == BA_KEY_LEFT_SHIFT && ch == BA_KEY_RIGHT_SHIFT) {
    switch(ch) {
      case 0x21:
      case 0x22:
      case 0x23:
      case 0x24:
      case 0x25:
      case 0x26:
      case 0x28:
      case 0x29:
      case 0x2A:
      case 0x2B:
      case 0x3A:
      case 0x3C:
      case 0x3E:
      case 0x3F:
      case 0x40:
      case 0x41:
      case 0x42:
      case 0x43:
      case 0x44:
      case 0x45:
      case 0x46:
      case 0x47:
      case 0x48:
      case 0x49:
      case 0x4A:
      case 0x4B:
      case 0x4C:
      case 0x4D:
      case 0x4E:
      case 0x4F:
      case 0x50:
      case 0x51:
      case 0x52:
      case 0x53:
      case 0x54:
      case 0x55:
      case 0x56:
      case 0x57:
      case 0x58:
      case 0x59:
      case 0x5A:
      case 0x5E:
      case 0x5F:
      case 0x7B:
      case 0x7C:
      case 0x7D:
      case 0x7E:
      case KEY_SBEG:
      case KEY_SCANCEL:
      case KEY_SCOMMAND:
      case KEY_SCOPY:
      case KEY_SCREATE:
      case KEY_SDC:
      case KEY_SDL:
      case KEY_SEND:
      case KEY_SEOL:
      case KEY_SEXIT:
      case KEY_SFIND:
      case KEY_SHELP:
      case KEY_SHOME:
      case KEY_SIC:
      case KEY_SLEFT:
      case KEY_SMESSAGE:
      case KEY_SMOVE:
      case KEY_SNEXT:
      case KEY_SOPTIONS:
      case KEY_SPREVIOUS:
      case KEY_SPRINT:
      case KEY_SREDO:
      case KEY_SREPLACE:
      case KEY_SRIGHT:
      case KEY_SRSUME:
      case KEY_SSAVE:
      case KEY_SSUSPEND:
      case KEY_SUNDO:
        value = true;
      default:
        value = false;
    }
  } else {
    switch(ch) {
      case 0x09:
        value = keycode == BA_KEY_TAB;
      case 0x1B:
        value = keycode == BA_KEY_ESCAPE;
      case 0x20:
        value = keycode == BA_KEY_SPACE;
      case 0x21:
        value = keycode == BA_KEY_0;
      case 0x22:
        value = keycode == BA_KEY_APOSTROPHE;
      case 0x23:
        value = keycode == BA_KEY_3;
      case 0x24:
        value = keycode == BA_KEY_4;
      case 0x25:
        value = keycode == BA_KEY_5;
      case 0x26:
        value = keycode == BA_KEY_7;
      case 0x27:
        value = keycode == BA_KEY_APOSTROPHE;
      case 0x28:
        value = keycode == BA_KEY_9;
      case 0x29:
        value = keycode == BA_KEY_0;
      case 0x2A:
        value = keycode == BA_KEY_8;
      case 0x2B:
        value = keycode == BA_KEY_EQUAL;
      case 0x2C:
        value = keycode == BA_KEY_COMMA;
      case 0x2D:
        value = keycode == BA_KEY_MINUS;
      case 0x2E:
        value = keycode == BA_KEY_PERIOD;
      case 0x2F:
        value = keycode == BA_KEY_SLASH;
      case 0x30:
        value = keycode == BA_KEY_0;
      case 0x31:
        value = keycode == BA_KEY_1;
      case 0x32:
        value = keycode == BA_KEY_2;
      case 0x33:
        value = keycode == BA_KEY_3;
      case 0x34:
        value = keycode == BA_KEY_4;
      case 0x35:
        value = keycode == BA_KEY_5;
      case 0x36:
        value = keycode == BA_KEY_6;
      case 0x37:
        value = keycode == BA_KEY_7;
      case 0x38:
        value = keycode == BA_KEY_8;
      case 0x39:
        value = keycode == BA_KEY_9;
      case 0x3A:
        value = keycode == BA_KEY_SEMIC;
      case 0x3B:
        value = keycode == BA_KEY_SEMIC;
      case 0x3C:
        value = keycode == BA_KEY_COMMA;
      case 0x3D:
        value = keycode == BA_KEY_EQUAL;
      case 0x3E:
        value = keycode == BA_KEY_PERIOD;
      case 0x3F:
        value = keycode == BA_KEY_SLASH;
      case 0x40:
        value = keycode == BA_KEY_2;
      case 0x41:
        value = keycode == BA_KEY_A;
      case 0x42:
        value = keycode == BA_KEY_B;
      case 0x43:
        value = keycode == BA_KEY_C;
      case 0x44:
        value = keycode == BA_KEY_D;
      case 0x45:
        value = keycode == BA_KEY_E;
      case 0x46:
        value = keycode == BA_KEY_F;
      case 0x47:
        value = keycode == BA_KEY_G;
      case 0x48:
        value = keycode == BA_KEY_H;
      case 0x49:
        value = keycode == BA_KEY_I;
      case 0x4A:
        value = keycode == BA_KEY_J;
      case 0x4B:
        value = keycode == BA_KEY_K;
      case 0x4C:
        value = keycode == BA_KEY_L;
      case 0x4D:
        value = keycode == BA_KEY_M;
      case 0x4E:
        value = keycode == BA_KEY_N;
      case 0x4F:
        value = keycode == BA_KEY_O;
      case 0x50:
        value = keycode == BA_KEY_P;
      case 0x51:
        value = keycode == BA_KEY_Q;
      case 0x52:
        value = keycode == BA_KEY_R;
      case 0x53:
        value = keycode == BA_KEY_S;
      case 0x54:
        value = keycode == BA_KEY_T;
      case 0x55:
        value = keycode == BA_KEY_U;
      case 0x56:
        value = keycode == BA_KEY_V;
      case 0x57:
        value = keycode == BA_KEY_W;
      case 0x58:
        value = keycode == BA_KEY_X;
      case 0x59:
        value = keycode == BA_KEY_Y;
      case 0x5A:
        value = keycode == BA_KEY_Z;
      case 0x5B:
        value = keycode == BA_KEY_LBRACK;
      case 0x5C:
        value = keycode == BA_KEY_BSLASH;
      case 0x5D:
        value = keycode == BA_KEY_RBRACK;
      case 0x5E:
        value = keycode == BA_KEY_6;
      case 0x5F:
        value = keycode == BA_KEY_MINUS;
      case 0x60:
        value = keycode == BA_KEY_2;
      case 0x61:
        value = keycode == BA_KEY_A;
      case 0x62:
        value = keycode == BA_KEY_B;
      case 0x63:
        value = keycode == BA_KEY_C;
      case 0x64:
        value = keycode == BA_KEY_D;
      case 0x65:
        value = keycode == BA_KEY_E;
      case 0x66:
        value = keycode == BA_KEY_F;
      case 0x67:
        value = keycode == BA_KEY_G;
      case 0x68:
        value = keycode == BA_KEY_H;
      case 0x69:
        value = keycode == BA_KEY_I;
      case 0x6A:
        value = keycode == BA_KEY_J;
      case 0x6B:
        value = keycode == BA_KEY_K;
      case 0x6C:
        value = keycode == BA_KEY_L;
      case 0x6D:
        value = keycode == BA_KEY_M;
      case 0x6E:
        value = keycode == BA_KEY_N;
      case 0x6F:
        value = keycode == BA_KEY_O;
      case 0x70:
        value = keycode == BA_KEY_P;
      case 0x71:
        value = keycode == BA_KEY_Q;
      case 0x72:
        value = keycode == BA_KEY_R;
      case 0x73:
        value = keycode == BA_KEY_S;
      case 0x74:
        value = keycode == BA_KEY_T;
      case 0x75:
        value = keycode == BA_KEY_U;
      case 0x76:
        value = keycode == BA_KEY_V;
      case 0x77:
        value = keycode == BA_KEY_W;
      case 0x78:
        value = keycode == BA_KEY_X;
      case 0x79:
        value = keycode == BA_KEY_Y;
      case 0x7A:
        value = keycode == BA_KEY_Z;
      case 0x7B:
        value = keycode == BA_KEY_LBRACK;
      case 0x7C:
        value = keycode == BA_KEY_BSLASH;
      case 0x7D:
        value = keycode == BA_KEY_RBRACK;
      case 0x7E:
        value = keycode == BA_KEY_GRAVE;
      case 0x7F:
        value = keycode == BA_KEY_DEL;
      case KEY_DOWN:
        value = keycode == BA_KEY_DOWN;
      case KEY_UP:
        value = keycode == BA_KEY_UP;
      case KEY_LEFT:
        value = keycode == BA_KEY_LEFT;
      case KEY_RIGHT:
        value = keycode == BA_KEY_RIGHT;
      case KEY_HOME:
        value = keycode == BA_KEY_HOME;
      case KEY_BACKSPACE:
        value = keycode == BA_KEY_BS;
      // case KEY_DL:
      // case KEY_IL:
      // case KEY_DC:
      // case KEY_IC:
      // case KEY_EIC:
      // case KEY_CLEAR:
      // case KEY_EOS:
      // case KEY_EOS:
      // case KEY_EOL:
      // case KEY_SF:
      // case KEY_SR:
      // case KEY_NPAGE:
      // case KEY_PPAGE:
      // case KEY_STAB;
      // case KEY_CTAB:
      // case KEY_CATAB:
      case KEY_ENTER:
        value = keycode == BA_KEY_ENTER;
      case KEY_PRINT:
        value = keycode == BA_KEY_PRINT_SCREEN;
      // case KEY_LL:
      // case KEY_A1:
      // case KEY_A3:
      // case KEY_B2:
      // case KEY_C1:
      // case KEY_C3:
      // case KEY_BTAB:
      // case KEY_BEG:
      // case KEY_CANCEL:
      // case KEY_CLOSE:
      // case KEY_COMMAND:
      // case KEY_COPY:
      // case KEY_CREATE:
      case KEY_END:
        value = keycode == BA_KEY_END;
      // case KEY_EXIT:
      // case KEY_FIND:
      // case KEY_HELP:
      // case KEY_MARK:
      // case KEY_MESSAGE:
      // case KEY_MOVE:
      // case KEY_NEXT:
      // case KEY_OPEN:
      // case KEY_OPTIONS:
      // case KEY_PREVIOUS:
      // case KEY_REDO:
      // case KEY_REFERENCE:
      // case KEY_REFRESH:
      // case KEY_REPLACE:
      // case KEY_RESTART:
      // case KEY_RESUME:
      // case KEY_SAVE:
      // case KEY_SBEG:
      // case KEY_SCANCEL:
      // case KEY_SCOMMAND:
      // case KEY_SCOPY:
      // case KEY_SCREATE:
      // case KEY_SDC:
      // case KEY_SDL:
      case KEY_SEND:
        value = keycode == BA_KEY_END;
      // case KEY_SEOL:
      // case KEY_SEXIT:
      // case KEY_SFIND:
      // case KEY_SHELP:
      case KEY_SHOME:
        value = keycode == BA_KEY_HOME;
      // case KEY_SIC:
      case KEY_SLEFT:
        value = keycode == BA_KEY_LEFT;
      // case KEY_SMESSAGE:
      // case KEY_SMOVE:
      // case KEY_SNEXT:
      // case KEY_SOPTIONS:
      // case KEY_SPREVIOUS:
      case KEY_SPRINT:
        value = keycode == BA_KEY_PRINT_SCREEN;
      // case KEY_SREDO:
      // case KEY_SREPLACE:
      case KEY_SRIGHT:
        value = keycode == BA_KEY_RIGHT;
      // case KEY_SRSUME:
      // case KEY_SSAVE:
      // case KEY_SUNDO:
      default:
        value = keycode == BA_KEY_UNKNOWN;
    }
  }
  cbreak();
  return value;
}
