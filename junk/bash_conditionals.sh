#!/bin/bash

iVar1=10
iVar2=20

# Example: Old style comparisions
if [ "$iVar1" -lt "$iVar2" ]; then
    echo "\$iVar1=$iVar1 is less than \$iVar2=$iVar2"
else
    echo "\$iVar1 is greater than or equal to \$iVar2"
fi

# Example: New style comparisions
if [[ "$iVar1" < "$iVar2" ]]
then
    echo "\$iVar1=$iVar1 is less than \$iVar2=$iVar2"
else
    echo "\$iVar1 is greater than or equal to \$iVar2"
fi
