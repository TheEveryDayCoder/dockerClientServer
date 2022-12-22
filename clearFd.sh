for fd in $(ls /proc/$$/fd); do
  eval "exec $fd>&-"
done
