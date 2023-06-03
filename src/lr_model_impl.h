#ifndef __LR_MODEL_IMPL_H__
#define __LR_MODEL_IMPL_H__

#include "lr_model.h"

#include <iostream>
using namespace std;

template <class KeyType, class ValueType>
LinearRegressionModel<KeyType, ValueType>::LinearRegressionModel(KeyType startKey_, size_t startPos_, double slope_, double intercept_) :
   startKey(startKey_), startPos(startPos_), slope(slope_), intercept(intercept_)
{
    
}

template <class KeyType, class ValueType>
LinearRegressionModel<KeyType, ValueType>::~LinearRegressionModel() 
{
    
}

template <class KeyType, class ValueType>
size_t LinearRegressionModel<KeyType, ValueType>::predict(const KeyType &key) 
{
    double predictKey = key - startKey;
    double res = slope * predictKey + intercept;
    return res >= 0 ? res + startPos: 0 + startPos;
}

#endif