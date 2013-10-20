//
//  XmlWorldReader.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "XmlWorldReader.h"
#include "World4.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "PMTextureCache.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

UniversalWorld *XmlWorldReader::readWorld(GameLayer *layer,std::string mapPath)
{
    gameLayer = layer;
    
    LIBXML_TEST_VERSION

	unsigned long size;

	char *buf = (char *)CCFileUtils::sharedFileUtils()->getFileData(mapPath.c_str(),"r", &size);

	xmlDocPtr doc = xmlReadMemory(buf, size,mapPath.c_str() ,NULL,0);
    
    if(doc == NULL)
    {
        xmlErrorPtr err= xmlGetLastError();
        error = err->message;
        
        return NULL;
    }
    
    xmlNodePtr mapNode = xmlDocGetRootElement(doc);
    
    worldType = (WorldType) atoi((char *) xmlGetProp(mapNode, BAD_CAST "worldtype"));
    
    curNode = mapNode;
    
    createWorldByType();
    
    for (xmlNodePtr cur_node = mapNode->children; cur_node != NULL; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            
            if(!strcmp((char *)cur_node->name, "robots"))
            {
                int robotCount = 0;
            
                for (xmlNodePtr robot_node = cur_node->children; robot_node != NULL; robot_node = robot_node->next)
                {
                    if (robot_node->type == XML_ELEMENT_NODE)
                    {
                        if(strcmp((char *)robot_node->name, "robot"))
                        {
                            error = "Not registered node :" + std::string((char *)robot_node->name);
                            delete world;
                            return NULL;
                        }
                        
                        RobotType robotType = (RobotType) atoi((char *)xmlGetProp(robot_node,BAD_CAST "type"));
                        
                        createRobot(robotType);
                        
                        curNode = robot_node;
                        
                        if (xmlGetProp(robot_node, BAD_CAST "baserobot") != NULL)
                        {
                            world->setBaseRobot(robotCount);
                        }
                        
                         PMTextureCache::instanse()->initRobotTextures(world);
                        
                        readRobotNode(robotType);
                    
                        ++robotCount;
                    }
                    
                }//end for
                
                if(robotCount == 1)
                {
                    world->setBaseRobot(0);
                }
                
                else if(world->getBaseRobot() == NULL)
                {
                    error = "No base Robot";
                    delete world;
                    return NULL;
                }
                
                for(int i = 0; i < world->robotCount(); ++i)
                {
                    if(world->robotAt(i)->parentType() == pmPlayerRobot )
                        ((PlayerRobot *)world->robotAt(i))->initTexture();
                    
                    if(world->robotAt(i)->parentType() == pmEnvRobot )
                        ((EnvRobot *)world->robotAt(i))->initTexture();
                }
                
            }
            else if(!strcmp((char *)cur_node->name, "graph"))
            {
                curNode = cur_node;
                
                if( !readMapNode() )
                    return NULL;
                
            }
            else if(!strcmp((char *)cur_node->name, "hints") && layer != NULL)
            {
                curNode = cur_node;
                
                readHintsNode();
            }
        } // end if
    }// end for

    xmlFreeDoc(doc);
    xmlCleanupParser();
    
    world->getMap()->drawMap();
    return world;
}

bool XmlWorldReader::readRobotNode(RobotType robotType)
{
    switch (robotType)
    {
        case pmRobot4:
            return readRobot4Node();
            
        case pmOven4:
            return readOven4Node();
            break;
            
        case pmTrasportRobot4:
            return readTransportRobot4Node();
            break;
            
        default:
            return false;
    }

}

bool XmlWorldReader::readMapNode()
{
    switch(worldType)
    {
        case pmWorld4:
            return readMap4Node();
            
        default:
            return false;
    }

}

