Xvfb :99 -screen 0, 1024x768x16 2>/dev/null &
THE_PID=$!
DISPLAY=:99 wxformbuilder -g $1 2>/dev/null
mv wxfb/wxfb_frame.h $2
mv wxfb/wxfb_frame.cpp $3
sed -i '10i #include <wx/docview.h>' $2
sed -i -e 's/wxFrame( /wxDocParentFrame( wxDocManager* manager, /g' $2
sed -i -e 's/wxFrame( /wxDocParentFrame( manager, /g' $3
sed -i -e 's/wxFrame/wxDocParentFrame/g' $2
sed -i -e 's/wxFrame/wxDocParentFrame/g' $3
sed -i -e 's/wxWindow\*/wxFrame\*/g' $2
sed -i -e 's/wxWindow\*/wxFrame\*/g' $3
sed -i -e 's/WxfbFrame( /WxfbFrame( wxDocManager* manager, /g' $2
sed -i -e 's/WxfbFrame( /WxfbFrame( wxDocManager* manager, /g' $3
kill -15 $THE_PID
rm /tmp/.X99-lock