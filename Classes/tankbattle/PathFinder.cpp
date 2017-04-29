//
//  PathFinder.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/10/17.
//
//

#include "PathFinder.h"
USING_NS_CC;


static PathFinder* _instance = nullptr;

PathFinder::PathFinder() {
    
}

PathFinder::~PathFinder() {
    for( int i = 0 ; i < _mapWidth ; i++ )
    {
        delete [] _grid[i] ;
    }
    delete [] _grid;
}

PathFinder* PathFinder::getInstance() {
    if (!_instance) {
        _instance = new PathFinder();
        
    }
    return _instance;
}
void PathFinder::setUp(TankMap* tankMap) {
    
    _tileWidth = tankMap->getTiledMap()->getTileSize().width;
    _tileHeight = tankMap->getTiledMap()->getTileSize().height;
    _mapWidth =tankMap->getTiledMap()->getMapSize().width;
    _mapHeight = tankMap->getTiledMap()->getMapSize().height;
    
    _grid= new int*[_tileWidth];
    for(int i = 0; i < _tileWidth; ++i)
        _grid[i] = new int[_tileHeight];
    
    
    
    for (int i = 0; i < _mapHeight; i++) {
        for (int j =0; j < _mapWidth; j++) {
            
            if (tankMap->getCanDesLayer()->getTileAt(Vec2(j,i)) || tankMap->getCannotDesLayer()->getTileAt(Vec2(j,i))) {
                _grid[i][j] = 0;
            } else {
                _grid[j][i] = 1;
            }
        }
    }


    for (int i = 0; i < _mapHeight; i++) {
        for (int j =0; j < 2; j++) {
            _grid[j][i] = 0;
        }
    }
  
    for (int i = 0; i < _mapHeight; i++) {
      for (int j =_mapWidth-1; j>= _mapWidth - 2 ; j--) {
            _grid[j][i] = 0;
        }
    }
 
    for (int i = 0; i < _mapHeight; i++) {
        for (int j =0; j < _mapWidth; j++) {
            printf("%d",_grid[j][i]);
        }
        printf("\n");
    }
    

    CCLOG("map[%d:%d] tile[%d:%d]",_mapWidth,_mapHeight,_tileWidth,_tileHeight);
//    std::vector<Point> path;
//        CCLOG("-----------AStar---------------------\n");
//     path = findWalkablePathAstar(tile2pos(Point(3,3)),tile2pos(Point(15,6)));
//        for (auto p : path) {
//            CCLOG("%f %f",p.x,p.y);
//        }
//     CCLOG("-----------AStar----%zd-----------------\n",path.size());
//
//    CCLOG("------------DFS---------------------\n");
//    path =  findWalkablePathDFS(tile2pos(Point(1,1)),tile2pos(Point(15,6)));
//    for (auto p : path) {
//        CCLOG("%f %f",p.x,p.y);
//    }
//        CCLOG("------------DFS---%zd------------------\n",path.size());
//    
//    CCLOG("--------------BFS---------------\n");
//    path =  findWalkablePathBFS(tile2pos(Point(1,1)),tile2pos(Point(15,6)));
//    for (auto p : path) {
//        CCLOG("%f %f",p.x,p.y);
//    }
//    CCLOG("--------------BFS---%zd------------\n",path.size());
}

void PathFinder::open(cocos2d::Point p) {
    Point tile = pos2tile(p);
    int x = tile.x;
    int y = tile.y;
    _grid[x][y] = 1;
}

void PathFinder::close(cocos2d::Point p) {
    Point tile = pos2tile(p);
    int x = tile.x;
    int y = tile.y;
    _grid[x][y] = 0;
}

cocos2d::Point PathFinder::tile2pos(cocos2d::Point tile) {
    Point pos = Vec2((tile.x * _tileWidth),((_mapHeight - tile.y - 1) * _tileHeight));
    return pos;
}

cocos2d::Point PathFinder::tile2poscenter(cocos2d::Point tile) {
    Point pos = Vec2((tile.x * _tileWidth),((_mapHeight - tile.y - 1) * _tileHeight));
    return pos + Vec2(_tileWidth*0.5,_tileHeight*0.5);
}

