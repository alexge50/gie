//
// Created by alex on 5/24/19.
//

#ifndef GUI_IMAGEDATA_H
#define GUI_IMAGEDATA_H

#include <nodes/NodeData>
#include <modules/Image.h>

class ImageData: public QtNodes::NodeData
{
public:
    explicit ImageData(): m_data(0, 0) {};
    ImageData(Image data): m_data{std::move(data)} {}

    ImageData(const ImageData&) = default;
    ImageData(ImageData&&) = default;

    QtNodes::NodeDataType type() const override
    {
        return {"Image", "Image"};
    }

    const Image& image() const
    {
        return m_data;
    }

private:
    Image m_data;
};

#endif //GUI_IMAGEDATA_H
