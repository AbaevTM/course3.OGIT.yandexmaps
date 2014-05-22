
/*
-----------------------------------------------------------------------------
This source file is part of OSTIS (Open Semantic Technology for Intelligent Systems)
For the latest info, see http://www.ostis.net

Copyright (c) 2010-2014 OSTIS

OSTIS is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OSTIS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with OSTIS. If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
 */
#include "search_yandexmap_description_builder.h"
#include "search_keynodes.h"
#include "search_utils.h"
#include "search_defines.h"
#include <glib.h>
#include <sc_helper.h>
#include <sc_memory_headers.h>
#include <stdio.h>

#define MAX_RESULT_STRING_LENGTH 5
#define MAX_RESULT_STRING_LENGTH_MULTIPIER 2
#define CONCEPT_MAPPED_POINT "concept_mapped_point"
#define CONCEPT_MAPPED_LINE_STRING  "concept_mapped_line_string"
#define CONCEPT_MAPPED_LINEAR_RING  "concept_mapped_linear_ring"
#define CONCEPT_MAPPED_POLYGON      "concept_mapped_polygon"
#define NREL_YANDEX_MAP             "nrel_yandex_map"
#define NREL_FORMAT                 "nrel_format"
#define FORMAT_YANDEXMAPLINK        "format_yandexmaplink"
////////////////////////////////////
///RESULT_STRING_HEADERS///////////
//////////////////////////////////

sc_char * resultString = 0;

size_t maxResultStringLength = MAX_RESULT_STRING_LENGTH;

size_t usedResultStringLength = 0;

void writeToResultString( const sc_char * string);

void relocateResultString( size_t stringSize);

void printResultsToNode();

void writeObjectType(const sc_char * objectType);
/////////////////////////////////
////////AGENT_HEADERS///////////
///////////////////////////////
        /////////////////////////////////
        ////////CONSTANT_NODES//////////
        ///////////////////////////////
        sc_addr concept_mapped_point;

        sc_addr concept_mapped_line_string;

        sc_addr concept_mapped_linear_ring;

        sc_addr concept_mapped_polygon;

        sc_addr nrel_yandex_map;

        sc_addr nrel_format;

        sc_addr format_yandexmaplink;
        /////////////////////////////////
        ////////COMMON_NODES////////////
        ///////////////////////////////
        sc_addr translatedNode;

sc_result initializeThisAgent();

void build_description_map_point(sc_addr object);

void build_description_map_line_string(sc_addr object);

void build_description_map_linear_ring(sc_addr object);

void build_description_map_polygon(sc_addr object);

sc_addr create_translated_node();


/////////////////////////////////
////////AGENT///////////////////
///////////////////////////////
sc_result agent_search_search_yandexmap_description_builder(const sc_event *event, sc_addr object){
    sc_iterator3 *typeIterator;
    if( initializeThisAgent() == SC_RESULT_OK){
        relocateResultString(0);
        //CHECK FOR OBJECT TYPE
        //POINT
        typeIterator = sc_iterator3_f_a_f_new(
                    concept_mapped_point,
                    sc_type_arc_pos_const_perm,
                    object
                    );
        if( sc_iterator3_next(typeIterator) == SC_TRUE){
            build_description_map_point(object);
            sc_iterator3_free(typeIterator);
            return SC_RESULT_OK;
        }
        sc_iterator3_free(typeIterator);
        //LINE STRING
        typeIterator = sc_iterator3_f_a_f_new(
                    concept_mapped_line_string,
                    sc_type_arc_pos_const_perm,
                    object
                    );
        if( sc_iterator3_next(typeIterator) == SC_TRUE){
            build_description_map_line_string(object);
            sc_iterator3_free(typeIterator);
            return SC_RESULT_OK;
        }
        sc_iterator3_free(typeIterator);
        //LINEAR RING
        typeIterator = sc_iterator3_f_a_f_new(
                    concept_mapped_linear_ring,
                    sc_type_arc_pos_const_perm,
                    object
                    );
        if( sc_iterator3_next(typeIterator) == SC_TRUE){
            build_description_map_linear_ring(object);
            sc_iterator3_free(typeIterator);
            return SC_RESULT_OK;
        }
        sc_iterator3_free(typeIterator);
        //POLYGON
        typeIterator = sc_iterator3_f_a_f_new(
                    concept_mapped_polygon,
                    sc_type_arc_pos_const_perm,
                    object
                    );
        if( sc_iterator3_next(typeIterator) == SC_TRUE){
            build_description_map_polygon(object);
            sc_iterator3_free(typeIterator);
            return SC_RESULT_OK;
        }
        sc_iterator3_free(typeIterator);
    }
    return SC_RESULT_OK;
}
/////////////////////////////////
////////AGENT_IMPLEMENTATION////
///////////////////////////////

