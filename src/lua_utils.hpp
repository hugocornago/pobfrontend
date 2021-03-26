#pragma once

struct lua_State;

void pushCallback(const char* name);

template<typename ...FmtArgs>
void LAssert(lua_State* L, int cond, const char* fmt, FmtArgs ...fmtArgs)
{
  if (!cond) {
    LError(L, fmt, fmtArgs...);
  }
}

void LError(lua_State* L, const char* fmt, ...);
