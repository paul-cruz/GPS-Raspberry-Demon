HOST=$1

if [[ -z $HOST ]]; then
  HOST="rpi-paul"
fi

echo "Compiling app..."

make

echo "Adding app to PATH..."

scp main $HOST:~/gps
ssh $HOST sudo mv gps /usr/local/bin/gps

echo "Adding service to Systemd"

scp gps.service $HOST:~/gps.service
ssh $HOST sudo mv gps.service /etc/systemd/system/gps.service
ssh $HOST sudo chmod -x /etc/systemd/system/gps.service


echo "Enabiling GPS service..."

ssh $HOST sudo systemctl start gps.service
ssh $HOST sudo systemctl enable gps.service
