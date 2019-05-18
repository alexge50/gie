//
// Created by alex on 5/15/19.
//

#include <string>
#include <iostream>

#include <gie/Program.h>
#include <gie/Node.h>
#include <gie/NodeLogic.h>
#include <gie/NodeMetadata.h>
#include <gie/NodeLogic.h>
#include <gie/NodeUtil.h>
#include <gie/Argument.h>
#include <gie/Value.h>

#include <boost/python.hpp>

#include <napi.h>

class GieProgram: public Napi::ObjectWrap<GieProgram>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports)
    {
        Napi::Function func = DefineClass(env, "GieProgram", {
            InstanceMethod("run", &GieProgram::run),
            InstanceMethod("addNode", &GieProgram::addNode),
            InstanceMethod("editNode", &GieProgram::editNode),
            InstanceMethod("removeNode", &GieProgram::removeNode),
            InstanceMethod("import", &GieProgram::import),
        });

        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();
        exports.Set("GieProgram", func);
        return exports;
    }

    GieProgram() = default;
    GieProgram(const GieProgram&) = default;
    GieProgram(GieProgram&&) = default;
    GieProgram(const Napi::CallbackInfo& info) : Napi::ObjectWrap<GieProgram>(info) {};

    Napi::Value run(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        auto result_opt = program.run();
        if(!result_opt.has_value())
            return Napi::Object::New(env);

        auto result = result_opt.value();

        {
            boost::python::extract<double> value(result.m_object);

            if(value.check())
                return Napi::Number::New(env, value());
        }

        {
            boost::python::extract<std::string> value(result.m_object);

            if(value.check())
                return Napi::String::New(env, value());
        }

        return Napi::Object::New(env);
    }

    Napi::Value addNode(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        std::string name = info[0].As<Napi::String>();
        std::vector<ArgumentValue> arguments = napiArrayToVector(info[1].As<Napi::Array>());

        auto id = program.addNode(makeNode(
                program.context(),
                name,
                arguments
        ));

        return Napi::Number::New(env, id);
    }

    Napi::Value editNode(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        NodeId id = info[0].As<Napi::Number>().Int64Value();
        std::string name = info[1].As<Napi::String>();
        std::vector<ArgumentValue> arguments = napiArrayToVector(info[2].As<Napi::Array>());

        program.editNode(id, makeNode(
                program.context(),
                name,
                arguments
        ));

        return Napi::Object::New(env);
    }

    Napi::Value removeNode(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        NodeId id = info[0].As<Napi::Number>().Int64Value();
        program.removeNode(id);

        return Napi::Object::New(env);
    }

    Napi::Value import(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        std::string module;
        program.import(module);

        return Napi::Object::New(env);
    }

private:
    static Napi::FunctionReference constructor;

    Program program;

private:
    static std::vector<ArgumentValue> napiArrayToVector(const Napi::Array& array)
    {
        std::vector<ArgumentValue> result;

        for(uint32_t i = 0; i < array.Length(); i++)
        {
            result.emplace_back([](const Napi::Value& x){
                if(x.IsNumber())
                {
                    double value = x.As<Napi::Number>().DoubleValue();
                    return ::Value{boost::python::object{value}};
                }

                if(x.IsString())
                {
                    std::string value = x.As<Napi::String>();
                    return ::Value{boost::python::object{std::move(value)}};
                }


            }(array.Get(i)));
        }

        return result;
    }

};

Napi::Object Init (Napi::Env env, Napi::Object exports)
{
    GieProgram::Init(env, exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
