#ifndef MAPCONFIG_H
#define MAPCONFIG_H

#include <QPoint>
#include <algorithm>
#include <vector>
using namespace std;

class MapConfig {
public:
    MapConfig();
    int map_x, map_y;
    char map[30][30];
    vector<vector<QPoint>> paths;
    vector<pair<int, pair<QString, int>>> generate;
    bool load_config_file(const char* file);
    void output_config_file(const char* file);
};

#endif // MAPCONFIG_H
