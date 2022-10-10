cd build
make install
cd ..
cp ./etc-init.d-dtrs_trans.txt  /etc/init.d/dtrs_trans
chmod 777 /etc/init.d/dtrs_trans
systemctl enable dtrs_trans
service dtrs_trans start