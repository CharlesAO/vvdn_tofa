#!/usr/bin/env bash

# user can override default port start and default port range using the
# environment variables:
#
# MOVISIM_PORT_RANGE_START
#  - this variable should indicate the starting port, such as 40000
#
# MOVISIM_PORT_RANGE_COUNT
#   - this variable should indicate the max port count we should use on
#   the Jenkins server. The build system might launch as many parallel
#   moviSim instances as MOVISIM_PORT_RANGE_COUNT value
#
if [[ -z ${MOVISIM_PORT_RANGE_START+x} ]] ; then
  PORT_RANGE_START=44000
else
  PORT_RANGE_START=$MOVISIM_PORT_RANGE_START
fi

if [[ -z ${MOVISIM_PORT_RANGE_COUNT+x} ]] ; then
  PORT_RANGE_COUNT=40
else
  PORT_RANGE_COUNT=$MOVISIM_PORT_RANGE_COUNT
fi
RETRY_DELAY=1

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $SCRIPT_DIR/movisim_port_common.sh

PORT=""
while [[ -z "$PORT" ]] ;
do
  PORT=$( (
      flock -s 200
      for i in $(seq 0 1 $PORT_RANGE_COUNT)
      do
        PORT=$(( $PORT_RANGE_START + $i))
        PORT_LOCK_FILE=$WORKING_DIR/${PORT_LOCK_BASENAME}_$PORT.lock
        if [[ ! -f $PORT_LOCK_FILE ]] ; then
          touch $PORT_LOCK_FILE
          echo $PORT
          exit 0
        fi
      done
      ) 200>"$MOVISIM_PORT_LOCK_FILE" )

  if [[ -z "$PORT" ]] ; then
    # once arrived here, we no longer have available ports
    sleep $RETRY_DELAY
  fi
done
echo $PORT