bool XmlWorldReader::readRobot4Node()
{
    Robot4 *robot = (Robot4 *) world->robotAt( world->robotCount() - 1 );
    
    
    int x = atoi((char *)xmlGetProp(curNode,BAD_CAST "x"));
    int y = atoi((char *)xmlGetProp(curNode,BAD_CAST "y"));
    int layer = atoi((char *)xmlGetProp(curNode,BAD_CAST "layer"));
    int state = atoi((char *)xmlGetProp(curNode,BAD_CAST "state"));
    
    robot->setPosition( ccp(x, y), layer, state);
    
    robot->setStartPosition( ccp(x,y) );
    robot->setStartDirection(state);
    robot->setStartLayer(layer);
    
    xmlNodePtr robotNode = curNode;
    
    for (xmlNodePtr cur_node = robotNode->children; cur_node != NULL; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if(!strcmp((char *)cur_node->name, "functions"))
            {
                curNode = cur_node;
                readFunctionsNode((UniversalRobot *)robot == world->getBaseRobot());
            }
            else if(!strcmp((char *)cur_node->name, "tasks"))
            {
                curNode = cur_node;
                readTaskNode();
            }
        }
    }
    
    
    return true;
}

bool XmlWorldReader::readOven4Node()
{
    Oven4 *robot = (Oven4 *) world->robotAt( world->robotCount() - 1 );
    
    
    int x = atoi((char *)xmlGetProp(curNode,BAD_CAST "x"));
    int y = atoi((char *)xmlGetProp(curNode,BAD_CAST "y"));
    int layer = atoi((char *)xmlGetProp(curNode,BAD_CAST "layer"));
    int state = atoi((char *)xmlGetProp(curNode,BAD_CAST "state"));
    
    robot->setPosition( ccp(x, y), layer, state);
    
    robot->setStartPosition( ccp(x,y) );
    robot->setStartDirection(state);
    robot->setStartLayer(layer);
        
    return true;
}

bool XmlWorldReader::readTransportRobot4Node()
{
    TransportRobot4 *robot = (TransportRobot4 *) world->robotAt( world->robotCount() - 1 );
    
    
    int x = atoi((char *)xmlGetProp(curNode,BAD_CAST "x"));
    int y = atoi((char *)xmlGetProp(curNode,BAD_CAST "y"));
    int layer = atoi((char *)xmlGetProp(curNode,BAD_CAST "layer"));
    int state = atoi((char *)xmlGetProp(curNode,BAD_CAST "state"));
    
    robot->setPosition( ccp(x, y), layer, state);
    
    robot->setStartPosition( ccp(x,y) );
    robot->setStartDirection(state);
    robot->setStartLayer(layer);
    
    return true;
}

bool getbit(int src, int index)
{
    return src &= (1 << index);
}

bool XmlWorldReader::readMap4Node()
{
    int layer = 0;
    
    World4 *world4 = (World4 *)world;
    
    for (xmlNodePtr layerNode = curNode->children; layerNode != NULL; layerNode = layerNode->next)
    {
        if (layerNode->type == XML_ELEMENT_NODE)
        {
            if(strcmp((char *)layerNode->name, "layer"))
            {
                error = "Not registered node :" + std::string((char *)layerNode->name);
                delete world;
                return false;
            }
            
            int width = world4->getMap4()->getWidth();
            int height = world4->getMap4()->getHeight();
            
            char * MapStr = new char[width*height*3];
            sscanf((char*)(layerNode->children->content), " %s", MapStr);
            
            std::strcpy(MapStr, base64_decode(MapStr).c_str());
            
            std::vector<std::vector<std::vector<MapElement *> > > &layers = world4->getMap4()->mapElements;
            
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    int tmp = y * width + x;
                    int walls = (unsigned char)MapStr[tmp*2]-1;
                    int type = MapStr[tmp*2+1]-2;
                    
                    layers[layer][y][x]->setElement(type);
                    
                    if(layers[layer][y][x]->getGrassSprite() != NULL)
                    {
                        layers[layer][y][x]->getGrassSprite()->setPosition(world4->getMap4()->Coordinates(x,y,0,0));
                    }
                    
                    if (type == MapElement::GreenGrass)
                    {
                        world4->getMap4()->incGrassCount();
                    }
                    
                    if(y < height-1 && getbit(walls, 7))
                        layers[layer][y][x]->nearElements[Map4::Down] = layers[layer][y+1][x];
                    else
                        layers[layer][y][x]->nearElements[Map4::Down] = NULL;
                    
                    if(x < width-1 && getbit(walls, 6))
                        layers[layer][y][x]->nearElements[Map4::Right] = layers[layer][y][x+1];
                    else
                        layers[layer][y][x]->nearElements[Map4::Right] = NULL;
                    
                    if(y > 0 && getbit(walls, 5))
                        layers[layer][y][x]->nearElements[Map4::Up] = layers[layer][y-1][x];
                    else
                        layers[layer][y][x]->nearElements[Map4::Up] = NULL;
                    
                    if(x > 0 && getbit(walls, 4))
                        layers[layer][y][x]->nearElements[Map4::Left] = layers[layer][y][x-1];
                    else
                        layers[layer][y][x]->nearElements[Map4::Left] = NULL;
                    
                    CCSprite* sprite = NULL;
                    
                    if ( x==0 || y == 0 )
                    {
                        sprite = NULL;
                    }
                    else if(layers[layer][y][x]->nearElements[Map4::Left] == NULL)
                    {
                        if (layers[layer][y][x]->nearElements[Map4::Up] == NULL)
                        {
                            sprite = PMTextureCache::instanse()->getMapWallTile(2);
                            //Лево верх 64x34
                        }
                        else
                        {
                            sprite = PMTextureCache::instanse()->getMapWallTile(0);
                            //Лево 43х34
                        }
                    }
                    else if (layers[layer][y][x]->nearElements[Map4::Up] == NULL)
                    {
                        sprite = PMTextureCache::instanse()->getMapWallTile(1);
                        //Верх
                    }
                    else
                    {
                        sprite = NULL;
                    }
                    
                    if (sprite!=NULL)
                    {
                        sprite->setPosition(world4->getMap4()->Coordinates(x,y,-7,7));
                    }
                    
                    layers[layer][y][x]->setWallSprite(sprite);
                } //end for x
            } // end for y
            
            ++layer;
        }//end if
        

    }// end for
    
    
    
    return true;
}

