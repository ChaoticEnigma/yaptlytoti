#!/bin/bash
if type node &>/dev/null; then
    echo Node found, running with node.
    node server.js
elif type nodejs &>/dev/null; then
    echo Nodejs found, running with nodejs.
    nodejs server.js
else
    echo Nodejs not detected! Did you install it?
fi
