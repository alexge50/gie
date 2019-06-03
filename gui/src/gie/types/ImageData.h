//
// Created by alex on 5/24/19.
//

#ifndef GUI_IMAGEDATA_H
#define GUI_IMAGEDATA_H

#include <boost/python.hpp>
#include "TypeData.h"
#include <types/Image.h>

class ImageData: public TypeData
{
public:
    explicit ImageData(): m_data(boost::python::object(Image{0, 0})) {}
    ImageData(const Image& data): m_data{boost::python::object(data)} {}
    ImageData(const Value& data): m_data{data} {}

    ImageData(const ImageData&) = default;
    ImageData(ImageData&&) = default;

    QtNodes::NodeDataType type() const override
    {
        return {"Image", "Image"};
    }

    const Image& image() const
    {
        return boost::python::extract<Image>(m_data.m_object);
    }

    Value value() override { return m_data; }

private:
    Value m_data;
};

#endif //GUI_IMAGEDATA_H
