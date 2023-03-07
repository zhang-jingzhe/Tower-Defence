#include "mapconfig.h"
#include <QDebug>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

MapConfig::MapConfig()
{
    map_x = 0;
    map_y = 0;
    memset(map, 0, sizeof(map));
    paths.clear();
    generate.clear();
}

bool MapConfig::load_config_file(const char* file)
{
    memset(map, 0, sizeof(map));
    paths.clear();
    generate.clear();
    FILE* fp = fopen(file, "r");
    fscanf(fp, "%d %d", &map_x, &map_y);
    qDebug() << "MAP Loading :" << map_x << map_y << Qt::endl;

    for (int i = 0; i < map_y; i++) {
        char tmp[30] {};
        fscanf(fp, "%s", tmp);
        for (int j = 0; j < map_x; j++)
            map[j][i] = tmp[j];
    }

    int cnt;
    fscanf(fp, "%d", &cnt);
    fgetc(fp);
    vector<QPoint> tmp;
    while (cnt--) {
        tmp.clear();
        while (1) {
            char ch = fgetc(fp);
            while (ch != '(' && ch != '\n')
                ch = fgetc(fp);
            if (ch == '\n') {
                reverse(tmp.begin(), tmp.end());
                qDebug() << "push" << Qt::endl;
                paths.push_back(tmp);
                break;
            }
            int x = -1, y = -1;
            fscanf(fp, "%d,%d", &x, &y);
            qDebug() << x << "," << y << Qt::endl;
            tmp.push_back(QPoint(x, y));
        }
    }
    int x, y;
    char str[20] {};
    while (fscanf(fp, "%d", &x) == 1) {
        fscanf(fp, "%s %d", str, &y);
        generate.push_back(make_pair(x, make_pair(QString(str), y)));
    }
    sort(generate.begin(), generate.end());
    reverse(generate.begin(), generate.end());
    return true;
}
void MapConfig::output_config_file(const char* file)
{
    FILE* fp = fopen(file, "w");
    fprintf(fp, "%d %d\n", map_x, map_y);
    for (int i = 0; i < map_y; i++)
        fprintf(fp, "%s\n", map[i]);
    fprintf(fp, "%d\n", (int)paths.size());
    for (int i = 0; i < (int)paths.size(); i++) {
        for (int j = ((int)paths[i].size()) - 1; j >= 0; j--) {
            fprintf(fp, "(%d,%d)", paths[i][j].x(), paths[i][j].y());
            if (j)
                fprintf(fp, "->");
            else
                fprintf(fp, "\n");
        }
    }
    for (auto x : generate) {
        fprintf(fp, "%d %s %d\n", x.first, x.second.first.toStdString().c_str(), x.second.second);
    }
    fclose(fp);
}
