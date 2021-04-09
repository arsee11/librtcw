import os

WEBRTC_HOME="${HOME}/libs/webrtc"
CUR_HOME="./webrtc"

file = open("webrtc_files.txt", "tr")
for line in file.readlines():
    line = line.strip()
    splits = os.path.split(line)
    path = splits[0]
    dst_path=CUR_HOME+"/"+path
    if not os.path.exists(dst_path) :
        print("mkdir "+dst_path)
        os.makedirs(dst_path)
        
    cmd = "cp "+WEBRTC_HOME+"/"+line +" "+dst_path
    print(cmd)
    os.system(cmd)

cmd = "cp -r "+WEBRTC_HOME+"/p2p/client/* " +CUR_HOME+"/p2p/client/"
print(cmd)
os.system(cmd)
cmd = "cp -r "+WEBRTC_HOME+"/p2p/base/* " +CUR_HOME+"/p2p/base/"
print(cmd)
os.system(cmd)
cmd = "cp -r "+WEBRTC_HOME+"/p2p/stunprober/* " +CUR_HOME+"/p2p/stunprober/"
print(cmd)
os.system(cmd)
