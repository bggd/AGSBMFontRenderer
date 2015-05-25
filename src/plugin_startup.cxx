#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define THIS_IS_THE_PLUGIN
#include "../thirdparty/agsplugin.h"

#include "renderer.hxx"


BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_PROCESS_DETACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
      break;
  }
  return TRUE;
}

IAGSEditor* editor;

const char* script_header = "import void SetBMFont(String file, int fontNumber);\r\n";

LPCSTR AGS_GetPluginName()
{
  return "AGSBMFontRenderer";
}

int AGS_EditorStartup(IAGSEditor* lpEditor)
{
  editor = lpEditor;
  editor->RegisterScriptHeader(script_header);
  return 0;
}

void AGS_EditorShutdown()
{
  editor->UnregisterScriptHeader(script_header);
}

void AGS_EditorProperties(HWND parent)
{
  MessageBox(parent, "AGSBMFontRenderer v0.1 By birthggd", "About", MB_OK | MB_ICONINFORMATION);
}

int AGS_EditorSaveGame(char* buffer, int bufsize)
{
  return 0;
}

void AGS_EditorLoadGame(char* buffer, int bufsize)
{
}

BMFontRenderer* renderer;

void set_bmfont(const char* file, int fontNumber)
{
  renderer->set_font(file, fontNumber);
}

void AGS_EngineStartup(IAGSEngine* lpEngine)
{
  renderer = new BMFontRenderer();
  renderer->engine = lpEngine;
  lpEngine->RegisterScriptFunction("SetBMFont", set_bmfont);
}

void AGS_EngineShutdown()
{
  delete renderer;
}

int AGS_EngineOnEvent(int event, int data)
{
  return 0;
}

