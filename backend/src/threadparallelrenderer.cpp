#include "threadparallelrenderer.h"
#include <thread>
#include <atomic>
#include <vector>
#include <functional>
#include <unistd.h>

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

const int RENDER_SLICES = 50;

ThreadParallelRenderer::ThreadParallelRenderer(ScenePtr scene)
: Renderer(scene), m_balancer(RENDER_SLICES, 1) {
}

std::shared_ptr<RenderResult> ThreadParallelRenderer::render(uint16_t h_res,
                                                             uint16_t v_res,
                                                             uint16_t left,
                                                             uint16_t right,
                                                             uint16_t top,
                                                             uint16_t bottom)
const
{
    unsigned int hw_threads = hardware_threads();
    dmcr::RenderResultPtr result = std::make_shared<dmcr::RenderResult>(
        left, right, top, bottom);

    uint16_t slice_width = (uint16_t)ceil((right - left) / (float)RENDER_SLICES);

    // TODO lock result slices before copying into them
    
    auto slice_fun = [&](int slice_no) {
        uint16_t l = left + slice_width * slice_no;
        uint16_t r = l + slice_width - 1;
        if (r > right)
            r = right;
        auto slice_result = dmcr::Renderer::render(
            h_res, v_res, l, r, top, bottom);
        slice_result->copyInto(result);
    };

    auto thread_fun = [&m_balancer, &slice_fun](int i) {
        while(true)
        {
            int slice = m_balancer.get();
            if( slice == -1) // Ran out of work, all done!
                break;
            std::cout << "Thread " << i << " slice " << slice << std::endl;
            slice_fun(slice);
            m_balancer.finish(slice);
        }
        std::cout << "Thread " << i << " done!" << std::endl;
    };

    std::vector< std::thread > workers;
    for (unsigned int i = 0; i < hw_threads; ++i) {
        workers.emplace_back(thread_fun, i);
    }
    for (unsigned int i = 0; i < workers.size(); ++i) {
        workers[i].join();
    }

    std::cout << "Frame finished!" << std::endl;

    return result;
}