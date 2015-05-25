#pragma once

#include <cstdio>
#include <map>

#include "../thirdparty/agsplugin.h"
#include "../thirdparty/utf8_v2_3_4/source/utf8.h"

#include "font.hxx"

#define STB_IMAGE_IMPLEMENTATION
#include "../thirdparty/stb_image.h"


class BMFontRenderer : public IAGSFontRenderer {
  public:
    IAGSEngine* engine;
    std::map<int, Font*> fonts;

    ~BMFontRenderer()
    {
      for (std::map<int, Font*>::iterator it = fonts.begin(); it != fonts.end(); ++it) {
        if (it->second) {
          delete it->second;
          it->second = NULL;
        }
      }
    }

    void set_font(const char* file, int fontNumber)
    {
      std::string path("");
      if (engine->IsRunningUnderDebugger()) {
        path = "Compiled/";
      }

      engine->PrintDebugConsole(("Loading "+path+file).c_str());
      Font* fnt = new Font();
      fnt->path = path;
      fnt->load(file);

      if (fnt->fnt == NULL) {
        engine->PrintDebugConsole(("Load Failed"+path+file).c_str());
        return;
      }

      fonts[fontNumber] = fnt;
      engine->ReplaceFontRenderer(fontNumber, this);
    }

    bool LoadFromDisk(int fontNumber, int fontSize) { return true; }

    void FreeMemory(int fontNumber)
    {
      std::map<int, Font*>::iterator it = fonts.find(fontNumber);
      if (it == fonts.end()) return;

      if (it->second) {
        delete it->second;
        it->second = NULL;
      }
    }

    bool SupportsExtendedCharacters(int fontNumber) { return true; }

    int GetTextWidth(const char* text, int fontNumber)
    {
      std::string str(text);
      if (utf8::is_valid(str.begin(), str.end())== false) return 0;

      Font* fnt = fonts[fontNumber];

      int width = 0;
      int prev_cp = 0;
      int last_xadvance = 0;
      int last_width = 0;
      for (std::string::iterator it = str.begin(); it != str.end();) {
        int cp = utf8::next(it, str.end());
        std::map<int, BMFntChar*>::iterator ci = fnt->chars.find(cp);
        if (ci == fnt->chars.end()) continue;
        BMFntChar* c = ci->second;

        int amount = 0;
        std::pair<std::multimap<int, BMFntKernings*>::iterator, std::multimap<int, BMFntKernings*>::iterator> ret;
        ret = fnt->kernings.equal_range(prev_cp);
        for (std::multimap<int, BMFntKernings*>::iterator it=ret.first; it != ret.second; ++it) {
          if (it->second->second == cp) {
            amount = it->second->amount;
            break;
          }
        }
        prev_cp = cp;

        width += c->xadvance + amount;
        last_xadvance = c->xadvance;
        last_width = c->width;
      }

      width = width - last_xadvance + last_width;

      return width;
    }

    int GetTextHeight(const char* text, int fontNumber)
    {
      return fonts[fontNumber]->fnt->common.lineHeight;
    }

    void RenderText(const char* text, int fontNumber, BITMAP* dst, int x, int y, int color)
    {
      std::string str(text);
      if (utf8::is_valid(str.begin(), str.end()) == false) return;

      int dw, dh, depth;
      engine->GetBitmapDimensions(dst, &dw, &dh, &depth);

      int r, g, b, a;
      engine->GetRawColorComponents(depth, color, &r, &g, &b, &a);
      int dc = engine->MakeRawColorPixel(depth, r, g, b, a);

      unsigned char** line_8 = engine->GetRawBitmapSurface(dst);
      unsigned short** line_16 = (unsigned short**)line_8;
      unsigned int** line_32 = (unsigned int**)line_8;

      Font* fnt = fonts[fontNumber];

      int xpos = 0;
      int prev_cp = 0;
      for (std::string::iterator it = str.begin(); it != str.end();) {
        int cp = utf8::next(it, str.end());
        std::map<int, BMFntChar*>::iterator ci = fnt->chars.find(cp);
        if (ci == fnt->chars.end()) continue;
        BMFntChar* c = ci->second;

        int amount = 0;
        std::pair<std::multimap<int, BMFntKernings*>::iterator, std::multimap<int, BMFntKernings*>::iterator> ret;
        ret = fnt->kernings.equal_range(prev_cp);
        for (std::multimap<int, BMFntKernings*>::iterator it=ret.first; it != ret.second; ++it) {
          if (it->second->second == cp) {
            amount = it->second->amount;
            break;
          }
        }
        prev_cp = cp;

        xpos += amount;

        for (int iy=c->y; iy < (c->y+c->height); ++iy)
          for (int ix=c->x; ix < (c->x+c->width); ++ix) {
            Font::Texture tex = fnt->pages[c->page];
            if (tex.data[iy*tex.w+ix]) {
              int dx = x + xpos + c->xoffset + (ix - c->x);
              int dy = y + c->yoffset + (iy - c->y);
              
              if (depth == 8) line_8[dy][dx] = dc;
              else if (depth == 16) line_16[dy][dx] = dc;
              else if (depth == 32) line_32[dy][dx] = dc;
            }
          }

        xpos += c->xadvance;
      }
      engine->ReleaseBitmapSurface(dst);
    }

    void AdjustYCoordinateForFont(int* ycoord, int fontNumber)
    {
      ycoord[0]--;
    }

    void EnsureTextValidForFont(char* text, int fontNumber) {}

};
