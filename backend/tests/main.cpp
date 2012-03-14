#include <iostream>
#include <functional>
#include <vector>

typedef std::function<void(void)> test_t;
#define DEF_TEST(NAME) tests.push_back(std::make_pair(#NAME, NAME));

// Simple unit testing framework
// Introduce your test functions here..
void test_sharedstream();
void test_loadbalancer();
void test_tasks();

int main()
{
    std::vector<std::pair<const char*, test_t> > tests;

    // And list them again here..
    DEF_TEST(test_sharedstream);
    DEF_TEST(test_loadbalancer);
    DEF_TEST(test_tasks);

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



