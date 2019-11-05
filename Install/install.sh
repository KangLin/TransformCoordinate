#!/bin/bash

# Use to install appimage in linux
# $1: install or remove
# $2: run

case "$1" in
    remove)
        echo "remove ..."
        rm -f /usr/share/applications/TransformCoordinate.desktop
        rm -f ~/.config/autostart/TransformCoordinate.desktop
        rm -f /usr/share/pixmaps/TransformCoordinate.png
    ;;

    install|*)
        echo "install ..."
        # Install destop
        if [ -f /usr/share/applications/TransformCoordinate.desktop ]; then
            rm /usr/share/applications/TransformCoordinate.desktop
        fi
        ln -s `pwd`/share/applications/TransformCoordinate.desktop /usr/share/applications/TransformCoordinate.desktop

        # Install auto run on boot
        if [ ! -d ~/.config/autostart ]; then
            mkdir -p ~/.config/autostart
            chmod a+wr ~/.config/autostart
        fi
        if [ -f ~/.config/autostart/TransformCoordinate.desktop ]; then
            rm ~/.config/autostart/TransformCoordinate.desktop
        fi
        ln -s `pwd`/share/applications/TransformCoordinate.desktop ~/.config/autostart/TransformCoordinate.desktop

        # Reset exec to desktop
        sed -i "s/Exec=.*//g" `pwd`/share/applications/TransformCoordinate.desktop
        echo "Exec=`pwd`/TransformCoordinate-x86_64.AppImage" >> `pwd`/share/applications/TransformCoordinate.desktop

        # Install applications icon
        if [ -f /usr/share/pixmaps/TransformCoordinate.png ]; then
            rm /usr/share/pixmaps/TransformCoordinate.png
        fi
        if [ ! -d /usr/share/pixmaps ]; then
            mkdir -p /usr/share/pixmaps
        fi
        ln -s `pwd`/share/pixmaps/TransformCoordinate.png /usr/share/pixmaps/TransformCoordinate.png
        
        # Whether run after install
        if [ "$2" = "run" ]; then
            ./TransformCoordinate-x86_64.AppImage
        fi
        ;;
esac
