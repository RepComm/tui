# export $COLUMNS
# export $LINES
LINES=$(tput lines)
COLUMNS=$(tput cols)

export LINES
export COLUMNS

./demo "$@"