#! /bin/bash

sudo cp 98-mcci.rules /etc/udev/rules.d/
sudo chmod +x /etc/udev/rules.d/98-mcci.rules
sudo udevadm control --reload
