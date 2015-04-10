# -*- coding: cp936 -*-
import os 

_path_in_nutshell = ''
_path_in_version = ''

def copy_res(name_in_nutshell, name_in_version):    
    global _path_in_nutshell
    global _path_in_version
    source_dir = _path_in_nutshell + name_in_nutshell
    target_dir = _path_in_version + name_in_version
    print('copy from %s to %s' % (source_dir, target_dir))
    if not os.path.exists(target_dir):  
        os.makedirs(target_dir)  
    for f in os.listdir(source_dir):
        source_file = os.path.join(source_dir, f)
        target_file = os.path.join(target_dir, f)
        print(source_file)
        if os.path.isfile(source_file):
            open(target_file, "wb").write(open(source_file, "rb").read())

def main():
    nutshell_path = 'G:\\Boids\\'
    
    global _path_in_nutshell
    global _path_in_version
    _path_in_nutshell = nutshell_path + '\\Art\\characters\\'
    _path_in_version = '../res/characters/'
    
    copy_res('狗头人','kobold')
    copy_res('野狼','wild_wolf')
    copy_res('白幽灵','white_ghost')
    copy_res('兽人斧头兵','axe_orc')
    copy_res('zombie僵尸','zombie')

    copy_res('saber_b','saber\saber_b')
    copy_res('saber_f','saber\saber_f')
    print("success")
    
if __name__ == "__main__":
  main()
