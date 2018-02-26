#ifndef INT_GHARIAL_CONTROL_HPP
#define INT_GHARIAL_CONTROL_HPP

#include "hashids.h"
#include <algorithm>
#include <vector>
#include "pistache/http.h"
#include "pistache/router.h"
#include "pistache/endpoint.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

using namespace Pistache;

Http::Code internal_encode (const std::string& in, json& reply, std::string& error) {

    try { 
    
        auto body = json::parse(in);
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
                    error = "Only unsigned numbers allowed";
                    return Http::Code::Not_Acceptable;
                }
            } 


        } else {
            error = "Invalid body. numbers array is missing or has wrong data type";
            return Http::Code::Not_Acceptable;

        }

        //hash operation 

        if ( v.size() > 10 ) {
            error = "Invalid body. numbers array should have at most 10 unsigned integers";
            return Http::Code::Not_Acceptable;
        }
 
        hashidsxx::Hashids hash;
        std::string out = hash.encode(v.begin(), v.end());

        reply["output"] = out;

        return Http::Code::Ok;

    } catch (...) {

        error = "Server error occurred when encoding";
        return Http::Code::Internal_Server_Error;
    } 

}

Http::Code internal_decode( const std::string& in, json& reply, std::string& error ) {

    try { 
    
        hashidsxx::Hashids hash;
        std::string input;
        std::vector<uint64_t> numbers;
        numbers.clear();

        auto body = json::parse(in);
        
        if ( !body["input"].is_null() 
              && body["input"].is_string() ) { 

            input = body["input"].get<std::string>();                
            numbers = hash.decode(input);
            json j_vec(numbers);

            reply["numbers"] = j_vec; 


        } else {

            error = "Invalid body. decode method, input field should be string";
            return Http::Code::Not_Acceptable;        

        }

        return Http::Code::Ok;

    } catch (...) {

        error = "Server error occurred when decoding";
        return Http::Code::Internal_Server_Error;
    } 

}


#endif
