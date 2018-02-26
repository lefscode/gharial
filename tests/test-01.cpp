#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "internal.hpp"

TEST_CASE( "test 1, success at encode" ) {

    std::string e;
    json r = json::object();
    std::string s = "{\"numbers\": [100,200,300, 400, 500]}";

    Http::Code c = internal_encode (s, r, e);

    REQUIRE ( c == Http::Code::Ok );
    REQUIRE ( e == "" );
    REQUIRE ( r["output"] == "ElGh1jcQlHzYSXv" );

}

TEST_CASE( "test 2, success at decode" ) {

    std::string e;
    json r = json::object();
    std::string s = "{\"input\":\"ElGh1jcQlHzYSXv\"}";

    Http::Code c = internal_decode (s, r, e);

    REQUIRE ( c == Http::Code::Ok );
    REQUIRE ( e == "" );
    REQUIRE ( r["numbers"].size() == 5 );
    REQUIRE ( r["numbers"][0] == 100 );
    REQUIRE ( r["numbers"][4] == 500 );

}


