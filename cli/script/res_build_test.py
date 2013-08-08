#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import unittest
from res_build import *

class ResourceReaderTest(unittest.TestCase):
    def test_init(self):
        res = ResourceFile(r'..\res\terrain-heightmap.png')
        self.assertEqual(['res'], res.package)
        self.assertEqual('TERRAIN_HEIGHTMAP_PNG', res.name)

        res = ResourceFile(r'..\res\font\console-14.xml')
        self.assertEqual(['res', 'font'], res.package)
        self.assertEqual('CONSOLE_14_XML', res.name)

        res = ResourceFile(r'..\res\test\sora2.png')
        self.assertEqual(['res', 'test'], res.package)
        self.assertEqual('SORA2_PNG', res.name)

        res = ResourceFile(r'..\res\test\sora__a.png')
        self.assertEqual(['res', 'test', 'sora'], res.package)
        self.assertEqual('A_PNG', res.name)

        res = ResourceFile(r'..\res\test\sora\b.png')
        self.assertEqual(['res', 'test', 'sora'], res.package)
        self.assertEqual('B_PNG', res.name)

class PackageTest(unittest.TestCase):
    def test_find_node(self):
        root = Package(['res'])

        pkg = ['res']
        node_root = root.find_node(pkg)
        self.assertEqual(node_root, root)

        pkg = ['res', 'font']
        node_res_font = root.find_node(pkg)
        self.assertEqual(node_res_font.package, pkg)

        pkg = ['res', 'menu']
        node_res_menu = root.find_node(pkg)
        self.assertEqual(node_res_menu.package, pkg)

        pkg = ['res', 'font', 'sora']
        node_res_font_sora = root.find_node(pkg)
        self.assertEqual(node_res_font_sora.package, pkg)

        # 완전 뜬금없는거 한번에 생성
        pkg = ['res', 'a', 'b']
        node_a_b = root.find_node(pkg)
        self.assertEqual(node_a_b.package, pkg)

        pkg = ['res', 'a']
        node_a = root.find_node(pkg)
        self.assertEqual(node_a.package, pkg)

        # 자동생성된 부모안에 자식이 제대로 존재해야됨
        self.assertEqual(True, node_a_b in node_a.child_list)

        # 있는거 다시 얻어도 똑같은거 나와야됨
        self.assertEqual(node_a_b, root.find_node(['res', 'a', 'b']))

if __name__ == '__main__':
    unittest.main()