[![Build Status](https://travis-ci.org/lefscode/gharial.svg?branch=master)](https://travis-ci.org/lefscode/gharial)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/nlohmann/json/master/LICENSE.MIT)



## Basic Overview

gharial is an http service that generates short unique ids from integers. It is based on pistache.io and hashdis.org 


## Installation 

To install gharial you need to have g++ and cmake installed. After cloning the repository in your local machine, execute the following commands.

    git submodule update --init
    mkdir build-release && cd build-release  
    cmake -DCMAKE_BUILD_TYPE=Release  .. 
    cmake --build .

## Usage
After successfully building gharial, run it.

> ./gharial

Use **/encode** endpoint and POST an array of positive integers. The server will reply back with a unique string. Example:

    curl -H "Content-Type: application/json" -X POST -d '{"numbers": [100,200,300]}' http://localhost:9912/encode 

output:

    {"output":"GZgIW9I09"}

Use **/decode** endpoint to get back the array of integers that created generated the encoded string. Example:

    curl  -X POST -d '{"input" : "GZgIW9I09"}' http://localhost:9912/decode -H "Content-Type: application/json"
    
    {
       "numbers" : [
          100,
          200,
          300
       ]
    }


## Submodules

gharial is based on the following c++ software                   

|                |Description|Link                         |
|----------------|-------------------------------|-----------------------------|
|pistache |`An elegant C++ REST framework.`            |http://pistache.io             |
|json          |`JSON for Modern C++`            |https://github.com/nlohmann/json            |
|hashids          |`generate short unique ids from integers`|https://github.com/schoentoon/hashidsxx|
