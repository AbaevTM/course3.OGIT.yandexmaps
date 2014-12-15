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
#include <sc_memory.h>
#include <sc_helper.h>
#include <stdio.h>
/**
 * @brief add_map_object_description_into_answer
 * Creates description of the map_object in json format and appends
 * node containing this description to answer set.
 * @param event
 * @param map_object
 * @param answer
 * @return
 */
sc_result add_map_object_description_into_answer(
        const sc_event *event,
        sc_addr map_object,
        sc_addr answer
        );


/////////////////////////////////
////////CONSTANTS//////////
///////////////////////////////

#define MAX_RESULT_STRING_LENGTH 5
#define MAX_RESULT_STRING_LENGTH_MULTIPIER 2
#define CONCEPT_MAPPED_POINT            "concept_mapped_point"
#define CONCEPT_MAPPED_LINE_STRING      "concept_mapped_line_string"
#define CONCEPT_MAPPED_LINEAR_RING      "concept_mapped_linear_ring"
#define CONCEPT_MAPPED_POLYGON          "concept_mapped_polygon"
#define CONCEPT_GEO_ROUTE               "concept_geo_route"
#define RREL_GEO_ROUTE_BEGIN            "rrel_geo_route_begin"
#define RREL_GEO_ROUTE_END              "rrel_geo_route_end"
#define NREL_YANDEX_MAP                 "nrel_yandex_map"
#define NREL_FORMAT                     "nrel_format"
#define FORMAT_YANDEXMAPLINK            "format_yandexmaplink"
#define NREL_GEODETIC_COORDINATES       "nrel_geodetic_coordinates"
#define NREL_WGS_84_TRANSLATION         "nrel_WGS_84_translation"
#define NREL_WGS_84_LONGITUDE_DEGREE    "nrel_WGS_84_longitude_degree"
#define NREL_WGS_84_LATITUDE_DEGREE     "nrel_WGS_84_latitude_degree"
#define RREL_EXTERIOR_POLYGON_BORDER    "rrel_exterior_polygon_border"
#define RREL_INTERIOR_POLYGON_BORDER    "rrel_interior_polygon_border"

sc_addr concept_mapped_point;

sc_addr concept_mapped_line_string;

sc_addr concept_mapped_linear_ring;

sc_addr concept_mapped_polygon;

sc_addr concept_geo_route;

sc_addr rrel_geo_route_begin;

sc_addr rrel_geo_route_end;

sc_addr nrel_yandex_map;

sc_addr nrel_format;

sc_addr format_yandexmaplink;

sc_addr nrel_geodetic_coordinates;

sc_addr nrel_WGS_84_translation;

sc_addr nrel_WGS_84_longitude_degree;

sc_addr nrel_WGS_84_latitude_degree;

sc_addr rrel_exterior_polygon_border;

sc_addr rrel_interior_polygon_border;

sc_result initializeYandexmapDescriptionConstants();
