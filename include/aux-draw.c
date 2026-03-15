#define mcr main_typr->cursor
#define cap main_typr->size
#define buf main_typr->data
#define DEF_BAR "%1$s[%2$zu]: %3$08hhb %3$02hhx %3$d  size: %4$zu "

char stex_name[256];
char fbar[256] = DEF_BAR;
char bar[256];

void printmb(char* str, size_t* pos)
  {
  int l = mblen(str+*pos, 6);
  if (l < 0) return (void)
    printf("\e[30;41m%02hhx\e[0m", str[(*pos)++]);
  printf("%.*s", l, str+*pos);
  *pos += l;
  }

size_t draw_row_wrapless(char* row)
  {
  size_t i = 0;
  while (row[i] && row[i]!='\n')
    if (i==ws.ws_col-1) return putchar('>'), i;
    else printmb(row, &i);
  return i;
  }

void draw_bar()
  {
  sprintf(bar, fbar, stex_name, mcr, buf[mcr], cap);
  if (*msg)
    sprintf(bar+strlen(bar), "msg: %s ", msg);
  if (*arg)
    sprintf(bar+strlen(bar), "arg: %s ", arg);
  printf("\e[%d;1H", ws.ws_row);//move to lowest row
  printf("\e[2K");//erase line
  draw_row_wrapless(bar);
  }


