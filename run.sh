
# restore=$(echo "stty -g $(stty -g)")
stty -cbreak

echo Running program
./demo "$@"

stty -cooked
