#ifndef GHARIAL_CONTROL_HPP
#define GHARIAL_CONTROL_HPP
#include "hashids.h"

#include <algorithm>
#include <vector>
#include "pistache/http.h"
#include "pistache/router.h"
#include "pistache/endpoint.h"
#include "nlohmann/json.hpp"


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

        try { 
        
            auto body = json::parse(request.body());
            std::vector<uint64_t> v;
            v.clear();       

            if ( !body["numbers"].is_null() 
                  && body["numbers"].is_array() 
                  && body["numbers"].size() < 11) {

                for (auto& element : body["numbers"]) {
                    
                    if ( element.is_number_unsigned() )
                        v.push_back(element.get<uint64_t>());
                    else {
                        //Not_Acceptable
                        response.send(Http::Code::Not_Acceptable, "Only unsigned numbers allowed");
                    }
                } 


            } else {
            
                response.send(Http::Code::Not_Acceptable, "Invalid body. numbers array is missing or has wrong data type");

            }

            //hash operation 

            if ( v.size() > 10 ) 
                response.send(Http::Code::Not_Acceptable, "Invalid body. numbers array should have at most 10 unsigned integers");
            
            hashidsxx::Hashids hash;
            std::string out = hash.encode(v.begin(), v.end());

            json reply = json::object();
            reply["output"] = out;

            response.setMime(MIME(Application, Json));
            response.send(Http::Code::Ok, reply.dump());

        } catch (...) {
    
            response.send(Http::Code::Internal_Server_Error, "Server error occurred when encoding");
        } 

    }

    void decode( const Rest::Request& request, Http::ResponseWriter response ) {

        try { 
        
            hashidsxx::Hashids hash;
            std::string input;
            std::vector<uint64_t> numbers;
            numbers.clear();

            auto body = json::parse(request.body());
            
            if ( !body["input"].is_null() 
                  && body["input"].is_string() ) { 

                json reply = json::object();

                input = body["input"].get<std::string>();                
                numbers = hash.decode(input);
                json j_vec(numbers);

                reply["numbers"] = j_vec; 

                response.setMime(MIME(Application, Json));
                response.send(Http::Code::Ok, reply.dump());

            } else {
            
                response.send(Http::Code::Not_Acceptable, "Invalid body. decode method, input field should be string");

            }

        } catch (...) {
    
            response.send(Http::Code::Internal_Server_Error, "Server error occurred when decoding");
        } 

    }

    std::shared_ptr<Http::Endpoint> endpoint_;
    Rest::Router router_;
};


#endif
