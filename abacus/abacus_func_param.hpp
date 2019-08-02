#ifndef ABACUS_ABACUS_FUNC_PARAM_HPP
#define ABACUS_ABACUS_FUNC_PARAM_HPP

namespace WW{
namespace abacus{

template<typename TargetType, typename DataType>
struct GemmParam{
    
    GemmParam(bool transA = false,
              bool transB = false,
              int m = 0,
              int n = 0,
              int k = 0,
              int lda = 0,
              int ldb = 0,
              int ldc = 0,
              DataType alpha = 0.0,
              DataType beta  = 0.0)
              : _transpose_x(transA),
                _transpose_y(transB),
                _m(m), _n(n), _k(k),
                _lda(lda), _ldb(ldb),
                _ldc(ldc), _alpha(alpha),
                _beta(beta){}
    
    GemmParam<TargetType, DataType>& operator= (const GemmParam<TargetType, DataType>& rhs){
        if(this == &rhs) return *this;
        
        this->_m = rhs._m;
        this->_n = rhs._n;
        this->_k = rhs._k;
        this->_lda = rhs._lda;
        this->_ldb = rhs._ldb;
        this->_ldc = rhs._ldc;
        this->_transpose_x = rhs._transpose_x;
        this->_transpose_y = rhs._transpose_y;
        this->_alpha = rhs._alpha;
        this->_beta = rhs._beta;
    }

    bool operator == (const GemmParam<TargetType, DataType>& rhs){
        if(this == &rhs) return true;
        
        if(this->_m == rhs._m &&
           this->_n == rhs._n &&
           this->_k == rhs._k &&
           this->_lda == rhs._lda &&
           this->_ldb == rhs._ldb &&
           this->_ldc == rhs._ldc &&
           this->_transpose_x == rhs._transpose_x &&
           this->_transpose_y == rhs._transpose_y &&
           this->_alpha == rhs._alpha &&
           this->_beta == rhs._beta) return true;
        else return false;
    }

    bool _transpose_x{false};
    bool _transpose_y{false};
    
    int _m{0};
    int _n{0};
    int _k{0};
    int _lda{0};
    int _ldb{0};
    int _ldc{0};

    DataType _alpha{0.0};
    DataType _beta{0.0};
};


} // namespace abacus
} // namespace WW

#endif