bool XmlWorldReader::readFunctionsNode(bool isBaseRobot)
{
    if(gameLayer == NULL)
        return true;
    
    
    ProgramLayer *programLayer = NULL;
    
    if(isBaseRobot)
    {
        programLayer = ProgramLayer::create(gameLayer);
        gameLayer->addChild(programLayer, 15, 1);
        gameLayer->setProgramLayer(programLayer);
    }
        
    xmlNodePtr functionNode = curNode;
    
    int cnt = 0;
    
    for (xmlNodePtr cur_node = functionNode->children; cur_node != NULL; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE && !strcmp((char *)cur_node->name, "func"))
        {
            curNode = cur_node;
            
            MethodLayer *methodLayer = NULL;
            
            if(isBaseRobot)
            {
                methodLayer = MethodLayer::create(programLayer);
                methodLayer->index = cnt;
                programLayer->layers.push_back(methodLayer);
            }
            
            readMethodNode(methodLayer);
            
            cnt++;
        }
    }
    
    if(isBaseRobot)
    {
        programLayer->drawLayers(world);
    }
    
    return true;
}

bool XmlWorldReader::readMethodNode(MethodLayer *methodLayer)
{
    Robot4 *robot = (Robot4 *) world->robotAt( world->robotCount() - 1 );
    
    char *functionContent = NULL;
    char *backgroundContent = NULL;
    
    for (xmlNodePtr cur_node = curNode->children; cur_node != NULL; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if(!strcmp((char *)cur_node->name, "function"))
            {
                functionContent = NodeContent(cur_node);
            }
            
            if(!strcmp((char *)cur_node->name, "background"))
            {
                backgroundContent = NodeContent(cur_node);
            }
        }
    }
    
    if(functionContent == NULL || backgroundContent == NULL)
    {
        error = "No content for functions!";
        return false;
    }

    std::vector<int> methods;
    std::vector<int> background;
    
    string name = (char *)xmlGetProp(curNode,BAD_CAST "name");
    int repeater = atoi((char *)xmlGetProp(curNode,BAD_CAST "repeat"));
    int condition = atoi((char *)xmlGetProp(curNode,BAD_CAST "condition"));
    bool useRepeater = atoi((char *)xmlGetProp(curNode,BAD_CAST "isRepeat"));
    bool useCondition = atoi((char *)xmlGetProp(curNode,BAD_CAST "isCondition"));
    int width = atoi((char *)xmlGetProp(curNode,BAD_CAST "width"));
    int height = atoi((char *)xmlGetProp(curNode,BAD_CAST "height"));
   
    bool hidden = atoi((char *)xmlGetProp(curNode,BAD_CAST "hide"));
    char *givenText = NULL;
    char *doText = NULL;
    
    bool resizable = atoi((char *)xmlGetProp(curNode,BAD_CAST "lockheight")) != 0;
    int maxHeight = atoi((char *)xmlGetProp(curNode,BAD_CAST "maxheight"));
    
    if(hidden)
    {
        givenText = (char *)xmlGetProp(curNode,BAD_CAST "giventext");
        doText = (char *)xmlGetProp(curNode,BAD_CAST "dotext");
    }
    
    if(repeater == 0)
        repeater = 1;
        
    int methodID;
    int methodID_Exec;
    
    if(name == "main"){
        methodID = CMDMAIN;
        methodID_Exec = CMDMAIN_EXECUTE;
    } else if(name == "Command 1") {
        methodID = CMDA;
        methodID_Exec = CMDA_EXECUTE;
    } else if(name == "Command 2") {
        methodID = CMDB;
        methodID_Exec = CMDB_EXECUTE;
    } else if(name == "Command 3") {
        methodID = CMDC;
        methodID_Exec = CMDC_EXECUTE;
    }

    
    for (int i = 0; i < width * height; i++)
    {
        switch(functionContent[i])
        {
            case 'A' : methods.push_back(CMDA_EXECUTE); break;
            case 'B' : methods.push_back(CMDB_EXECUTE); break;
            case 'C' : methods.push_back(CMDC_EXECUTE);break;
            default : methods.push_back(functionContent[i] - '0');break;
        }
        
        background.push_back(backgroundContent[i] - '0');
        
    }
    
    if(methodLayer == NULL)
    {
        vector<_instruction> instrVector = RobotMethod::makeMethod(methods, -1);
        vector<_instruction> execVector = RobotMethod::makeExecuteMethod(methodID, useRepeater, repeater, useCondition, condition);
        
        robot->addMethod(methodID,  RobotMethod(robot, instrVector, RobotMethod::ProgrammedMethod) );
        robot->addMethod(methodID_Exec,  RobotMethod(robot, execVector, RobotMethod::ProgrammedMethod) );
    }
    else
    {
        methodLayer->methodID = methodID;
        methodLayer->methodID_Exec = methodID_Exec;
        
        methodLayer->name = name;
        methodLayer->repeater = repeater;
        methodLayer->condition = condition;
        methodLayer->useRepeater = useRepeater;
        methodLayer->useCondition = useCondition;
        methodLayer->width = width;
        methodLayer->height = height;
        
        methodLayer->methods = methods;
        methodLayer->backGround = background;
        
        methodLayer->hidden = hidden;
        
        if(hidden)
        {
            methodLayer->givenText = givenText;
            methodLayer->doText = doText;
        }
        
        methodLayer->resizable = resizable;
        methodLayer->maxHeight = maxHeight;
        methodLayer->minHeight = height;
        
        methodLayer->drawFunctions();
    }
    
    return true;
}

