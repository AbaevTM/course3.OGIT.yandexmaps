
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
#include "../merge/merge.h"
#include "search_defines.h"
#include <glib.h>
#include <sc_helper.h>
#include <sc_memory_headers.h>
#include <stdio.h>

////////////////////////////////////
///RESULT_STRING_HEADERS///////////
//////////////////////////////////

sc_char * resultString = 0;

size_t maxResultStringLength = MAX_RESULT_STRING_LENGTH;

size_t usedResultStringLength = 0;

void writeToResultString( const sc_char * string);

void relocateResultString( size_t stringSize);

void flushStringToDescriptionNode();

void writeObjectType(const sc_char * objectType);

/////////////////////////////////
////////AGENT_HEADERS///////////
///////////////////////////////

sc_addr createDescriptionNode();

sc_bool checkIfMapHasBeenBuilt(sc_addr object);

void createDescriptionGeoRoute(sc_addr mapObject);

void createDescriptionMapPoint(sc_addr mapObject);

void createDescriptionMapLineString(sc_addr mapObject);

void createDescriptionMapLinearRing(sc_addr mapObject);

void createDescriptionMapPolygon(sc_addr mapObject);

void appendDescriptionNodeToAnswer(sc_addr translatedNode, sc_addr answer);

void describeCoordinatesPoint(sc_addr object);

void describeCoordinatesLine(sc_addr object);

void describeCoordinatesPolygon(sc_addr object);

void describeCoordinate(sc_addr coordinate);

/////////////////////////////////
////////AGENT_IMPLEMENTATION///////////////////
///////////////////////////////
sc_result add_map_object_description_into_answer(
        const sc_event *event,
        sc_addr mapObject,
        sc_addr answer
        ){
    sc_iterator3 *typeIterator;
    sc_addr descriptionNode;
    if( initializeYandexmapDescriptionConstants() == SC_RESULT_OK){
        if(checkIfMapHasBeenBuilt(mapObject) == SC_TRUE){
            return SC_RESULT_OK;
        }
        relocateResultString(0);
        //CHECK FOR OBJECT TYPE
        //GEO_ROUTE
        typeIterator = sc_iterator3_f_a_f_new(s_default_ctx,
                    concept_geo_route,
                    sc_type_arc_pos_const_perm,
                    mapObject
                    );
        if( sc_iterator3_next(typeIterator) == SC_TRUE){
            sc_iterator3_free(typeIterator);
            writeToResultString("[");
            descriptionNode = createDescriptionNode(mapObject);
            createDescriptionGeoRoute(mapObject);
            appendDescriptionNodeToAnswer(
                        descriptionNode,
                        answer
                        );
            flushStringToDescriptionNode(descriptionNode);
            return SC_RESULT_OK;
        }
        sc_iterator3_free(typeIterator);
        //POINT
        typeIterator = sc_iterator3_f_a_f_new(s_default_ctx,
                    concept_mapped_point,
                    sc_type_arc_pos_const_perm,
                    mapObject
                    );
        if( sc_iterator3_next(typeIterator) == SC_TRUE){
            sc_iterator3_free(typeIterator);
            writeToResultString("[");
            descriptionNode = createDescriptionNode(mapObject);
            createDescriptionMapPoint(mapObject);
            appendDescriptionNodeToAnswer(
                        descriptionNode,
                        answer
                        );
            flushStringToDescriptionNode(descriptionNode);
            return SC_RESULT_OK;
        }
        sc_iterator3_free(typeIterator);
        //LINE STRING
        typeIterator = sc_iterator3_f_a_f_new(s_default_ctx,
                    concept_mapped_line_string,
                    sc_type_arc_pos_const_perm,
                    mapObject
                    );
        if( sc_iterator3_next(typeIterator) == SC_TRUE){
            sc_iterator3_free(typeIterator);
            writeToResultString("[");
            descriptionNode = createDescriptionNode(mapObject);
            createDescriptionMapLineString(mapObject);
            appendDescriptionNodeToAnswer(
                        descriptionNode,
                        answer
                        );
            flushStringToDescriptionNode(descriptionNode);
            return SC_RESULT_OK;
        }
        sc_iterator3_free(typeIterator);
        //LINEAR RING
        typeIterator = sc_iterator3_f_a_f_new(s_default_ctx,
                    concept_mapped_linear_ring,
                    sc_type_arc_pos_const_perm,
                    mapObject
                    );
        if( sc_iterator3_next(typeIterator) == SC_TRUE){
            sc_iterator3_free(typeIterator);
            writeToResultString("[");
            descriptionNode = createDescriptionNode(mapObject);
            createDescriptionMapLinearRing(mapObject);
            appendDescriptionNodeToAnswer(
                        descriptionNode,
                        answer
                        );
            flushStringToDescriptionNode(descriptionNode);
            return SC_RESULT_OK;
        }
        sc_iterator3_free(typeIterator);
        //POLYGON
        typeIterator = sc_iterator3_f_a_f_new(s_default_ctx,
                    concept_mapped_polygon,
                    sc_type_arc_pos_const_perm,
                    mapObject
                    );
        if( sc_iterator3_next(typeIterator) == SC_TRUE){
            writeToResultString("[");
            sc_iterator3_free(typeIterator);
            descriptionNode = createDescriptionNode(mapObject);
            createDescriptionMapPolygon(mapObject);
            appendDescriptionNodeToAnswer(
                        descriptionNode,
                        answer
                        );
            flushStringToDescriptionNode(descriptionNode);
            return SC_RESULT_OK;
        }
        sc_iterator3_free(typeIterator);
    }
    return SC_RESULT_OK;
}
/////////////////////////////////
////////BUILD_DESCRIPTION_NODES/
///////////////////////////////

