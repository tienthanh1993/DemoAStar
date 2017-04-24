//
//  PathFinderDemo.h
//  TankBattle
//
//  Created by Pham Thanh on 4/6/17.
//
//

#ifndef PathFinderDemo_h
#define PathFinderDemo_h

#include <cocos2d.h>
#include "TankMap.h"
#include "constants.h"


class PathFinderDemo : public cocos2d::Layer {
public:
    PathFinderDemo();
    
    ~PathFinderDemo();
    
    static PathFinderDemo* getInstance();
    
    void setUp(TankMap* tankMap);
    
    void open(cocos2d::Point p);
    
    void close(cocos2d::Point p);
    
    std::vector<cocos2d::Point> findWalkablePathAstar(cocos2d::Point from, cocos2d::Point to);
    
    std::vector<cocos2d::Point> findWalkablePathBestFirstSearch(cocos2d::Point from, cocos2d::Point to);
    
    std::vector<cocos2d::Point> findWalkablePathDijsktra(cocos2d::Point from, cocos2d::Point to);
    
    std::vector<cocos2d::Point> reconstructPath(MapNode *p,MapNode* start, MapNode* end);
    
    bool walkable(int tileX,int tileY);
    
    bool walkable(cocos2d::Point pos);
    
    cocos2d::Point tile2pos(cocos2d::Point tile);
    
    cocos2d::Point tile2poscenter(cocos2d::Point tile);
    
    cocos2d::Point pos2tile(cocos2d::Point pos);
    
private:
     ;
    CC_SYNTHESIZE(std::deque<MapNode*>, _steps, Steps);
    std::deque<MapNode*>& getSteps() {
        return _steps;
    }
    CC_SYNTHESIZE(std::vector<cocos2d::Vec2>, _path, Path);
    int _grid[MAX_WIDTH][MAX_HEIGHT];
    
    int _mapWidth;
    
    int _mapHeight;
    
    int _tileWidth;
    
    int _tileHeight;
};

#endif /* PathFinderDemo_h */