bool XmlWorldReader::readTaskNode()
{
    Robot4 *robot = (Robot4 *) world->robotAt( world->robotCount() - 1 );
    
    Task *task = Task::create(robot);
    
    for (xmlNodePtr cur_Node = curNode->children; cur_Node!=NULL; cur_Node=cur_Node->next)
    {
        if (cur_Node->type == XML_ELEMENT_NODE)
        {
            if(!strcmp((char *)cur_Node->name, "paint"))
            {
                task->setGrassToPaint(atoi(NodeProp(cur_Node, "count")));
                
                if(task->getGrassToPaint() == 0)
                {
                    task->setGrassToPaint( ((Map4*)(world->getMap()))->getGrassCount() );
                }
            }
            else if(!strcmp((char *)cur_Node->name, "furnace"))
            {
                task->setFurnaceToBurn(atoi(NodeProp(cur_Node, "count")));
                
//                if(task->getFurnaceToBurn() == 0)
//                {
//                    for (int i = 0; i < GameParams::getInstanse()->world->robotCount(); ++i)
//                    {
//                        if (world->robotAt(i)->type() == pmOven4)
//                        {
//                            task->FurnaceToBurnInc();
//                        }
//                    }
//
//                }
            }
            else if(!strcmp((char *)cur_Node->name, "position"))
            {
                
                CCPoint endPoint;
                
                endPoint.x = atoi(NodeProp(cur_Node, "x"));
                endPoint.y = atoi(NodeProp(cur_Node, "y"));
                task->setEndLayer( atoi(NodeProp(cur_Node, "layer")) );
                
                                
                task->setEndPoint(endPoint);
            }
            
        }//end if
    }//end for
    
    robot->setTask(task);
    
    
    return true;
}

