#ifndef ABACUS_CORE_SHAPE_HPP
#define ABACUS_CORE_SHAPE_HPP

#include<vector>
#include<algorithm>

#include"abacus/abacus_type.h"

namespace WW{
namespace abacus{

class Shape{
public:
    Shape() 
        : _shape(),_layout(){
    }
    
    Shape(std::vector<int> shape, LAYOUT_TYPE type = LAYOUT_NCHW)
        :_shape(shape), _layout(){
        _layout = LayoutSimpleFactory::create(type);
        CHECK_EQ(_layout->dims(), _shape.size()) \
            << "Invalid shape vector!";
    }

    ~Shape(){}

    Shape(const Shape& rhs)
        :_shape(), _layout(){
        if(this == &rhs) return;
        
        _layout = rhs.getLayout();
        _shape.assign(rhs.getShape().begin(), rhs.getShape().end());
    }

    Shape& operator= (const Shape& rhs){
        if(this == &rhs) return *this;
        _layout = rhs.getLayout();
        _shape.assign(rhs.getShape().begin(), rhs.getShape().end());
        return *this;
    }

    Shape operator+(const Shape& rhs){
        CHECK_EQ(this->getLayout().get(), rhs.getLayout().get()) \
            << "Unmatch layout type!";
        
        Shape ans(*this);
        for(int i = 0; i < ans.getShape().size(); ++i){
            ans.getShape()[i] += rhs.getShape()[i];
        }

        return ans;
    }

    Shape operator-(const Shape& rhs){
        CHECK_EQ(this->getLayout().get(), rhs.getLayout().get()) \
            << "Unmatch layout type!";
        
        Shape ans(*this);
        for(int i = 0; i < ans.getShape().size(); ++i){
            ans.getShape()[i] = (ans.getShape()[i] <= rhs.getShape()[i]) ? 0 : ans.getShape()[i] - rhs.getShape()[i];
        }

        return ans;
    }

    bool operator==(const Shape& rhs) const {
        if(this->getLayout().get() != rhs.getLayout().get()) return false;
        for(int i = 0; i < this->getShape().size(); ++i){
            if(this->getShape()[i] != rhs.getShape()[i])
                return false;
        }

        return true;
    }

    bool operator> (const Shape& rhs) const{
        if(this->_shape.size() != rhs._shape.size()) return false;
        
        for(int i = 0; i < rhs._shape.size(); ++i){
            if(this->_shape[i] <= rhs._shape[i]) return false;
        }

        return true;
    }

    bool operator>= (const Shape& rhs) const{
        if(this->_shape.size() != rhs._shape.size()) return false;
        
        for(int i = 0; i < rhs._shape.size(); ++i){
            if(this->_shape[i] < rhs._shape[i]) return false;
        }

        return true;
    }


    bool operator <(const Shape& rhs) const{
        if(this->_shape.size() != rhs._shape.size()) return false;
        
        for(int i = 0; i < rhs._shape.size(); ++i){
            if(this->_shape[i] >= rhs._shape[i]) return false;
        }

        return true;
    }

    bool operator <= (const Shape& rhs) const{
        if(this->_shape.size() != rhs._shape.size()) return false;
        
        for(int i = 0; i < rhs._shape.size(); ++i){
            if(this->_shape[i] > rhs._shape[i]) return false;
        }

        return true;
    }

    int batchIndex() const{
        return (_layout) ? _layout->batchIndex() : -1;
    }

    int channelIndex() const{
        return (_layout) ? _layout->channelIndex() : -1;
    }

    int heightIndex() const{
        return (_layout) ? _layout->heightIndex() : -1;
    }

    int widthIndex() const{
        return (_layout) ? _layout->widthIndex() : -1;
    }

    int batch() const{
        return (_layout) ? _shape[batchIndex()] : -1;
    }

    int channel() const{
        return (_layout) ? _shape[channelIndex()] : -1;
    }

    int height() const{
        return (_layout) ? _shape[heightIndex()] : -1;
    }

    int width() const{
        return (_layout) ? _shape[widthIndex()] : -1;
    }

    int dims() const{
        return _shape.size();
    }

    long long count(int start, int end) const{
        if(0 == dims()) return 0;
        if(start >= end) return 0;
        if(start >= dims()) start = dims() - 1;
        if(start < 0) start = 0;
        if(end > dims()) end = dims();
        
        long long sum = 1;
        std::for_each(_shape.begin() + start, _shape.begin() + end, [&](int n){
            sum *= n;
        });

        return sum;
    }

    long long count(int start = 0) const{
        return count(start, dims());
    }

    
    Shape getStride() const{
        Shape stride = Shape::zero(*this);
        
        for(int i = 0; i < dims(); ++i){
            stride.getShape()[i] = count(i);
        }

        return stride;
    }

    void setBatch(int num){
        CHECK_GT(num, 0);
        
        if(batchIndex() != -1){
            getShape()[batchIndex()] = num;
        }
    }

    void setChannel(int num){
        CHECK_GT(num, 0);
        
        if(channelIndex() != -1){
            getShape()[channelIndex()] = num;
        }
    }

    void setHeight(int num){
        CHECK_GT(num, 0);
        
        if(heightIndex() != -1){
            getShape()[heightIndex()] = num;
        }
    }

    void setWidth(int num){
        CHECK_GT(num, 0);
        
        if(widthIndex() != -1){
            getShape()[widthIndex()] = num;
        }
    }

    void setLayout(LAYOUT_TYPE layout){
        if(layout == getLayoutType()) return;
        
        _layout.reset();
        _layout = LayoutSimpleFactory::create(layout);
        _shape.resize(_layout->dims());
    }


    void setShape(const std::vector<int>& shape){
        if(!_layout){
            LOG(ERROR) << "Invalid layout!";
            return; 
        }
        
        CHECK_EQ(_layout->dims(),shape.size()) \
            << "dims between shape and layout is not matched.";

        _shape.assign(shape.begin(), shape.end());
    }

    const std::vector<int>&  getShape() const{
        return _shape;
    }

    std::vector<int>& getShape(){
        return _shape;
    }
    
    const std::shared_ptr<Layout> getLayout() const{
        return _layout;
    }

    LAYOUT_TYPE getLayoutType() const{
        return (_layout) ? _layout->type() : LAYOUT_INVALID;
    }

    
    static Shape zero(const Shape& rhs){
        Shape ans = rhs;
        
        for(int i = 0; i < ans.getShape().size(); ++i){
            ans.getShape()[i] = 0;
        }
        
        return ans;
    }

    static Shape minusone(const Shape& rhs){
        Shape ans = rhs;
        
        for(int i = 0; i < ans.getShape().size(); ++i){
            ans.getShape()[i] = -1;
        }
        return ans;
    }
    

private:
    std::vector<int> _shape;
    std::shared_ptr<Layout> _layout;
    
};

} // namespace abacus
} // namespace WW
#endif