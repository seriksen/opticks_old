#!/bin/bash 
##
## Copyright (c) 2019 Opticks Team. All Rights Reserved.
##
## This file is part of Opticks
## (see https://bitbucket.org/simoncblyth/opticks).
##
## Licensed under the Apache License, Version 2.0 (the "License"); 
## you may not use this file except in compliance with the License.  
## You may obtain a copy of the License at
##
##   http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software 
## distributed under the License is distributed on an "AS IS" BASIS, 
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
## See the License for the specific language governing permissions and 
## limitations under the License.
##


gogo-find(){ find . -name go.sh ; }
gogo-all(){
    local go
    gogo-find | while read go ; do 
        gogo-one $go
    done
}


gogo-one-()
{
    local rc
    ./go.sh > /dev/null 2>&1
    rc=$?
    printf "%-40s %d \n" $go $rc 
}

gogo-one(){
    local go=$1 
    local iwd=$PWD

    local dir=$(dirname $go)
    cd $dir 

    printf "%-40s \n" $go 

    if [ -f "SKIP" ]; then
        printf "%-40s %s %s \n" $go "SKIP" "$(head -1 SKIP)"
    else 
        gogo-one-  
    fi 

    cd $iwd
}

gogo-all


