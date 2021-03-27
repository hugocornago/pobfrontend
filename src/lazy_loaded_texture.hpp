#pragma once

#include <QString>
#include <QSize>

enum class LoadState
{
  NotLoaded,
  Loading,
  Loaded,
  LoadFailed,
};

struct TextureIndex
{
public:
    TextureIndex() = default;

    TextureIndex(size_t idx) : _idx(idx) {}

    size_t GetIndex() const {
        return _idx;
    }

    bool IsValid() const {
        return _idx != 0;
    }

private:
    size_t _idx = 0;
};

struct LazyLoadedTexture
{
  TextureIndex index = 0;
  QString path;
  QSize size = {1, 1};
  LoadState state = LoadState::NotLoaded;
};
