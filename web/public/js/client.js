$(function () {

'use strict';

// server
var server = {
    // current server the user is in
    current: null,
    // server list
    list: [],

    // join a server (either via the tab or the join button)
    join: function (id) {

    },

    initEvents: function () {
        $(document).on('click', '#server-tabs a', function () {
            var id = $(this).data().id;
            server.join(id);
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

    // join a chat channel
    join: function (id) {

    },

    // send text to the current channel
    send: function (text) {

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

voice .initEvents();
chat  .initEvents();
server.initEvents();

});
