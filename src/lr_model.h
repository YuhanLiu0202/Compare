#ifndef __LR_MODEL_H__
#define __LR_MODEL_H__

template <class KeyType, class ValueType>
class LinearRegressionModel{
public:
    LinearRegressionModel(KeyType startKey_, size_t startPos_, double slope_, double intercept_);
    ~LinearRegressionModel();
    size_t predict(const KeyType &key);
private:
    double slope;
    double intercept;
    const KeyType startKey;
    const size_t startPos;
};

#endif