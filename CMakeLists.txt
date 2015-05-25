cmake_minimum_required(VERSION 2.8)

project(AGSBMFontRenderer)

add_library(AGSBMFontRenderer SHARED
  src/bmfnt.c
  src/plugin_startup.cxx
)

install(TARGETS AGSBMFontRenderer DESTINATION bin)

