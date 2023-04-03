 SECONDS=$1
 INTERVALS=$2
 FOLDERPATH=$3

 for (( c=0; c<$INTERVALS; c++ ))
 do
    SUFFIX=".jpg"
    DATE=$(date '+%Y%m%d')
    FILENAME="$FOLDERPATH$DATE"_"$SECONDS"s""_"$INTERVALS"I""_"$c$SUFFIX"

    if [ ${#FOLDERPATH} = 0 ]
    then
        gphoto2 --set-config bulb=1 --wait-event=$1 --set-config bulb=0 --wait-event=1ms
    else
        gphoto2 --filename=$FILENAME --set-config bulb=1 --wait-event=$1 --set-config bulb=0 --wait-event-and-download=1s
    fi
done