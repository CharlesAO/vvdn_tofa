
WORKING_DIR=/tmp

[[ ! -d "$WORKING_DIR" ]] && echo "mosim_port_common.sh: No /tmp directory found!"

MOVISIM_PORT_LOCK_FILE=$WORKING_DIR/get_movisim_port.lock
PORT_LOCK_BASENAME=movisim_port

