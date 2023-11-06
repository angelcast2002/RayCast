#include <vector>

class Map {
public:
    int key;
    const char* map;
    std::vector<std::vector<int>> winPoints;
    bool isWin = false;
    std::vector<int> end;
    const char* music;
    const char* switchSound;
    const char* finalDoorSound;
};