Xvfb :99 -screen 0, 1024x768x16 2>/dev/null &
THE_PID=$!
DISPLAY=:99 wxformbuilder -g $1 2>/dev/null
mv wxfb/wxfb_frame.h $2
mv wxfb/wxfb_frame.cpp $3
kill -15 $THE_PID
rm /tmp/.X99-lock