void createDescriptionGeoRoute(sc_addr object){
    sc_iterator3 *routeIterator;
    sc_iterator3 *typeIterator;
    sc_addr route_element;
    routeIterator = sc_iterator3_f_a_a_new(s_default_ctx,
                object,
                sc_type_arc_pos_const_perm,
                sc_type_const | sc_type_node
                );
    while( sc_iterator3_next(routeIterator)){
        route_element = sc_iterator3_value(routeIterator,2);
        //POINT
        typeIterator = sc_iterator3_f_a_f_new(s_default_ctx,
                    concept_mapped_point,
                    sc_type_arc_pos_const_perm,
                    route_element
                    );
        if( sc_iterator3_next(typeIterator) == SC_TRUE){
            sc_iterator3_free(typeIterator);
            createDescriptionMapPoint(route_element);
            writeToResultString("},");
            continue;
        }
        sc_iterator3_free(typeIterator);
        //LINE STRING
        typeIterator = sc_iterator3_f_a_f_new(s_default_ctx,
                    concept_mapped_line_string,
                    sc_type_arc_pos_const_perm,
                    route_element
                    );
        if( sc_iterator3_next(typeIterator) == SC_TRUE){
            sc_iterator3_free(typeIterator);
            createDescriptionMapLineString(route_element);
            writeToResultString("},");
            continue;
        }
        sc_iterator3_free(typeIterator);
        //LINEAR RING
        typeIterator = sc_iterator3_f_a_f_new(s_default_ctx,
                    concept_mapped_linear_ring,
                    sc_type_arc_pos_const_perm,
                    route_element
                    );
        if( sc_iterator3_next(typeIterator) == SC_TRUE){
            sc_iterator3_free(typeIterator);
            createDescriptionMapLinearRing(route_element);
            writeToResultString("},");
            continue;
        }
        sc_iterator3_free(typeIterator);
        //POLYGON
        typeIterator = sc_iterator3_f_a_f_new(s_default_ctx,
                    concept_mapped_polygon,
                    sc_type_arc_pos_const_perm,
                    route_element
                    );
        if( sc_iterator3_next(typeIterator) == SC_TRUE){
            sc_iterator3_free(typeIterator);
            createDescriptionMapPolygon(route_element);
            writeToResultString("},");
            continue;
        }
        sc_iterator3_free(typeIterator);
    }
    writeToResultString("{\"dolzni\": \"6000evro\"");
    sc_iterator3_free(routeIterator);
}

void createDescriptionMapPoint(sc_addr object){
    writeObjectType(CONCEPT_MAPPED_POINT);
    describeCoordinatesPoint(object);
}

void createDescriptionMapLineString(sc_addr object){
    writeObjectType(CONCEPT_MAPPED_LINE_STRING);
    describeCoordinatesLine(object);
}

void createDescriptionMapLinearRing(sc_addr object){
    writeObjectType(CONCEPT_MAPPED_LINEAR_RING);
    describeCoordinatesLine(object);
}

