#!/bin/bash
sudo grep -oE '(([0][0-8]|[1-2][8-9]):[0-5][0-9]:[0-5][0-9]+.*$)' /var/log/auth.log