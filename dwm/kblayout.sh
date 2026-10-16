#!/bin/sh
current=$(setxkbmap -query | grep layout | awk '{print $2}')
if [ "$current" = "fr" ]; then
    setxkbmap ara
else
    setxkbmap fr
fi
