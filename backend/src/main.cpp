#include <iostream>
#include <vector>
#include <string>
#include "vector.h"
#include "socket.h"

// Bring some common standard library classes to scope cos we're lazy and want to type less
using std::string;
using std::vector;
using std::cout;
using std::endl; 

int main(int argc, char * argv[])
{
    // Parse command line parameters to a vector of strings
    vector<string> args(argv, argv+argc);

    cout << "Hello, World!" << endl;

    if (args.size() > 1) {
        dmcr::Socket s(args[1], 9393);

        try {
            s.connect();
        } catch (const dmcr::SocketException& e) {
            cout << "SocketException: " << e.message << endl;
        }
    }

    return 0;
}