void createDescriptionMapPolygon(sc_addr object){
    writeObjectType(CONCEPT_MAPPED_POLYGON);
    describeCoordinatesPolygon(object);
}

void describeCoordinatesPoint(sc_addr object){
    sc_iterator5 * coordinatesIterator;
    sc_iterator3 * coordinateIterator;
    sc_addr coordinates;
    sc_addr coordinate;
    writeToResultString("\n\t\"coordinates\" : ");
    coordinatesIterator = sc_iterator5_a_a_f_a_f_new(s_default_ctx,
                sc_type_node | sc_type_const,
                sc_type_arc_common | sc_type_const,
                object,
                sc_type_arc_pos_const_perm,
                nrel_geodetic_coordinates
                );
    if( sc_iterator5_next(coordinatesIterator)){
        coordinates = sc_iterator5_value(coordinatesIterator, 0);
        coordinateIterator = sc_iterator3_f_a_a_new(s_default_ctx,
                    coordinates,
                    sc_type_arc_pos_const_perm,
                    sc_type_const | sc_type_node
                    );
        if( sc_iterator3_next(coordinateIterator)){
            coordinate = sc_iterator3_value(coordinateIterator,2);
            describeCoordinate(coordinate);
        }
        sc_iterator3_free(coordinateIterator);
    }
    sc_iterator5_free(coordinatesIterator);
    writeToResultString("\n\t\"scMode\" : \"on\"");
}

void describeCoordinatesPolygon(sc_addr object){
    sc_iterator5 * coordinatesIterator, * coordinateIterator;
    sc_addr coordinates;
    sc_addr coordinate;
    writeToResultString("\n\t\"coordinates\" : [");
    coordinatesIterator = sc_iterator5_a_a_f_a_f_new(s_default_ctx,
                sc_type_node | sc_type_const,
                sc_type_arc_common | sc_type_const,
                object,
                sc_type_arc_pos_const_perm,
                nrel_geodetic_coordinates
                );
    if( sc_iterator5_next(coordinatesIterator)){
        coordinates = sc_iterator5_value(coordinatesIterator, 0);
        //exterior border
        writeToResultString("\n\t\t[");
        coordinateIterator = sc_iterator5_f_a_a_a_f_new(s_default_ctx,
                    coordinates,
                    sc_type_arc_pos_const_perm,
                    sc_type_const | sc_type_node,
                    sc_type_arc_pos_const_perm,
                    rrel_exterior_polygon_border
                    );
        while( sc_iterator5_next(coordinateIterator)){
            coordinate = sc_iterator5_value(coordinateIterator,2);
            describeCoordinate(coordinate);
        }
        sc_iterator5_free(coordinateIterator);
        writeToResultString("\n\t\t\"scMode\"");
        writeToResultString("\n\t\t],");
        //interior border
        writeToResultString("\n\t\t[");
        coordinateIterator = sc_iterator5_f_a_a_a_f_new(s_default_ctx,
                    coordinates,
                    sc_type_arc_pos_const_perm,
                    sc_type_const | sc_type_node,
                    sc_type_arc_pos_const_perm,
                    rrel_interior_polygon_border
                    );
        while( sc_iterator5_next(coordinateIterator)){
            coordinate = sc_iterator5_value(coordinateIterator,2);
            describeCoordinate(coordinate);
        }
        sc_iterator5_free(coordinateIterator);
        writeToResultString("\n\t\t\"scMode\"");
        writeToResultString("\n\t\t]");
    }
    sc_iterator5_free(coordinatesIterator);
    writeToResultString("\n\t]");
}

void describeCoordinatesLine(sc_addr object){
    sc_iterator5 * coordinatesIterator;
    sc_iterator3 * coordinateIterator;
    sc_addr coordinates;
    sc_addr coordinate;
    writeToResultString("\n\t\"coordinates\" : [");
    coordinatesIterator = sc_iterator5_a_a_f_a_f_new(s_default_ctx,
                sc_type_node | sc_type_const,
                sc_type_arc_common | sc_type_const,
                object,
                sc_type_arc_pos_const_perm,
                nrel_geodetic_coordinates
                );
    if( sc_iterator5_next(coordinatesIterator)){
        coordinates = sc_iterator5_value(coordinatesIterator, 0);
        coordinateIterator = sc_iterator3_f_a_a_new(s_default_ctx,
                    coordinates,
                    sc_type_arc_pos_const_perm,
                    sc_type_const | sc_type_node
                    );
        while( sc_iterator3_next(coordinateIterator)){
            coordinate = sc_iterator3_value(coordinateIterator,2);
            describeCoordinate(coordinate);
        }
        sc_iterator3_free(coordinateIterator);
    }
    sc_iterator5_free(coordinatesIterator);
    writeToResultString("\n\t\t\"scMode\"");
    writeToResultString("\n\t]");
}


