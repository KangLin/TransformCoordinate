#!/bin/bash

EXIT_CODE=0
PROJECT_NAME="TransformCoordinate"

if [ -n "$1" ]; then
    echo "$PROJECT_NAME"
	PROJECT_NAME=$1
fi

if [ ! -f /opt/${PROJECT_NAME}/share/applications/io.github.KangLin.${PROJECT_NAME}.desktop ]; then
	echo "There are not /opt/share/applications/io.github.KangLin.${PROJECT_NAME}.desktop"
	EXIT_CODE=$[EXIT_CODE+1]
fi

if [ ! -f /usr/share/applications/io.github.KangLin.${PROJECT_NAME}.desktop ]; then
	echo "There are not /usr/share/applications/io.github.KangLin.${PROJECT_NAME}.desktop"
	EXIT_CODE=$[EXIT_CODE+1]
fi

if [ ! -f /opt/${PROJECT_NAME}/share/pixmaps/io.github.KangLin.${PROJECT_NAME}.png ]; then
        echo "There are not /opt/${PROJECT_NAME}/share/pixmaps/io.github.KangLin.${PROJECT_NAME}.png"
	EXIT_CODE=$[EXIT_CODE+1]
fi

if [ ! -f /usr/share/pixmaps/io.github.KangLin.${PROJECT_NAME}.png ]; then
    echo "There are not /usr/share/pixmaps/io.github.KangLin.${PROJECT_NAME}.png"
    EXIT_CODE=$[EXIT_CODE+1]
fi


exit $EXIT_CODE
