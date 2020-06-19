#include "image_buffer.hpp"

#include <tbb/parallel_for.h>

ImageBuffer::ImageBuffer() {
    this->resize(glm::i32vec2(0));
}

ImageBuffer::ImageBuffer(const glm::i32vec2 &size) {
    this->resize(size);
}

void ImageBuffer::resize(const glm::i32vec2 &size) {
    auto arrayLength = (uint64_t)size.x * size.y;
    this->size = size;
    this->normal = std::shared_ptr<glm::vec3[]>(new glm::vec3[arrayLength]);
    this->albedo = std::shared_ptr<glm::vec3[]>(new glm::vec3[arrayLength]);
    this->buffer = std::shared_ptr<glm::vec3[]>(new glm::vec3[arrayLength]);
    this->textureBuffer = std::shared_ptr<glm::u8vec3[]>(new glm::u8vec3[arrayLength]);

    this->reset();
}

void ImageBuffer::reset() {
    const auto arrayLength = (uint64_t)size.x * size.y;
    memset(this->normal.get(), 0, sizeof(glm::vec3) * arrayLength);
    memset(this->albedo.get(), 0, sizeof(glm::vec3) * arrayLength);
    memset(this->buffer.get(), 0, sizeof(glm::vec3) * arrayLength);
    memset(this->textureBuffer.get(), 0, sizeof(glm::u8vec3) * arrayLength);
}

void ImageBuffer::updateTextureBuffer() {
    auto width = getWidth();
    auto height = getHeight();
    auto len = width * height;

    tbb::parallel_for(size_t(0), size_t(len), [&](size_t index) {
        const auto &src = GetReadonlyBuffer()[index];
        textureBuffer[index] = glm::u8vec3(
            static_cast<uint8_t>(
                glm::clamp(std::pow(src.r, 1.0f / 2.0f), 0.0f, 1.0f) * 255.0f),
            static_cast<uint8_t>(
                glm::clamp(std::pow(src.g, 1.0f / 2.0f), 0.0f, 1.0f) * 255.0f),
            static_cast<uint8_t>(
                glm::clamp(std::pow(src.b, 1.0f / 2.0f), 0.0f, 1.0f) * 255.0f));
    });
}