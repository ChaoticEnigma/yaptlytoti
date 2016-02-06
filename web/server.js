var express = require('express'),
    app     = require('express')(),
    http    = require('http').Server(app),
    io      = require('socket.io')(http),
    less    = require('less-middleware'),
    fs      = require('fs'),

    // package.json;
    pack;

// less config
app.use(less(__dirname + '/public'));
app.use(express.static(__dirname + '/public'));
// serve static files here
app.use(express.static('public'));
// use jade with express
app.set('views', 'views');
app.set('view engine', 'jade');
// used mostly for debuging
app.locals.pretty = true;

var config = {
    port: 8080
};

fs.readFile('package.json', function (err, data) {
    if (err) return console.log(err);
    pack = JSON.parse(data);
});

// main pages
app.get('/', function (req, res) {
    res.render('index', {
        pack: pack,
    });
});

// 404 requests
app.use(function (req, res, next) {
    res.status(404);

    // respond with html page
    if (req.accepts('html')) {
        res.render('404', { url: req.url });
        console.log('Page 404 by ' + req.connection.remoteAddress +
            ' on ' + req.originalUrl);
        return;
    }

    // respond with json
    if (req.accepts('json')) {
        res.send({ error: 'Not found' });
        return;
    }

    // default to plain-text. send()
    res.type('txt').send('Not found');
});

// io is for everyone, socket is for the single client
io.on('connection', function (socket) {
});

http.listen(config.port, function () {
    console.log('Listening on *:' + config.port);
});

// error handling
http.on('error', function (err) {
    if (err.code == 'EADDRINUSE') {
        console.error('Web server port already in use! ' +
            'Edit config.cfg to change the port.');
        process.exit(2);
    } else {
        console.error(err);
        process.exit(1);
    }
});