cocos2d::Point PathFinder::pos2tile(cocos2d::Point pos) {
    int x = (pos.x / _tileWidth);
    int y = ((((_mapHeight - 1) * _tileHeight) - pos.y) /_tileHeight);
    return Vec2(x,y+1);
}

struct compare
{
    bool operator()( MapNode* l, MapNode* r)
    {
        return l->getFScore() > r->getFScore();
    }
};

bool isLowerMapNode(MapNode* l, MapNode* r) { return l->getFScore()<r->getFScore(); }

std::vector<cocos2d::Point> PathFinder::findWalkablePathAstar(cocos2d::Point from, cocos2d::Point to) {
    Point curTile = pos2tile(from);
    Point tarTile = pos2tile(to);
    
    MapNode* start = new MapNode(curTile,nullptr);
    MapNode* end = new MapNode(tarTile,nullptr);
    
    int startX = start->getPosition().x;
    int startY = start->getPosition().y;
    
    int endX = end->getPosition().x;
    int endY = end->getPosition().y;
    if (endX >_mapWidth - 1 || endX <= 1 || !_grid[endX][endY] || !_grid[startX][startY]) {
        delete start;
        delete end;
        
        std::vector<Point> result;
        return result;
    }

    
    
    std::list<MapNode*> openList;
    openList.push_back(start);
    std::list<MapNode*> closeList;
    closeList.push_back(start);


    
    while (!openList.empty()) {
        auto minVal = std::min_element(openList.begin(), openList.end(), isLowerMapNode);
        
        MapNode* top = *minVal;
        if (top->getPosition().equals(end->getPosition())) {
            return reconstructPath(top, start, end);
        }
        openList.remove(*minVal);
        closeList.push_back(*minVal);

        for (int i=0; i<4; i++) {
            int nearX = top->getPosition().x + gDir2[i][0];
            int nearY = top->getPosition().y + gDir2[i][1];
            if (nearX <0 || nearX >= _mapWidth) {
                continue;
            }
            if (nearY <0 || nearY >= _mapHeight) {
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
                        //delete successor;
                        (*exist2)->setParrent((top));
                    }
                }
                
                
            }
        }
        
    }
    
    std::vector<Point> result;
    return result;
}


std::vector<cocos2d::Point> PathFinder::reconstructPath(MapNode *p,MapNode* start, MapNode* end) {
    std::vector<Point> result;
   
    while (p->getParrent() && p != start) {
            result.push_back(Point(tile2poscenter(p->getPosition())) );
            p = p->getParrent();
    }
     result.push_back(Point(tile2poscenter(start->getPosition())));
    
    return result;
}

std::vector<cocos2d::Point> PathFinder::findWalkablePathDFS(cocos2d::Point from, cocos2d::Point to) {
    
    Point curTile = pos2tile(from);
    Point tarTile = pos2tile(to);
    MapNode* start = new MapNode(curTile,nullptr);
    MapNode* goalNode = new MapNode(tarTile,nullptr);
    
    
    int startX = start->getPosition().x;
    int startY = start->getPosition().y;
    
    int endX = goalNode->getPosition().x;
    int endY = goalNode->getPosition().y;
    if (!_grid[endX][endY] || !_grid[startX][startY]) {
        std::vector<Point> result;
        return result;
    }

    std::list<MapNode*> paths;
    std::vector<MapNode*> markedList;
    start->setClose(true);
    paths.push_back(start);
    while (!paths.empty()) {
        MapNode* current = paths.back();
        paths.pop_back();
        if(current->getPosition().equals(goalNode->getPosition())) {
            return reconstructPath(current, start, goalNode);
        }
        markedList.push_back(current);
        for (int i=0; i< 4;i++) {
            int nearX = current->getPosition().x + gDir2[i][0];
            int nearY = current->getPosition().y + gDir2[i][1];
            if (nearX <0 || nearX >= _mapWidth) {
                continue;
            }
            if (nearY <0 || nearY >= _mapHeight) {
                continue;
            }
            if (_grid[nearX][nearY]) {
                MapNode* successor = new MapNode(Point(nearX,nearY),current);
                
                bool marked = false;
                for(auto mark : markedList) {
                    if(mark->getPosition().equals(successor->getPosition())) {
                        marked = true;
                        break;
                    }
                }
                if(marked) {
                    continue;
                }
                paths.push_back(successor);
                
            }
            
        }
    }
    
    std::vector<Point> result;
    return result;
}



