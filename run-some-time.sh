#!/bin/bash -eu

P=$(basename $1)
cd "$(dirname $0)"

if [ "$2" == "" ]; then
    echo "Usage: $0 <polyfile> <seconds>"
    exit 1
fi

checkpoint() {
    echo "$(date -Ins) Checkpointing $P to $P-snapshot..."
    mkdir -p $P-snapshot
    sudo criu dump -t $1 -D $P-snapshot -vvv -o dump.log
    echo "$(date -Ins) Checkpointing $P to $P-snapshot...done"
}

restore() {
    echo "$(date -Ins) Restoring $1..."
    sudo criu restore --restore-sibling --restore-detach -D $1 -vvv -o restore.log
    EXIT_CODE=$?
    if [ $EXIT_CODE == 0 ]; then
        PID=$(ps --no-headers -C FGb -o pid= | tr -d \ )
        echo "$(date -Ins) Restore from snapshot $1 successfull. Running with pid $PID."
    else
        echo "$(date -Ins) Restore from snapshot $1 failed with error code $EXIT_CODE."
        exit 1
    fi
}

ELAPSED=0
MIN_AVAIL=1073741824 # 1 GB
if [ -e "$P-snapshot" ]; then
    restore "$P-snapshot"
    rm -rf "$P-snapshot-old"
    mv "$P-snapshot" "$P-snapshot-old"
else
    echo "$(date -Ins) Starting FGb on $1"
    ./FGb-logged $1 &
    PID=$!
    #PID=$(ps --no-headers -C FGb -o pid= | tr -d\ )
fi

AVAIL=$(free -b | head -n 2 | tail -n 1 | rev | cut -d\  -f 1 | rev)
while ps $PID >/dev/null && [ $AVAIL -gt $MIN_AVAIL ] && [ $ELAPSED -lt $2 ]; do
    sleep 1
    let ELAPSED+=1
done
if ps $PID >/dev/null; then
    checkpoint $PID
fi

exit 0

restore() {
    cp $1/$P.log .
    cp $1/$P.out .
    echo "$(date -Ins) Resuming from snapshot $1" | tee -a $P.log
    SNAPSHOT=$(echo $1 | cut -d- -f2)
    let SNAPSHOT+=1
    sudo criu restore -j --restore-sibling --restore-detach --pidfile -D ./$1 -vvv -o restore.log
    EXIT_CODE=$?
    if [ $EXIT_CODE == 0 ]; then
        PID=$(ps --no-headers -C FGb -o pid= | tr -d \ )
        echo "$(date -Ins) Resume from snapshot $LATEST_SNAPSHOT successfull. Running with pid $PID." | tee -a $P.log
    else
        echo "$(date -Ins) Resume from snapshot $LATEST_SNAPSHOT failed with error code $EXIT_CODE." | tee -a $P.log
        exit 1
    fi
}

LATEST_SNAPSHOT=$(ls -d $P-* 2>/dev/null | sort -g -t- -k2 | tail -n 1)

if [ "$LATEST_SNAPSHOT" != "" ]; then
    restore $LATEST_SNAPSHOT
else
    echo "$(date -Ins) Starting FGb for $P" | tee $P.log
    SNAPSHOT=0
    LD_PRELOAD=./intercept.so ./FGb poly/$P < /dev/null &> $P.out &
    sleep 1
    PID=$(ps --no-headers -C FGb -o pid= | tr -d \ )
    echo "$(date -Ins) Started FGb for $P with pid $PID" | tee $P.log
    echo $PID > $P.pid
fi

ELAPSED=1
SLEEP=10
MIN_FREE=1073741824 # 1 GB
MAX_TIME=10

checkpoint() {
    echo "$(date -Ins) Starting snapshot $SNAPSHOT..." | tee -a $P.log
    mkdir $P-$SNAPSHOT
    sudo criu dump -j -t $PID -D $P-$SNAPSHOT -vvv -o dump.log $*
    EXIT_CODE=$?
    if [ $EXIT_CODE == "0" ]; then
        echo "$(date -Ins) Snapshot $P-$SNAPSHOT... completed successfully" | tee -a $P.log
        cp $P.log $P-$SNAPSHOT
        cp $P.out $P-$SNAPSHOT
    else
        echo "$(date -Ins) Snapshot $P-$SNAPSHOT... failed with exit code $EXIT_CODE" | tee -a $P.log
        rm -rf $P-$SNAPSHOT
    fi
    let SNAPSHOT+=1
    ELAPSED=0
}

while PS=$(ps u --no-headers $PID)
do
    echo "$(date -Ins) $PS" >> $P.log
    AVAIL=$(free -b | head -n 2 | tail -n 1 | rev | cut -d\  -f 1 | rev)
    if [ $ELAPSED -gt $MAX_TIME ]; then
        checkpoint
        LATEST_SNAPSHOT=$(ls -d $P-* 2>/dev/null | sort -g -t- -k2 | tail -n 1)
        restore $LATEST_SNAPSHOT
    elif [ "$AVAIL" -lt "$MIN_FREE" ]; then
        checkpoint
        echo "$(date -Ins) Out of memory: only $AVAIL bytes available" | tee -a $P.log
        exit 0
    fi
    sleep $SLEEP
    let ELAPSED+=$SLEEP
done
