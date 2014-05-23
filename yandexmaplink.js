YandexMapLinkComponent = {
    formats: ['format_yandexmaplink'],
    factory: function(sandbox) {
        return new YandexMapLinkViewer(sandbox);
    }
};

var YandexMapLinkViewer = function(sandbox){
    this._initWindow(sandbox);
};

YandexMapLinkViewer.prototype = {
    
    _container: null,
    
    _initWindow: function(sandbox) {
        this._container = '#' + sandbox.container;
        this.sandbox = sandbox;
        if (this.sandbox.link_addr) {           
            this.sandbox.getLinkContent(this.sandbox.link_addr, 
                                            $.proxy(this.receiveData, this),
                                            function () {});
        }
    },
    
    // ---- window interface -----
    receiveData: function(data) {
    	//27.529405,53.83897
    	//15 scale
    	//CONSTANTS
    	//"concept_mapped_point"
    	//"concept_mapped_line_string"
    	//"concept_mapped_linear_ring"
    	//"concept_mapped_polygon"
        $(this._container).empty();
        $(this._container).append('<div id="' + this._container + '" style=" width: 400px;height:400px;padding: 5px;" ></div>');
        var dataJSON = JSON.parse(data);
        switch(dataJSON.objectType){
        	case "concept_mapped_point" :{
        		var myMap = new ymaps.Map(this._container, {
        	        center: dataJSON.coordinates,
        	        zoom: 10
        	    }),
        	    myGeoObject = new ymaps.GeoObject({
        	        geometry: {
        	            type: "Point",
        	            coordinates: dataJSON.coordinates
        	       }
        	    });
        	myMap.geoObjects.add(myGeoObject);
        		break;
        	}
        	case "concept_mapped_line_string" :{
        		var myMap = new ymaps.Map(this._container, {
        	            center: dataJSON.coordinates[0],
        	            zoom: 15
        	        }); 
        		dataJSON.coordinates.pop();
        		myGeoObject = new ymaps.GeoObject({
        	        geometry: {
        	            type: "LineString",
        	            coordinates: dataJSON.coordinates
        	       }
        	    });
        		myMap.geoObjects.add(myGeoObject);
        		break;
        	}
        	case "concept_mapped_linear_ring" :{
	        	var myMap = new ymaps.Map(this._container, {
	    	            center: dataJSON.coordinates[0],
	    	            zoom: 15
	    	        }); 
	    		dataJSON.coordinates.push(dataJSON.coordinates[0]);
	    		myGeoObject = new ymaps.GeoObject({
	    	        geometry: {
	    	            type: "LineString",
	    	            coordinates: dataJSON.coordinates
	    	       }
	    	    });
	    		myMap.geoObjects.add(myGeoObject);
	    		break;
        	}
        	case "concept_mapped_polygon" :{
        		var myMap = new ymaps.Map(this._container, {
    	            center: dataJSON.coordinates[0][0],
    	            zoom: 15
	    	        }); 
        		dataJSON.coordinates[0].pop();
                dataJSON.coordinates[1].pop();
	    		myGeoObject = new ymaps.GeoObject({
	    	        geometry: {
	    	            type: "Polygon",
	    	            coordinates: [
	    	                          dataJSON.coordinates[0],
	    	                          dataJSON.coordinates[1],
	    	                         ]
	    	       }
	    	    });
	    		myMap.geoObjects.add(myGeoObject);
	    		break;
        	}
        }
    },
};

SCWeb.core.ComponentManager.appendComponentInitialize(YandexMapLinkComponent);
