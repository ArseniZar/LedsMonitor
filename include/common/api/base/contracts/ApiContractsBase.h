#pragma once
#ifndef API_CONTRACTS_BASE_H
#define API_CONTRACTS_BASE_H

namespace api
{
    /*=========================Request=============================*/

    struct Request
    {
        virtual bool isOk() const = 0;
        virtual ~Request() = default;
    };

    /*=========================Response=============================*/

    struct Response
    {
        virtual bool isOk() const = 0;
        virtual int getCode() const = 0;
        virtual ~Response() = default;
    };
}
#endif // API_CONTRACTS_BASE_H