std::vector<cocos2d::Point> PathFinder::findWalkablePathBFS(cocos2d::Point from, cocos2d::Point to) {
    
    Point curTile = pos2tile(from);
    Point tarTile = pos2tile(to);
    MapNode* start = new MapNode(curTile,nullptr);
    MapNode* goalNode = new MapNode(tarTile,nullptr);
    
    int startX = start->getPosition().x;
    int startY = start->getPosition().y;
    
    int endX = goalNode->getPosition().x;
    int endY = goalNode->getPosition().y;
    if (!_grid[endX][endY] || !_grid[startX][startY]) {
        std::vector<Point> result;
        return result;
    }
    
    std::deque<MapNode*> paths;
    std::vector<MapNode*> markedList;
    start->setClose(true);
    paths.push_back(start);
    while (!paths.empty()) {
        MapNode* current = paths.front();
        paths.pop_front();
        if(current->getPosition().equals(goalNode->getPosition())) {
            return reconstructPath(current, start, goalNode);
        }
        
        
        for (int i=0; i< 4;i++) {
            int nearX = current->getPosition().x + gDir2[i][0];
            int nearY = current->getPosition().y + gDir2[i][1];
            if (nearX <0 || nearX >= _mapWidth) {
                continue;
            }
            if (nearY <0 || nearY >= _mapHeight) {
                continue;
            }
            if (_grid[nearX][nearY]) {
                MapNode* successor = new MapNode(Point(nearX,nearY),current);
                
                bool marked = false;
                for(auto mark : markedList) {
                    if(mark->getPosition().equals(successor->getPosition())) {
                        marked = true;
                        break;
                    }
                }
                if(marked) {
                    continue;
                }
                markedList.push_back(successor);
                paths.push_back(successor);
                
            }
            
        }
    }
    
    std::vector<Point> result;
    return result;
}


std::vector<cocos2d::Point> PathFinder::findWalkablePathBestFirstSearch(cocos2d::Point from, cocos2d::Point to) {
    Point curTile = pos2tile(from);
    Point tarTile = pos2tile(to);
    
    MapNode* start = new MapNode(curTile,nullptr);
    MapNode* end = new MapNode(tarTile,nullptr);
    
    int startX = start->getPosition().x;
    int startY = start->getPosition().y;
    
    int endX = end->getPosition().x;
    int endY = end->getPosition().y;
    if (endX >_mapWidth - 1 || endX <= 1 || !_grid[endX][endY] || !_grid[startX][startY]) {
        delete start;
        delete end;
        
        std::vector<Point> result;
        return result;
    }
    

    std::priority_queue<MapNode*,std::vector<MapNode*>, compare > pq;
    pq.push(start);
    std::list<MapNode*> closeList;
 
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
            if (nearX <0 || nearX >= _mapWidth) {
                continue;
            }
            if (nearY <0 || nearY >= _mapHeight) {
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

                closeList.push_back(successor);
                pq.push(successor);
            }
        }
        
        
    }
    
    std::vector<Point> result;
    return result;
    
}

