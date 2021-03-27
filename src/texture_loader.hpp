#pragma once

#include <condition_variable>
#include <functional>
#include <memory>

#include <QThread>
#include <mutex>
#include <vector>

#include "lazy_loaded_texture.hpp"

class QOpenGLTexture;

class TextureLoader: public QThread
{
public:
    void request_load(const LazyLoadedTexture& tex);
    void collect_loaded_textures(std::vector<std::pair<TextureIndex, std::unique_ptr<QImage>>>& loaded);
    void stop();

    void run() override;

private:
    void collect_to_load(bool block);
    void push_loaded(bool block);

private:
    bool _loop = true;

    std::mutex _to_load_mtx;
    std::condition_variable _to_load_cond;
    std::vector<const LazyLoadedTexture*> _to_load;

    std::mutex _loaded_mtx;
    std::condition_variable _loaded_cond;
    std::vector<std::pair<TextureIndex, std::unique_ptr<QImage>>> _loaded;

    std::vector<const LazyLoadedTexture*> _to_load_th;
    std::vector<std::pair<TextureIndex, std::unique_ptr<QImage>>> _loaded_th;
    size_t _loaded_mem_size = 0;
};