void describeCoordinate( sc_addr coordinate){
    sc_iterator5 * wgs84TranslationIterator,  * longitudeIterator,  * latitudeIterator;
    sc_addr wgs84Translation,  longitude,  latitude;
    sc_stream * contentStream;
    sc_char * contentBuffer;
    size_t buferLength = 30;
    sc_uint32 contentBytesRead;
    wgs84TranslationIterator = sc_iterator5_f_a_a_a_f_new(s_default_ctx,
                coordinate,
                sc_type_arc_common | sc_type_const,
                sc_type_const | sc_type_node,
                sc_type_arc_pos_const_perm,
                nrel_WGS_84_translation
                );
    if( sc_iterator5_next(wgs84TranslationIterator)){
        wgs84Translation = sc_iterator5_value(wgs84TranslationIterator, 2);
        //Format : latitude , longitude
        //latitude
        latitudeIterator = sc_iterator5_f_a_a_a_f_new(s_default_ctx,
                    wgs84Translation,
                    sc_type_arc_common | sc_type_const,
                    sc_type_link,
                    sc_type_arc_pos_const_perm,
                    nrel_WGS_84_latitude_degree
                    );
        if( sc_iterator5_next(latitudeIterator)){
            latitude = sc_iterator5_value(latitudeIterator, 2);
            writeToResultString("\n\t\t[");
            sc_memory_get_link_content(s_default_ctx,latitude,&contentStream);
            contentBuffer = calloc(buferLength,sizeof(sc_char));
            sc_stream_read_data(contentStream,contentBuffer,buferLength,&contentBytesRead);
            writeToResultString(contentBuffer);
            free(contentBuffer);
            sc_stream_free(contentStream);
            writeToResultString(" , ");
        }
        sc_iterator5_free(latitudeIterator);
        //longitude
        longitudeIterator = sc_iterator5_f_a_a_a_f_new(s_default_ctx,
                    wgs84Translation,
                    sc_type_arc_common | sc_type_const,
                    sc_type_link,
                    sc_type_arc_pos_const_perm,
                    nrel_WGS_84_longitude_degree
                    );
        if( sc_iterator5_next(longitudeIterator)){
            longitude = sc_iterator5_value(longitudeIterator, 2);
            sc_memory_get_link_content(s_default_ctx,longitude,&contentStream);
            contentBuffer = calloc(buferLength,sizeof(sc_char));
            sc_stream_read_data(contentStream,contentBuffer,buferLength,&contentBytesRead);
            writeToResultString(contentBuffer);
            free(contentBuffer);
            sc_stream_free(contentStream);
            writeToResultString("],");
        }
        sc_iterator5_free(longitudeIterator);
    }
    sc_iterator5_free(wgs84TranslationIterator);
}

sc_addr createDescriptionNode(sc_addr mapObject){
    sc_addr formatTuple;
    sc_addr translatedNode = sc_memory_link_new(s_default_ctx);
    sc_addr translatedNodeTuple = sc_memory_arc_new(s_default_ctx,
                sc_type_const|sc_type_arc_common,
                mapObject,
                translatedNode
                );
    sc_memory_arc_new(s_default_ctx,
                sc_type_arc_pos_const_perm,
                nrel_yandex_map,
                translatedNodeTuple
                );
    formatTuple = sc_memory_arc_new(s_default_ctx,
                sc_type_const|sc_type_arc_common,
                translatedNode,
                format_yandexmaplink
                );
    sc_memory_arc_new(s_default_ctx,
                sc_type_arc_pos_const_perm,
                nrel_format,
                formatTuple
                );
    return translatedNode;
}

void appendDescriptionNodeToAnswer(sc_addr translatedNode, sc_addr answer){
    sc_addr arc;
    if (sc_helper_check_arc(s_default_ctx,answer, translatedNode, sc_type_arc_pos_const_perm) == SC_TRUE)
        return;
    arc = sc_memory_arc_new(s_default_ctx,sc_type_arc_pos_const_perm, answer, translatedNode);
    SYSTEM_ELEMENT(arc);
}

