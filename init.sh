#!/bin/bash

version=ics2018

log=""

function init() {
  if [ -d $1 ]; then
    echo "$1 is already initialized, exiting..."
    return
  fi

  while [ ! -d $1 ]; do
    proxychains4 -q git clone -b $version https://github.com/NJU-ProjectN/$1.git
  done
  log="$log$1 `cd $1 && git log --oneline --no-abbrev-commit -n1`"$'\n'
  rm -rf $1/.git

  if [ $2 ] ; then
    sed -i -e "/^export $2=.*/d" ~/.bashrc
    echo "export $2=`readlink -e $1`" >> ~/.bashrc
  fi
}

read -r -p "Are you sure to initialize everything? [y/n] " input
case $input in
  [yY])
    init nemu NEMU_HOME
    init nexus-am AM_HOME
    init nanos-lite
    init navy-apps NAVY_HOME
    source ~/.bashrc

    git add -A
    git commit -am "$version initialized"$'\n\n'"$log"

    echo "Initialization finishes!"
    echo "By default this script will add enviornment variables into ~/.bashrc."
    echo "If you use shell other than bash, please add these enviornment variables manually."
    ;;

  [nN])
    ;;

  *)
    echo "Invalid input..."
    ;;
esac
