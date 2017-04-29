//
//  PathFinder.h
//  TankBattle
//
//  Created by Pham Thanh on 3/10/17.
// http://web.mit.edu/eranki/www/tutorials/search/
//

#ifndef PathFinder_h
#define PathFinder_h

#include <cocos2d.h>
#include "TankMap.h"

#include "constants.h"
class PathFinder {
public:
    PathFinder();
    
    ~PathFinder();
    
    static PathFinder* getInstance();
    
    void setUp(TankMap* tankMap);
    
    void open(cocos2d::Point p);
    
    void close(cocos2d::Point p);
    
    std::vector<cocos2d::Point> findWalkablePathAstar(cocos2d::Point from, cocos2d::Point to);
    
    std::vector<cocos2d::Point> findWalkablePathDFS(cocos2d::Point from, cocos2d::Point to);
    
    std::vector<cocos2d::Point> findWalkablePathBFS(cocos2d::Point from, cocos2d::Point to);
    
    std::vector<cocos2d::Point> findWalkablePathBestFirstSearch(cocos2d::Point from, cocos2d::Point to);
    
    std::vector<cocos2d::Point> findWalkablePathDijsktra(cocos2d::Point from, cocos2d::Point to);
    
    std::vector<cocos2d::Point> reconstructPath(MapNode *p,MapNode* start, MapNode* end);
    
    cocos2d::Point findNearbyPos(cocos2d::Point from, int distance);
    
    bool walkable(int tileX,int tileY);
    
    bool walkable(cocos2d::Point pos);
    
    cocos2d::Point tile2pos(cocos2d::Point tile);
    
    cocos2d::Point tile2poscenter(cocos2d::Point tile);
    
    cocos2d::Point pos2tile(cocos2d::Point pos);
    
private:

    
    int **_grid;//[MAX_WIDTH][MAX_HEIGHT];
    
    int _mapWidth;
    
    int _mapHeight;
    
    int _tileWidth;
    
    int _tileHeight;
    
};
#endif /* PathFinder_h */
