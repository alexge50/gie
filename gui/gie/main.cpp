//
// Created by alex on 5/15/19.
//

#include <napi.h>

Napi::String HelloWorld(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    return Napi::String::New(env, std::string{"Hello World!"});
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(
            "hello",
            Napi::Function::New(env, HelloWorld)
            );

    return exports;
}