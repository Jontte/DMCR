/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>
#include "../src/sharedstream.h"

using dmcr::SharedStream;
using dmcr::StreamTimeoutException;

struct NonCopyable
{
    int payload;

    NonCopyable(int p ) : payload(p){}
    NonCopyable( NonCopyable&& ) = default;

    NonCopyable(const NonCopyable& )
    {
        // Copying must not be triggered.
        assert(false);
    }
};

void test_sharedstream()
{
    {
        // Test basic functionality:
        SharedStream<int> stream;
        for(int i = 0; i < 100; i++)
            stream.push(i);
        for(int i = 0; i < 100; i++)
            assert(stream.pull() == i);
    }
    {
        // Test move semantics
        SharedStream<NonCopyable> stream;
        for(int i = 0; i < 100; i++)
            stream.push(NonCopyable(i));
        for(int i = 0; i < 100; i++)
            assert(stream.pull().payload == i);

    }
    {
        // Test concurrency and timeouts
        SharedStream<int> stream;

        auto task = [](SharedStream<int>& stream)
        {
            // Produce integers
            for(int i = 0; i < 10; i++)
            {
                stream.push(i);
                usleep(10000);
            }
        };

        std::vector<std::thread> threads;
        for(int i = 0; i < 5 ; i++)
            threads.emplace_back(task, std::ref(stream));

        // We expect to get 5*10 integers and then finally run into a timeout
        int counter = 0;

        try
        {
            while(true)
            {
                // Consume integers
                stream.pull(std::chrono::seconds(1));
                counter ++;
            }
        }
        catch(StreamTimeoutException&)
        {
            assert(counter == 50);
        }

        for(int i = 0; i < 5 ; i++)
            threads[i].join();
    }
    {
        SharedStream<std::unique_ptr<int>> stream;
        {
            std::unique_ptr<int> foo(new int);
            *foo = 10;
            assert(foo && *foo == 10);
            stream.push(std::move(foo));
            assert(!foo);
        }
        {
            std::unique_ptr<int> foo(stream.pull());
            assert(foo);
            assert(*foo == 10);
        }
    }
}


