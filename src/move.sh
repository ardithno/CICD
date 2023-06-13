#!/bin/bash


sudo scp src/cat/s21_cat src/grep/s21_grep ws2@10.20.0.10:. 
echo sudo mv s21_cat s21_grep /usr/local/bin > move2.sh
sudo ssh ws2@10.20.0.10 'echo "12345" | sudo -Sv && bash -s' < move2.sh
sudo rm move2.sh