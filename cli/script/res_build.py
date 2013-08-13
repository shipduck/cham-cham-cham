#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
from os import walk
import copy
import sys
from cStringIO import StringIO

# 무시할 파일명은 대소문자 무시
IGNORE_DIR_NAME = ['osx']
IGNORE_FILE_NAME = ['thumbs.db', 'desktop.ini', 'Icon\r']
SCRIPT_PATH = None

def get_script_path():
    global SCRIPT_PATH
    if SCRIPT_PATH == None:
        SCRIPT_PATH = os.path.relpath(os.path.realpath(__file__), os.path.realpath(os.curdir))
    
    return SCRIPT_PATH

def get_res_dir():
    curr_dir = os.path.dirname(get_script_path())
    res_dir = os.path.join(curr_dir, '..', 'res')
    if os.path.isdir(res_dir) == False:
       print u'directory %s is not exist' % res_dir
       raise SystemExit
    return res_dir

class ResourceFile(object):
    def __init__(self, filename):
        assert os.path.isfile(filename)
        self.raw_filename = filename

        self.filename = filename[3:]
        token_list = self.filename.split(os.path.sep)
        assert len(token_list) > 1

        token_list = token_list[:-1] + token_list[-1].split('__')
        self.package = token_list[:-1]
        self.name = token_list[-1].replace('.', '_').replace('-', '_').upper()

    def __repr__(self):
        return unicode(self).encode('utf-8')

    def __unicode__(self):
        return u'<%s package=%s, name=%s>' % (type(self).__name__, self.package, self.name)

class Package(object):
    def __init__(self, package):
        self.package = package
        self.res_list = []
        self.child_list = []

    def find_node(self, package):
        assert len(package) >= len(self.package)

        # 같은 레벨의 노드이면 package만 같아도 동일한것으로 볼수있다
        if len(package) == len(self.package):
            if package == self.package:
                return self
            else:
                return None

        # package 관계도를 볼떄 parent-child가 될리 없는 경우
        if package[:len(self.package)] != self.package:
            return None

        for child in self.child_list:
            retval = child.find_node(package)
            if retval != None:
                return retval

        if len(package) == len(self.package) + 1:
            node = Package(package)
            self.child_list.append(node)
            return node

        # a,b,c 패키지인데 a,b가 없을떄 진입한다.
        # 이경우, 빈 노드를 추가해주면 될듯
        for x in range(1, len(package)):
            parent_package = package[0:x]
            self.find_node(parent_package)
        return self.find_node(package)

    def render_stdout(self):
        indent_level = len(self.package) - 1
        indent = '\t' * indent_level

        print indent + 'namespace %s {' % (self.package[-1],)

        for x in self.res_list:
            #print indent + '\tconst char *%s = "%s";' % (x.name, x.filename.replace('\\', '\\\\'))
            print indent + '\tstatic const std::string %s = "%s";' % (x.name, x.filename.replace('\\', '\\\\'))

        for x in self.child_list:
            x.render_stdout()

        print indent+ '}'

def get_res_header_content():
    res_dir = get_res_dir()
    res_file_list = create_res_file_list(res_dir)
    #for x in res_file_list:
    #    print x

    root = Package(['res'])
    for x in res_file_list:
        node = root.find_node(x.package)
        assert node.package == x.package
        node.res_list.append(x)


    # setup the environment
    backup = sys.stdout

    sys.stdout = StringIO()     # capture output
    print '#pragma once'
    print '#include <string>'
    root.render_stdout()
    out = sys.stdout.getvalue() # release output

    sys.stdout.close()  # close the stream
    sys.stdout = backup # restore original stdout

    return out

def main():
    out = get_res_header_content()

    curr_dir = os.path.dirname(get_script_path())
    target_file = os.path.join(curr_dir, '..', 'src', 'res.h')
    if os.path.exists(target_file) == False:
        f = open(target_file, 'w')
        f.write(out)
        f.close()
        return

    with open(target_file, 'r') as prev_file:
        prev = prev_file.read()
        if prev == out:
            return

    f = open(target_file, 'w')
    f.write(out)
    f.close()

def filter_filename_list(filenames):
    for ignore in IGNORE_FILE_NAME:
        filenames = filter(lambda x: x.lower() != ignore.lower(), filenames)
    return filenames

def get_ignore_dir_list(res_dir):
    # 1 level director중에서 무시하는 디렉토리 이름 잡아내기
    dir_list = [res_dir]
    for (dirpath, dirnames, filenames) in walk(res_dir):
        dir_list += map(lambda x: os.path.join(dirpath, x), dirnames)

    base_ignore_list = map(lambda x: os.path.join(res_dir, x), IGNORE_DIR_NAME)

    ignore_list = []
    for dirname in dir_list:
        if dirname in base_ignore_list:
            ignore_list.append(dirname)
            continue

        for ignore in base_ignore_list:
            if (ignore + os.path.sep).lower() in dirname.lower():
                ignore_list.append(dirname)

    ignore_list += base_ignore_list
    return ignore_list

def create_res_file_list(res_dir):
    ignore_dir_list = get_ignore_dir_list(res_dir)
    res_file_list = []
    for (dirpath, dirnames, filenames) in walk(res_dir):
        if dirpath in ignore_dir_list:
            continue

        filenames = filter_filename_list(filenames)
        filenames = map(lambda x: os.path.join(dirpath, x), filenames)
        res_file_list += map(lambda x: ResourceFile(x), filenames)
    return res_file_list

if __name__ == '__main__':
    main()
