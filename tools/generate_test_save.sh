#!/bin/bash
cd data/test_files/vic3_world/world/
rm test_save.vic3
zip test_save.vic3 gamestate -v
sed -i '1s/^/SAV01020000000000000000\n/' test_save.vic3