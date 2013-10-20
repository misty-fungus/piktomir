//
//  XmlWorldSaver.cpp
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 22.07.13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "XmlWorldSaver.h"
#include <libxml/xmlsave.h>

using namespace std;

XMLWorldSaver::XMLWorldSaver(UniversalWorld * world, GameLayer * gamelayer, std::string path)
{
    xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr map = xmlNewDocNode(doc, NULL, BAD_CAST "map", BAD_CAST "");
    xmlDocSetRootElement(doc, map);
    
    world_ = world;
    layer_ = gamelayer;
    path_ = path;
    
    if (world_->type() == pmWorld4) {
        saveWorld4(map);
    }
    xmlSaveFile(path.c_str(), doc);
    
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

void XMLWorldSaver::saveWorld4(xmlNodePtr mapNode)
{
    World4* world = (World4*)world_;
    Map4* map = (Map4*)(world_->getMap());
    
    xmlSetProp(mapNode, BAD_CAST "width", BAD_CAST to_string(map->getWidth()).c_str());
    xmlSetProp(mapNode, BAD_CAST "height", BAD_CAST to_string(map->getHeight()).c_str());
    xmlSetProp(mapNode, BAD_CAST "layers", BAD_CAST to_string((int)map->layers.size()).c_str());
    xmlSetProp(mapNode, BAD_CAST "worldtype", BAD_CAST "0");
    xmlSetProp(mapNode, BAD_CAST "tileset", BAD_CAST to_string(map->getTileSet()).c_str());
    
    xmlNodePtr graphNode = xmlNewNode(NULL, BAD_CAST "graph");
    xmlAddChild(mapNode, graphNode);
    for (int i = 0; i < map->layers.size(); ++i)
    {
        xmlAddChild(graphNode, saveMap4Layer(i));
    }
    
    xmlNodePtr robotsNode = xmlNewNode(NULL, BAD_CAST "robots");
    xmlAddChild(mapNode, robotsNode);
    for (int i = 0; i < world->robotCount(); ++i) {
        
        xmlNodePtr robotNode = saveRobot(i);
        
        xmlAddChild(robotsNode, robotNode);
        if(world->getBaseRobot() == world->robotAt(i))
        {
            xmlAddChild(robotNode, saveProgram());
        }
        Task* task = world->robotAt(i)->getTask();
        if (task) {
            xmlAddChild(robotNode, saveTask(i));
        }
    }
    
    xmlNodePtr hintsNode = xmlNewNode(NULL, BAD_CAST "hints");
    xmlAddChild(mapNode, hintsNode);
#ifdef MAP_EDITOR
    layer_->readyForSave();
#endif
    if(layer_->getHint())xmlAddChild(hintsNode, saveHint(-1));
    for (int i = 0; i < layer_->getProgramLayer()->layers.size(); ++i) {
        
#ifdef MAP_EDITOR
        layer_->getProgramLayer()->layers[i]->readyForSave();
#endif
        
        MethodLayer * method = layer_->getProgramLayer()->layers[i];
        xmlNodePtr hint = NULL;
        if(method->getHint())
        {
            hint = saveHint(i);
            xmlAddChild(hintsNode, hint);
            xmlSetProp(hint, BAD_CAST "target", BAD_CAST method->name.c_str());
        }
    }
}

xmlNodePtr XMLWorldSaver::saveMap4Layer (int layer)
{
    Map4* map = (Map4*)(world_->getMap());
    
    xmlNodePtr layerNode = xmlNewNode(NULL, (xmlChar*)"layer");
    
    unsigned char * Map = new unsigned char[2*map->width*map->height+1];
    
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; j++) {
            Map[2*(i*map->width + j)] = 0;
            for(int k = 0; k < 4; k++) Map[2*(i*map->width + j)] += (map->mapElements[layer][i][j]->nearElements[k]!=NULL)*pow(2, 7-k);
            Map[2*(i*map->width + j)]++;
            Map[2*(i*map->width + j)+1] = map->mapElements[layer][i][j]->getType()+2;
        }
    }
    std::string str = base64_encode(Map, 2*map->width*map->height+1);
    xmlNodeSetContent(layerNode, (xmlChar*)str.c_str());
    
    return layerNode;
}

