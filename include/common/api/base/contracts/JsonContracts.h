#pragma once
#ifndef JSON_CONTRACTS_H
#define JSON_CONTRACTS_H

#include <Arduino.h>

namespace api::json
{
    /*=====================Convertible=========================*/

    struct Convertible
    {
        virtual bool isOk() const = 0;
        virtual ~Convertible() = default;
    };
}
#endif // JSON_CONTRACTS_BASE_H