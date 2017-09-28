#./condor_scripts/condor_test.py 100 "./main -c ./configs/config_YH.txt -d /data/users/fengyb/80Xresult/histos_QCD_YH -l v3 -s QCDHT700To1000_YH -n 10"

#./condor_scripts/condor_test.py 200 "./main -c ./configs/config_YH.txt -d /data/users/fengyb/80Xresult/histos_QCD_YH -l v3 -s QCDHT700To1000x_YH -n 10"

./condor_scripts/condor_test.py 100 "./main -c ./configs/config_YH.txt -d /data/users/fengyb/80Xresult/histos_QCD_YH -l v3 -s QCDHT1000To1500_YH -n 10"

./condor_scripts/condor_test.py 150 "./main -c ./configs/config_YH.txt -d /data/users/fengyb/80Xresult/histos_QCD_YH -l v3 -s QCDHT1000To1500x_YH -n 10"

./condor_scripts/condor_test.py 100 "./main -c ./configs/config_YH.txt -d /data/users/fengyb/80Xresult/histos_QCD_YH -l v3 -s QCDHT1500To2000_YH -n 10"

./condor_scripts/condor_test.py 150 "./main -c ./configs/config_YH.txt -d /data/users/fengyb/80Xresult/histos_QCD_YH -l v3 -s QCDHT1500To2000x_YH -n 10"

./condor_scripts/condor_test.py 100 "./main -c ./configs/config_YH.txt -d /data/users/fengyb/80Xresult/histos_QCD_YH -l v3 -s QCDHT2000ToInf_YH -n 10"

./condor_scripts/condor_test.py 150 "./main -c ./configs/config_YH.txt -d /data/users/fengyb/80Xresult/histos_QCD_YH -l v3 -s QCDHT2000ToInfx_YH -n 10"
