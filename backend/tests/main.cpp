/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <iostream>
#include <functional>
#include <vector>

typedef std::function<void(void)> test_t;
#define DEF_TEST(NAME) tests.push_back(std::make_pair(#NAME, NAME));

// Simple unit testing framework
// Introduce your test functions here..
void test_sharedstream();
void test_tasks();
void test_intersections();
void test_vector();
void test_random();

int main()
{
    std::vector<std::pair<const char*, test_t> > tests;

    // And list them again here..
    DEF_TEST(test_vector);
    DEF_TEST(test_sharedstream);
    DEF_TEST(test_tasks);
    DEF_TEST(test_intersections);
    DEF_TEST(test_random);

    for(size_t i = 0; i < tests.size(); i++)
    {
        std::cout << "Running test #" << i+1 << ": " << tests[i].first << " ..." << std::endl;
        try
        {
            tests[i].second();
        }
        catch(std::exception& e)
        {
            std::cout << "Unhandled exception: " << e.what() << std::endl;
            throw;
        }
        catch(...)
        {
            std::cout << "Unhandled exception!" << std::endl;
            throw;
        }
    }
    std::cout << "All OK!" << std::endl;
    
    return 0;
}



