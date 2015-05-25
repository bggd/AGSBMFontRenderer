#pragma once

#include <cstdio>
#include <vector>
#include <string>
#include <map>

#include "bmfnt.h"

#include "../thirdparty/stb/stb_image.h"


struct Font {
  struct Texture {
    int w, h, n;
    unsigned char* data;
  };

  BMFnt* fnt;
  std::string path;
  std::vector<Texture> pages;
  std::map<int, BMFntChar*> chars;
  std::multimap<int, BMFntKernings*> kernings;

  Font() : fnt(NULL), path("") {}
  ~Font()
  {
    if (fnt) {
      for (int i=0; i < pages.size(); ++i) {
        if (pages[i].data) stbi_image_free(pages[i].data);
      }
      bmfnt_free(fnt);
      fnt = NULL;
    }
  }

  void load(const char* file)
  {
    fnt = bmfnt_load_from_file((path+file).c_str());

    this->pages.reserve(fnt->common.pages);

    for (int i=0; i < fnt->common.pages; ++i) {
      std::string name(fnt->page_list[i].file+1);
      name[name.size()-1] = '\0';

      Texture tex;
      tex.data = stbi_load((path+name).c_str(), &tex.w, &tex.h, &tex.n, 0);
      this->pages.push_back(tex);
    }

    for (int i=0; i < fnt->chars.count; ++i) {
      BMFntChar* c = fnt->char_list + i;
      this->chars[c->id] = c;
    }


    for (int i=0; i < fnt->kernings.count; ++i) {
      BMFntKernings* k = fnt->kerning_list + i;
      std::pair<int, BMFntKernings*> v(k->first, k);
      this->kernings.insert(v);
    }
  }
};

