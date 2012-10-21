var Observations = new Meteor.Collection("observations");

// For DDP, use: 
// ./ddp-client.py localhost:3000
// >>> call createObservation [{"windSpeed": 10.2, "windDirection": "NW", "temp": 34}]

Meteor.methods({
  createObservation: function(o) {
    console.log("Creating observation:" + JSON.stringify(o))
    o.date = new Date();
    return Observations.insert(o);
  }
});

if (Meteor.isClient) {
  Template.body.helpers({
    latestObservation: function () {
      return Observations.findOne({}, {sort: {date: -1}});
    }
  });

  Template.observations.helpers({
    observations: function () {
      var os = Observations.find({}, {sort: {date: -1}});
      return os.fetch().slice(0, 10);
      // return [Observations.findOne({}, {sort: {date: -1}})];
    }
  });
  
  Template.observation.helpers({
    formattedDate: function (d) {
      //return moment(d).fromNow();
      return moment(d).format('h:mm:ss a MMM Do');
    }
  });
}

if (Meteor.isServer) {
  var URL = __meteor_bootstrap__.require("url");
  
  // setup a request handler to serve primitive GET requests
  __meteor_bootstrap__.app
      .use(function(req, res, next) {
        Fiber(function () {
          console.log("Hit middleware, url:" + req.url);
          var parsed = URL.parse(req.url, true);
          // console.log("Hit middleware, url:" + req.url);
          
          if (parsed.pathname == '/create_observation') {
            Meteor.call('createObservation', {
              'windSpeed': parsed.query.windSpeed,
              'windDirection': parsed.query.windDirection,
              'temp': parsed.query.temp
            });
            
            res.writeHead(200, {'Content-Type': 'text/html'});
            res.write('OK');
            res.end();
            
            return;
          }
          
          next();
          return;
        }).run();
      });
      
  Meteor.startup(function () {
      Meteor.publish("observations");
  });
}
