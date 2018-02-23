#include "controller.hpp"

using namespace std;
using namespace Pistache;


int main(int argc, char *argv[]) {
    
    Port port(9912);

    int thr = 2;

    if (argc >= 2) {
        port = std::stol(argv[1]);

        if (argc == 3)
            thr = std::stol(argv[2]);
    }

    Address a(Ipv4::any(), port);

    Controller c(a);

    c.initialize(thr);
    c.start();

    c.shutdown();

    std::cout << "Stop server" << std::endl;

}

