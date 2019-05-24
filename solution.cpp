#include "Common.h"

using namespace std;

class Shape : public IShape {
private:
    Size size;
    Point position;
    shared_ptr<ITexture> texture;
    ShapeType shapeType;

public:
    void SetShapeType(ShapeType shapeType_) {
        shapeType = shapeType_;
    }

    void SetPosition(Point pos) override {
        position = pos;
    }

    Point GetPosition() const override {
        return position;
    }

    void SetSize(Size size_) override {
        size = size_;
    }

    Size GetSize() const override {
        return size;
    }

    void SetTexture(shared_ptr<ITexture> texture_) override {
        texture = texture_;
    }

    ITexture* GetTexture() const override {
        return texture.get();
    }

    unique_ptr<IShape> Clone() const override {
        unique_ptr<IShape> shape = make_unique<Shape>();
        shape->SetPosition(position);
        shape->SetSize(size);
        shape->SetTexture(texture);
        return move(shape);
    }

    void Draw(Image& image) const override {
        size_t w = min(int(position.x + size.width), int(!image.empty() ? image.front().size() : 0));
        size_t h = min(int(position.y + size.height), int(image.size()));
        bool flag = texture != nullptr;
        size_t size_of_image_w, size_of_image_h;
        if (flag) {
            size_of_image_h = texture->GetImage().size();
            size_of_image_w = size_of_image_h != 0 ? texture->GetImage().front().size() : 0;
        }
        for (size_t i = position.x; i < w; ++i) {
            for (size_t j = position.y; j < h; ++j) {
                if (shapeType == ShapeType::Ellipse) {
                    if (!IsPointInEllipse({int(i - position.x), int(j - position.y)}, size)) {
                        continue;
                    }
                }
                if (flag) {
                    if (i < position.x + size_of_image_w && j < position.y + size_of_image_h) {
                        image[j][i] = texture->GetImage()[j - position.y][i - position.x];
                    } else {
                        image[j][i] = '.';
                    }
                } else {
                    image[j][i] = '.';
                }
            }
        }
    }
};

unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    unique_ptr<Shape> res = make_unique<Shape>();
    res->SetShapeType(shape_type);
    return move(res);
}
