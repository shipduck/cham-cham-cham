#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
from os import walk
import copy

# 무시할 파일명은 대소문자 무시
IGNORE_DIR_NAME = ['osx']
IGNORE_FILE_NAME = ['thumbs.db', 'desktop.ini']

def get_res_dir():
    curr_dir = os.path.dirname(__file__)
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

        if len(package) == len(self.package):
            if package == self.package:
                return self

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

        return None

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

def main():
    res_dir = get_res_dir()
    res_file_list = create_res_file_list(res_dir)
    #for x in res_file_list:
    #    print x

    root = Package(['res'])
    for x in res_file_list:
        node = root.find_node(x.package)
        assert node.package == x.package
        node.res_list.append(x)

    # print stdout
    print '#pragma once'
    print '#include <string>'
    root.render_stdout()

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
