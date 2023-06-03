#include "util.h"

typedef uint64_t KeyType;
typedef uint64_t ValueType;

struct Configs {
    int dataset;
    int error;
};

Configs config;

char dataset[6][50] = {"../dataset/data/normal_200M_uint64", "../dataset/data/lognormal_200M_uint64", 
                        "../dataset/data/fb_200M_uint64", "../dataset/data/wiki_ts_200M_uint64",
                        "../dataset/data/osm_cellids_200M_uint64", "../dataset/data/books_200M_uint64"};

void loadData(std::vector<KeyType>& keys) {
    FILE *fp = fopen(dataset[config.dataset], "rb");
    size_t len = 0;
    int now = fread(&len, 8, 1, fp);
    for (size_t i = 0; i < len; i++) {
        KeyType key = 0;
        now = fread(&key, 8, 1, fp);
        if (!now) {
            break;
        }
        keys.push_back(key);
    }
    fclose(fp);
    std::cout << "Total Key Nums: " << len << std::endl;
}
