#include "texture_loader.hpp"

#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

#include <QImage>
#include <QOpenGLTexture>

namespace {
    constexpr size_t LoadedLowWaterMark = 1024 * 1024 * 1024;
    constexpr size_t LoadedHighWaterMark = 2 * LoadedLowWaterMark;
}

void TextureLoader::request_load(const LazyLoadedTexture& tex)
{
    auto lock = std::lock_guard(_to_load_mtx);
    _to_load.push_back(&tex);
    _to_load_cond.notify_one();
}

void TextureLoader::collect_loaded_textures(std::vector<std::pair<TextureIndex, std::unique_ptr<QImage>>>& loaded)
{
    auto lock = std::lock_guard(_loaded_mtx);
    std::swap(loaded, _loaded);
    _loaded_cond.notify_one();
}

void TextureLoader::stop()
{
    _loop = false;
    _to_load_cond.notify_one();
    _loaded_cond.notify_one();
}

void TextureLoader::run()
{
    while (_loop) {
        bool sleep = false;
        if (_to_load_th.empty()) {
            collect_to_load(_loaded_th.empty());
        }

        auto iter = begin(_to_load_th);
        for (auto e = end(_to_load_th); iter != e; ++iter) {
            auto* llt = *iter;
            if (!_loop) {
                return;
            }
            if (_loaded_mem_size >= LoadedHighWaterMark) {
                break;
            }

            auto& loaded_tex = _loaded_th.emplace_back(
                    std::make_pair(llt->index, nullptr)
                    );
            auto img = std::make_unique<QImage>(llt->path);
            if (!img->isNull()) {
                _loaded_mem_size = img->size().width() * img->size().height() * 4;
                loaded_tex.second = std::move(img);
            }
        }
        sleep = _to_load_th.empty() || _loaded_mem_size >= LoadedHighWaterMark;
        _to_load_th.erase(begin(_to_load_th), iter);

        if (not _loaded_th.empty() && _loop) {
            push_loaded(_loaded_mem_size >= LoadedLowWaterMark);
            if (_loaded_th.empty()) {
                _loaded_mem_size = 0;
            }
        }

        sleep = sleep && not _loaded_th.empty();
        if (sleep) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
        }
    }
}

void TextureLoader::collect_to_load(bool block)
{
    auto lock = std::unique_lock(_to_load_mtx);
    if (block) {
        while (_to_load.empty()) {
            _to_load_cond.wait(lock);
            if (not _loop) {
                return;
            }
        }
    }
    std::swap(_to_load, _to_load_th);
}

void TextureLoader::push_loaded(bool block)
{
    auto lock = std::unique_lock(_loaded_mtx);
    if (block) {
        while (not _loaded.empty()) {
            _loaded_cond.wait(lock);
            if (not _loop) {
                return;
            }
        }
    }
    if (_loaded.empty()) {
        std::swap(_loaded, _loaded_th);
    }
}
