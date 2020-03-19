#!/bin/sh

# when we use meson build we can't use something like /*.cpp
# for that reson we made this script to fetch files
for i in src/Core/*.cpp src/Renderer/*.cpp; do
  echo $i
done
