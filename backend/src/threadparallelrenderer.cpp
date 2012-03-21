/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "threadparallelrenderer.h"
#include "settings.h"
#include <thread>
#include <atomic>
#include <vector>
#include <functional>
#include <unistd.h>
#include <mutex>

#include <iostream>

using namespace dmcr;

static int hardware_threads() {
    unsigned int hw_threads = std::thread::hardware_concurrency();

    if (hw_threads == 0) {
#if defined(__linux__)
        hw_threads = sysconf(_SC_NPROCESSORS_ONLN);
#endif
    }

    if (hw_threads == 0) {
        std::cout << "WARNING: Could not detect count of hardware threads. " <<
                     "Assuming 2." << std::endl;
        hw_threads = 2;             
    }

    return hw_threads;
}

const int RENDER_SLICES = 1;

ThreadParallelRenderer::ThreadParallelRenderer(ScenePtr scene,
                                               uint32_t iterations)
: Renderer(scene), m_iterations(iterations) {
}

std::shared_ptr<RenderResult> ThreadParallelRenderer::render(uint16_t h_res,
                                                             uint16_t v_res,
                                                             uint16_t left,
                                                             uint16_t right,
                                                             uint16_t top,
                                                             uint16_t bottom)
const
{
    int set_render_threads = dmcr::Settings::get().readInt("render-threads",-1);
    unsigned int hw_threads;
    if (set_render_threads < 0)
        hw_threads = hardware_threads();
    else
        hw_threads = set_render_threads;

    dmcr::RenderResultPtr result = std::make_shared<dmcr::RenderResult>(
        left, right, top, bottom);

    std::mutex lock;
    unsigned int iters_claimed = 0;
    unsigned int iters_done = 0;
    
    auto slice_fun = [&]() {
        auto slice_result = dmcr::Renderer::render(
            h_res, v_res, left, right, top, bottom);
        std::lock_guard<std::mutex> g(lock);
        slice_result->blendInto(result, iters_claimed);
        ++iters_done;
    };

    auto thread_fun = [&](int i) {
        while(true)
        {
            {
                if (iters_claimed >= m_iterations)
                    break;
                std::lock_guard<std::mutex> g(lock);
                ++iters_claimed;
            }
            std::cout << "Thread " << i << " iter " << iters_claimed << std::endl;
            slice_fun();
        }
        std::cout << "Thread " << i << " done!" << std::endl;
    };

    std::vector< std::thread > workers;
    for (unsigned int i = 0; i < hw_threads; ++i) {
        workers.emplace_back(thread_fun, i);
    }
    for(auto& th: workers)
        th.join();

    std::cout << "Frame finished!" << std::endl;

    result->setIterationsDone(iters_done);

    return result;
}