xmlNodePtr XMLWorldSaver::saveProgram ()
{
    int resize = 0;
    
    xmlNodePtr functions = xmlNewNode(NULL, BAD_CAST "functions");
    char tmp[10] = "";
    sprintf(tmp,"%d",resize);
    xmlSetProp(functions, BAD_CAST "resizable", BAD_CAST tmp);
    for (int i = 0; i < layer_->getProgramLayer()->layers.size(); ++i) {
        xmlAddChild(functions, saveMethod(i));
    }
    return functions;
}


xmlNodePtr XMLWorldSaver::saveMethod (int layer)
{
    MethodLayer* methodLayer = layer_->getProgramLayer()->layers[layer];
    
    xmlNodePtr funcNode = xmlNewNode(NULL,(xmlChar*)"func");
    
    xmlNodePtr functionNode = xmlNewNode(NULL,(xmlChar*)"function");
    xmlNodePtr backgroundNode = xmlNewNode(NULL,(xmlChar*)"background");

    xmlSetProp(funcNode, BAD_CAST "name", BAD_CAST methodLayer->name.c_str());
    
    xmlSetProp(funcNode, BAD_CAST "repeat", BAD_CAST to_string(methodLayer->repeater).c_str());
    xmlSetProp(funcNode, BAD_CAST "condition", BAD_CAST to_string(methodLayer->condition).c_str());
    xmlSetProp(funcNode, BAD_CAST "isRepeat", BAD_CAST to_string(methodLayer->useRepeater).c_str());
    xmlSetProp(funcNode, BAD_CAST "isCondition", BAD_CAST to_string(methodLayer->useCondition).c_str());
    xmlSetProp(funcNode, BAD_CAST "width", BAD_CAST to_string(methodLayer->width).c_str());
    xmlSetProp(funcNode, BAD_CAST "height", BAD_CAST to_string(methodLayer->height).c_str());
    xmlSetProp(funcNode, BAD_CAST "lockheight", BAD_CAST to_string(methodLayer->resizable).c_str());
    xmlSetProp(funcNode, BAD_CAST "maxheight", BAD_CAST to_string(methodLayer->maxHeight).c_str());
    xmlSetProp(funcNode, BAD_CAST "hide", BAD_CAST to_string(methodLayer->hidden).c_str());
    
    
    std::string func = "";
    std::string back = "";
    for (int j = 0; j < methodLayer->methods.size(); ++j) {
        switch(methodLayer->methods[j])
        {
            case CMDA_EXECUTE : func+=('A');break;
            case CMDB_EXECUTE : func+=('B');break;
            case CMDC_EXECUTE : func+=('C');break;
            default : func+=('0'+methodLayer->methods[j]);break;
        }
    }
    
    xmlNodeSetContent(functionNode, BAD_CAST func.c_str());
    xmlAddChild(funcNode, functionNode);
    
    for (int j = 0; j < methodLayer->backGround.size(); ++j) {
        switch(methodLayer->backGround[j])
        {
            case 0 : back+=('0');break; // не существует
            case 1 : back+=('1');break; // нормальный
            case 2 : back+=('2');break; // залоченный
        }
    }
    
    xmlNodeSetContent(backgroundNode, BAD_CAST back.c_str());
    xmlAddChild(funcNode, backgroundNode);
    
    if(methodLayer->hidden)
    {
        xmlNodePtr startNode = xmlNewNode(NULL, BAD_CAST "start");
        xmlNodeSetContent(startNode, BAD_CAST methodLayer->givenText.c_str());
        xmlAddChild(funcNode, startNode);
        xmlNodePtr endNode = xmlNewNode(NULL, BAD_CAST "end");
        xmlNodeSetContent(endNode, BAD_CAST methodLayer->doText.c_str());
        xmlAddChild(funcNode, endNode);
    }
    
    return funcNode;
}

