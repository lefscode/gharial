#ifndef GHARIAL_CONTROL_HPP
#define GHARIAL_CONTROL_HPP

#include "internal.hpp"

using json = nlohmann::json;

using namespace Pistache;

class Controller {
public:
    Controller(Address addr)
        : endpoint_(std::make_shared<Http::Endpoint>(addr))
    { }

    void initialize(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
            .threads(thr)
            .flags(Tcp::Options::InstallSignalHandler);
        endpoint_->init(opts);
        configureRoutes();
    }

    void start() {
        endpoint_->setHandler(router_.handler());
        endpoint_->serve();
    }

    void shutdown() {
        std::cout << "Controller stops" << std::endl;
        endpoint_->shutdown();
        
    }

private:
    void configureRoutes() {
        using namespace Rest;

        Routes::Post(router_, "/encode", Routes::bind(&Controller::encode, this));
        Routes::Post(router_, "/decode", Routes::bind(&Controller::decode, this));
    }

    void encode( const Rest::Request& request, Http::ResponseWriter response ) {

        std::string e; 
        json r = json::object();

        Http::Code c = internal_encode (request.body(), r, e);

        if ( c == Http::Code::Ok ) {

            response.setMime(MIME(Application, Json));
            response.send(c, r.dump());

        } else {
        
            response.send(c, e);

        }
    }

    void decode( const Rest::Request& request, Http::ResponseWriter response ) {

        std::string e; 
        json r = json::object();

        Http::Code c = internal_decode (request.body(), r, e);

        if ( c == Http::Code::Ok ) {

            response.setMime(MIME(Application, Json));
            response.send(c, r.dump());

        } else {
        
            response.send(c, e);

        }

    }

    std::shared_ptr<Http::Endpoint> endpoint_;
    Rest::Router router_;
};


#endif
