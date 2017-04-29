//
//  RPathFinder.cpp
//  TankBattle
//
//  Created by Pham Thanh on 3/10/17.
//
//

#include "PathFinder.h"
USING_NS_CC;


static RPathFinder* _instance = nullptr;

RPathFinder::RPathFinder() {
    
}

RPathFinder::~RPathFinder() {
    for( int i = 0 ; i < _mapWidth ; i++ )
    {
        delete [] _grid[i] ;
    }
    delete [] _grid;
}

RPathFinder* RPathFinder::getInstance() {
    if (!_instance) {
        _instance = new RPathFinder();
        
    }
    return _instance;
}
void RPathFinder::setUp(TMXTiledMap* tankMap) {
    _tileWidth = tankMap->getTileSize().width;
    _tileHeight = tankMap->getTileSize().height;
    _mapWidth =tankMap->getMapSize().width;
    _mapHeight = tankMap->getMapSize().height;
    
    _grid= new int*[_tileWidth];
    for(int i = 0; i < _tileWidth; ++i)
        _grid[i] = new int[_tileHeight];

    
    TMXLayer * colliable = tankMap->getLayer("collidable");
    for (int i = 0; i < tankMap->getMapSize().width; i++) {
        for (int j =0; j < tankMap->getMapSize().height; j++) {
            if(colliable->getTileGIDAt(Vec2(i,j)))
                _grid[i][j] = 0;
            else
                _grid[i][j] = 1;
        }
    }

 
    for (int i = 0; i < tankMap->getMapSize().height; i++) {
        for (int j =0; j < tankMap->getMapSize().width; j++) {
            printf("%d",_grid[j][i]);
        }
        printf("\n");
    }

    CCLOG("map[%d:%d] tile[%d:%d]",_mapWidth,_mapHeight,_tileWidth,_tileHeight);
    std::vector<Point> path;
        CCLOG("-----------AStar---------------------\n");
     path = findWalkablePathAstar(tile2pos(Point(1,10)),tile2pos(Point(20,13)));
        for (auto p : path) {
            CCLOG("%f %f",p.x,p.y);
        }
     CCLOG("-----------AStar----%zd-----------------\n",path.size());

    CCLOG("------------DFS---------------------\n");
    path =  findWalkablePathDFS(tile2pos(Point(1,10)),tile2pos(Point(20,13)));
    for (auto p : path) {
        CCLOG("%f %f",p.x,p.y);
    }
        CCLOG("------------DFS---%zd------------------\n",path.size());
    
    CCLOG("--------------BFS---------------\n");
    path =  findWalkablePathBFS(tile2pos(Point(1,10)),tile2pos(Point(20,13)));
    for (auto p : path) {
        CCLOG("%f %f",p.x,p.y);
    }
    CCLOG("--------------BFS---%zd------------\n",path.size());
}

void RPathFinder::open(cocos2d::Point p) {
    Point tile = pos2tile(p);
    int x = tile.x;
    int y = tile.y;
    _grid[x][y] = 1;
}

void RPathFinder::close(cocos2d::Point p) {
    Point tile = pos2tile(p);
    int x = tile.x;
    int y = tile.y;
    _grid[x][y] = 0;
}

cocos2d::Point RPathFinder::tile2pos(cocos2d::Point tile) {
    Point pos = Vec2((tile.x * _tileWidth),((_mapHeight - tile.y - 1) * _tileHeight));
    return pos;
}

cocos2d::Point RPathFinder::tile2poscenter(cocos2d::Point tile) {
    Point pos = Vec2((tile.x * _tileWidth),((_mapHeight - tile.y - 1) * _tileHeight));
    return pos + Vec2(_tileWidth*0.5,_tileHeight*0.5);
}

cocos2d::Point RPathFinder::pos2tile(cocos2d::Point pos) {
    int x = (pos.x / _tileWidth);
    int y = ((((_mapHeight - 1) * _tileHeight) - pos.y) /_tileHeight);
    return Vec2(x,y+1);
}

struct compare
{
    bool operator()( RMapNode* l, RMapNode* r)
    {
        return l->getFScore() > r->getFScore();
    }
};

bool isLowerRMapNode(RMapNode* l, RMapNode* r) { return l->getFScore()<r->getFScore(); }

