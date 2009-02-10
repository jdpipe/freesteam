#!/bin/bash
RSYNC_RSH=ssh rsync --delete --progress -Cvaz /home/john/freesteam-website/ jdpipe,freesteam@web.sourceforge.net:/home/groups/f/fr/freesteam/htdocs/