std::vector<cocos2d::Point> PathFinder::findWalkablePathDijsktra(cocos2d::Point from, cocos2d::Point to) {
    Point curTile = pos2tile(from);
    Point tarTile = pos2tile(to);
    
    MapNode* start = new MapNode(curTile,nullptr);
    MapNode* end = new MapNode(tarTile,nullptr);
    
    int startX = start->getPosition().x;
    int startY = start->getPosition().y;
    
    int endX = end->getPosition().x;
    int endY = end->getPosition().y;
    if (endX >_mapWidth - 1 || endX <= 1 || !_grid[endX][endY] || !_grid[startX][startY]) {
        delete start;
        delete end;
        
        std::vector<Point> result;
        return result;
    }
    
    std::priority_queue<MapNode*,std::vector<MapNode*>, compare > pq;
    pq.push(start);
    std::list<MapNode*> closeList;
    closeList.push_back(start);

    while (!pq.empty()) {
        
        MapNode* top = pq.top();
        pq.pop();
        
        
        for (int i=0; i<4; i++) {
            int nearX = top->getPosition().x + gDir2[i][0];
            int nearY = top->getPosition().y + gDir2[i][1];
            if (nearX <0 || nearX >= _mapWidth) {
                continue;
            }
            if (nearY <0 || nearY >= _mapHeight) {
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


cocos2d::Point PathFinder::findNearbyPos(cocos2d::Point from, int direction) {
    
    Point curTile = pos2tile(from);
    int tileX ,tileY;
    int dirIndex = 0;
    if (direction == 0) {
        dirIndex = 0;
    } else if (direction == 90) {
        dirIndex = 3;
    } else if (direction == 180) {
        dirIndex = 2;
    } else if (direction == -90) {
        dirIndex = 1;
    }
    
    tileX = curTile.x + gDir2[dirIndex][0];
    tileY = curTile.y + gDir2[dirIndex][1];
    if (walkable(tileX, tileY)) {
        return tile2poscenter(Point(tileX,tileY));
    }
    int curDir = dirIndex;
    while (1) {
        curDir++;
        if (curDir > 3) {
            curDir = 0;
        }
        tileX = curTile.x + gDir2[curDir][0];
        tileY = curTile.y + gDir2[curDir][1];
        if (walkable(tileX, tileY)) {
            return tile2poscenter(Point(tileX,tileY));
        }
        CCLOG("curIndex=%d nexCheckDir=%d",dirIndex,curDir);
        if (curDir == dirIndex) {
            return Point::ZERO;
        }
    }
    
    
    if (direction == 0) {
        tileX = curTile.x + gDir2[0][0];
        tileY = curTile.y + gDir2[0][1];
        if (walkable(tileX, tileY)) {
            return tile2poscenter(Point(tileX,tileY));
        }
        for (int i=0; i<=3; i++) {
            if (direction == i) {
                continue;
            }
            tileX = curTile.x + gDir2[i][0];
            tileY = curTile.y + gDir2[i][1];
            if (walkable(tileX, tileY)) {
                return tile2poscenter(Point(tileX,tileY));
            }
        }
    } else if (direction == 90) {
        tileX = curTile.x + gDir2[3][0];
        tileY = curTile.y + gDir2[3][1];
        if (walkable(tileX, tileY)) {
            return tile2poscenter(Point(tileX,tileY));
        }
        for (int i=0; i<=3; i++) {
            if (direction == i) {
                continue;
            }
            tileX = curTile.x + gDir2[i][0];
            tileY = curTile.y + gDir2[i][1];
            if (walkable(tileX, tileY)) {
                return tile2poscenter(Point(tileX,tileY));
            }
        }
    } else if (direction == 180) {
        tileX = curTile.x + gDir2[2][0];
        tileY = curTile.y + gDir2[2][1];
        if (walkable(tileX, tileY)) {
            return tile2poscenter(Point(tileX,tileY));
        }
        for (int i=3; i<=3; i++) {
            if (direction == i) {
                continue;
            }
            tileX = curTile.x + gDir2[i][0];
            tileY = curTile.y + gDir2[i][1];
            if (walkable(tileX, tileY)) {
                return tile2poscenter(Point(tileX,tileY));
            }
        }
    } else if (direction == -90) {
        tileX = curTile.x + gDir2[1][0];
        tileY = curTile.y + gDir2[1][1];
        if (walkable(tileX, tileY)) {
            return tile2poscenter(Point(tileX,tileY));
        }
        for (int i=0; i<=3; i++) {
            if (direction == i) {
                continue;
            }
            tileX = curTile.x + gDir2[i][0];
            tileY = curTile.y + gDir2[i][1];
            if (walkable(tileX, tileY)) {
                return tile2poscenter(Point(tileX,tileY));
            }
        }
    }

    return Point::ZERO;
}

bool PathFinder::walkable(int tileX, int tileY) {
    
    if (tileX <=2 || tileX >= _mapWidth) {
        return false;
    }
    if (tileY <0 || tileY >= _mapHeight) {
        return false;
    }
    return _grid[tileX][tileY];
}

bool PathFinder::walkable(cocos2d::Point pos) {
    cocos2d::Point tile = pos2tile(pos);
    int tileX = tile.x;
    int tileY = tile.y;
    if (tileX <=2 || tileX >= _mapWidth) {
        return false;
    }
    if (tileY <0 || tileY >= _mapHeight) {
        return false;
    }
    return _grid[tileX][tileY];
}