std::vector<cocos2d::Point> RPathFinder::findWalkablePathAstar(cocos2d::Point from, cocos2d::Point to) {
    Point curTile = pos2tile(from);
    Point tarTile = pos2tile(to);
    
    RMapNode* start = new RMapNode(curTile,nullptr);
    RMapNode* end = new RMapNode(tarTile,nullptr);
    
    int startX = start->getPosition().x;
    int startY = start->getPosition().y;
    
    int endX = end->getPosition().x;
    int endY = end->getPosition().y;
    if (endX >_mapWidth - 1 || endX <= 1 || !_grid[endX][endY] || !_grid[startX][startY]) {
        delete start;
        delete end;
        CCLOG("enter here %d %d %d %d",endX >_mapWidth - 1 , endX <= _mapWidth , !_grid[endX][endY], !_grid[startX][startY]);
        std::vector<Point> result;
        return result;
    }

    
    
    std::list<RMapNode*> openList;
    openList.push_back(start);
    std::list<RMapNode*> closeList;
    closeList.push_back(start);


    
    while (!openList.empty()) {
        auto minVal = std::min_element(openList.begin(), openList.end(), isLowerRMapNode);
        
        RMapNode* top = *minVal;
        if (top->getPosition().equals(end->getPosition())) {
            return reconstructPath(top, start, end);
        }
        openList.remove(*minVal);
        closeList.push_back(*minVal);

        for (int i=0; i<8; i++) {
            int nearX = top->getPosition().x + gDir2[i][0];
            int nearY = top->getPosition().y + gDir2[i][1];
            if (nearX <0 || nearX >= _mapWidth) {
                continue;
            }
            if (nearY <0 || nearY >= _mapHeight) {
                continue;
            }
            if (_grid[nearX][nearY]) {
                
                RMapNode* successor = new RMapNode(Point(nearX,nearY),top);
                successor->setGScore(top->getGScore() + top->getPosition().getDistance(successor->getPosition()));
                
                
                successor->setHScore(end->getPosition().getDistance(successor->getPosition()));
                
                successor->setFScore(successor->getGScore() + successor->getHScore());
                
                auto exist = std::find_if (closeList.begin(), closeList.end(),[successor](RMapNode* const& n){
                    return successor->getPosition().equals(n->getPosition());
                });
                if (exist != closeList.end()) {
                  
                    continue;
                }
                
                auto exist2 = std::find_if (openList.begin(), openList.end(), [successor](RMapNode* const& n){
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


std::vector<cocos2d::Point> RPathFinder::reconstructPath(RMapNode *p,RMapNode* start, RMapNode* end) {
    std::vector<Point> result;
   
    while (p->getParrent() && p != start) {
            result.push_back(Point(tile2poscenter(p->getPosition())) );
            p = p->getParrent();
    }
     result.push_back(Point(tile2poscenter(start->getPosition())));
    
    return result;
}

std::vector<cocos2d::Point> RPathFinder::findWalkablePathDFS(cocos2d::Point from, cocos2d::Point to) {
    
    Point curTile = pos2tile(from);
    Point tarTile = pos2tile(to);
    RMapNode* start = new RMapNode(curTile,nullptr);
    RMapNode* goalNode = new RMapNode(tarTile,nullptr);
    
    
    int startX = start->getPosition().x;
    int startY = start->getPosition().y;
    
    int endX = goalNode->getPosition().x;
    int endY = goalNode->getPosition().y;
    if (!_grid[endX][endY] || !_grid[startX][startY]) {
        std::vector<Point> result;
        return result;
    }

    std::list<RMapNode*> paths;
    std::vector<RMapNode*> markedList;
    start->setClose(true);
    paths.push_back(start);
    while (!paths.empty()) {
        RMapNode* current = paths.back();
        paths.pop_back();
        if(current->getPosition().equals(goalNode->getPosition())) {
            return reconstructPath(current, start, goalNode);
        }
        markedList.push_back(current);
        for (int i=0; i< 8;i++) {
            int nearX = current->getPosition().x + gDir2[i][0];
            int nearY = current->getPosition().y + gDir2[i][1];
            if (nearX <0 || nearX >= _mapWidth) {
                continue;
            }
            if (nearY <0 || nearY >= _mapHeight) {
                continue;
            }
            if (_grid[nearX][nearY]) {
                RMapNode* successor = new RMapNode(Point(nearX,nearY),current);
                
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



std::vector<cocos2d::Point> RPathFinder::findWalkablePathBFS(cocos2d::Point from, cocos2d::Point to) {
    
    Point curTile = pos2tile(from);
    Point tarTile = pos2tile(to);
    RMapNode* start = new RMapNode(curTile,nullptr);
    RMapNode* goalNode = new RMapNode(tarTile,nullptr);
    
    int startX = start->getPosition().x;
    int startY = start->getPosition().y;
    
    int endX = goalNode->getPosition().x;
    int endY = goalNode->getPosition().y;
    if (!_grid[endX][endY] || !_grid[startX][startY]) {
        std::vector<Point> result;
        return result;
    }
    
    std::deque<RMapNode*> paths;
    std::vector<RMapNode*> markedList;
    start->setClose(true);
    paths.push_back(start);
    while (!paths.empty()) {
        RMapNode* current = paths.front();
        paths.pop_front();
        if(current->getPosition().equals(goalNode->getPosition())) {
            return reconstructPath(current, start, goalNode);
        }
        
        
        for (int i=0; i< 8;i++) {
            int nearX = current->getPosition().x + gDir2[i][0];
            int nearY = current->getPosition().y + gDir2[i][1];
            if (nearX <0 || nearX >= _mapWidth) {
                continue;
            }
            if (nearY <0 || nearY >= _mapHeight) {
                continue;
            }
            if (_grid[nearX][nearY]) {
                RMapNode* successor = new RMapNode(Point(nearX,nearY),current);
                
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


std::vector<cocos2d::Point> RPathFinder::findWalkablePathBestFirstSearch(cocos2d::Point from, cocos2d::Point to) {
    Point curTile = pos2tile(from);
    Point tarTile = pos2tile(to);
    
    RMapNode* start = new RMapNode(curTile,nullptr);
    RMapNode* end = new RMapNode(tarTile,nullptr);
    
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
    

    std::priority_queue<RMapNode*,std::vector<RMapNode*>, compare > pq;
    pq.push(start);
    std::list<RMapNode*> closeList;
 
    while (!pq.empty()) {
        
        RMapNode* top = pq.top();
        pq.pop();
        closeList.push_back(top);
        //CCLOG("add to closes %00f %00f",top->getPosition().x,top->getPosition().y);
        if (top->getPosition().equals(end->getPosition())) {
            return reconstructPath(top, start, end);
        }
        
        for (int i=0; i< 8;i++) {
            int nearX = top->getPosition().x + gDir2[i][0];
            int nearY = top->getPosition().y + gDir2[i][1];
            if (nearX <0 || nearX >= _mapWidth) {
                continue;
            }
            if (nearY <0 || nearY >= _mapHeight) {
                continue;
            }
            if (_grid[nearX][nearY]) {
                
                RMapNode* successor = new RMapNode(Point(nearX,nearY),top);
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

std::vector<cocos2d::Point> RPathFinder::findWalkablePathDijsktra(cocos2d::Point from, cocos2d::Point to) {
    Point curTile = pos2tile(from);
    Point tarTile = pos2tile(to);
    
    RMapNode* start = new RMapNode(curTile,nullptr);
    RMapNode* end = new RMapNode(tarTile,nullptr);
    
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
    
    std::priority_queue<RMapNode*,std::vector<RMapNode*>, compare > pq;
    pq.push(start);
    std::list<RMapNode*> closeList;
    closeList.push_back(start);

    while (!pq.empty()) {
        
        RMapNode* top = pq.top();
        pq.pop();
        
        
        for (int i=0; i<8; i++) {
            int nearX = top->getPosition().x + gDir2[i][0];
            int nearY = top->getPosition().y + gDir2[i][1];
            if (nearX <0 || nearX >= _mapWidth) {
                continue;
            }
            if (nearY <0 || nearY >= _mapHeight) {
                continue;
            }
            if (_grid[nearX][nearY]) {
                
                RMapNode* successor = new RMapNode(Point(nearX,nearY),top);
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

