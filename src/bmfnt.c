#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "bmfnt.h"


static int
bmfnt_has_key(const char* key, char* value, const char* line, int i, int first, int second)
{
  if (strncmp(key, line+first, i-first) == 0) {
    strncpy(value, line+i+1, second-i);
    value[second-i] = '\0';
    return 1;
  }

  return 0;
}

static void
bmfnt_parse(struct BMFnt* bfnt, const char* line)
{
  int i;
  char value[512] = { '\0' };
  enum bmfnt_tag { INFO, COMMON, PAGE, CHARS, CHAR, KERNINGS, KERNING };
  enum bmfnt_tag type;
  if (strncmp("info", line, 4) == 0) {
    type = INFO;
  }
  else if (strncmp("common", line, 6) == 0) {
    type = COMMON;
  }
  else if (strncmp("page", line, 4) == 0) {
    type = PAGE;
  }
  else if (strncmp("chars", line, 5) == 0) {
    type = CHARS;
  }
  else if (strncmp("char", line, 4) == 0) {
    type = CHAR;
  }
  else if (strncmp("kernings", line, 8) == 0) {
    type = KERNINGS;
  }
  else if (strncmp("kerning", line, 7) == 0) {
    type = KERNING;
  }

  #define BMFNT_HAS_KEY(k) (bmfnt_has_key(k, value, line, i, first, second))
  #define BMFNT_SET_VALUE(tag, key) if (BMFNT_HAS_KEY(#key)) { bfnt->tag.key = atoi(value); }
  #define BMFNTCHAR_SET_VALUE(key) if (BMFNT_HAS_KEY(#key)) { struct BMFntChar* c = bfnt->char_list + bfnt->i; c->key = atoi(value); }
  #define BMFNTKERNING_SET_VALUE(key) if (BMFNT_HAS_KEY(#key)) { struct BMFntKernings* c = bfnt->kerning_list + bfnt->i; c->key = atoi(value); }

  for (i = 0; line[i] != '\n' && line[i] != '\r'; ++i) {
    if (line[i] == '=') {
      int first = i-1;
      int second = i+1;
      int quote = -1;
      char c = line[first];
      while (c != ' ') {
        --first;
        c = line[first];
      }
      first = first + 1;
      c = line[second];
      while (c != '\n' && c != '\r') {
        if (c == '"') quote *= -1;
        if (quote < 0 && c == ' ') break;
        ++second;
        c = line[second];
      }
      second = second - 1;

      if (type == INFO) {
        BMFNT_SET_VALUE(info, size);
        BMFNT_SET_VALUE(info, bold);
        BMFNT_SET_VALUE(info, italic);
        BMFNT_SET_VALUE(info, unicode);
        BMFNT_SET_VALUE(info, stretchH);
        BMFNT_SET_VALUE(info, smooth);
        BMFNT_SET_VALUE(info, aa);
        if (BMFNT_HAS_KEY("padding")) {
          int index = 0;
          char* token = strtok(value, ",");
          while (token) {
            bfnt->info.padding[index] = atoi(token);
            token = strtok(NULL, ",");
            ++index;
          }
        }
        if (BMFNT_HAS_KEY("spacing")) {
          int index = 0;
          char* token = strtok(value, ",");
          while (token) {
            bfnt->info.spacing[index] = atoi(token);
            token = strtok(NULL, ",");
            ++index;
          }
        }
        BMFNT_SET_VALUE(info, outline);
      }
      else if (type == COMMON) {
        BMFNT_SET_VALUE(common, lineHeight);
        BMFNT_SET_VALUE(common, base);
        BMFNT_SET_VALUE(common, scaleW);
        BMFNT_SET_VALUE(common, scaleH);
        BMFNT_SET_VALUE(common, pages);
        BMFNT_SET_VALUE(common, packed);
        BMFNT_SET_VALUE(common, alphaChnl);
        BMFNT_SET_VALUE(common, redChnl);
        BMFNT_SET_VALUE(common, greenChnl);
        BMFNT_SET_VALUE(common, blueChnl);
      }
      else if (type == PAGE) {
        if (BMFNT_HAS_KEY("id")) {
          struct BMFntPage* page = bfnt->page_list + bfnt->i;
          page->id = atoi(value);
        }
        if (BMFNT_HAS_KEY("file")) {
          struct BMFntPage* page = bfnt->page_list + bfnt->i;
          size_t len = sizeof(char)*strlen(value);
          page->file = (char*)malloc(len);
          memset(page->file, '\0', len);
          strcpy(page->file, value);
        }
      }
      else if (type == CHARS) {
        BMFNT_SET_VALUE(chars, count);
      }
      else if (type == CHAR) {
        BMFNTCHAR_SET_VALUE(id);
        BMFNTCHAR_SET_VALUE(x);
        BMFNTCHAR_SET_VALUE(y);
        BMFNTCHAR_SET_VALUE(width);
        BMFNTCHAR_SET_VALUE(height);
        BMFNTCHAR_SET_VALUE(xoffset);
        BMFNTCHAR_SET_VALUE(yoffset);
        BMFNTCHAR_SET_VALUE(xadvance);
        BMFNTCHAR_SET_VALUE(page);
        BMFNTCHAR_SET_VALUE(chnl);
      }
      else if (type == KERNINGS) {
        BMFNT_SET_VALUE(kernings, count);
      }
      else if (type == KERNING) {
        BMFNTKERNING_SET_VALUE(first);
        BMFNTKERNING_SET_VALUE(second);
        BMFNTKERNING_SET_VALUE(amount);
      }
    }
  }
  if (type == INFO) {}
  else if (type == COMMON) {
    bfnt->i = 0;
    bfnt->page_list = (struct BMFntPage*)malloc(sizeof(struct BMFntPage)*bfnt->common.pages);
  }
  else if (type == PAGE) {
    bfnt->i += 1;
  }
  if (type == CHARS) {
    bfnt->i = 0;
    bfnt->char_list = (struct BMFntChar*)malloc(sizeof(struct BMFntChar)*bfnt->chars.count);
  }
  if (type == CHAR) {
    bfnt->i += 1;
  }
  if (type == KERNINGS) {
    bfnt->i = 0;
    bfnt->kerning_list = (struct BMFntKernings*)malloc(sizeof(struct BMFntKernings)*bfnt->kernings.count);
  }
  if (type == KERNING) {
    bfnt->i += 1;
  }

  #undef BMFNTKERNING_SET_VALUE
  #undef BMFNTCHAR_SET_VALUE
  #undef BMFNT_SET_VALUE
  #undef BMFNT_HAS_KEY
}


struct BMFnt*
bmfnt_load_from_file(const char* path)
{
  struct BMFnt* bfnt;
  char line[1028];
  FILE* fp = fopen(path, "rb");
  if (!fp) return NULL;

  bfnt = (struct BMFnt*)malloc(sizeof(struct BMFnt));
  memset(bfnt, 0, sizeof(struct BMFnt));

  rewind(fp);

  while (fgets(line, sizeof(line), fp)) {
    bmfnt_parse(bfnt, line);
  }

  fclose(fp);

  return bfnt;
}

void
bmfnt_free(struct BMFnt* bfnt)
{
  int i;

  if (!bfnt) return;

  for (i=0; i < bfnt->common.pages; ++i) {
    struct BMFntPage* page = bfnt->page_list + i;
    if (page->file) {
      free(page->file);
      page->file = NULL;
    }
  }

  if (bfnt->page_list) {
    free(bfnt->page_list);
    bfnt->page_list = NULL;
  }
  if (bfnt->char_list) {
    free(bfnt->char_list);
    bfnt->char_list = NULL;
  }
  if (bfnt->kerning_list) {
    free(bfnt->kerning_list);
    bfnt->kerning_list = NULL;
  }

  free(bfnt);
  bfnt = NULL;
}
