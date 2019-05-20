#!/bin/bash

cat `grep -rl SekInit src/burn/drv/ | sort | uniq` | awk 'c&&!--c;/struct BurnDriver/{c=2}' | sort
