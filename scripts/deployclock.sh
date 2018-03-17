clear
echo Deployment of clockstream started
sleep 5
echo Looking for a running stream
victim="$(pgrep -f clock_refactor.py)"
echo Killing process $victim
kill -9 $victim
sleep 5
cd Bedroom_Telemetry_Online_Shift
pwd
echo Updateing code from GitHub
git pull
echo Building Arduino code
platformio run --target upload
echo Restarting stream
nohup python3 clock_refactor.py > /dev/null 2> clockstream.err &
cd ..
pwd