sc_bool checkIfMapHasBeenBuilt(sc_addr object){
    sc_iterator5 * mapIterator;
    mapIterator = sc_iterator5_f_a_a_a_f_new(s_default_ctx,
                object,
                sc_type_arc_common | sc_type_const,
                sc_type_link,
                sc_type_arc_pos_const_perm,
                nrel_yandex_map
                );
    while( sc_iterator5_next(mapIterator)){
        sc_iterator5_free(mapIterator);
        return SC_TRUE;
    }
    sc_iterator5_free(mapIterator);
    return SC_FALSE;
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
        relocateResultString(sizeOfString+strlen(resultString));
    }
    strcat(resultString,string);
    usedResultStringLength += sizeOfString;
}

void writeObjectType(const sc_char * objectType){
    writeToResultString("{\n\t\"objectType\" : \"");
    writeToResultString(objectType);
    writeToResultString("\",");
}

void flushStringToDescriptionNode(sc_addr descriptionNode){
    sc_char * temp;
    size_t index = 0;
    writeToResultString("\n}\n]");
    temp = calloc(strlen(resultString), sizeof(sc_char));
    for( index; index < strlen(resultString); ++index){
        temp[index] = resultString[index];
    }
    sc_stream * stream = sc_stream_memory_new( temp, strlen(resultString),
                                               SC_STREAM_READ, SC_FALSE);
    sc_memory_set_link_content(s_default_ctx,descriptionNode,stream);
    free(temp);
    sc_stream_free(stream);
    free(resultString);
    resultString = 0;
    usedResultStringLength = 0;
    maxResultStringLength = MAX_RESULT_STRING_LENGTH;
}

sc_result initializeYandexmapDescriptionConstants(){
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                CONCEPT_MAPPED_POINT,
                strlen(CONCEPT_MAPPED_POINT),
                &concept_mapped_point
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                CONCEPT_MAPPED_LINE_STRING,
                strlen(CONCEPT_MAPPED_LINE_STRING),
                &concept_mapped_line_string
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                CONCEPT_MAPPED_LINEAR_RING,
                strlen(CONCEPT_MAPPED_LINEAR_RING),
                &concept_mapped_linear_ring
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                CONCEPT_MAPPED_POLYGON,
                strlen(CONCEPT_MAPPED_POLYGON),
                &concept_mapped_polygon
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                NREL_YANDEX_MAP,
                strlen(NREL_YANDEX_MAP),
                &nrel_yandex_map
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                NREL_FORMAT,
                strlen(NREL_FORMAT),
                &nrel_format
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                FORMAT_YANDEXMAPLINK,
                strlen(FORMAT_YANDEXMAPLINK),
                &format_yandexmaplink
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                NREL_GEODETIC_COORDINATES,
                strlen(NREL_GEODETIC_COORDINATES),
                &nrel_geodetic_coordinates
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                NREL_WGS_84_TRANSLATION,
                strlen(NREL_WGS_84_TRANSLATION),
                &nrel_WGS_84_translation
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                NREL_WGS_84_LONGITUDE_DEGREE,
                strlen(NREL_WGS_84_LONGITUDE_DEGREE),
                &nrel_WGS_84_longitude_degree
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                NREL_WGS_84_LATITUDE_DEGREE,
                strlen(NREL_WGS_84_LATITUDE_DEGREE),
                &nrel_WGS_84_latitude_degree
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                RREL_EXTERIOR_POLYGON_BORDER,
                strlen(RREL_EXTERIOR_POLYGON_BORDER),
                &rrel_exterior_polygon_border
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                RREL_INTERIOR_POLYGON_BORDER,
                strlen(RREL_INTERIOR_POLYGON_BORDER),
                &rrel_interior_polygon_border
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                CONCEPT_GEO_ROUTE,
                strlen(CONCEPT_GEO_ROUTE),
                &concept_geo_route
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                RREL_GEO_ROUTE_BEGIN,
                strlen(RREL_GEO_ROUTE_BEGIN),
                &rrel_geo_route_begin
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    if(sc_helper_find_element_by_system_identifier(s_default_ctx,
                RREL_GEO_ROUTE_END,
                strlen(RREL_GEO_ROUTE_END),
                &rrel_geo_route_end
                ) != SC_RESULT_OK) {
        return SC_RESULT_ERROR;
    }
    return SC_RESULT_OK;
}
