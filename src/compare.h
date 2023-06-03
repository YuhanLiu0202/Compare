#include "lr_model.h"
#include "lr_model_impl.h"
#include "plr.h"
#include "../util/util.h"

template<class KeyType, class ValueType>
class CompareIndex {
public:
    typedef LinearRegressionModel<KeyType, ValueType> lrModel;
    CompareIndex(const int maxError_, const int capacity_);
    ~CompareIndex();
    bool Train(const std::vector<KeyType>& key, const std::vector<ValueType>& value);
    bool Get(const KeyType& key, ValueType& value);

private:
    size_t makeSegmentation(const std::vector<KeyType>& key, const std::vector<ValueType>& value, size_t n);
    size_t locate(const KeyType& key, size_t pos);
    const int maxError;
    const int capacity;
    KeyType* keys;
    ValueType* values;
    vector<lrModel> models;
    vector<KeyType> modelKeys; 
};

template<class KeyType, class ValueType>
CompareIndex<KeyType, ValueType>::CompareIndex(const int maxError_, const int capacity_) : maxError(maxError_), capacity(capacity_)
{
    keys = new KeyType[capacity_];
    values = new ValueType[capacity_];
}

template<class KeyType, class ValueType>
CompareIndex<KeyType, ValueType>::~CompareIndex()
{
    delete[] keys;
    delete[] values;
}

template<class KeyType, class ValueType>
bool CompareIndex<KeyType, ValueType>::Train(const std::vector<KeyType>& key, const std::vector<ValueType>& value)
{
    size_t size = key.size();
    for (size_t i = 0; i < size; i++) {
        keys[i] = key[i];
        values[i] = value[i];
    }

    // learned index
    std::cout << "Training begin, length of training data is: " << key.size() << " , maxError: " << maxError << std::endl;
    // make segment and train linear regression model
    size_t segments = makeSegmentation(key, value, size);
    std::cout << "[Learned Index] get models -> " << segments << std::endl;
    return true;
}

template<class KeyType, class ValueType>
bool CompareIndex<KeyType, ValueType>::Get(const KeyType& key, ValueType& value)
{
    // binary search
    // size_t pos = binary_search_branchless(keys, capacity, key);
    // pos = pos >= capacity ? capacity - 1 : pos;
    // if (keys[pos] == key) {
    //     value = values[pos];
    //     return true;
    // }
    // return false;

    size_t pos = binary_search_branchless(&modelKeys[0], modelKeys.size(), key);
    pos = pos >= models.size() ? models.size() - 1 : pos;
    size_t loc = models[pos].predict(key);
    loc = locate(key, loc);
    if (keys[loc] == key) {
        value = values[loc];
        return true;
    }
    return false;
}

template<class KeyType, class ValueType>
size_t CompareIndex<KeyType, ValueType>::makeSegmentation(const std::vector<KeyType>& key, const std::vector<ValueType>& value, size_t n)
{
    if (n == 0)
        return 0;

    size_t segment = 0;
    size_t startPos = 0;
    KeyType startKey = key[0];

    OptimalPiecewiseLinearModel opt(maxError);
    opt.addPoint(key[0] - startKey, 0);

    for (size_t i = 1; i < n; i++) {
        bool addSuccess = opt.addPoint(key[i] - startKey, i - startPos);
        if (!addSuccess) {
            std::pair<double, double> meta = opt.GetMeta();
            models.emplace_back(startKey, startPos, meta.first, meta.second);
            modelKeys.push_back(key[i]);
            startKey = key[i];
            startPos = i;
            opt.addPoint(key[i] - startKey, i - startPos);
            segment++;
        }
    }

    std::pair<double, double> meta = opt.GetMeta();
    models.emplace_back(startKey, startPos, meta.first, meta.second);
    modelKeys.push_back(key[n - 1]);
    return ++segment;
}

template<class KeyType, class ValueType>
size_t CompareIndex<KeyType, ValueType>::locate(const KeyType& key, size_t pos)
{
    size_t upbound = capacity - 1;
    size_t begin, end, mid;
    if(key > keys[pos]){
        begin = pos + 1 < upbound ? (pos + 1) : upbound;
        end = begin + maxError < upbound ? (begin + maxError) : upbound;
    } else {
        end = pos;
        begin = end > maxError ? (end - maxError) : 0;
    }
    
    while(begin != end){
        mid = (end + begin + 2) / 2;
        if(keys[mid] <= key) {
            begin = mid;
        } else
            end = mid - 1;
    }
    return begin;
}