void build_description_map_point(sc_addr object){
    create_translated_node(object);
    writeObjectType(CONCEPT_MAPPED_POINT);
    printResultsToNode();
}

void build_description_map_line_string(sc_addr object){
    create_translated_node(object);
    writeObjectType(CONCEPT_MAPPED_LINE_STRING);
    printResultsToNode();
}

void build_description_map_linear_ring(sc_addr object){
    create_translated_node(object);
    writeObjectType(CONCEPT_MAPPED_LINEAR_RING);
    printResultsToNode();
}

void build_description_map_polygon(sc_addr object){
    create_translated_node(object);
    writeObjectType(CONCEPT_MAPPED_POLYGON);
    printResultsToNode();
}

sc_addr create_translated_node(sc_addr object){
    sc_addr translatedNodeTuple, formatTuple;
    translatedNode = sc_memory_link_new();
    translatedNodeTuple = sc_memory_arc_new(
                sc_type_const|sc_type_arc_common,
                object,
                translatedNode
                );
    sc_memory_arc_new(
                sc_type_arc_pos_const_perm,
                nrel_yandex_map,
                translatedNodeTuple
                );
    formatTuple = sc_memory_arc_new(
                sc_type_const|sc_type_arc_common,
                translatedNode,
                format_yandexmaplink
                );
    sc_memory_arc_new(
                sc_type_arc_pos_const_perm,
                nrel_format,
                formatTuple
                );
    return translatedNode;
}

void printResultsToNode(){
    writeToResultString("\n}");
    sc_stream * stream = sc_stream_memory_new( resultString, maxResultStringLength,
                                               SC_STREAM_READ, SC_FALSE);
    sc_memory_set_link_content(translatedNode,stream);
    sc_stream_free(stream);
    free(resultString);
    resultString = 0;
    usedResultStringLength = 0;
    maxResultStringLength = MAX_RESULT_STRING_LENGTH;
}
////////////////////////////////////
///RESULT_STRING_IMPLEMENTATION////
//////////////////////////////////

void relocateResultString(size_t stringSize){
    sc_char * temp;
    maxResultStringLength += MAX_RESULT_STRING_LENGTH_MULTIPIER*stringSize;
    temp = calloc(maxResultStringLength, sizeof(sc_char));
    if( resultString != 0){
        strcpy(temp, resultString);
        free(resultString);
    }
    resultString = temp;
}

size_t symbolsLeft(){
    return maxResultStringLength - usedResultStringLength;
}

void printResultStringInfo(){
    printf("\nString:%s", resultString);
    printf("\nUsed:%d", (int) usedResultStringLength);
    printf("\nMax:%d", (int) maxResultStringLength);
}

void writeToResultString( const sc_char *string){
    size_t sizeOfString = strlen(string);
    if( sizeOfString > symbolsLeft()){
        relocateResultString(sizeOfString);
    }
    strcat(resultString,string);
    usedResultStringLength += sizeOfString;
}

void writeObjectType(const sc_char * objectType){
    writeToResultString("{\n\t\"objectType\" : \"");
    writeToResultString(objectType);
    writeToResultString("\"");
}

sc_result initializeThisAgent(){
    if(sc_helper_find_element_by_system_identifier(
                CONCEPT_MAPPED_POINT,
                strlen(CONCEPT_MAPPED_POINT),
                &concept_mapped_point
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(
                CONCEPT_MAPPED_LINE_STRING,
                strlen(CONCEPT_MAPPED_LINE_STRING),
                &concept_mapped_line_string
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(
                CONCEPT_MAPPED_LINEAR_RING,
                strlen(CONCEPT_MAPPED_LINEAR_RING),
                &concept_mapped_linear_ring
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(
                CONCEPT_MAPPED_POLYGON,
                strlen(CONCEPT_MAPPED_POLYGON),
                &concept_mapped_polygon
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(
                NREL_YANDEX_MAP,
                strlen(NREL_YANDEX_MAP),
                &nrel_yandex_map
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(
                NREL_FORMAT,
                strlen(NREL_FORMAT),
                &nrel_format
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(
                FORMAT_YANDEXMAPLINK,
                strlen(FORMAT_YANDEXMAPLINK),
                &format_yandexmaplink
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    return SC_RESULT_OK;
}
