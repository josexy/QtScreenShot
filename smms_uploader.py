# -*- encoding: utf-8 -*-
import requests
import os.path
import json
import sys

def uploadImage(smms_auth,filename,proxies_s):
    
    headers = {
        "Authorization": smms_auth
    }
    proxies=None
    if proxies_s is None:
        proxies=None
    else:
        proxies={
            'http':proxies_s,
            'https':proxies_s
        }   
    files={'smfile':(os.path.basename(filename),open(filename,'rb'))}

    with requests.session().request("POST", "https://sm.ms/api/v2/upload",
                                    headers=headers,proxies=proxies,
                                    files=files) as resp:
        print(resp.text)

def main(smms_auth,filename,proxies_s):
    uploadImage(smms_auth,filename,proxies_s)

if __name__ == '__main__':
    if len(sys.argv) <=2: sys.exit(0)
    if len(sys.argv) == 4:
        main(sys.argv[1],sys.argv[2],sys.argv[3])
    else:
        main(sys.argv[1],sys.argv[2],None)

