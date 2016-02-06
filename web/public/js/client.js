$(function () {

"use strict";

var chat = {

    initEvents: function () {
        // detect enter key
        $('#chat-box-input').keyup(function (e) {
            if (e.keyCode == 13) {
                var searchVal = $('#search-browser').val();
            }
        });
    }
};

chat.initEvents();

});