xmlNodePtr XMLWorldSaver::saveTask (int robot) {
    xmlNodePtr taskNode = xmlNewNode(NULL, BAD_CAST "tasks");
    
    if(world_->robotAt(robot)->getTask()->grassToPaint >= 1) {
        char tmp[10] = "";
        xmlNodePtr paintNode = xmlNewNode(NULL, BAD_CAST "paint");
        sprintf(tmp, "%d",world_->robotAt(robot)->getTask()->grassToPaint);
        xmlSetProp(paintNode, (xmlChar*)"count", BAD_CAST tmp);
        xmlAddChild(taskNode, paintNode);
    }
    if(world_->robotAt(robot)->getTask()->endPoint.x >= 0) {
        char tmp[10] = "";
        xmlNodePtr positionNode = xmlNewNode(NULL, (xmlChar*)"position");
        
        sprintf(tmp, "%d",(int)world_->robotAt(robot)->getTask()->endPoint.x);
        xmlSetProp(positionNode, BAD_CAST "x", BAD_CAST tmp);
        
        sprintf(tmp, "%d",(int)world_->robotAt(robot)->getTask()->endPoint.y);
        xmlSetProp(positionNode, BAD_CAST "y", BAD_CAST tmp);
        
        sprintf(tmp, "%d", world_->robotAt(robot)->getTask()->endLayer);
        xmlSetProp(positionNode, BAD_CAST "layer", BAD_CAST tmp);
        xmlAddChild(taskNode, positionNode);
    }
    if(world_->robotAt(robot)->getTask()->furnaceToBurn >= 1) {
        char tmp[10] = "";
        xmlNodePtr FurnaceNode = xmlNewNode(NULL, (xmlChar*)"furnace");
        sprintf(tmp, "%d",world_->robotAt(robot)->getTask()->furnaceToBurn);
        xmlSetProp(FurnaceNode, (xmlChar*)"count", (xmlChar*)tmp);
        xmlAddChild(taskNode, FurnaceNode);
    }
    
    return taskNode;
}

xmlNodePtr XMLWorldSaver::saveHint(int methodlayer)
{
    Hint* toSave = NULL;
    if(methodlayer != -1)toSave = layer_->getProgramLayer()->layers[methodlayer]->getHint();
    else toSave = layer_->getHint();
    
    xmlNodePtr hintNode = xmlNewNode(NULL, BAD_CAST "hint");
    for (int i = 0; i < toSave->hintParts.size(); ++i)
    {
        xmlNodePtr cur_node = xmlNewNode(NULL, (xmlChar*)"hintpart");
        if (toSave->hintParts[i].type == TextHint)
        {
            xmlSetProp(cur_node, (xmlChar*)"type", (xmlChar*)"text");
            xmlNodeSetContent(cur_node, BAD_CAST toSave->hintParts[i].hintText.c_str());
        }
        else if (toSave->hintParts[i].type == GlobalHint)
        {
            xmlSetProp(cur_node, (xmlChar*)"target", (xmlChar*)"global");
            xmlNodeSetContent(cur_node, BAD_CAST toSave->hintParts[i].hintText.c_str());
        }
        else
        {
            xmlSetProp(cur_node, (xmlChar*)"type", (xmlChar*)"function");
            std::string str = "";
            for (int j = 0; j < toSave->hintParts[i].funcHint.size(); ++j)
            {
                char tmp[10] = "";
                switch(toSave->hintParts[i].funcHint[j])
                {
                    case -6 : sprintf(tmp, "A");break;
                    case -7 : sprintf(tmp, "B");break;
                    case -8 : sprintf(tmp, "C");break;
                    default : sprintf(tmp, "%d",toSave->hintParts[i].funcHint[j]);break;
                }
                str+=tmp;
            }
            xmlNodeSetContent(cur_node, (xmlChar*)str.c_str());
        }
        xmlAddChild(hintNode, cur_node);
    }
    return hintNode;
}

xmlNodePtr XMLWorldSaver::saveRobot (int robot)
{
    UniversalRobot * robot_ = world_->robotAt(robot);
    xmlNodePtr robotNode = NULL;
    
    switch (robot_->type()) {
        case pmRobot4:
        {
            Robot4 * toSave = (Robot4*)robot_;
            char tmp[10] = "";
            robotNode = xmlNewNode(NULL, (xmlChar*)"robot");
            sprintf(tmp,"%d",(int)toSave->getCurPosition().x);
            xmlSetProp(robotNode, (xmlChar*)"x", (xmlChar*)tmp);
            sprintf(tmp,"%d",(int)toSave->getCurPosition().y);
            xmlSetProp(robotNode, (xmlChar*)"y", (xmlChar*)tmp);
            sprintf(tmp,"%d",(int)toSave->getCurLayer());
            xmlSetProp(robotNode, (xmlChar*)"layer", (xmlChar*)tmp);
            sprintf(tmp,"%d",toSave->getCurDirection());
            xmlSetProp(robotNode, (xmlChar*)"state", (xmlChar*)tmp);
            xmlSetProp(robotNode, (xmlChar*)"type", (xmlChar*)"0");
            if (world_->getBaseRobot() == toSave) {
                xmlSetProp(robotNode, (xmlChar*)"baserobot", (xmlChar*)"yes");
            }
            break;
        }

        case pmOven4:
            break;
            
        default:
            break;
    }
    return robotNode;
}
