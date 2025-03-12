const maxApi = require ("max-api");

maxApi.outlet("hello world");

maxApi.addHandler("test", () => {
	maxApi.outlet("yest nice test good job")
}
);

const weather = require('openweather-apis');

	weather.setLang('en');
	// English - en, Russian - ru, Italian - it, Spanish - es (or sp),
	// Ukrainian - uk (or ua), German - de, Portuguese - pt,Romanian - ro,
	// Polish - pl, Finnish - fi, Dutch - nl, French - fr, Bulgarian - bg,
	// Swedish - sv (or se), Chinese Tra - zh_tw, Chinese Sim - zh (or zh_cn),
	// Turkish - tr, Croatian - hr, Catalan - ca

	// set city by name
	//weather.setCity('Fairplay')
 	// or set the coordinates (latitude,longitude)
  var coords = [52.51489900054654, 13.426344168414456];
  weather.setCoordinate(coords[0], coords [1]);
	// or set city by ID (recommended by OpenWeatherMap)
	//weather.setCityId(4367872)

  // or set zip code
	//weather.setZipCode(15201);

	// 'metric'  'internal'  'imperial'
 	weather.setUnits('metric');

	// check http://openweathermap.org/appid#get for get the APPID
 	weather.setAPPID("227014702f79e028a632b444a615dbe4");


// get all the JSON file returned from server (rich of info)
//weather.getAllWeather(function(err, JSONObj){
//maxApi.outlet(JSONObj);
//});

maxApi.addHandler("getWeather", () => {
	weather.setCoordinate(coords[0], coords [1]);
  weather.getAllWeather(function(err, JSONObj){
  	maxApi.outlet(JSONObj);
  });
}
);

maxApi.addHandler("setCoords", (lat, lon) => {
  coords [0] = lat;
  coords [1] = lon;
}
);
