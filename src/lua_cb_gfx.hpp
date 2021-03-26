#pragma once

struct lua_State;

int l_NewImageHandle(lua_State* L);
int l_imgHandleGC(lua_State* L);
int l_imgHandleLoad(lua_State* L) ;
int l_imgHandleUnload(lua_State* L);
int l_imgHandleIsValid(lua_State* L);
int l_imgHandleIsLoading(lua_State* L);
int l_imgHandleSetLoadingPriority(lua_State* L);
int l_imgHandleImageSize(lua_State* L);
int l_RenderInit(lua_State* L);
int l_GetScreenSize(lua_State* L);
int l_SetClearColor(lua_State* L);
int l_SetDrawLayer(lua_State* L);
int l_SetViewport(lua_State* L);
int l_SetDrawColor(lua_State* L);
int l_DrawImage(lua_State* L);
int l_DrawImageQuad(lua_State* L);
int l_DrawString(lua_State* L);
int l_DrawStringWidth(lua_State* L) ;
int l_DrawStringCursorIndex(lua_State* L) ;
