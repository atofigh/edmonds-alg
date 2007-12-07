#!/bin/sh
PATH=$1:$PATH
export PATH
sh -c "`cat $2`"
