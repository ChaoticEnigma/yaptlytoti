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
        var html = '<li><a href="", data-id="v' + id + '">' + id + '</a></li>';
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
        var html = '<li><a href="", data-id="c' + id + '">' + id + '</a></li>';
        $('#channel-chat ul').append(html);
    },

    // send text to the current channel
    send: function (user, text) {
        var html = '<div class="media"><div class="media-left">IMG</div><div class="media-body"><h4 class="media-heading">' + user + '<small> - DATE</small></h4>' + text + '</div></div>';
        $('#chat-text-container').append(html);
        $('#chat-top').scrollTop($('#chat-text-wrap')[0].scrollHeight);
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



//
//
//
// wcDocker configuration
//
//
//

// default layout
var myDefault = '{"floating":[],"root":{"type":"wcSplitter","horizontal":true,"isDrawer":false,"pane0":{"type":"wcSplitter","horizontal":false,"isDrawer":false,"pane0":{"type":"wcFrame","floating":false,"isFocus":false,"tabOrientation":"top","pos":{"x":0.5,"y":0.5},"size":{},"tab":0,"panels":[{"type":"wcPanel","panelType":"Chat Channel","size":{"x":200,"y":-1},"customData":{}}]},"pane1":{"type":"wcSplitter","horizontal":false,"isDrawer":false,"pane0":{"type":"wcFrame","floating":false,"isFocus":false,"tabOrientation":"top","pos":{"x":0.5,"y":0.5},"size":{},"tab":0,"panels":[{"type":"wcPanel","panelType":"Voice Channel","size":{"x":-1,"y":-1},"customData":{}}]},"pane1":{"type":"wcFrame","floating":false,"isFocus":false,"tabOrientation":"top","pos":{"x":0.5,"y":0.5},"size":{},"tab":0,"panels":[{"type":"wcPanel","panelType":"User Panel","size":{"x":-1,"y":-1},"customData":{}}]},"pos":0.6907216494845361},"pos":0.38461538461538464},"pane1":{"type":"wcSplitter","horizontal":false,"isDrawer":false,"pane0":{"type":"wcFrame","floating":false,"isFocus":false,"tabOrientation":"top","pos":{"x":0.5,"y":0.5},"size":{},"tab":0,"panels":[{"type":"wcPanel","panelType":"Server Panel","size":{"x":-1,"y":120},"customData":{}}]},"pane1":{"type":"wcFrame","floating":false,"isFocus":false,"tabOrientation":"top","pos":{"x":0.5,"y":0.5},"size":{},"tab":0,"panels":[{"type":"wcPanel","panelType":"Chat Panel","size":{"x":-1,"y":-1},"customData":{}}]},"pos":0},"pos":0.20833333333333334}}';

// init docker
var myDocker = new wcDocker($('#container'), {
    themePath: 'css/Themes',
    theme: 'default',
    allowContextMenu: false
});

/*this.myDocker.registerPanelType('Top Panel', {
    isPrivate: true,

    onCreate: function(myPanel) {
        // Constrain the sizing of this window so the user can't resize it.
        myPanel.maxSize(Infinity, 50);
        myPanel.title(false);

        // Do not allow the user to move or remove this panel, this will remove the title bar completely from the frame.
        myPanel.moveable(false);
        myPanel.closeable(false);
        myPanel.scrollable(false, false);

        //$test = '<div id="test"></div>';
        myPanel.layout().addItem($('#menubar'));
    }
});*/

myDocker.registerPanelType('Chat Panel', {
    isPersistent: true,
    onCreate: function (myPanel) {
        myPanel.closeable(false);

        var item = '<div id="chat"><div id="channel-name-current" class="text-center"><p>Channel name</p></div><div id="chat-top"><div id="chat-text-wrap"><div id="chat-text-container"></div></div></div><div id="chat-bottom"><input id="chat-box-input" type="text" placeholder="Enter text for &lt;channel name&gt;"/></div></div>';

        myPanel.layout().addItem(item);
    }
});

myDocker.registerPanelType('Server Panel', {
    layout: wcDocker.LAYOUT_SIMPLE,
    isPersistent: true,
    onCreate: function (myPanel) {
        myPanel.closeable(false);
        myPanel.scrollable(false, false);
        myPanel.initSize(undefined, 120);
        myPanel.minSize(10, 120);

        var item = '<div id="server-header"><div id="server-top"><div id="server-left-pane"><div id="server-title"><p>Server title</p></div><div id="server-description"><p>Server description</p></div><div id="server-icon"></div></div><div id="server-right-pane"><button id="server-settings" title="Edit server settings" data-toggle="modal" data-target="#modal-server-settings"><i class="fa fa-fw fa-gear"></i></button><button id="server-info" title="View server information" data-toggle="modal" data-target="#modal-server-info"><i class="fa fa-fw fa-info-circle"></i></button><div id="server-icon">IMG</div></div></div><div id="server-bottom"><ul id="server-tabs" class="nav nav-tabs"></ul><button id="server-join-btn" title="Join a server" data-toggle="modal" data-target="#modal-server-join"><i class="fa fa-fw fa-plus"></i></button></div></div>';

        myPanel.layout().addItem(item);
    }
});

myDocker.registerPanelType('Chat Channel', {
    isPersistent: true,
    onCreate: function (myPanel) {
        var item = '<div id="channel-chat"><ul></ul></div>';
        myPanel.layout().addItem(item);

        // EVENT.CLOSED doesnt work?
        myPanel.on(wcDocker.EVENT.CLOSING, function () {
            $('#view-chat-channel').prop('checked', false);
            return true;
        });
    }
});

myDocker.registerPanelType('Voice Channel', {
    isPersistent: true,
    onCreate: function (myPanel) {
        var item = '<div id="channel-voice"><ul></ul></div>';
        myPanel.layout().addItem(item);

        // EVENT.CLOSED doesnt work?
        myPanel.on(wcDocker.EVENT.CLOSING, function () {
            $('#view-voice-channel').prop('checked', false);
            return true;
        });
    }
});

myDocker.registerPanelType('User Panel', {
    isPersistent: true,
    onCreate: function (myPanel) {
        myPanel.minSize(10, 120);

        var item = '<div id="pane-user"><div id="user-picture"><p>IMG</p></div><div id="user-name"><p>User Name</p></div><div id="pane-user-right"><button id="user-mute" title="Mute microphone"><i class="fa fa-fw fa-microphone"></i></button><button id="user-deaf" title="Mute speakers"><i class="fa fa-fw fa-volume-up"></i></button><button id="client-settings" title="Edit user settings" data-toggle="modal" data-target="#modal-user-settings"><i class="fa fa-fw fa-gear"></i></button></div></div>';

        myPanel.layout().addItem(item);

        var panelTypes = myDocker.panelTypes(false);

        // EVENT.CLOSED doesnt work?
        myPanel.on(wcDocker.EVENT.CLOSING, function () {
            $('#view-user-panel').prop('checked', false);
            return true;
        });
    }
});

//var topPanel = myDocker.addPanel('Top Panel', wcDocker.DOCK_TOP);

// *** uncomment this if changing the layout ***
//serverPanel = myDocker.addPanel('Server Panel', wcDocker.DOCK.RIGHT);
//chatPanel = myDocker.addPanel('Chat Panel', wcDocker.DOCK.BOTTOM, serverPanel);

//channelChatPanel = myDocker.addPanel('Chat Channel', wcDocker.DOCK.LEFT, null, {w: '200px'});
//channelVoicePanel = myDocker.addPanel('Voice Channel', wcDocker.DOCK.BOTTOM, channelChatPanel);
//userPanel = myDocker.addPanel('User Panel', wcDocker.DOCK.BOTTOM, channelVoicePanel);

// *** comment this if changing the layout ***
var layout = localStorage.getItem('yap-layout');
if (layout) {
    myDocker.restore(layout);
} else {
    myDocker.restore(myDefault);
}

myDocker.on(wcDocker.EVENT.LOADED, function () {
    console.log('loaded');

    setTimeout(function () {
        $('#chat-top').scrollTop($('#chat-text-wrap')[0].scrollHeight);
    }, 500);

    checkVisibility();
});

function checkVisibility() {
    var chatChannel = myDocker.findPanels('Chat Channel');
    var voiceChannel = myDocker.findPanels('Voice Channel');
    var userPanel = myDocker.findPanels('User Panel');

    if (chatChannel.length)
        $('#view-chat-channel').prop('checked', true);
    else
        $('#view-chat-channel').prop('checked', false);
    if (voiceChannel.length)
        $('#view-voice-channel').prop('checked', true);
    else
        $('#view-voice-channel').prop('checked', false);
    if (userPanel.length)
        $('#view-user-panel').prop('checked', true);
    else
        $('#view-user-panel').prop('checked', false);
}

$('#view-chat-channel').change(function () {
    var use = $(this).prop('checked');

    var chatChannel = myDocker.findPanels('Chat Channel');
    var voiceChannel = myDocker.findPanels('Voice Channel');
    var userPanel = myDocker.findPanels('User Panel');

    if (use && !chatChannel.length) {
        if (voiceChannel.length)
            myDocker.addPanel('Chat Channel', wcDocker.DOCK.TOP, voiceChannel[0]);
        else if (userPanel.length)
            myDocker.addPanel('Chat Channel', wcDocker.DOCK.TOP, userPanel[0]);
        else
            myDocker.addPanel('Chat Channel', wcDocker.DOCK.LEFT, null, {w: '200px'});

        // TODO if hidden from layout, it will not have dynamic content when shown the first time. Load panels respective content when shown.
    } else if (!use && chatChannel.length) {
        chatChannel[0].close();
    }
});

$('#view-voice-channel').change(function () {
    var use = $(this).prop('checked');

    var chatChannel = myDocker.findPanels('Chat Channel');
    var voiceChannel = myDocker.findPanels('Voice Channel');
    var userPanel = myDocker.findPanels('User Panel');

    if (use && !voiceChannel.length) {
        if (chatChannel.length)
            myDocker.addPanel('Voice Channel', wcDocker.DOCK.BOTTOM, chatChannel[0]);
        else if (userPanel.length)
            myDocker.addPanel('Voice Channel', wcDocker.DOCK.TOP, userPanel[0]);
        else
            myDocker.addPanel('Voice Channel', wcDocker.DOCK.LEFT, null, {w: '200px'});

        // TODO if hidden from layout, it will not have dynamic content when shown the first time. Load panels respective content when shown.
    } else if (!use && voiceChannel.length) {
        voiceChannel[0].close();
    }
});

$('#view-user-panel').change(function () {
    var use = $(this).prop('checked');

    var chatChannel = myDocker.findPanels('Chat Channel');
    var voiceChannel = myDocker.findPanels('Voice Channel');
    var userPanel = myDocker.findPanels('User Panel');

    if (use && !userPanel.length) {
        if (voiceChannel.length)
            myDocker.addPanel('User Panel', wcDocker.DOCK.BOTTOM, voiceChannel[0]);
        else if (chatChannel.length)
            myDocker.addPanel('User Panel', wcDocker.DOCK.BOTTOM, chatChannel[0]);
        else
            myDocker.addPanel('User Panel', wcDocker.DOCK.LEFT, null, {w: '200px'});

        // TODO if hidden from layout, it will not have dynamic content when shown the first time. Load panels respective content when shown.
    } else if (!use && userPanel.length) {
        userPanel[0].close();
    }
});

$('#reset-layout').click(function () {
    myDocker.restore(myDefault);
    localStorage.removeItem('yap-layout');

    voice .initEvents();
    chat  .initEvents();
    server.initEvents();
    tests();

    checkVisibility();
});

$('#save-layout').click(function () {
    var saved = myDocker.save();
    //console.log(saved);
    localStorage.setItem('yap-layout', saved);
});

// too many events
//this.userPanel.on(wcDocker.EVENT.UPDATED, function () {
    //console.log(myDocker.save());
//});

// TODO report wcDocker "maybe" bugs or alternatives?
// panel.isVisible doesn't take in account if the panel is / turned into a tab.
// adding a pannel to a panel that turned into a "side" tab, creates a normal panel inside the side tab bar, instead of making it part of the side tab bar.
// wcCreatePanel should have a way to check for persistence?


//
//
//
// initialize everything and tests
//
//
//

voice .initEvents();
chat  .initEvents();
server.initEvents();

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
