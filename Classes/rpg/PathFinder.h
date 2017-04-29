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


#include "constants.h"

class RMapNode {
public:
    RMapNode(cocos2d::Point position,RMapNode * parrent) {
        _parrent = parrent;
        _position = position;
        _hScore = 0;
        _gScore = 0;
        _fScore = 0;
        _open = false;
        _close = false;
    }
    ~RMapNode() {
        if (_parrent) {
            delete _parrent;
        }
    }
    
    // For each node, the cost of getting from the start node to that node.
    inline int getGScore(RMapNode* p) {
        _gScore = p->getPosition().getDistance(getPosition());
        return _gScore;
    }
    
    inline int getHScore(RMapNode* p) {
        _hScore = p->getPosition().getDistanceSq(getPosition());
        return _hScore;
    }
    
    inline int getScore(RMapNode* p) {
        _fScore = getGScore(p) + getHScore(p);
        return _fScore;
    }
    
    CC_SYNTHESIZE(bool,_open,Open);
    CC_SYNTHESIZE(bool,_close,Close);
    CC_SYNTHESIZE(float,_gScore,GScore);
    CC_SYNTHESIZE(float,_hScore,HScore);
    CC_SYNTHESIZE(float,_fScore,FScore);
    CC_SYNTHESIZE(cocos2d::Point,_position,Position);
    CC_SYNTHESIZE(RMapNode*,_parrent,Parrent);
};


class RPathFinder {
public:
    RPathFinder();
    
    ~RPathFinder();
    
    static RPathFinder* getInstance();
    
    void setUp(cocos2d::TMXTiledMap* tankMap);
    
    void open(cocos2d::Point p);
    
    void close(cocos2d::Point p);
    
    std::vector<cocos2d::Point> findWalkablePathAstar(cocos2d::Point from, cocos2d::Point to);
    
    std::vector<cocos2d::Point> findWalkablePathDFS(cocos2d::Point from, cocos2d::Point to);
    
    std::vector<cocos2d::Point> findWalkablePathBFS(cocos2d::Point from, cocos2d::Point to);
    
    std::vector<cocos2d::Point> findWalkablePathBestFirstSearch(cocos2d::Point from, cocos2d::Point to);
    
    std::vector<cocos2d::Point> findWalkablePathDijsktra(cocos2d::Point from, cocos2d::Point to);
    
    std::vector<cocos2d::Point> reconstructPath(RMapNode *p,RMapNode* start, RMapNode* end);

    
    cocos2d::Point tile2pos(cocos2d::Point tile);
    
    cocos2d::Point tile2poscenter(cocos2d::Point tile);
    
    cocos2d::Point pos2tile(cocos2d::Point pos);
    
private:

    
    int **_grid;//[27][21];
    
    int _mapWidth;
    
    int _mapHeight;
    
    int _tileWidth;
    
    int _tileHeight;
    
};
#endif /* PathFinder_h */