bool XmlWorldReader::readHintsNode()
{    
    for (xmlNodePtr hintNode = curNode->children; hintNode != NULL; hintNode = hintNode->next)
    {
        if (hintNode->type == XML_ELEMENT_NODE && !strcmp((char *)hintNode->name, "hint"))
        {
            Hint *hint = Hint::create();
            
			char *targetString = NodeProp(hintNode,"target");

			if(targetString == NULL)
				targetString = "global";

            std::string target = targetString;
            
            if(target == "global")
            {
                HintPart part = {};
                part.type = MethodHint;
                part.hintText = NodeContent(hintNode);
                
                hint->hintParts.push_back(part);
                
                gameLayer->setHint(hint);
            }
            else
            {
                MethodLayer *layer;
                
                for (int i = 0; i < gameLayer->getProgramLayer()->layers.size();++i )
                {
                    if(gameLayer->getProgramLayer()->layers[i]->name == target)
                        layer = gameLayer->getProgramLayer()->layers[i];
                }

                
                for (xmlNodePtr cur_node = hintNode->children; cur_node != NULL; cur_node = cur_node->next)
                {
                    if (cur_node->type == XML_ELEMENT_NODE)
                    {
                        HintPart part = {};
                        
                        std::string _type = NodeProp(cur_node,"type");
                        
                        if (_type == "text")
                        {
                            part.type = TextHint;
                            part.hintText = NodeContent(cur_node);
                            
                        }
                        else if(_type == "function")
                        {
                            part.type = MethodHint;
                            part.hintText = "";
                            
                            
                            char * content = NodeContent(cur_node);
                            
                            for (int i = 0; i < layer->methods.size(); ++i)
                            {
                                switch(content[i])
                                {
                                    case '-' : part.funcHint.push_back(EMPTY_METHOD); break;
                                    case 'A' : part.funcHint.push_back(CMDA_EXECUTE); break;
                                    case 'B' : part.funcHint.push_back(CMDB_EXECUTE); break;
                                    case 'C' : part.funcHint.push_back(CMDC_EXECUTE);break;
                                    default : part.funcHint.push_back(content[i] - '0');break;
                                }

                            }
                        }
                        
                        hint->hintParts.push_back(part);
                        layer->setHint(hint);
                    }//end if
                }//end for
                
                
            }// end else
            
        }//end if
    }// end for


    
    return true;
}

void XmlWorldReader::createRobot(RobotType robotType)
{
    UniversalRobot *robot;
    
    switch (robotType)
    {
        case pmRobot4:
            robot = Robot4::create(world);
            break;
            
        case pmOven4:
            robot = Oven4::create(world);
            break;
        
        case pmTrasportRobot4:
            robot = TransportRobot4::create(world);
            break;
            
        default:
            robot = NULL;
            break;
    }
    
    world->addRobot(robot);
}

void XmlWorldReader::createWorldByType()
{
    switch(worldType)
    {
        case pmWorld4:
            world  = World4::create(
                        atoi((char *)xmlGetProp(curNode,BAD_CAST "width")),
                        atoi((char *)xmlGetProp(curNode,BAD_CAST "height")),
                        atoi((char *)xmlGetProp(curNode,BAD_CAST "layers")),
                        atoi((char *)xmlGetProp(curNode,BAD_CAST "tileset"))
                    );
            break;
        default: return;
    }
    
    PMTextureCache::instanse()->initMapTexture(world->type(),atoi((char *)xmlGetProp(curNode,BAD_CAST "tileset")));
}


