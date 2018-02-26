#include "controller.hpp"
#include "clara.hpp"

using namespace std;
using namespace Pistache;
using namespace clara;

int main(int argc, char *argv[]) {

    int port_val = -1;
    int thread_count = -1;
    auto cli
        = Opt( port_val, "port" )
            ["-p"]["--port"]
            ("How wide should it be?")
        | Opt( thread_count, "threads" )
            ["-t"]["--threads"]
            ("By what name should I be known");

    auto result = cli.parse( Args( argc, argv ) );
    if( !result ) {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        exit(1);
    }

    if ( port_val == -1 )
        port_val = 9912;

    if ( thread_count == -1 )
        thread_count = 2;

    std::cout << "Starting Server at port: " << port_val << " using: " << thread_count << " thread(s)\n";

    Port port(port_val);

    Address a(Ipv4::any(), port);

    Controller c(a);

    c.initialize(thread_count);
    c.start();

    c.shutdown();

    std::cout << "Stop server" << std::endl;

}

