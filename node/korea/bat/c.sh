
#!/bin/bash
# docker run --rm -v /Users/honggwangseon/wm-sdk:/home/wirepas/wm-sdk -w /home/wirepas/wm-sdk -ti wirepas/sdk-builder make app_name=rack target_board=rack
# docker run --rm -v /Users/honggwangseon/wm-sdk:/home/wirepas/wm-sdk -w /home/wirepas/wm-sdk -ti wirepas/sdk-builder make app_name=node target_board=node
docker run --rm -v /Users/honggwangseon/wm-sdk:/home/wirepas/wm-sdk -w /home/wirepas/wm-sdk -ti wirepas/sdk-builder make app_name=ssd1306 target_board=ssd1306
# docker run --rm -v /Users/honggwangseon/wm-sdk:/home/wirepas/wm-sdk -w /home/wirepas/wm-sdk -ti wirepas/sdk-builder make app_name=tokai target_board=pca10040
# chmod +x compile.sh
# ./compile.sh or sudo ./compile.sh
