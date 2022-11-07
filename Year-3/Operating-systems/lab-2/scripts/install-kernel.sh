#!/bin/bash

sudo make O=build ARCH=x86 modules_install
sudo make O=build ARCH=x86 install
