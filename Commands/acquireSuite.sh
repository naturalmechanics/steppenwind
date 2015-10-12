#!/usr/bin/bash

    # launch bachata
    /projects/Bachata/current/bachata.gambas &

    # launch xfce-terminal
    xfce4-terminal &

    # launch file manager
    xfce4-terminal -x tmux -c mc &
    spacefm &