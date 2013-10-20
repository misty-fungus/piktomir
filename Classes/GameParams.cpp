//
//  GameParams.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#include "GameParams.h"

GameParams *GameParams::instanse = 0;

USING_NS_CC;

GameParams *GameParams::getInstanse(){
    if(instanse == 0)
        instanse = new GameParams();
    
    return instanse;    
}

void GameParams::save() {
    xmlSaveFile(optionsPath.c_str(),options);
}

bool GameParams::isXMLFileExist() {
    FILE * fp = fopen(optionsPath.c_str(),"r");
    if (fp) {
        fclose(fp);
        return true;
    }
    return false;
}

xmlNodePtr GameParams::getXMLNodeForKey (const char* pKey) {
    xmlNodePtr node = NULL;
    if (optionsNode->children == NULL) {
        return  NULL;
    }
    for (xmlNodePtr cur_Node = optionsNode->children; cur_Node != NULL; cur_Node = cur_Node->next) {
        if (cur_Node->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(cur_Node->name, BAD_CAST pKey) == 0) {
                node = cur_Node;
                break;
            }
        }
    }
    return node;
}

void GameParams::setValueForKey(const char* pKey, const char* pValue)
{
    xmlNodePtr node = getXMLNodeForKey(pKey);
    
    if (!node)
    {
        node = xmlNewNode(NULL, BAD_CAST pKey);
        xmlAddChild(optionsNode, node);
        
    }
    
    xmlNodeSetContent(node,BAD_CAST pValue);
    save();
}

const char* GameParams::getValueForKey(const char* pKey)
{
    const char* ret = NULL;
    xmlNodePtr node = getXMLNodeForKey(pKey);
    if (node)ret = (const char*)xmlNodeGetContent(node);
    return ret;
}

void GameParams::setintValueForKey(const char* pKey, int value) {
    char tmp[50];
    memset(tmp, 0, 50);
    sprintf(tmp, "%d", value);
    
    setValueForKey(pKey, tmp);
}

int GameParams::getintValueForKey(const char* pKey, int defValue)
{
    const char* value = getValueForKey(pKey);
    int ret = defValue;

    if (value)
    {
        ret = atoi(value);
        xmlFree((void*)value);
    }

    return ret;
}

void GameParams::setboolValueForKey(const char* pKey, bool value)
{
    if(value)setValueForKey(pKey, "true");
    else setValueForKey(pKey, "false");
}

bool GameParams::getboolValueForKey(const char* pKey, bool defValue)
{
    const char* value = getValueForKey(pKey);
    bool ret = defValue;
    
    if (value)
    {
        if (strcmp(value, "true") == 0) {
            ret = true;
        } else ret = false;
        xmlFree((void*)value);
    }
    
    return ret;
}


void GameParams::setdoubleValueForKey(const char* pKey, double value)
{
    char tmp[50];
    memset(tmp, 0, 50);
    sprintf(tmp, "%lg", value);
    
    setValueForKey(pKey, tmp);
}

double GameParams::getdoubleValueForKey(const char* pKey, double defValue)
{
    const char* value = getValueForKey(pKey);
    double ret = defValue;
    
    if (value)
    {
        sscanf(value, "%lg",&ret);
        xmlFree((void*)value);
    }
    
    return ret;
}

void GameParams::loadWorlds()
{
    CCString WorldsPath;
    WorldsPath.initWithFormat("%s/Worlds.xml",mapPath.m_sString.c_str());
    
    LIBXML_TEST_VERSION
    
    worlds.clear();
    
	unsigned long size;

	char *buf = (char *)CCFileUtils::sharedFileUtils()->getFileData(WorldsPath.m_sString.c_str(),"r", &size);

    xmlDocPtr doc = xmlReadMemory(buf, size,"Worlds.xml" ,NULL,0);
  
	if(doc == NULL)
	{
		CCMessageBox(xmlGetLastError()->message, "1");
		return;
	}

	xmlNodePtr cur_node = xmlDocGetRootElement(doc);
    
    for (cur_node = xmlDocGetRootElement(doc)->children; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            worlds.push_back( atoi((char *)xmlGetProp(cur_node,BAD_CAST "count")));
        }
    }
    
}
