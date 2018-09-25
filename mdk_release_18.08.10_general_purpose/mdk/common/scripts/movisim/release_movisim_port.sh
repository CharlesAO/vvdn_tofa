#!/bin/env bash

PORT=$1

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $SCRIPT_DIR/movisim_port_common.sh

PORT_LOCK_FILE=$WORKING_DIR/${PORT_LOCK_BASENAME}_$PORT.lock

(
  flock -s 201
  [[ -f $PORT_LOCK_FILE ]] && rm $PORT_LOCK_FILE
) 201>"$MOVISIM_PORT_LOCK_FILE"

