#pragma once
#ifndef HTTP_METHOD_H
#define HTTP_METHOD_H
#include <Arduino.h>
#include <cstdint>
#include <StringUtils.h>

enum class HTTPMethod: uint8_t { ANY, GET, HEAD, POST, PUT, PATCH, DELETE, OPTIONS };

inline HTTPMethod parseMethod(const Text& str) {
    switch (str.hash()) {
        case su::SH("GET"):     return HTTPMethod::GET;
        case su::SH("POST"):    return HTTPMethod::POST;
        case su::SH("PUT"):     return HTTPMethod::PUT;
        case su::SH("DELETE"):  return HTTPMethod::DELETE;
        case su::SH("OPTIONS"): return HTTPMethod::OPTIONS;
        case su::SH("HEAD"):    return HTTPMethod::HEAD;
        case su::SH("PATCH"):   return HTTPMethod::PATCH;
        default:                return HTTPMethod::ANY;
    }
}

inline const __FlashStringHelper* methodToString(HTTPMethod method) {
    switch (method) {
        case HTTPMethod::GET:     return F("GET");
        case HTTPMethod::POST:    return F("POST");
        case HTTPMethod::PUT:     return F("PUT");
        case HTTPMethod::DELETE:  return F("DELETE");
        case HTTPMethod::OPTIONS: return F("OPTIONS");
        case HTTPMethod::HEAD:    return F("HEAD");
        case HTTPMethod::PATCH:   return F("PATCH");
        default:                  return F("ANY");
    }
}

#endif // HTTP_METHOD_H