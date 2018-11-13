#!/bin/sh
### BEGIN INIT INFO
# Provides:
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start daemon at boot time
# Description:       Enable service provided by daemon.
### END INIT INFO

dir=""
cmd=""
user=""

name=`basename $0`
stdout_log="../log/init.log"
timestamp=$(date)

case "$1" in
    start)
        echo "Starting $name"
        sleep 0.8
        echo "Started $name"
        echo "$timestamp Starting $name" >> $stdout_log
    ;;
    stop)
        echo "Stopping $name.."
        echo "$timestamp Stoping $name" >> $stdout_log
    ;;
    restart)
        $0 stop
        $0 start
    ;;
    status)
        echo "Running"
        echo "$timestamp Status $name" >> $stdout_log
    ;;
    *)
        echo "Usage: $0 {start|stop|restart|status}"
        exit 1
    ;;
esac

exit 0

