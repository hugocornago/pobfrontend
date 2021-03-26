#include "lua_utils.hpp"

extern "C" {
    #include "lua.h"
}

extern lua_State *L;

void pushCallback(const char* name) {
    lua_getfield(L, LUA_REGISTRYINDEX, "uicallbacks");
    lua_getfield(L, -1, "MainObject");
    lua_remove(L, -2);
    lua_getfield(L, -1, name);
    lua_insert(L, -2);
}

void LError(lua_State* L, const char* fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  lua_pushvfstring(L, fmt, va);
  va_end(va);
  lua_error(L);
}
