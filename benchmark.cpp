#include <iostream>
#include <cstdio>
#include <vector>
#include <sys/time.h>
#include "util/function.h"
#include "src/compare.h"

using namespace std;

typedef CompareIndex<KeyType, ValueType> Index;

void parseArgs(int argc, char* argv[])
{
    int n;
    char junk;
    for (int i = 1; i < argc; i++) {
        if (sscanf(argv[i], "--dataset=%d%c", &n, &junk) == 1) {
            config.dataset = n;
        } else if (sscanf(argv[i], "--error=%d%c", &n, &junk) == 1) {
            config.error = n;
        }
    }
    
    cout << "---------------------------------" << endl;
    cout << "Dataset : " << config.dataset << endl;
    cout << "MaxError : " << config.error << endl;
    cout << "---------------------------------" << endl;
}

void Get()
{
    std::vector<KeyType> non_exist_keys;
    loadData(non_exist_keys);
    
    Index* index = new Index(config.error, non_exist_keys.size());
    index->Train(non_exist_keys, non_exist_keys);
    
    int ans = 0;
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    for (size_t i = 0; i < non_exist_keys.size(); i++) {
        ValueType val = 0;
        index->Get(non_exist_keys[i], val);
        if (non_exist_keys[i] != val) {
            ans += 1;
        }
    };
    gettimeofday(&end_time, NULL);    
    long long elapsed_seconds = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);
    
    delete index;
    cout << "---------------result---------------" << endl;
    cout << "TimeCost : " << elapsed_seconds << " ms <=> " << elapsed_seconds / 1e6 << " s" << endl;
    printf("Throughput : %.2f M op/s\n", 1.0 * non_exist_keys.size() / elapsed_seconds);
    cout << "Miss : " << ans << endl; 
    cout << "------------------------------------" << endl;
}


int main(int argc, char* argv[])
{
    parseArgs(argc, argv);
    Get();
}