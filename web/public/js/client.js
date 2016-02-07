$(function () {

'use strict';

// server
var server = {
    // current server the user is in
    current: null,
    // server list
    list: [],

    // join a server (either via the tab or the join button)
    join: function (id, password) {

    },

    // add a server to the list
    add: function (id) {
        // temp
        this.current = 0;
        var html = '';

        if (id == this.current) {
            html += '<li class="active">';
        } else {
            html += '<li>';
        }

        html += '<a href="", data-id="s' + id + '">Server ' + id + '</a></li>';
        $('#server-tabs').append(html);
    },

    initEvents: function () {
        $(document).on('click', '#server-tabs a', function () {
            var id = $(this).data().id;
            server.join(id);
        });

        $(document).on('click', '#server-confirm', function () {
            var id = $('#server-id').val();
            var password = $('#server-password').val();
            server.join(id, password);
        });
    }
};

// voice channel
var voice = {
    // the current voice channel
    current: null,

    // join a voice channel
    join: function (id) {

    },

    // add a voice channel to the list
    add: function (id) {
        var html = '<li><a href="", data-id="v' + id + '">Voice Channel ' + id + '</a></li>';
        $('#channel-voice ul').append(html);
    },

    initEvents: function () {
        $(document).on('click', '#channel-voice a', function () {
            var id = $(this).data().id;
            voice.join(id);
        });
    }
};

// chat channel
var chat = {
    // the current chat channel the user is in
    current: null,
    // max items in the chat (before loading old history)
    max: 100,

    // join a chat channel
    join: function (id) {

    },

    // add a chat channel to the list
    add: function (id) {
        var html = '<li><a href="", data-id="c' + id + '">Chat Channel ' + id + '</a></li>';
        $('#channel-chat ul').append(html);
    },

    // send text to the current channel
    send: function (user, text) {
        var html = '<div class="media"><div class="media-left">IMG</div><div class="media-body"><h4 class="media-heading">' + user + '<small> - DATE</small></h4>' + text + '</div></div>';
        $('#chat-text-container').append(html);
        //$('#chat-top').scrollTop($('#chat-top')[0].scrollHeight);
    },

    initEvents: function () {
        // detect enter key
        $('#chat-box-input').keyup(function (e) {
            if (e.keyCode == 13) {
                var searchVal = $('#search-browser').val();
            }
        });

        $(document).on('click', '#channel-chat a', function () {
            var id = $(this).data().id;
            chat.join(id);
        });
    }
};

// the current user
var user = {
    name: null,
    mutedMicrophone: false,
    mutedSpeaker: false,

    // toggle a microphone mute
    muteMicrophone: function () {

    },

    // toggle a speaker mute
    muteSpeaker: function () {

    }
};

// users in the server
var users = {
    list: [],

    message: function (user, text) {
        chat.join(user);
        chat.message(text);
    }
};

// this may be changed in the future
var dragPanes = {
    init: function () {
        $('.split-pane').splitPane();

        var leftPane = localStorage.getItem('yap-left-pane-size');
        if (leftPane !== null) {
            $('#container').splitPane('firstComponentSize', leftPane);
        }

        var channelPane = localStorage.getItem('yap-channel-pane-size');
        if (channelPane !== null) {
            // +15 to account for padding
            $('#channels').splitPane('firstComponentSize', parseInt(channelPane) + 15);
        }

        $('.split-pane').on('dividerdragstart', function (event, obj) {
            // a bug with ff makes it select all the text the last pane
            $('body').addClass('no-select');
        });

        $('.split-pane').on('dividerdragend', function (event, obj) {
            $('body').removeClass('no-select');
        });

        $('#container').on('dividerdragend', function (event, obj) {
            if (event.target === this) {
                localStorage.setItem('yap-left-pane-size', obj.firstComponentSize);
            }
        });

        $('#channels').on('dividerdragend', function (event, obj) {
            if (event.target === this) {
                console.log(obj);
                localStorage.setItem('yap-channel-pane-size', obj.firstComponentSize);
            }
        });
    }
};

voice .initEvents();
chat  .initEvents();
server.initEvents();

dragPanes.init();

// used for testing. May be removed in the future.
function tests () {
    var i = 0;

    for (i; i < 10; i++)
        voice.add('voice channel ' + i);

    i = 0;
    for (i; i < 10; i++)
        chat.add('chat channel ' + i);

    i = 0;
    for (i; i < 10; i++)
        chat.send('user ' + i, 'The users message that they would be sending ' + i);

    i = 0;
    for (i; i < 3; i++)
        server.add(i);
}

tests();

});
