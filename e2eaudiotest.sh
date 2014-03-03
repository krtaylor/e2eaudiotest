# determine type of dev board and attempt configuration
if grep -q "Panda" /proc/cpuinfo ; then
   # we are testing a pandaboard
   echo "e2eaudiiotest: Pandaboard device configuration" 
   ./pandaboard.sh
else
   echo "e2eaudiotest: Unknown device, no configuration done"
fi

#play a sine wave at A 440 hz
speaker-test -t sine -f 440 -l 1 &

STPID=$!
trap "kill $STPID" TERM INT EXIT

# check the loopback frequency
if testfreq ; then
   # do the successful LAVA return 
   echo "e2eaudiotest: pass 440 sinewave detected"
else
   echo "e2eaudiotest: fail 440 sinewave not detected" 
fi
