//
//  PathFinderDemo.cpp
//  TankBattle
//
//  Created by Pham Thanh on 4/6/17.
//
//

#include "PathFinderDemo.h"

USING_NS_CC;


static PathFinderDemo* _instance = nullptr;

PathFinderDemo::PathFinderDemo() {
    
}

PathFinderDemo::~PathFinderDemo() {
    
}

PathFinderDemo* PathFinderDemo::getInstance() {
    if (!_instance) {
        _instance = new PathFinderDemo();
        
    }
    return _instance;
}
void PathFinderDemo::setUp(TankMap* tankMap) {
    for (int i = 0; i < MAX_HEIGHT; i++) {
        for (int j =0; j < MAX_WIDTH; j++) {
            _grid[j][i] = 1;
        }
    }
    for (int i = 0; i < MAX_WIDTH; i++) {
        for (int j =0; j < MAX_HEIGHT; j++) {
            if (tankMap->getCanDesLayer()->getTileAt(Vec2(i,j))) {
                _grid[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < MAX_WIDTH; i++) {
        for (int j =0; j < MAX_HEIGHT; j++) {
            if (tankMap->getCannotDesLayer()->getTileAt(Vec2(i,j))) {
                _grid[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < MAX_HEIGHT; i++) {
        for (int j =0; j < 2; j++) {
            _grid[j][i] = 0;
        }
    }
    for (int i = 0; i < MAX_HEIGHT; i++) {
        for (int j =0; j < 2; j++) {
            _grid[j][i] = 0;
        }
    }
    //        for (int i = 0; i < MAX_HEIGHT; i++) {
    //            for (int j =0; j <= 3; j++) {
    //                _grid[j][i] = 0;
    //            }
    //        }
    //
    //    for (int i = 0; i < MAX_HEIGHT; i++) {
    //      for (int j =MAX_WIDTH-1; j>= MAX_WIDTH - 3  ; j--) {
    //            _grid[j][i] = 0;
    //        }
    //    }
    //
    
    for (int i = 0; i < MAX_HEIGHT; i++) {
        for (int j =0; j < MAX_WIDTH; j++) {
            printf("%d",_grid[j][i]);
            //_grid[j][i] = 1;
        }
        printf("\n");
    }
    
    _tileWidth = tankMap->getTiledMap()->getTileSize().width;
    _tileHeight = tankMap->getTiledMap()->getTileSize().height;
    _mapWidth = tankMap->getTiledMap()->getMapSize().width;
    _mapHeight = tankMap->getTiledMap()->getMapSize().height;
//    CCLOG("map[%d:%d] tile[%d:%d]",_mapWidth,_mapHeight,_tileWidth,_tileHeight);
  
//            CCLOG("-----------AStar---------------------\n");
//         _path = findWalkablePathAstar(tile2pos(Point(11,2)),tile2pos(Point(19,6)));
//            for (auto p : _path) {
//                CCLOG("%f %f",p.x,p.y);
//            }
//         CCLOG("-----------AStar----%zd-----------------\n",_path.size());
    
//        CCLOG("------------Dijsktra---------------------\n");
//        path =  findWalkablePathDijsktra(tile2pos(Point(15,6)),tile2pos(Point(19,6)));
//        for (auto p : path) {
//            CCLOG("%f %f",p.x,p.y);
//        }
//            CCLOG("------------Dijsktra---%zd------------------\n",path.size());
    
//        CCLOG("--------------BEST F---------------\n");
//        path =  findWalkablePathBestFirstSearch(tile2pos(Point(11,2)),tile2pos(Point(19,6)));
//        for (auto p : path) {
//            CCLOG("%f %f",p.x,p.y);
//        }
//        CCLOG("--------------BEST F---%zd------------\n",path.size());
}

void PathFinderDemo::open(cocos2d::Point p) {
    Point tile = pos2tile(p);
    int x = tile.x;
    int y = tile.y;
    _grid[x][y] = 1;
}

void PathFinderDemo::close(cocos2d::Point p) {
    Point tile = pos2tile(p);
    int x = tile.x;
    int y = tile.y;
    _grid[x][y] = 0;
}

cocos2d::Point PathFinderDemo::tile2pos(cocos2d::Point tile) {
    Point pos = Vec2((tile.x * _tileWidth),((_mapHeight - tile.y - 1) * _tileHeight));
    return pos;
}

cocos2d::Point PathFinderDemo::tile2poscenter(cocos2d::Point tile) {
    Point pos = Vec2((tile.x * _tileWidth),((_mapHeight - tile.y - 1) * _tileHeight));
    return pos + Vec2(_tileWidth*0.5,_tileHeight*0.5);
}

cocos2d::Point PathFinderDemo::pos2tile(cocos2d::Point pos) {
    int x = (pos.x / _tileWidth);
    int y = ((((_mapHeight - 1) * _tileHeight) - pos.y) /_tileHeight);
    return Vec2(x,y+1);
}

struct compare
{
    bool operator()( MapNode* l, MapNode* r)
    {
        return l->getFScore() < r->getFScore();
    }
}Compare;
bool myfn(MapNode* l, MapNode* r) { return l->getFScore()<r->getFScore(); }





std::vector<cocos2d::Point> PathFinderDemo::findWalkablePathAstar(cocos2d::Point from, cocos2d::Point to) {
    Point curTile = pos2tile(from);
    Point tarTile = pos2tile(to);
    
    MapNode* start = new MapNode(curTile,nullptr);
    MapNode* end = new MapNode(tarTile,nullptr);
    
    int startX = start->getPosition().x;
    int startY = start->getPosition().y;
    
    int endX = end->getPosition().x;
    int endY = end->getPosition().y;
    if (endX >MAX_WIDTH - 1 || endX <= 1 || !_grid[endX][endY] || !_grid[startX][startY]) {
        delete start;
        delete end;
        
        std::vector<Point> result;
        return result;
    }
    _steps.clear();
    
    
    std::list<MapNode*> openList;
    openList.push_back(start);
    std::list<MapNode*> closeList;
    closeList.push_back(start);
    _steps.push_back(start);
    
    
    auto calcH= [&](MapNode* n) {
        float dx = abs(n->getPosition().x - end->getPosition().x);
        float dy = abs(n->getPosition().y - end->getPosition().y);
        return 1 * (dx + dy);
    };;
    
    while (!openList.empty()) {
        
        auto minVal = std::min_element(openList.begin(), openList.end(), myfn);
        
        MapNode* top = *minVal;
        if (top->getPosition().equals(end->getPosition())) {
            return reconstructPath(top, start, end);
        }
        openList.remove(*minVal);
        closeList.push_back(*minVal);
        _steps.push_back(*minVal);
        
        for (int i=0; i<4; i++) {
            int nearX = top->getPosition().x + gDir2[i][0];
            int nearY = top->getPosition().y + gDir2[i][1];
            if (nearX <0 || nearX >= MAX_WIDTH) {
                continue;
            }
            if (nearY <0 || nearY >= MAX_HEIGHT) {
                continue;
            }
            if (_grid[nearX][nearY]) {
                
                MapNode* successor = new MapNode(Point(nearX,nearY),top);
                successor->setGScore(top->getGScore() + top->getPosition().getDistance(successor->getPosition()));
                
                
                successor->setHScore(end->getPosition().getDistanceSq(successor->getPosition()));

                successor->setFScore(successor->getGScore() + successor->getHScore());
                
                auto exist = std::find_if (closeList.begin(), closeList.end(),[successor](MapNode* const& n){
                    return successor->getPosition().equals(n->getPosition());
                });
                if (exist != closeList.end()) {
                    continue;
                }
                
                auto exist2 = std::find_if (openList.begin(), openList.end(), [successor](MapNode* const& n){
                          return successor->getPosition().equals(n->getPosition());
                });
                 if (exist2 == openList.end()) {
                     openList.push_back(successor);
                 } else {
                     if (successor->getGScore() < (*exist2)->getGScore()) {
                         (*exist2)->setParrent((top));
                     }
                 }


            }
        }
        
    }
    
    std::vector<Point> result;
    return result;
}


std::vector<cocos2d::Point> PathFinderDemo::findWalkablePathBestFirstSearch(cocos2d::Point from, cocos2d::Point to) {
    Point curTile = pos2tile(from);
    Point tarTile = pos2tile(to);
    
    MapNode* start = new MapNode(curTile,nullptr);
    MapNode* end = new MapNode(tarTile,nullptr);
    
    int startX = start->getPosition().x;
    int startY = start->getPosition().y;
    
    int endX = end->getPosition().x;
    int endY = end->getPosition().y;
    if (endX >MAX_WIDTH - 1 || endX <= 1 || !_grid[endX][endY] || !_grid[startX][startY]) {
        delete start;
        delete end;
        
        std::vector<Point> result;
        return result;
    }
    
    _steps.clear();
    std::priority_queue<MapNode*,std::vector<MapNode*>, compare > pq;
    pq.push(start);
    std::list<MapNode*> closeList;
    _steps.push_back(start);
    while (!pq.empty()) {
        
        MapNode* top = pq.top();
        pq.pop();
        closeList.push_back(top);
        //CCLOG("add to closes %00f %00f",top->getPosition().x,top->getPosition().y);
        if (top->getPosition().equals(end->getPosition())) {
            return reconstructPath(top, start, end);
        }
        
        for (int i=0; i< 4;i++) {
            int nearX = top->getPosition().x + gDir2[i][0];
            int nearY = top->getPosition().y + gDir2[i][1];
            if (nearX <0 || nearX >= MAX_WIDTH) {
                continue;
            }
            if (nearY <0 || nearY >= MAX_HEIGHT) {
                continue;
            }
            if (_grid[nearX][nearY]) {
                
                MapNode* successor = new MapNode(Point(nearX,nearY),top);
                bool exist = false;
                for (auto a : closeList) {
                    if (a->getPosition().equals(successor->getPosition())) {
                        exist = true;
                        break;
                    }
                }
                if (exist) {
                    continue;
                }
                //successor.g = q.g + distance between successor and q
                successor->setGScore(0);
                //successor.h = distance from goal to successor
                successor->setHScore(successor->getPosition().getDistanceSq(end->getPosition()));
                //successor.f = successor.g + successor.h
                successor->setFScore(successor->getGScore() + successor->getHScore());
                // CCLOG("add to queue %00f %00f",successor->getPosition().x,successor->getPosition().y);
                _steps.push_back(successor);
                closeList.push_back(successor);
                pq.push(successor);
            }
        }
        
        
    }
    
    std::vector<Point> result;
    return result;

}

std::vector<cocos2d::Point> PathFinderDemo::findWalkablePathDijsktra(cocos2d::Point from, cocos2d::Point to) {
    Point curTile = pos2tile(from);
    Point tarTile = pos2tile(to);
    
    MapNode* start = new MapNode(curTile,nullptr);
    MapNode* end = new MapNode(tarTile,nullptr);
    
    int startX = start->getPosition().x;
    int startY = start->getPosition().y;
    
    int endX = end->getPosition().x;
    int endY = end->getPosition().y;
    if (endX >MAX_WIDTH - 1 || endX <= 1 || !_grid[endX][endY] || !_grid[startX][startY]) {
        delete start;
        delete end;
        
        std::vector<Point> result;
        return result;
    }
    _steps.clear();
    
    
    std::priority_queue<MapNode*,std::vector<MapNode*>, compare > pq;
    pq.push(start);
    std::list<MapNode*> closeList;
    closeList.push_back(start);
   _steps.push_back(start);
    while (!pq.empty()) {
        
        MapNode* top = pq.top();
        pq.pop();

        
        for (int i=0; i<4; i++) {
            int nearX = top->getPosition().x + gDir2[i][0];
            int nearY = top->getPosition().y + gDir2[i][1];
            if (nearX <0 || nearX >= MAX_WIDTH) {
                continue;
            }
            if (nearY <0 || nearY >= MAX_HEIGHT) {
                continue;
            }
            if (_grid[nearX][nearY]) {
                
                MapNode* successor = new MapNode(Point(nearX,nearY),top);
                successor->setGScore(top->getGScore() + top->getPosition().getDistanceSq(successor->getPosition()));
                successor->setHScore(0);
                successor->setFScore(successor->getGScore() + successor->getHScore());
                
                
                bool exist = false;
                for (auto a : closeList) {
                    if (a->getPosition().equals(successor->getPosition())) {
                        exist = true;
                        break;
                    }
                }
                if (exist) {
                    continue;
                }


                _steps.push_back(successor);
                closeList.push_back(successor);
                pq.push(successor);
                if (successor->getPosition().equals(end->getPosition())) {
                    return reconstructPath(successor, start, end);
                }
            }
        }
        
    }
    
    std::vector<Point> result;
    return result;

}

std::vector<cocos2d::Point> PathFinderDemo::reconstructPath(MapNode *p,MapNode* start, MapNode* end) {
    std::vector<Point> result;
    
    while (p->getParrent() && p != start) {
        result.push_back(Point(tile2poscenter(p->getPosition())) );
        p = p->getParrent();
        
    }
    result.push_back(Point(tile2poscenter(start->getPosition())));
    _path = result;
    return result;
}

bool PathFinderDemo::walkable(int tileX, int tileY) {
    
    if (tileX <=2 || tileX >= MAX_WIDTH) {
        return false;
    }
    if (tileY <0 || tileY >= MAX_HEIGHT) {
        return false;
    }
    return _grid[tileX][tileY];
}

bool PathFinderDemo::walkable(cocos2d::Point pos) {
    cocos2d::Point tile = pos2tile(pos);
    int tileX = tile.x;
    int tileY = tile.y;
    if (tileX <=2 || tileX >= MAX_WIDTH) {
        return false;
    }
    if (tileY <0 || tileY >= MAX_HEIGHT) {
        return false;
    }
    return _grid[tileX][tileY];
}
