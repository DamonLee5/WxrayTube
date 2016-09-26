#!/bin/bash

for((i = 0; i < 10; ++i)) #launch the simulations in different terminals
do
    #xterm -e "cd ..; . ./setupg4101.sh; cd WxrayTube; source ~/.zshrc; WxrayTube simulate.mac" &
    WxrayTube simulate.mac &
done

for job in `jobs -p` # wait for the jobs to terminate, jobs -p lists the pid of the children
do
    echo $job
    wait $job # wait for a job with a specific pid to terminate
done

exit 0
