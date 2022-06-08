TEMPLATE = subdirs

SUBDIRS = \
    wm-common \
    wm3000i \
    wm3000u

wm3000i.depends = wm-common
wm3000u.depends = wm-common

CONFIG += c++11
