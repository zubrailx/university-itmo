#!/bin/zsh

s=’t??’; if [[ $s = +(tom|cat|tomcat) ]]; then
  echo "SHIT"
  print $?; 
fi

echo $?
