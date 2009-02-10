#!/bin/bash
RSYNC_RSH=ssh rsync --delete --progress -Cvaz /home/john/frame3dd-website/ jdpipe,freesteam@web.sourceforge.net:/home/groups/f/fr/freesteam/htdocs/

