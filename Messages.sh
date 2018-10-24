#! /usr/bin/env bash

$XGETTEXT *.cpp -o $podir/slack_library.pot
rm -f rc.cpp
