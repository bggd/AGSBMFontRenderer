#ifndef _BMFNT_H
#define _BMFNT_H

#if defined(__cplusplus)
extern "C" {
#endif

struct BMFntPage {
  int id;
  char* file;
};

struct BMFntChar {
  unsigned int id;
  int x;
  int y;
  int width;
  int height;
  int xoffset;
  int yoffset;
  int xadvance;
  int page;
  int chnl;
};

struct BMFntKernings {
  unsigned int first;
  unsigned int second;
  int amount;
};


struct BMFnt {
  struct {
    /* char* face; */
    int size;
    int bold;
    int italic;
    /* char* charset; */
    int unicode;
    int stretchH;
    int smooth;
    int aa;
    int padding[4];
    int spacing[2];
    int outline;
  } info;

  struct {
    int lineHeight;
    int base;
    int scaleW;
    int scaleH;
    int pages;
    int packed;
    int alphaChnl;
    int redChnl;
    int greenChnl;
    int blueChnl;
  } common;

  struct BMFntPage* page_list;

  struct {
    int count;
  } chars;

  struct BMFntChar* char_list;

  struct {
    int count;
  } kernings;

  struct BMFntKernings* kerning_list;

  int i;
};

extern struct BMFnt* bmfnt_load_from_file(const char* path);
extern void bmfnt_free(struct BMFnt* bfnt);

#if defined(__cplusplus)
} /* extern "C" { */
#endif

#endif /* _BMFNT_H */
